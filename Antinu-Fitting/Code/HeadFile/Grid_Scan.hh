#ifndef GRID_SCAN_HH
#define GRID_SCAN_HH
//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"
#include "./Fitter.hh"
#include "./Fit_Parameters.hh"

void Scan_U_Th_Ratio(Double_t Ratio_Min, Double_t Ratio_Max, Int_t Steps)
{
//Example Histogram
    TH1D *Example_Hist = new TH1D("Example", "", 20, 0.8, 8.0);
//Models
    Geonu *geonu = Geonu::Get_Global_Point();
    Reactor *reactor = Reactor::Get_Global_Point();
    AN *an = AN::Get_Global_Point();

    geonu->Setup_PDFs(Example_Hist);
    reactor->Setup_PDFs(Example_Hist);
    an->Setup_PDFs(Example_Hist);
//Fit Parameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Fit_Par->Compute_Events(145.17, 300733, 309277);
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();

//Scan Setting
    Double_t interval = (Ratio_Max - Ratio_Min) / Steps;
    Double_t ratio;
    Double_t res;
    std::vector<Double_t> LogL;
    std::vector<std::string> Names;
    std::vector<Double_t> Values, Errors;
    Names.push_back(NAME_GEO_RATIO);
    Values.push_back(0);
    for(int ii1 = 0; ii1 < Steps; ii1++)
    {
        ratio = Ratio_Min + interval * ii1;
        Values.at(0) = ratio;
        Fit_Par->Compute_Events(145.17, 300733, 309277, Names, Values);
        fitter->Initialize();
        fitter->Initialize_Hist(Example_Hist);
        fitter->Load_Data(DATA_IBD_CANDIDATE);
        fitter->Fitting();
        LogL.push_back(- fitter->Get_Fitting_Variable());
    };
//Find Maxinum and Mininum Value
    Int_t Min_Index = 0;
    Double_t Best_ratio = 0;
    Double_t Max_LogL = LogL.at(0);
    Double_t Min_LogL = LogL.at(0);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        if( Min_LogL > LogL.at(ii1))
        {
            Min_LogL = LogL.at(ii1);
            Min_Index = ii1;
        };
        if(Max_LogL < LogL.at(ii1))
        {
            Max_LogL = LogL.at(ii1);
        }
    };
//Compute \Delta LogL
    TH2D *Hist = new TH2D("U/Th Ratio", "", Steps, Ratio_Min, Ratio_Max, 100, 0, Max_LogL - Min_LogL);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        ratio = Ratio_Min + interval * ii1;
        if(Min_Index == ii1) { Best_ratio = ratio; };
        Hist->Fill(ratio, LogL.at(ii1) - Min_LogL);
    };
    Hist->Draw();
    std::cout << "Complete! Best:" << Best_ratio << std::endl;
};

void Scan_Reactor(Double_t Num_Min, Double_t Num_Max, Int_t Steps)
{
//Example Histogram
    TH1D *Example_Hist = new TH1D("Example", "", 20, 0.8, 8.0);
//Models
    Geonu *geonu = Geonu::Get_Global_Point();
    Reactor *reactor = Reactor::Get_Global_Point();
    AN *an = AN::Get_Global_Point();

    geonu->Setup_PDFs(Example_Hist);
    reactor->Setup_PDFs(Example_Hist);
    an->Setup_PDFs(Example_Hist);
//Fit Parameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Fit_Par->Compute_Events(145.17, 300733, 309277);
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();

//Scan Setting
    Double_t interval = (Num_Max - Num_Min) / Steps;
    Double_t number;
    Double_t res;
    std::vector<Double_t> LogL;
    std::vector<std::string> Names;
    std::vector<Double_t> Values, Errors;
    Names.push_back(NAME_REACTOR);
    Values.push_back(0);
    for(int ii1 = 0; ii1 < Steps; ii1++)
    {
        number = Num_Min + interval * ii1;
        Values.at(0) = number;
        Fit_Par->Compute_Events(145.17, 300733, 309277, Names, Values);
        fitter->Initialize();
        fitter->Initialize_Hist(Example_Hist);
        fitter->Load_Data(DATA_IBD_CANDIDATE);
        fitter->Fitting();
        LogL.push_back( - fitter->Get_Fitting_Variable());
    };
//Find Maxnium and Minium Value
    Int_t Min_Index = 0;
    Double_t Best_Number = 0;
    Double_t Min_LogL = LogL.at(0);
    Double_t Max_LogL = LogL.at(0);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        if( Min_LogL > LogL.at(ii1))
        {
            Min_LogL = LogL.at(ii1);
            Min_Index = ii1;
        };
        if(Max_LogL < LogL.at(ii1))
        {
            Max_LogL = LogL.at(ii1);
        }
    };
//Compute \Delta LogL
    TH2D *Hist = new TH2D("Reactor Number", "", Steps, Num_Min, Num_Max, 100, 0, Max_LogL - Min_LogL);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        number = Num_Min + interval * ii1;
        if(Min_Index == ii1) { Best_Number = number; };
        Hist->Fill(number, LogL.at(ii1) - Min_LogL);
    };
    
    Hist->Draw();
    std::cout << "Complete! Best Fit:" << Best_Number << std::endl;
};

void Scan_Geo(Double_t Num_Min, Double_t Num_Max, Int_t Steps)
{
//Example Histogram
    TH1D *Example_Hist = new TH1D("Example", "", 20, 0.8, 8.0);
//Models
    Geonu *geonu = Geonu::Get_Global_Point();
    Reactor *reactor = Reactor::Get_Global_Point();
    AN *an = AN::Get_Global_Point();

    geonu->Setup_PDFs(Example_Hist);
    reactor->Setup_PDFs(Example_Hist);
    an->Setup_PDFs(Example_Hist);
//Fit Parameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Fit_Par->Compute_Events(145.17, 300733, 309277);
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();

//Scan Setting
    Double_t interval = (Num_Max - Num_Min) / Steps;
    Double_t number;
    Double_t res;
    std::vector<Double_t> LogL;
    std::vector<std::string> Names;
    std::vector<Double_t> Values, Errors;
    Names.push_back(NAME_GEO);
    Values.push_back(0);
    for(int ii1 = 0; ii1 < Steps; ii1++)
    {
        number = Num_Min + interval * ii1;
        Values.at(0) = number;
        Fit_Par->Compute_Events(145.17, 300733, 309277, Names, Values);
        fitter->Initialize();
        fitter->Initialize_Hist(Example_Hist);
        fitter->Load_Data(DATA_IBD_CANDIDATE);
        fitter->Fitting();
        LogL.push_back( - fitter->Get_Fitting_Variable());
    };
//Find Maxnium and Minium Value
    Int_t Min_Index = 0;
    Double_t Best_Number = 0;
    Double_t Min_LogL = LogL.at(0);
    Double_t Max_LogL = LogL.at(0);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        if( Min_LogL > LogL.at(ii1))
        {
            Min_LogL = LogL.at(ii1);
            Min_Index = ii1;
        };
        if(Max_LogL < LogL.at(ii1))
        {
            Max_LogL = LogL.at(ii1);
        }
    };
//Compute \Delta LogL
    TH2D *Hist = new TH2D("Geo Number", "", Steps, Num_Min, Num_Max, 100, 0, Max_LogL - Min_LogL);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        number = Num_Min + interval * ii1;
        if(Min_Index == ii1) { Best_Number = number; };
        Hist->Fill(number, LogL.at(ii1) - Min_LogL);
    };
    
    Hist->Draw();
    std::cout << "Complete! Best Fit:" << Best_Number << std::endl;
};

#endif