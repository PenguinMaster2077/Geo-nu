#ifndef FITTER_HH
#define FITTER_HH
//CPP
#include <iostream>
#include <cstring>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVirtualFitter.h>
//RAT
//Self-Defined
#include "./Fit_Parameters.hh"
#include "./Models.hh"
#include "./Constants_Setting.hh"

class Fitter
{
public:
    //Constructor and destructor
    Fitter(): Fit_Method(FITTER_EXTENDED_MAXIMUM_LIKELIHOOD), Is_Initialized_Data(false), Is_Binned(false), Is_Initialized_Hist(false), Is_Test(false)
    {
        std::cout << "[Fitter::Fitter] Haven't been Initialized." << std::endl;
    };
    //Global Point
    static Fitter *Point_Fitter_;
    //Member Functions
    static Fitter *Get_Global_Point();
    void Initialize();
    void Initialize_Hist( TH1D* Example_Hist );
    void Initialize_Data( TH1D *Example_Hist );
    Int_t Get_Data_Size () { return Data_Prompt_Energy.size(); };
    std::vector<Double_t> Get_Data() { return Data_Prompt_Energy; };
    void Load_Data( std::string Data_File, Bool_t Binned);
    TVirtualFitter *Get_Minuit() { return minuit; };
    TH1D *Get_Hist_Total() { return Hist_Total; };
    TH1D *Get_Hist_Data() { return Hist_Data; };
    Bool_t &Get_Is_Initialized_Data() { return Is_Initialized_Data; };
    Bool_t &Get_Is_Binned_Data() { return Is_Binned; };
    Bool_t &Get_Is_Initialized_Hist() { return Is_Initialized_Hist; };
    void Record_Fitting_Variable( Double_t res ) { Fitting_Variable = res; };
    Double_t Get_Fitting_Variable() { return Fitting_Variable; };
    void Set_Fitting_Method( std::string Method );
    std::string &Get_Fit_Method() { return Fit_Method; };
    void Use_Extended_Maximum_Likelihood() { Fit_Method = FITTER_EXTENDED_MAXIMUM_LIKELIHOOD; };
    void Use_Chi_Square() { Fit_Method = FITTER_CHI_SQUARE; };
    void Open_Test_Mode() { Is_Test = true; };
    void Close_Test_Mode() { Is_Test = false; };
    Bool_t Get_Test_Index() { return Is_Test; };
    void Fitting();
    void Get_All_Hists();
    void Draw_All();
    void Show_Results();
    Double_t Get_Fit_Covariance_Matrix_Element(unsigned int i, unsigned int j) {return minuit->GetCovarianceMatrixElement(i, j);};
private:
//Minuit
    TVirtualFitter *minuit;
    double arg_list[2];
    Int_t Number_Fit_Parameters;//Total Number of Fitted Parameters
    std::string Fit_Method;
//Data
    TH1D *Hist_Data;
    const std::string Name_Hist_Data = "Data", Title_Hist_Data = "Data Histogram";
    Bool_t Is_Initialized_Data, Is_Binned;
    std::vector<Double_t> Data_Prompt_Energy;
//Models
    TH1D *Hist_Total;
    const std::string Name_Hist_Total = "Model_Total", Title_Hist_Total = "Total Fit Specturm";
    Bool_t Is_Initialized_Hist;
    std::vector<TH1D*> Hists;
    std::vector<std::string> Names;
//Fitting Results
    Double_t Fitting_Variable; // LogL or chi-square; After completing fitting, it will record the last value ( i.e. the minuim value );
//Test 
    Bool_t Is_Test;
};

Fitter *Fitter::Point_Fitter_ = new Fitter();

Fitter *Fitter::Get_Global_Point()
{
    return Point_Fitter_;
};

Double_t Extended_Maximum_Likelihood(Double_t *parameters)
{
    Fitter *fitter = Fitter::Get_Global_Point();
    Double_t LogL = 0, bin_model, bin_data;
    if(fitter->Get_Is_Initialized_Data() == false)
    {
        std::cout << "[Fitter] Haven't Load Data. Quit!" << std::endl;
        exit(1);
    };
//Binned Data
    // LogL = -N_{expeaction} + \sum^{Number of Data} Log N_{i, Expection}(E_i)
    TH1D *temp_total = fitter->Get_Hist_Total();
    if( fitter->Get_Is_Binned_Data() == true)
    {
        TH1D *temp_data = fitter->Get_Hist_Data();
        for(int iBin = 1; iBin < temp_data->GetNbinsX() + 1; iBin++)
        {
            bin_model = temp_total->GetBinContent(iBin);
            bin_data = temp_data->GetBinContent(iBin);
            if(bin_model <= 0){continue;};
            LogL = LogL - bin_model + bin_data * log(bin_model);
        };
    }
    else
//Unbinned Data
    {
        LogL = LogL - temp_total->Integral();
        Int_t temp_index;
        std::vector<Double_t> data = fitter->Get_Data();
        for(int index = 0; index < data.size(); index++)
        {
            temp_index = temp_total->FindBin(data.at(index));
            bin_model = temp_total->GetBinContent(temp_index);
            LogL = LogL + log(bin_model);
        };
    };
//Penalty
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    for(int iPars = 0; iPars < Fit_Par->Get_Total_Number(); iPars++)
    {
        if( Fit_Par->Get_Constant(iPars) == false)
        {
            LogL = LogL - 0.5 * pow( parameters[iPars] - Fit_Par->Get_Prior_Value(iPars), 2) /pow( Fit_Par->Get_Prior_Error(iPars), 2);
        };
        // We want to find the minuim of -LogL, -LogL + Penalty can help minuit to find the best value around the prior values.
        // In this funciton, LogL should be LogL - Penalty
    };
    fitter->Record_Fitting_Variable( LogL );
//Test Code
    if(fitter->Get_Test_Index() == true)
    {
        std::cout << "[Fitter::Extended_Maximum_Likelihood] LogL:" << fitter->Get_Fitting_Variable() << std::endl;
    };
//Return
    return LogL;

};

Double_t Chi_Square(Double_t *parameters)
{
    Fitter *fitter = Fitter::Get_Global_Point();
    Double_t chi2 = 0, bin_model, bin_data, bin_error;
    if(fitter->Get_Is_Initialized_Data() == false)
    {
        std::cout << "[Fitter] Haven't Load Data. Quit!" << std::endl;
        exit(1);
    };
//Binned Data
    TH1D *temp_total = fitter->Get_Hist_Total();
    if(fitter->Get_Is_Binned_Data() == true)
    {
        TH1D *temp_data = fitter->Get_Hist_Data();
        for(int iBin = 1; iBin < temp_data->GetNbinsX() + 1; iBin++)
        {
            bin_model = temp_total->GetBinContent(iBin);
            bin_data = temp_data->GetBinContent(iBin);
            bin_error = temp_data->GetBinError(iBin);
            if(bin_error <= 0) { continue; };

            chi2 = chi2 + pow( (bin_data - bin_model), 2) / pow(bin_error, 2);
        }
    }
    else
//Unbinned Data
    {
        std::cout << "[Fitter::Chi_Square] There isn't chi-square fit for unbinned data. Quit!" << std::endl;
        exit(1);
    };
//Penalty
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    for(int iPars = 0; iPars < Fit_Par->Get_Total_Number(); iPars++)
    {
        if( Fit_Par->Get_Constant(iPars) == false)
        {
            chi2 = chi2 + pow( parameters[iPars] - Fit_Par->Get_Prior_Value(iPars), 2) / pow( Fit_Par->Get_Prior_Error(iPars), 2);
        };
    };
    fitter->Record_Fitting_Variable(chi2);
//Test Code
    if(fitter->Get_Test_Index() == true)
    {
        std::cout << "[Fitter::Chi_Square] Chi-Square:" << fitter->Get_Fitting_Variable() << std::endl;
    }
    return chi2;
};

void Fit_Function(Int_t &, Double_t *, Double_t &value, Double_t *parameters, Int_t)
{
    Fitter *fitter = Fitter::Get_Global_Point();
    if( fitter->Get_Is_Initialized_Hist() == false)
    {
        std::cout << "[Fitter] The Total Prediction Histogram isn't initizlied. Initializing it with Models." << std::endl;
    }
    else { fitter->Get_Hist_Total()->Reset("ICES"); }; // Reset Total Histogram
//Fit Parameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Int_t Total_Number = Fit_Par->Get_Total_Number();
//Record The Fitted Numbers of Events
    for(int Index = 0; Index < Total_Number; Index++)
    {
        Fit_Par->Set_Value(Index, parameters[Index]);
    };
//Compute Total Predictions
    AN *an = AN::Get_Global_Point();
    Reactor *reactor = Reactor::Get_Global_Point();
    Geonu *geonu = Geonu::Get_Global_Point();
    TH1D *temp = fitter->Get_Hist_Total();
    if( an->Is_Initialize() == false || reactor->Is_Initialize() == false || geonu->Is_Initialize() == false)
    {
        std::cout << "[Fitter::Fit_Function] There is at least one model that hasn't been initialized. Quit" << std::endl;
        exit(1);
    }
    //AN Setting
    if( an->Is_Initialize() == true)
    {
        if( fitter->Get_Is_Initialized_Hist() == false)
        {
            fitter->Initialize_Hist(an->Get_Total_Prediction());
            temp->Reset("ICES");
        };
        an->Compute_Predictions();
        temp->Add(an->Get_Total_Prediction());
    };
    //Reactor Setting
    if( reactor->Is_Initialize() == true)
    {
        if( fitter->Get_Is_Initialized_Hist() == false)
        {
            fitter->Initialize_Hist(reactor->Get_Total_Prediction());
            temp->Reset("ICES");
        };
        reactor->Compute_Predictions();
        temp->Add(reactor->Get_Total_Prediction());
    };
    //Geonu Setting
    if( geonu->Is_Initialize() == true)
    {
        if( fitter->Get_Is_Initialized_Hist() == false)
        {
            fitter->Initialize_Hist(geonu->Get_Total_Prediction());
            temp->Reset("ICES");
        };
        geonu->Compute_Predictions();
        temp->Add(geonu->Get_Total_Prediction());
    };

//Use Fitting Method
    if( fitter->Get_Fit_Method() == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
    {
        value = - Extended_Maximum_Likelihood( parameters );
    }
    else if( fitter->Get_Fit_Method() == FITTER_CHI_SQUARE)
    {
        value = Chi_Square( parameters );
    };
};

void Fitter::Initialize()
{
//Setup FitParameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Number_Fit_Parameters = Fit_Par->Get_Total_Number();
    if( Number_Fit_Parameters == 0)
    {
        std::cout << "[Fitter::Initialize] There is not fitted parameter. You need to set up wanted parameters." << std::endl;

    };
//Minuit
    Fitter *fitter = Fitter::Get_Global_Point();
    TVirtualFitter::SetDefaultFitter("Minuit");
    if(fitter->Get_Is_Initialized_Hist() == false)
    {
        minuit = TVirtualFitter::Fitter(0, Number_Fit_Parameters);
    }
    else
    {
        minuit->Clear();
        minuit = TVirtualFitter::Fitter(0, Number_Fit_Parameters);
    }
    //Set Print Level
    arg_list[0] = 0;
    minuit->ExecuteCommand("SET PRINT", arg_list, 2);//PRINT only receives the 0th element; 2 is the length of arglist
    //Set minimiser
    arg_list[0] = 5000;// Maxmium calls
    arg_list[1] = 0.01;// Torelance
    std::cout << "[Fitter::Initialize] Setup Print Level" << std::endl;
    //Set Function
    minuit->SetFCN( Fit_Function );
    std::cout << "[Fitter::Initialize] Setup Functions. Method:" << Fit_Method << std::endl;
    //Minuit uses Fit_Function in fitting and there is a standard format of this used function; This function must be static!!

    //Set Fit Parameters
    for(int Index = 0; Index < Number_Fit_Parameters; Index++)
    {
        minuit->SetParameter(Index, Fit_Par->Get_Name(Index).c_str(), Fit_Par->Get_Prior_Value(Index), Fit_Par->Get_Error(Index), 0, 0);//There are no limits for fitted parameters
    };
    std::cout << "[Fitter::Initialize] Setup Fitted Parameter. Total:" << Fit_Par->Get_Total_Number() << std::endl;
};

void Fitter::Initialize_Hist( TH1D* Example_Hist )
{
//Total Histogram
    Hist_Total = (TH1D*) Example_Hist->Clone(Name_Hist_Total.c_str());
    Hist_Total->Reset("ICES");
    Hist_Total->SetTitle(Title_Hist_Total.c_str());
    Is_Initialized_Hist = true;
//Data Histogram
    Hist_Data = (TH1D*) Example_Hist->Clone(Name_Hist_Data.c_str());
    Hist_Data->Reset("ICES");
    Hist_Data->SetTitle(Title_Hist_Data.c_str());
    Is_Initialized_Data = true;
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./Fitter_Initialize_Hist_Total.jpg", Hist_Total, "Total", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
        SavePlot("./Fitter_Initialize_Hist_Data.jpg", Hist_Data, "Data", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };
};

void Fitter::Load_Data( std::string Data_File, Bool_t Binned)
{
    if(Is_Initialized_Data == false)
    {
        std::cout << "[Fitter::Load_Data] Haven't Initialized Data Histogram. Quit!" << std::endl;
        exit(1);
    };
    Is_Binned = true;
//Histogram
    Double_t x_min = Hist_Data->GetXaxis()->GetXmin();
    Double_t x_max = Hist_Data->GetXaxis()->GetXmax();
//Read Data File
    TFile *file_data = new TFile(Data_File.c_str());
    TTree *tree_data = (TTree*) file_data->Get("output");
    Double_t PromptEnergy;
    tree_data->SetBranchAddress("PromptEnergy", &PromptEnergy);
    for(int ii1 = 0; ii1 < tree_data->GetEntries(); ii1++)
    {
        tree_data->GetEntry(ii1);
        if(PromptEnergy >= x_min && PromptEnergy <= x_max)
        {
            Hist_Data->Fill(PromptEnergy);
            Data_Prompt_Energy.push_back(PromptEnergy);
        };
    };
    Is_Binned = Binned;
    if(Is_Binned == true)
    {
        std::cout << "[Fitter::Load_Data] Binned Data" << std::endl;
    }
    else
    {
        std::cout << "[Fitter::Load_Data] Unbinned Data" << std::endl;
    }
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./Fitter_Load_Data.jpg", Hist_Data, "Data", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };

};

void Fitter::Set_Fitting_Method( std::string Method)
{
    if( Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
    {
        Fitter::Use_Extended_Maximum_Likelihood();
    }
    else if( Method == FITTER_CHI_SQUARE)
    {
        Fitter::Use_Chi_Square();
    }
    else
    {
        std::cout << "[Fitter::Set_Fitting_Method] Wrong Method. Quit!" << std::endl;
        exit(1);
    };
}

void Fitter::Fitting()
{
    int index_error_state = 1;
    unsigned int num_fitting_steps = 0;
//Details of Fitting
    while (index_error_state) {
        ++num_fitting_steps;
        index_error_state = minuit->ExecuteCommand("MIGRAD", arg_list, 2);
        //std::cout << num_fitting_steps << "," << minuit->GetParameter(0) << std::endl;
        if (num_fitting_steps >= 15) break;
    };
//Record 
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    std::string name;
    Double_t value, error, value_min, value_max;
    for(int Index = 0; Index < Fit_Par->Get_Total_Number(); Index++)
    {   
        std::string name_str = Fit_Par->Get_Name(Index);
        char name[100];  // Assuming the parameter names are not longer than 100 characters
        std::strcpy(name, name_str.c_str());

        minuit->GetParameter(Index, name, value, error, value_min, value_max);

        Fit_Par->Set_Value(Index, value);
        Fit_Par->Set_Error(Index, error);
        Fit_Par->Set_Value_Min(Index, value_min);
        Fit_Par->Set_Value_Max(Index, value_max);
    };
};

void Fitter::Get_All_Hists()
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    AN *an = AN::Get_Global_Point();
    Geonu *geonu = Geonu::Get_Global_Point();
    Reactor *reactor = Reactor::Get_Global_Point();
    TH1D* temp;
//Data
    if( Is_Initialized_Data == true)
    {
        temp = Hist_Data;
        Hists.push_back((TH1D*) temp->Clone(NAME_FITTER_DATA.c_str()));
        Names.push_back(NAME_FITTER_DATA);
    };
//AN
    if( an->Is_Initialize() == true)
    {
        temp = an->Get_Predictions();
        Hists.push_back((TH1D*) temp->Clone(NAME_FITTER_AN.c_str()));
        Names.push_back(NAME_FITTER_AN);
    };
//Geonu
    if( geonu->Is_Initialize() == true)
    {
        temp = geonu->Get_Predictions();
        Hists.push_back((TH1D*) temp->Clone(NAME_FITTER_GEO.c_str()));
        Names.push_back(NAME_FITTER_GEO);
    };
//Reactor
    if( reactor->Is_Initialize() == true)
    {
        temp = reactor->Get_Predictions();
        Hists.push_back((TH1D*) temp->Clone(NAME_FITTER_REACTOR.c_str()));
        Names.push_back(NAME_FITTER_REACTOR);
    };
//Test Code
    if( Is_Test == true)
    {
        for(int Index = 0; Index < Hists.size(); Index++)
        {
            SavePlot(("./" + Names.at(Index) + "_Prediction.jpg").c_str(), Hists.at(Index), Names.at(Index).c_str(), "E_{Prompt}/MeV",  Names.at(Index).c_str(), 0, 0, 0); //For Testing
        };
       
    };
    
};

void Fitter::Draw_All()
{
//Get Models and Data;
    if(Hists.size() == 0){ Get_All_Hists();};
    Hists.push_back(Hist_Total);
    Names.push_back("Total");
//Setup about Draw
    TCanvas *c1 = new TCanvas("c1", "All Histograms", 800, 600);
    TLegend *legend = new TLegend(0.7, 0.6, 0.9, 0.8);
    
    for(int Index = 0; Index < Hists.size(); Index++)
    {
        if( Names.at(Index) == NAME_FITTER_DATA)
        {
            Hists.at(Index)->Draw("E1");
            Hists.at(Index)->SetMarkerStyle(8);
            Hists.at(Index)->SetLineColor(kBlack);
            legend->AddEntry(Hists.at(Index), Names.at(Index).c_str(), "p");
        }
        else
        {
            Hists.at(Index)->Draw("HIST SAME");
            Hists.at(Index)->SetLineColor(Index);
            legend->AddEntry(Hists.at(Index), Names.at(Index).c_str(), "l");
        };
        Hists.at(Index)->SetXTitle("E_{Prompt}/MeV");
        Double_t Bin_Width = Hists.at(Index)->GetBinWidth(1);
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(3) << Bin_Width;
        std::string Bin_Width_Annotation = "X Width:" + ss.str();
        Hists.at(Index)->SetYTitle(Bin_Width_Annotation.c_str());
    };

    legend->Draw();
    c1->Print("./Draw_All.jpg");
};

void Fitter::Show_Results()
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    //Reactor
    Double_t Number_Reactor, Error_Reactor;
    Number_Reactor = Fit_Par->Get_Value(NAME_REACTOR);
    Error_Reactor = Fit_Par->Get_Error(NAME_REACTOR);
    std::cout << "[Fitter::Show_Results] Reactor:" << Number_Reactor << ", Error:" << Error_Reactor << std::endl;
//Geo
    Int_t Index_Geo = Fit_Par->Get_Index(NAME_GEO);
    Int_t Index_Geo_Ratio = Fit_Par->Get_Index(NAME_GEO_RATIO);
    Double_t Number_U, Number_Th, Number_Geo, Geo_Ratio;
    Double_t Error_U, Error_Th, Error_Geo, Error_Geo_Ratio;
    //Geo
    Number_Geo = Fit_Par->Get_Value(Index_Geo);
    Error_Geo = Fit_Par->Get_Error(Index_Geo);
    //Geo Ratio
    Geo_Ratio = Fit_Par->Get_Value(Index_Geo_Ratio);
    Error_Geo_Ratio = Fit_Par->Get_Error(Index_Geo_Ratio);
    //Covariance
    Double_t Sigma_Geo_Ratio = Fitter::Get_Fit_Covariance_Matrix_Element(Index_Geo, Index_Geo_Ratio);
    //U
    Number_U = Number_Geo * Geo_Ratio/(Geo_Ratio + 1);
    Double_t Part1 = pow(Number_Geo * Error_Geo_Ratio, 2) /pow((Geo_Ratio + 1), 4);
    Double_t Part2 = pow(Geo_Ratio * Error_Geo, 2) / pow(Geo_Ratio + 1, 2);
    Double_t Part3 = 2 * Geo_Ratio * Number_Geo*  Sigma_Geo_Ratio/ pow(Geo_Ratio + 1, 3);
    Error_U = sqrt(Part1 + Part2 + Part3);
    //Th
    Number_Th = Number_Geo * 1/(Geo_Ratio + 1);
    Part1 = pow(Number_Geo * Error_Geo_Ratio, 2) /pow((Geo_Ratio + 1), 4);
    Part2 = pow(Error_Geo, 2) / pow(Geo_Ratio + 1, 2);
    Part3 = -2 * Number_Geo * Sigma_Geo_Ratio/ pow(Geo_Ratio + 1, 3);
    Error_Th = sqrt(Part1 + Part2 + Part3);
    

    std::cout << "[Fitter::Show_Results] Geo:" << Number_Geo << ", Error:" << Error_Geo << std::endl;
    std::cout << "[Fitter::Show_Results] Geo Ratio:" << Geo_Ratio << ", Error:" << Error_Geo_Ratio << std::endl;
    std::cout << "[Fitter::Show_Results] Geo U:" << Number_U << ", Error:" << Error_U << std::endl;
    std::cout << "[Fitter::Show_Results] Geo Th:" << Number_Th << ", Error:" << Error_Th << std::endl;
//AN
    Int_t Index_Ground = Fit_Par->Get_Index(NAME_AN_GROUND);
    Int_t Index_Exicted = Fit_Par->Get_Index(NAME_AN_EXICTED);
    Double_t Number_Ground, Number_Exicted, Number_AN;
    Double_t Error_Ground, Error_Exicted, Error_AN;
    Number_Ground = Fit_Par->Get_Value(Index_Ground);
    Error_Ground = Fit_Par->Get_Error(Index_Ground);
    Number_Exicted = Fit_Par->Get_Value(Index_Exicted);
    Error_Exicted = Fit_Par->Get_Error(Index_Exicted);
    Double_t Sigma_Ground_Exicted = Fitter::Get_Fit_Covariance_Matrix_Element(Index_Ground, Index_Exicted);
    Number_AN = Number_Ground + Number_Exicted;
    Error_AN = sqrt( pow(Error_Ground, 2) + pow(Error_Exicted, 2) + 2 * Sigma_Ground_Exicted);
    std::cout << "[Fitter::Show_Results] AN:" << Number_AN << ", Error:" << Error_AN << std::endl;
    std::cout << "[Fitter::Show_Results] AN Ground:" << Number_Ground << ", Error:" << Error_Ground << std::endl;
    std::cout << "[Fitter::Show_Results] AN Exicted:" << Number_Exicted << ", Error:" << Error_Exicted << std::endl;
//Total
    Double_t Total = 0, Error_Total;
    for(int Index = 0; Index < Fit_Par->Get_Total_Number(); Index++)
    {
        Total = Total + Fit_Par->Get_Value(Index);
    };
    for(int ii1 = 0; ii1 < Fit_Par->Get_Total_Number(); ii1++)
    {
        if(Fit_Par->Get_Name(ii1) == NAME_GEO_RATIO){continue;};
        for(int ii2 = 0; ii2 < Fit_Par->Get_Total_Number(); ii2++)
        {
            if(Fit_Par->Get_Name(ii1) == NAME_GEO_RATIO){continue;};
            Error_Total = Error_Total + Fitter::Get_Fit_Covariance_Matrix_Element(ii1, ii2);
        };
    };
    Error_Total = sqrt(Error_Total);
    std::cout << "[Fitter::Show_Results] Total:" << Total << ", Error:" << Error_Total << std::endl;
    std::cout << "[Fitter::Show_Results] Data Observed:" << Hist_Data->Integral() << std::endl;
//Geo Rate
    Double_t Fitted_Geo_Rate = Number_Geo/Fit_Par->Get_Effective_Duration();
    Double_t Error_Fitted_Geo_Rate = Error_Geo/Fit_Par->Get_Effective_Duration();
    Double_t Fitted_Geo_TNU = Fitted_Geo_Rate * Rate_TNU;
    Double_t Error_Fitter_Geo_TNU = Error_Fitted_Geo_Rate * Rate_TNU;
    std::cout << "[Fitter::Show_Results] Effective Duration:" << Fit_Par->Get_Effective_Duration() << " year or " << Fit_Par->Get_Effective_Duration() * 365 << " days" << std::endl;
    std::cout << "[Fitter::Show_Results] Fitted Geo Rate:" << Fitted_Geo_Rate << "+-" << Error_Fitted_Geo_Rate << " events per year, Corresponding to " << Fitted_Geo_TNU << "+-" << Error_Fitter_Geo_TNU << " TNU" << std::endl;
}

#endif