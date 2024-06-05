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
#include "./Constant_Setting.hh"

class Fitter
{
public:
    //Constructor and destructor
    Fitter(): Fit_Method(FITTER_MAXIMUM_LIKELIHOOD), Is_Initialized_Data(false), Is_Binned(false), Is_Initialized_Hist(false), Is_Test(false)
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
    void Load_Data( std::string Data_File );
    TVirtualFitter *Get_Minuit() { return minuit; };
    TH1D *Get_Hist_Total() { return Hist_Total; };
    TH1D *Get_Hist_Data() { return Hist_Data; };
    Bool_t &Get_Is_Initialized_Data() { return Is_Initialized_Data; };
    Bool_t &Get_Is_Binned_Data() { return Is_Binned; };
    Bool_t &Get_Is_Initialized_Hist() { return Is_Initialized_Hist; };
    std::string &Get_Fit_Method() { return Fit_Method; };
    void Use_Maximum_Likelihood() { Fit_Method = FITTER_MAXIMUM_LIKELIHOOD; };
    void Use_Chi_Square() { Fit_Method = FITTER_CHI_SQUARE; };
    void Open_Test_Mode() { Is_Test = true; };
    void Close_Test_Mode() { Is_Test = false; };
    void Fitting();
    void Get_All_Hists();
    void Draw_All();
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
    //Models
    TH1D *Hist_Total;
    const std::string Name_Hist_Total = "Model_Total", Title_Hist_Total = "Total Fit Specturm";
    Bool_t Is_Initialized_Hist;
    std::vector<TH1D*> Hists;
    std::vector<std::string> Names;
    //Test 
    Bool_t Is_Test;
};

Fitter *Fitter::Point_Fitter_ = new Fitter();

Fitter *Fitter::Get_Global_Point()
{
    return Point_Fitter_;
};

Double_t Maximum_Likelihood(Double_t *parameters)
{
    Fitter *fitter = Fitter::Get_Global_Point();
    Double_t LogL = 0, bin_model, bin_data;
    if(fitter->Get_Is_Initialized_Data() == false)
    {
        std::cout << "[Fitter] Haven't Load Data. Quit." << std::endl;
        exit(1);
    };
//Binned Data
    if( fitter->Get_Is_Binned_Data() == true)
    {
        TH1D *temp_data = fitter->Get_Hist_Data();
        TH1D *temp_total = fitter->Get_Hist_Total();
        for(int iBin = 1; iBin < temp_data->GetNbinsX() + 1; iBin++)
        {
            bin_model = temp_total->GetBinContent(iBin);
            bin_data = temp_data->GetBinContent(iBin);
            if(bin_model <= 0){continue;};
            LogL = LogL - bin_model + bin_data * log(bin_model);
        };
    };
//Penalty
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    for(int iPars = 0; iPars < Fit_Par->Get_Total_Number(); iPars++)
    {
        LogL = LogL - 0.5 * pow( parameters[iPars] - Fit_Par->Get_Prior_Value(iPars), 2) /pow( Fit_Par->Get_Prior_Error(iPars), 2);
    };
//Return
    return LogL;
};

Double_t Chi_Square(Double_t *parameters)
{
    return 0;
};

void Fit_Function(Int_t &, Double_t *, Double_t &value, Double_t *parameters, Int_t)
{
    Fitter *fitter = Fitter::Get_Global_Point();
    if( fitter->Get_Is_Initialized_Hist() == false)
    {
        std::cout << "[Fitter] The Total Prediction Histogram isn't initizlied. Initializing it with Models." << std::endl;
    }
    else { fitter->Get_Hist_Total()->Reset("ICES"); };
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
        std::cout << "[Fitter] There is at least one model that hasn't been initialized. Quit" << std::endl;
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
    if( fitter->Get_Fit_Method() == FITTER_MAXIMUM_LIKELIHOOD)
    {
        value = -2 * Maximum_Likelihood( parameters );
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
    TVirtualFitter::SetDefaultFitter("Minuit");
    minuit = TVirtualFitter::Fitter(0, Number_Fit_Parameters);
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
    Hist_Total->SetTitle(Title_Hist_Data.c_str());
    Is_Initialized_Data = true;
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./Fitter_Initialize_Hist_Total.jpg", Hist_Total, "Total", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
        SavePlot("./Fitter_Initialize_Hist_Data.jpg", Hist_Data, "Data", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };
};

void Fitter::Load_Data( std::string Data_File )
{
    if(Is_Initialized_Data == false)
    {
        std::cout << "[Fitter::Load_Data] Haven't Initialized Data Histogram. Quit." << std::endl;
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
        };
    };
    Is_Binned = true;
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./Fitter_Load_Data.jpg", Hist_Data, "Data", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };

};

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
    std::cout << "Data Complete" << std::endl;
//AN
    if( an->Is_Initialize() == true)
    {
        temp = an->Get_Predictions();
        Hists.push_back((TH1D*) temp->Clone(NAME_FITTER_AN.c_str()));
        Names.push_back(NAME_FITTER_AN);
    };
    std::cout << "AN Complete" << std::endl;
//Geonu
    if( geonu->Is_Initialize() == true)
    {
        temp = geonu->Get_Predictions();
        Hists.push_back((TH1D*) temp->Clone(NAME_FITTER_GEO.c_str()));
        Names.push_back(NAME_FITTER_GEO);
    };
    std::cout << "Geonu Complete" << std::endl;
//Reactor
    if( reactor->Is_Initialize() == true)
    {
        temp = reactor->Get_Predictions();
        Hists.push_back((TH1D*) temp->Clone(NAME_FITTER_REACTOR.c_str()));
        Names.push_back(NAME_REACTOR);
    };
    std::cout << "Reactor Complete" << std::endl;
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
    Names.push_back("All");
//Setup about Draw
    TCanvas *c1 = new TCanvas("c1", "All Histograms", 800, 600);
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    
    for(int Index = 0; Index < Hists.size(); Index++)
    {
        if( Names.at(Index) == NAME_FITTER_DATA)
        {
            Hists.at(Index)->Draw("E1");
            Hists.at(Index)->SetMarkerStyle(8);
            Hists.at(Index)->SetLineColor(kBlack);
        }
        else
        {
            Hists.at(Index)->Draw("HIST SAME");
            Hists.at(Index)->SetLineColor(Index);
        };
        legend->AddEntry(Hists.at(Index), Names.at(Index).c_str(), "l");
    };

    legend->Draw();
    c1->Print("./Draw_All.jpg");
}

#endif