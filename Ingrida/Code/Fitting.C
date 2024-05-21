//CPP
#include <iostream>
#include <cmath>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TMath.h>
#include <RooDataHist.h>
#include <RooHistPdf.h>
#include <RooRealVar.h>
#include <RooAddPdf.h>
#include <RooArgList.h>
#include <RooHistPdf.h>
#include <RooFitResult.h>
#include <TMatrixD.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
//Self-Defined
//#include "/home/penguin/Rat/rat-7.0.14-ROOT5/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

#include "/rat/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

Double_t GiveExpection(Double_t Parameter_1, Double_t Parameter_2, Double_t Parameter_3 ,Int_t Index, TH1D *h1, TH1D *h2, TH1D *h3, TH1D *h4);

TMatrixD GiveCorMatrix(Double_t *Parameters, TH1D *data, TH1D *h1, TH1D *h2, TH1D *h3, TH1D *h4);


TH1D *h1, *h2, *h3, *h4;

double self_function(double *x, double*par)
{
    double x_component = x[0];//横坐标分量
    //std::cout << x_component << "," << par[0] << "," << par[1] << "," << par[2] << std::endl;
    int bin = h1->GetXaxis()->FindBin(x_component);
    double part1 = par[0] * h1->GetBinContent(bin);
    double part2 = par[1] * h2->GetBinContent(bin);
    double part3 = par[2] * h3->GetBinContent(bin);
    double part4 = par[2] * par[3] * h4->GetBinContent(bin);
    return part1 + part2 + part3 + part4;
};

int Fitting()
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
    h1 = new TH1D(Name1.c_str(), "", BinNumber, down, up);
    h2 = new TH1D(Name2.c_str(), "", BinNumber, down, up);
    h3 = new TH1D(Name3.c_str(), "", BinNumber, down, up);
    h4 = new TH1D(Name4.c_str(), "", BinNumber, down, up);
    TH1D *data_hist = new TH1D("data_hist", "", BinNumber, down, up);

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
    h1->Scale(1.0/h1->Integral());
    h2->Scale(1.0/h2->Integral());
    h3->Scale(1.0/h3->Integral());
    h4->Scale(1.0/h4->Integral());

    h1_Plot->Scale(1.0/h1_Plot->Integral());
    h2_Plot->Scale(1.0/h2_Plot->Integral());
    h3_Plot->Scale(1.0/h3_Plot->Integral());
    h4_Plot->Scale(1.0/h4_Plot->Integral());
    //Theory Predictions
    Int_t Total_Number = datatree->GetEntries();
    Double_t Pre_Number1 = 17.2, Pre_Number1_Min = Pre_Number1 - 5.9, Pre_Number1_Max = Pre_Number1 + 5.9;
    Double_t Pre_Number2 = 24.88;
    Double_t Pre_Number3 = 5.22;
    Double_t Pre_Number4 = 1.39;
    //Fit Function
    TF1 *ftotal = new TF1("ftotal", self_function , down, up, 4);
    //Setting of Parameters
    // ftotal->SetParameters(Pre_Number1, Pre_Number2, Pre_Number3, 1/3.7);
    // ftotal->SetParLimits(0, Pre_Number1_Min, Pre_Number1_Max);
    // ftotal->SetParLimits(1, Pre_Number2*(0.97), Pre_Number2*(1.03))
    // ftotal->SetParLimits(3, 1/4.0, 1/2.4);

    ftotal->SetParameters(Pre_Number1, Pre_Number2, Pre_Number3, 1/3.7);
    ftotal->SetParLimits(0, 0, 0);
    ftotal->SetParLimits(1, 24.0, 25.6);
    ftotal->SetParLimits(2, 0, 0);
    ftotal->SetParLimits(3, 1/4, 1/2.7);

    data_hist->Fit("ftotal", "W");
    //Get Results
    Double_t Parameter_Best_1 = ftotal->GetParameter(0);
    Double_t Parameter_Error_1 = ftotal->GetParError(0);
    Double_t Parameter_Best_2 = ftotal->GetParameter(1);
    Double_t Parameter_Error_2 = ftotal->GetParError(1);
    Double_t Parameter_Best_3 = ftotal->GetParameter(2);
    Double_t Parameter_Error_3 = ftotal->GetParError(2);
    Double_t Parameter_Best_4 = ftotal->GetParameter(3);
    Double_t Parameter_Error_4 = ftotal->GetParError(3);
    //Compute Events
    Double_t Number_1 = Parameter_Best_1 * h1->Integral();
    Double_t Error_1 = Parameter_Error_1 * h1->Integral();
    Double_t Number_2 = Parameter_Best_2 * h2->Integral();
    Double_t Error_2 = Parameter_Error_2 * h2->Integral();
    Double_t Number_3 = Parameter_Best_3 * h3->Integral();
    Double_t Error_3 = Parameter_Error_3 * h3->Integral();

    Double_t Number_4 = Parameter_Best_3 * Parameter_Best_4 * h4->Integral();
    Double_t Error_4 = sqrt(pow( Parameter_Error_3/Parameter_Best_3, 2) + pow( Parameter_Error_4/ Parameter_Best_4, 2)) * Number_4;

    Double_t Number_Sum = Number_1 + Number_2 + Number_3 + Number_4;
    Double_t Error_Sum = Error_1 + Error_2 + Error_3 + Error_4;

    Double_t ChiSquare = ftotal->GetChisquare();
    Double_t NDF = ftotal->GetNDF();

    Double_t Ratio_Best = 1/Parameter_Best_4;
    Double_t Ration_Error = Parameter_Error_4/pow(Parameter_Best_4, 2);

    std::cout << "AN:" << Number_1 << ", Error:" << Error_1 << ", Relative Error:" << Error_1/Number_1 << std::endl;
    std::cout << "Reactor:" << Number_2 << ", Error:" << Error_2 << ", Relative Error:" << Error_2/Number_2 << std::endl;
    std::cout << "U:" << Number_3 << ", Error:" << Error_3 << ", Relative Error:" << Error_3/Number_3 << std::endl;
    std::cout << "Th:" << Number_4 << ", Error:" << Error_4 << ", Relative Error:" << Error_4/Number_4 << std::endl;
    std::cout << "Total:" << Number_Sum << ", Error:" << Error_Sum << ", Relative Error:" << Error_Sum/Number_Sum << std::endl;
    std::cout << "U/Th:" << Ratio_Best << ", Error:" << Ration_Error << ",Relative Error:" << Ration_Error/Ratio_Best << std::endl;
    std::cout << "chi square:" << ChiSquare << ", NDF:" << NDF << ", Ratio:" << ChiSquare / NDF << std::endl;
    return 0;
    //Rescaling TH1D for Plot
    h1_Plot->Scale(5 * Number_1);
    h2_Plot->Scale(5 * Number_2);
    h3_Plot->Scale(5 * Number_3);
    //ReSet Total Bin
    delete data_hist;
    data_hist = new TH1D("data_hist", "", BinNumber, down, up);
    datatree->Project("data_hist", BranchName.c_str());
    TH1D *Sum_Hist = (TH1D*)h1_Plot->Clone();
    Sum_Hist->Add(h2_Plot,1.0);
    Sum_Hist->Add(h3_Plot,1.0);
    SaveFivePlots(OutPut_Prompt_Energy.c_str(), data_hist, "Data", h1_Plot, "AN", h2_Plot, "Reactor", h3_Plot, "Geo", Sum_Hist, "Sum" , "E_{Prompt}/MeV", 0, 0, 15, "Prompt Event Fitting", 0.7, 0.6, 0.9, 0.8 );
    return 0;
};

int Fitting_ConstantRation()
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
    h1 = new TH1D(Name1.c_str(), "", BinNumber, down, up);
    h2 = new TH1D(Name2.c_str(), "", BinNumber, down, up);
    TH1D *h_U = new TH1D(Name3.c_str(), "", BinNumber, down, up);
    TH1D *h_Th = new TH1D(Name4.c_str(), "", BinNumber, down, up);
    h3 = new TH1D("Geo", "", BinNumber, down, up);
    TH1D *data_hist = new TH1D("data_hist", "", BinNumber, down, up);

    TH1D *h1_Plot = new TH1D("PDF1_Plot", "", 100, down, up);
    TH1D *h2_Plot = new TH1D("PDF2_Plot", "", 100, down, up);
    TH1D *h3_Plot = new TH1D("PDF3_Plot", "", 100, down, up);
    TH1D *h_Th_Plot = new TH1D("PDF_Th_Plot", "", 100, down, up);
    //Load Data and PDF
    std::string BranchName = "PromptEnergy";
    intree1->Project(Name1.c_str(), BranchName.c_str());
    intree2->Project(Name2.c_str(), BranchName.c_str());
    intree3->Project(Name3.c_str(), BranchName.c_str());
    intree4->Project(Name4.c_str(), BranchName.c_str());
    datatree->Project("data_hist", BranchName.c_str());
    intree3->Project("Geo", BranchName.c_str());
    h3->Add(h_Th, 1.0/3.7);

    intree1->Project("PDF1_Plot", BranchName.c_str());
    intree2->Project("PDF2_Plot", BranchName.c_str());
    intree3->Project("PDF3_Plot", BranchName.c_str());
    intree4->Project("PDF_Th_Plot", BranchName.c_str());
    h3_Plot->Add(h_Th_Plot, 1.0/3.7);
    //Normalized
    h1->Scale(1.0/h1->Integral());
    h2->Scale(1.0/h2->Integral());
    h3->Scale(1.0/h3->Integral());

    h1_Plot->Scale(1.0/h1_Plot->Integral());
    h2_Plot->Scale(1.0/h2_Plot->Integral());
    h3_Plot->Scale(1.0/h3_Plot->Integral());
    //Theory Predictions
    Int_t Total_Number = datatree->GetEntries();
    Double_t Pre_Number1 = 17.2, Pre_Number1_Min = Pre_Number1 - 5.9, Pre_Number1_Max = Pre_Number1 + 5.9;
    Double_t Pre_Number2 = 24.88;
    Double_t Pre_Number3 = 5.22;
    Double_t Pre_Number4 = 1.39;
    //Fit Function
    TF1 *ftotal = new TF1("ftotal", self_function , down, up, 3);
    //Setting of Parameters
    ftotal->SetParameters(Pre_Number1, Pre_Number2, Pre_Number3 + Pre_Number4);
    ftotal->SetParLimits(0, Pre_Number1_Min, Pre_Number1_Max);

    data_hist->Fit("ftotal", "P");
    //Get Results
    Double_t Parameter_Best_1 = ftotal->GetParameter(0);
    Double_t Parameter_Error_1 = ftotal->GetParError(0);
    Double_t Parameter_Best_2 = ftotal->GetParameter(1);
    Double_t Parameter_Error_2 = ftotal->GetParError(1);
    Double_t Parameter_Best_3 = ftotal->GetParameter(2);
    Double_t Parameter_Error_3 = ftotal->GetParError(2);
    //Compute Events
    Double_t Number_1 = Parameter_Best_1 * h1->Integral();
    Double_t Error_1 = Parameter_Error_1 * h1->Integral();
    Double_t Number_2 = Parameter_Best_2 * h2->Integral();
    Double_t Error_2 = Parameter_Error_2 * h2->Integral();
    Double_t Number_3 = Parameter_Best_3 * h3->Integral();
    Double_t Error_3 = Parameter_Error_3 * h3->Integral();

    Double_t Number_Sum = Number_1 + Number_2 + Number_3;
    Double_t Error_Sum = Error_1 + Error_2 + Error_3;

    Double_t ChiSquare = ftotal->GetChisquare();
    Double_t NDF = ftotal->GetNDF();

    std::cout << "AN:" << Number_1 << ", Error:" << Error_1 << ", Relative Error:" << Error_1/Number_1 << std::endl;
    std::cout << "Reactor:" << Number_2 << ", Error:" << Error_2 << ", Relative Error:" << Error_2/Number_2 << std::endl;
    std::cout << "Geo:" << Number_3 << ", Error:" << Error_3 << ", Relative Error:" << Error_3/Number_3 << std::endl;
    std::cout << "Total:" << Number_Sum << ", Error:" << Error_Sum << ", Relative Error:" << Error_Sum/Number_Sum << std::endl;
    std::cout << "chi square:" << ChiSquare << ", NDF:" << NDF << ", Ratio:" << ChiSquare / NDF << std::endl;

    //Rescaling TH1D for Plot
    h1_Plot->Scale(5 * Number_1);
    h2_Plot->Scale(5 * Number_2);
    h3_Plot->Scale(5 * Number_3);
    //ReSet Total Bin
    delete data_hist;
    data_hist = new TH1D("data_hist", "", BinNumber, down, up);
    datatree->Project("data_hist", BranchName.c_str());
    TH1D *Sum_Hist = (TH1D*)h1_Plot->Clone();
    Sum_Hist->Add(h2_Plot,1.0);
    Sum_Hist->Add(h3_Plot,1.0);
    SaveFivePlots(OutPut_Prompt_Energy.c_str(), data_hist, "Data", h1_Plot, "AN", h2_Plot, "Reactor", h3_Plot, "Geo", Sum_Hist, "Sum" , "E_{Prompt}/MeV", 0, 0, 15, "Prompt Event Fitting", 0.7, 0.6, 0.9, 0.8 );
    return 0;
};

// int RooFit()
// {
//     std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/MC/";
//     std::string InFile1 = InPWD + "AN/MC_AN_300000-307612.root";
//     std::string InFile2 = InPWD + "Reactor/MC_Reactor_OffLine_300000-307612.root";
//     std::string InFile3 = InPWD + "U/MC_U_300000-307612.root";
//     std::string InFile4 = InPWD + "Th/MC_Th_300000-307612.root";
//     std::string DataFile = "/rat/MyCode/Work/Geo-nu-Data/IBDCandidate/Gold_20R_IBD_Candidate_0000300000-0000307612.root";

//     std::string Name1 = "AN";
//     std::string Name2 = "Reactor";
//     std::string Name3 = "U"; 
//     std::string Name4 = "Th";
//     //Create TFile
//     TFile infile1(InFile1.c_str());
//     TTree *intree1 = (TTree*) infile1.Get("output");
//     TFile infile2(InFile2.c_str());
//     TTree *intree2 = (TTree*) infile2.Get("output");
//     TFile infile3(InFile3.c_str());
//     TTree *intree3 = (TTree*) infile3.Get("output");
//     TFile infile4(InFile4.c_str());
//     TTree *intree4 = (TTree*) infile4.Get("output");
//     TFile datafile(DataFile.c_str());
//     TTree *datatree = (TTree*) datafile.Get("output");
//     //Create Histogram
//     int BinNumber = 15;
//     Double_t down = 0.8, up = 8.0;
//     TH1D *h1 = new TH1D(Name1.c_str(), "", BinNumber, down, up);
//     TH1D *h2 = new TH1D(Name2.c_str(), "", BinNumber, down, up);
//     TH1D *h_U = new TH1D(Name3.c_str(), "", BinNumber, down, up);
//     TH1D *h_Th = new TH1D(Name4.c_str(), "", BinNumber, down, up);
//     TH1D *h3 = new TH1D("Geo", "", BinNumber, down, up);
//     TH1D *data_hist = new TH1D("data_hist", "", BinNumber, down, up);
//     //Load Histogram
//     std::string BranchName = "PromptEnergy";
//     intree1->Project(Name1.c_str(), BranchName.c_str());
//     intree2->Project(Name2.c_str(), BranchName.c_str());
//     intree3->Project(Name3.c_str(), BranchName.c_str());
//     intree4->Project(Name4.c_str(), BranchName.c_str());
//     datatree->Project("Data", BranchName.c_str());
//     intree3->Project("Geo", BranchName.c_str());
//     h3->Add(h_Th, 1.0/3.7);
//     //Normalized
//     h1->Scale(1.0/h1->Integral());
//     h2->Scale(1.0/h2->Integral());
//     h3->Scale(1.0/h3->Integral());
//     //Define Energy Variable
//     RooRealVar *prompt_energy = new RooRealVar("prompt_energy", "prompt_energy", down, up);
//     //Load Data
//     RooDataHist data("data", "data", *prompt_energy, data_hist);
//     RooDataHist pdf1_hist("AN_PDF", "AN_PDF", *prompt_energy, h1);
//     RooDataHist pdf2_hist("Reactor_PDF", "Reactor_PDF", *prompt_energy, h2);
//     RooDataHist pdf3_hist("Geo_PDF", "Geo_PDF", *prompt_energy, h3);

//     // 转换 RooDataHist 到 RooHistPdf
//     RooHistPdf pdf1_pdf("AN_PDF", "AN_PDF", *prompt_energy, pdf1_hist);
//     RooHistPdf pdf2_pdf("Reactor_PDF", "Reactor_PDF", *prompt_energy, pdf2_hist);
//     RooHistPdf pdf3_pdf("Geo_PDF", "Geo_PDF", *prompt_energy, pdf3_hist);

//     RooArgList pdflist(pdf1_pdf, pdf2_pdf, pdf3_pdf);
//     //Parameters to Be Fitted
//     RooRealVar a("a", "a", 10, 0, 100);
//     RooRealVar b("b", "b", 15, 0, 100);
//     RooRealVar c("c", "c", 5, 0, 100);

//     RooArgList coefflist(a, b, c);
//     //Get Total PDF
//     RooAddPdf total_pdf("total_pdf", "total_pdf", pdflist, coefflist);
//     //Fitting!
//     total_pdf.fitTo(data);
//     //Get Results
//     Double_t best_fit_a = a.getVal();
//     Double_t best_fit_b = b.getVal();
//     Double_t best_fit_c = c.getVal();
//     std::cout << best_fit_a << "," << best_fit_b << "," << best_fit_c << std::endl;

//     return 0;
// };

// int Fitting_ML()
// {   
//     std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/MC/";
//     std::string InFile1 = InPWD + "AN/MC_AN_300000-307612.root";
//     std::string InFile2 = InPWD + "Reactor/MC_Reactor_OffLine_300000-307612.root";
//     std::string InFile3 = InPWD + "U/MC_U_300000-307612.root";
//     std::string InFile4 = InPWD + "Th/MC_Th_300000-307612.root";
//     std::string DataFile = "/rat/MyCode/Work/Geo-nu-Data/IBDCandidate/Gold_20R_IBD_Candidate_0000300000-0000307612.root";

//     std::string Name1 = "AN";
//     std::string Name2 = "Reactor";
//     std::string Name3 = "U"; 
//     std::string Name4 = "Th";
//     //Create TFile
//     TFile infile1(InFile1.c_str());
//     TTree *intree1 = (TTree*) infile1.Get("output");
//     TFile infile2(InFile2.c_str());
//     TTree *intree2 = (TTree*) infile2.Get("output");
//     TFile infile3(InFile3.c_str());
//     TTree *intree3 = (TTree*) infile3.Get("output");
//     TFile infile4(InFile4.c_str());
//     TTree *intree4 = (TTree*) infile4.Get("output");
//     TFile datafile(DataFile.c_str());
//     TTree *datatree = (TTree*) datafile.Get("output");
//     //Create Histogram
//     int BinNumber = 15;
//     Double_t down = 0.8, up = 8.0;
//     h1 = new TH1D(Name1.c_str(), "", BinNumber, down, up);
//     h2 = new TH1D(Name2.c_str(), "", BinNumber, down, up);
//     h3 = new TH1D(Name3.c_str(), "", BinNumber, down, up);
//     TH1D *h4 = new TH1D(Name4.c_str(), "", BinNumber, down, up);
//     TH1D *data = new TH1D("Data", "", BinNumber, down, up);
//     //Load Histogram
//     std::string BranchName = "PromptEnergy";
//     intree1->Project(Name1.c_str(), BranchName.c_str());
//     intree2->Project(Name2.c_str(), BranchName.c_str());
//     intree3->Project(Name3.c_str(), BranchName.c_str());
//     intree4->Project(Name4.c_str(), BranchName.c_str());
//     datatree->Project("Data", BranchName.c_str());
//     //Normalized
//     h1->Scale(1.0/h1->Integral());
//     h2->Scale(1.0/h2->Integral());
//     h3->Scale(1.0/h3->Integral());
//     h4->Scale(1.0/h4->Integral());
//     //Theory Predictions
//     Double_t Pre_Number1 = 17.2, Pre_Number1_Min = 17.2 - 5.9, Pre_Number1_Max = 17.2 + 5.9;
//     Double_t Pre_Number2 = 24.88;
//     Double_t Pre_Number3 = 5.22;
//     Double_t Pre_Number4 = 1.39;
//     h1->Scale(Pre_Number1);
//     h2->Scale(Pre_Number2);
//     h3->Scale(Pre_Number3);
//     h4->Scale(Pre_Number4);
//     //Begin to Fit
//     Int_t Total_Number = 56;
//     Double_t Parameter_1_Up = Pre_Number1_Max/Pre_Number1, Parameter_1_Down = Pre_Number1_Min/Pre_Number1;
//     Double_t Parameter_2_Up = (Total_Number - Pre_Number1_Min)/Pre_Number2, Parameter_2_Down = 0;
//     Double_t Parameter_3_Up = (Total_Number - Pre_Number1_Min)/(Pre_Number3 + Pre_Number4), Parameter_3_Down = 0;
//     Double_t StepLength_1 = 0.01 * (Parameter_1_Up - Parameter_1_Down);
//     Double_t StepLength_2 = 0.01 * (Parameter_2_Up - Parameter_2_Down);
//     Double_t StepLength_3 = 0.01 * (Parameter_3_Up - Parameter_3_Down);

//     Double_t Parameter_1 = 0, Parameter_2  = 0 , Parameter_3  = 0;
//     Double_t Sum = 0;//-2log L
//     Double_t MinSum = 99999, ParameterSet[3] = {0, 0, 0};
//     int n_i;
//     Double_t mu_i;
//     for(int ii1 = 0; ii1 < 100 + 1; ii1++)
//     {
//         Parameter_1 = Parameter_1_Down + ii1 * StepLength_1; 
//         for(int ii2 = 0; ii2 < 100 + 1; ii2++)
//         {
//             Parameter_2 = Parameter_2_Down + ii2 * StepLength_2;
//             for(int ii3 = 0; ii3 < 100 + 1; ii3++)
//             {
//                 Parameter_3 = Parameter_3_Down + ii3 * StepLength_3; 
//                 Sum = 0;
//                 //if(Parameter_1 * Pre_Number1 + Parameter_2 * Pre_Number2 + Parameter_3 * Pre_Number3 + Parameter_3 * Pre_Number4/3.7 > Total_Number){ continue;};
//                 for(int iBin = 1; iBin < data->GetNbinsX() + 1; iBin++)
//                 {
//                     n_i = data->GetBinContent(iBin);
//                     if(n_i == 0){continue;};
//                     mu_i = GiveExpection(Parameter_1, Parameter_2, Parameter_3, iBin, h1, h2, h3, h4);
//                     Sum = Sum + 2.0 * (mu_i + log(TMath::Gamma(n_i + 1)) - n_i * log(mu_i));
//                     // std::cout << iBin << "," << n_i << "," << mu_i << "," << Sum << std::endl;
//                 };
//                 if(Sum == MinSum)
//                 {
//                     std::cout << "Find Same Value:" << Parameter_1 << "," << Parameter_2 << "," << Parameter_3 << ",Sum:" << Sum << ",Min Sum:" << MinSum << std::endl;
//                 };
//                 if(Sum < MinSum)
//                 {
//                     MinSum = Sum;
//                     ParameterSet[0] = Parameter_1;
//                     ParameterSet[1] = Parameter_2;
//                     ParameterSet[2] = Parameter_3;
//                 };
//             };
//         };
//     };
//     std::cout << "Min Sum:" << MinSum << ",Parameters:" << ParameterSet[0] << "," << ParameterSet[1] << "," << ParameterSet[2] << std::endl;
//     std::cout << "AN:" << Pre_Number1 * ParameterSet[0] << ",Reactor:" << Pre_Number2 * ParameterSet[1] << ",Geo:" << Pre_Number3 * ParameterSet[2] + Pre_Number4 * ParameterSet[2]/3.7  << ",Sum:" << Pre_Number1 * ParameterSet[0] + Pre_Number2 * ParameterSet[1] + Pre_Number3 * ParameterSet[2] + Pre_Number4 * ParameterSet[2]/3.7<< std::endl;
//     //Compute Matrix
//     //GiveCorMatrix(ParameterSet, data, h1, h2, h3, h4);
//     return 0;
// };

// Double_t GiveExpection(Double_t Parameter_1, Double_t Parameter_2, Double_t Parameter_3 ,Int_t Index, TH1D *h1, TH1D *h2, TH1D *h3, TH1D *h4)
// {
//     Double_t Part1 = Parameter_1 * h1->GetBinContent(Index);
//     Double_t Part2 = Parameter_2 * h2->GetBinContent(Index);
//     Double_t Part3 = Parameter_3 * h3->GetBinContent(Index);
//     Double_t Part4 = Parameter_3 * h4->GetBinContent(Index) / 3.7;
//     Double_t res =  Part1 + Part2 + Part3 + Part4;
//     return res;
// };

// void GiveCorMatrix(Double_t *Parameters, TH1D *data, TH1D *h1, TH1D *h2, TH1D *h3, TH1D *h4)
// {
//     Double_t Total_Mu = 0;
//     Int_t n_i = 0;
//     std::vector<Double_t> Mu_1, Mu_2, Mu_3, Factor;
//     for(int iBin = 1; iBin < data->GetNbinsX() + 1; iBin++)
//     {
//         Mu_1.push_back(h1->GetBinContent(iBin));
//         Mu_2.push_back(h2->GetBinContent(iBin));
//         Mu_3.push_back(h3->GetBinContent(iBin) + h4->GetBinContent(iBin)/3.7);
//         Factor.push_back(GiveExpection(Parameters[0], Parameters[1], Parameters[2], iBin, h1, h2, h3, h4));
//     };
    
    
// };