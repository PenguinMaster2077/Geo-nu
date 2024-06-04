#ifndef FITTER_HH
#define FITTER_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVirtualFitter.h>
//RAT
//Self-Defined
#include "./Fit_Parameters.hh"
#include "./Models.hh"
#include "./Fit_Parameters.hh"
#include "./Constant_Setting.hh"

class Fitter
{
public:
    //Constructor and destructor
    Fitter(): Fit_Method(FITTER_MAXIMUM_LIKELIHOOD), Is_Initialized_Data(false), Is_Initialized_Hist(false)
    {
        Initialize();
    };
    //Global Point
    static Fitter *Point_Fitter_;
    //Member Functions
    static Fitter *Get_Global_Point();
    void Initialize();
    void Initialize_Hist( TH1D* Example_Hist );
    void Initialize_Data( TH1D *Example_Hist );
    static void Fit_Function(Int_t &, Double_t *, Double_t &value, Double_t *parameters, Int_t);//args must be like these
    static Double_t Maximum_Likelihood(Double_t *parameters);
    static Double_t Chi_Square(Double_t *parameters);
    void Use_Maximum_Likelihood() { Fit_Method = FITTER_MAXIMUM_LIKELIHOOD; };
    void Use_Chi_Square() { Fit_Method = FITTER_CHI_SQUARE; };
private:
    //Minuit
    static TVirtualFitter *minuit;
    static double arg_list[2];
    Int_t Number_Fit_Parameters;//Total Number of Fitted Parameters
    std::string Fit_Method;
    //Data
    static TH1D *Data_Hist;
    const std::string Title_Data_Hist = "Data Histogram";
    Bool_t Is_Initialized_Data, Is_Binned;
    //Models
    static TH1D *Total_Hist;
    const std::string Name_Total_Hist = "Model_Total", Title_Total_Hist = "Total Fit Specturm";
    Bool_t Is_Initialized_Hist;
};

Fitter *Fitter::Point_Fitter_ = new Fitter();

Fitter *Fitter::Get_Global_Point()
{
    return Point_Fitter_;
};


void Fitter::Initialize()
{
//Setup FitParameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Number_Fit_Parameters = Fit_Par->Get_Total_Number();
    if( Number_Fit_Parameters == 0)
    {
        std::cout << "[Fitter::Initialize] There is not fitted parameter. You need to set up wanted parameters. Quit!!" << std::endl;
        exit(1);
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
    //Set Function
    minuit->SetFCN( Fit_Function );
    //Minuit uses Fit_Function in fitting and there is a standard format of this used function; This function must be static!!

    //Set Fit Parameters
    for(int Index = 0; Index < Number_Fit_Parameters; Index++)
    {
        minuit->SetParameter(Index, Fit_Par->Get_Name(Index).c_str(), Fit_Par->Get_Prior_Value(Index), Fit_Par->Get_Error(Index), 0, 0);//There are no limits for fitted parameters
    };
};

void Fitter::Fit_Function(Int_t &, Double_t *, Double_t &value, Double_t *parameters, Int_t)
{
    Fitter *fitter = Fitter::Get_Global_Point();
    if( fitter->Is_Initialized_Hist == false)
    {
        std::cout << "[Fitter] The Total Prediction Histogram isn't initizlied. Initializing it with Models." << std::endl;
    }
    else { Total_Hist->Reset("ICES"); };
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
    if( an->Is_Initialize() == false || reactor->Is_Initialize() == false || geonu->Is_Initialize() == false)
    {
        std::cout << "[Fitter] There is at least one model that hasn't been initialized. Quit" << std::endl;
        exit(1);
    }
    //AN Setting
    if( an->Is_Initialize() == true)
    {
        if( fitter->Is_Initialized_Hist == false)
        {
            fitter->Initialize_Hist(an->Get_Total_Prediction());
            Total_Hist->Reset("ICES");
        };
        an->Compute_Predictions();
        Total_Hist->Add(an->Get_Total_Prediction());
    };
    //Reactor Setting
    if( reactor->Is_Initialize() == true)
    {
        if( fitter->Is_Initialized_Hist == false)
        {
            fitter->Initialize_Hist(reactor->Get_Total_Prediction());
            Total_Hist->Reset("ICES");
        };
        reactor->Compute_Predictions();
        Total_Hist->Add(reactor->Get_Total_Prediction());
    };
    //Geonu Setting
    if( geonu->Is_Initialize() == true)
    {
        if( fitter->Is_Initialized_Hist == false)
        {
            fitter->Initialize_Hist(geonu->Get_Total_Prediction());
            Total_Hist->Reset("ICES");
        };
        geonu->Compute_Predictions();
        Total_Hist->Add(geonu->Get_Total_Prediction());
    };

//Use Fitting Method
    if( fitter->Fit_Method == FITTER_MAXIMUM_LIKELIHOOD)
    {
        value = -2 * Maximum_Likelihood( parameters );
    }
    else if( fitter->Fit_Method == FITTER_CHI_SQUARE)
    {
        value = Chi_Square( parameters );
    };
};

Double_t Fitter::Maximum_Likelihood(Double_t *parameters)
{
    Fitter *fitter = Fitter::Get_Global_Point();
    Double_t LogL = 0, bin_model, bin_data;
    if(fitter->Is_Initialized_Data == false)
    {
        std::cout << "[Fitter] Haven't Load Data. Quit." << std::endl;
        exit(1);
    };
//Binned Data
    if( fitter->Is_Binned == true)
    {
        for(int iBin = 1; iBin < fitter->Data_Hist->GetNbinsX() + 1; iBin++)
        {
            bin_model = fitter->Total_Hist->GetBinContent(iBin);
            bin_data = fitter->Data_Hist->GetBinContent(iBin);
            if(bin_model <= 0){continue;};
            LogL = LogL - bin_model + bin_data * log(bin_model);
        };
    };
//Penalty
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    for(int iPars = 0; iPars < Fit_Par->Get_Total_Number(); iPars++)
    {
        LogL = LogL - 0.5 * pow( parameters[iPars] - Fit_Par->Get_Prior_Value(iPars), 2) /pow( Fit_Par->Get_Error(iPars), 2);
    };
//Return
    return LogL;
};

Double_t Fitter::Chi_Square(Double_t *parameters)
{

};

void Fitter::Initialize_Hist( TH1D* Example_Hist )
{
    Total_Hist = (TH1D*) Example_Hist->Clone(Name_Total_Hist.c_str());
    Total_Hist->Reset("ICES");
    Total_Hist->SetTitle(Title_Total_Hist.c_str());
    Is_Initialized_Hist = true;
};

void Fitter::Initialize_Data( TH1D *Example_Hist )
{
    Data_Hist = (TH1D*) Example_Hist->Clone(FITTER_DATA_HIST.c_str());
    Data_Hist->Reset("ICES");
    Data_Hist->SetTitle(Title_Data_Hist.c_str());
    Is_Initialized_Data = true;
    Is_Binned = false;
};

#endif