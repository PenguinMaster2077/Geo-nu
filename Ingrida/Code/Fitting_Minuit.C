//CPP
#include <iostream>
#include <cmath>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVirtualFitter.h>

TH1D *pdf_1_hist, *pdf_2_hist, *pdf_3_hist, *pdf_4_hist, *pdf_total_hist, *data_hist;
TVirtualFitter *minuit;
double arg_list[2];
int number_parmeters, total_number_parameters;

// static Double_t Pre_Number[4] = {17.2, 24.88, 5.22, 1.39};
// static Double_t Pre_Number_Error[4] = {5.9, 0.8, 1.26, 0.34};
static Double_t Pre_Number[4] = {18.85, 28.06, 5.24, 1.41};
static Double_t Pre_Number_Error[4] = {1.89, 0.79, 5.24, 1.41};
static Double_t Pre_Number_Min[4], Pre_Number_Max[4];

Double_t Maxmium_Likelihood(Double_t *parameters)
{
    double logL = 0, pdf_content, data_content;
    //Binned data
    for(int iBin = 1; iBin < data_hist->GetNbinsX() + 1; iBin++)
    {
        pdf_content = pdf_total_hist->GetBinContent(iBin);
        data_content = data_hist->GetBinContent(iBin);
        if(pdf_content <= 0){continue;};
        logL = logL - pdf_content + data_content * log(pdf_content);
    };
    //惩罚项
    for(int iVars = 0; iVars < 4; iVars++)
    {
        logL = logL - 0.5 * pow(parameters[iVars] - Pre_Number[iVars], 2)/ pow(Pre_Number_Error[iVars], 2);
    };

    return logL;
};

void minuit_MLL(Int_t &, Double_t *, Double_t &value, Double_t *parameters, Int_t)//args must be like these
{
    
    //Compute the Number of Events
    Double_t number_1 = parameters[0];
    Double_t number_2 = parameters[1];
    Double_t number_3 = parameters[2];
    Double_t number_4 = parameters[3];
    //Reset total pdf
    pdf_total_hist = (TH1D*)(pdf_1_hist->Clone("pdf_total"));
    pdf_total_hist->Scale(number_1);
    pdf_total_hist->Add(pdf_2_hist, number_2);
    pdf_total_hist->Add(pdf_3_hist, number_3);
    pdf_total_hist->Add(pdf_4_hist, number_4);
    //Execute Maximum Likilihood
    value =  -2 * Maxmium_Likelihood(parameters);
};

void fitting_models()
{
    int index_error_state = 1;
    unsigned int num_fitting_steps = 0;
    while (index_error_state) {
        ++num_fitting_steps;
        index_error_state = minuit->ExecuteCommand("MIGRAD", arg_list, 2);
        //std::cout << num_fitting_steps << "," << minuit->GetParameter(0) << std::endl;
        if (num_fitting_steps >= 15) break;
    };
    //Get Results
    Double_t mininum_value, estimated_dis_minimum, error;
    minuit->GetStats(mininum_value, estimated_dis_minimum, error, number_parmeters, total_number_parameters);
    //std::cout << "[fitting_models]: miniun_value:" << mininum_value << ", edm:" << estimated_dis_minimum << ", error:" << error << ",number of parmeters:" << number_parmeters << ", total number of parmeters:" << total_number_parameters << std::endl;
}

int Fitting_Minuit()
{
    std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/MC/";
    std::string InFile1 = InPWD + "AN/MC_AN_300000-307612.root";
    std::string InFile2 = InPWD + "Reactor/MC_Reactor_OffLine_300000-307612.root";
    std::string InFile3 = InPWD + "U/MC_U_300000-307612.root";
    std::string InFile4 = InPWD + "Th/MC_Th_300000-307612.root";
    std::string DataFile = "/rat/MyCode/Work/Geo-nu-Data/IBDCandidate/Gold_20R_IBD_Candidate_0000300000-0000307612.root";
    std::string OutPWD = "/rat/MyCode/Work/Geo-nu/Ingrida/Pic/";
    std::string OutPut_Prompt_Energy = OutPWD + "Prompt_Energy.png";

    std::string Name1 = "AN";
    std::string Name2 = "Reactor";
    std::string Name3 = "U"; 
    std::string Name4 = "Th";
    //Create TFile
    TFile infile1(InFile1.c_str());
    TTree *intree1 = (TTree*) infile1.Get("output");
    TFile infile2(InFile2.c_str());
    TTree *intree2 = (TTree*) infile2.Get("output");
    TFile infile3(InFile3.c_str());
    TTree *intree3 = (TTree*) infile3.Get("output");
    TFile infile4(InFile4.c_str());
    TTree *intree4 = (TTree*) infile4.Get("output");
    TFile datafile(DataFile.c_str());
    TTree *datatree = (TTree*) datafile.Get("output");
    //Create Histogram
    int BinNumber = 10;
    Double_t down = 0.8, up = 8.0;
    pdf_1_hist = new TH1D(Name1.c_str(), "", BinNumber, down, up);
    pdf_2_hist = new TH1D(Name2.c_str(), "", BinNumber, down, up);
    pdf_3_hist = new TH1D(Name3.c_str(), "", BinNumber, down, up);
    pdf_4_hist = new TH1D(Name4.c_str(), "", BinNumber, down, up);
    data_hist = new TH1D("data_hist", "", BinNumber, down, up);

    TH1D *h1_Plot = new TH1D("PDF1_Plot", "", 100, down, up);
    TH1D *h2_Plot = new TH1D("PDF2_Plot", "", 100, down, up);
    TH1D *h3_Plot = new TH1D("PDF3_Plot", "", 100, down, up);
    TH1D *h4_Plot = new TH1D("PDF4_Plot", "", 100, down, up);
    //Load Data and PDF
    std::string BranchName = "PromptEnergy";
    intree1->Project(Name1.c_str(), BranchName.c_str());
    intree2->Project(Name2.c_str(), BranchName.c_str());
    intree3->Project(Name3.c_str(), BranchName.c_str());
    intree4->Project(Name4.c_str(), BranchName.c_str());
    datatree->Project("data_hist", BranchName.c_str());

    intree1->Project("PDF1_Plot", BranchName.c_str());
    intree2->Project("PDF2_Plot", BranchName.c_str());
    intree3->Project("PDF3_Plot", BranchName.c_str());
    intree4->Project("PDF4_Plot", BranchName.c_str());
    //Normalized
    pdf_1_hist->Scale(1.0/pdf_1_hist->Integral());
    pdf_2_hist->Scale(1.0/pdf_2_hist->Integral());
    pdf_3_hist->Scale(1.0/pdf_3_hist->Integral());
    pdf_4_hist->Scale(1.0/pdf_4_hist->Integral());

    h1_Plot->Scale(1.0/h1_Plot->Integral());
    h2_Plot->Scale(1.0/h2_Plot->Integral());
    h3_Plot->Scale(1.0/h3_Plot->Integral());
    h4_Plot->Scale(1.0/h4_Plot->Integral());
    // Test
    // pdf_total_hist = (TH1D*)(pdf_1_hist->Clone("pdf_total"));
    // pdf_total_hist->Scale(10);
    // pdf_total_hist->Add(pdf_2_hist, 2);
    // std::cout << pdf_total_hist->Integral() << std::endl;
    //Theory Predictions
    Int_t Total_Number = datatree->GetEntries();
    for(int iVars = 0; iVars < 4; iVars++)
    {
        Pre_Number_Min[iVars] = Pre_Number[iVars] - 3 * Pre_Number_Error[iVars];
        Pre_Number_Max[iVars] = Pre_Number[iVars] + 3 * Pre_Number_Error[iVars];
    };
//Setting Minuit
    TVirtualFitter::SetDefaultFitter("Minuit");
    minuit = TVirtualFitter::Fitter(0, 4);//0: No Fitting Function now; 4 parameters
    // minuit->SetParameter(0, Name1.c_str(), Pre_Number[0], Pre_Number_Error[0], Pre_Number_Min[0], Pre_Number_Max[0]);
    // minuit->SetParameter(1, Name2.c_str(), Pre_Number[1], Pre_Number_Error[1], Pre_Number_Min[1], Pre_Number_Max[1]);
    // minuit->SetParameter(2, Name3.c_str(), Pre_Number[2], Pre_Number_Error[2], Pre_Number_Min[2], Pre_Number_Max[2]);
    // minuit->SetParameter(3, Name4.c_str(), Pre_Number[3], Pre_Number_Error[3], Pre_Number_Min[3], Pre_Number_Max[3]);
    minuit->SetParameter(0, Name1.c_str(), Pre_Number[0], Pre_Number_Error[0], 0, 0);
    minuit->SetParameter(1, Name2.c_str(), Pre_Number[1], Pre_Number_Error[1], 0, 0);
    minuit->SetParameter(2, Name3.c_str(), Pre_Number[2], Pre_Number_Error[2], 0, 0);
    minuit->SetParameter(3, Name4.c_str(), Pre_Number[3], Pre_Number_Error[3], 0, 0);
    
    //Set Print Level
    arg_list[0] = 0;
    minuit->ExecuteCommand("SET PRINT", arg_list, 2);//PRINT only receives the 0th element; 2 is the length of arglist
    //Set minimiser
    arg_list[0] = 5000;// Maxmium calls
    arg_list[1] = 0.01;// Torelance
    //Set Function
    minuit->SetFCN(minuit_MLL);//Minuit uses this function in fitting and there is a standard format of this used function;
    //Fitting
    fitting_models();
    //Get Results
    // Double_t Value[4], Error_Min[4], Error_Max[4], Error_Para[4], Global_Corelation[4];
    std::string Name[4] = {Name1, Name2, Name3, Name4};
    Double_t Value[4], Error[4], Value_Min[4], Value_Max[4];
    Int_t tempInt;
    for(Int_t ii1 = 0; ii1 < 4; ii1++)
    {
        minuit->GetParameter(ii1, Name[ii1].c_str(), Value[ii1], Error[ii1], Value_Min[ii1], Value_Max[ii1]);
        //Value[ii1] = minuit->GetParameter(ii1);
        // minuit->GetErrors(ii1, Error_Max[ii1], Error_Min[ii1], Error_Para[ii1], Global_Corelation[ii1]);
        std::cout << Name[ii1] + ":" << Value[ii1] << ", Error:" << Error[ii1] << ", Max Value:" << Value_Max[ii1] << ", Min Value:" << Value_Min[ii1] << ", Relative Error:" << Error[ii1]/Value[ii1] << std::endl; 
    };
    Double_t Sum = Value[0] + Value[1] + Value[2] + Value[3];
    Double_t Sum_Error = Error[0] + Error[1] + Error[2] + Error[3];
    std::cout << "Sum:" << Sum << ", Error:" << Sum_Error << ", Relative Error:" << Sum_Error/Sum << std::endl; 
    
    return 0;
};