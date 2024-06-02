#ifndef REACTOR_HH
#define REACTOR_HH
//CPP
#include <iostream>
#include <vector>
#include <cmath>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "../Constant_Setting.hh"
#include "../Plot_Code.hh"
#include "../Fit_Parameters.hh"

class Reactor
{
public:
    //Constructor and Destructor
    Reactor(): Is_Initialized(false), Is_Test(false) {};
    //Global Point
    static Reactor *Point_Reactor_;
    //Member Functions
    static Reactor *Get_Global_Point();
    void Initialize();
    bool Is_Initialize() {return Is_Initialized;};
    void Get_Default_PDFs();
    void Setup_PDFs(TH1D *Example_Hist);
    void Compute_Predictions(std::vector<TH1D*> &Hists);
    void Open_Test_Mode() { Is_Test = true; };
    void Close_Test_Mode() { Is_Test = false; };
private:
    Bool_t Is_Initialized;
    TH1D * Hist_Reactor;
    std::string Name_Reactor = "Hist_Reactor";
    Bool_t Is_Test;
};

Reactor *Reactor::Point_Reactor_ = new Reactor();

Reactor *Reactor::Get_Global_Point()
{
    if(Point_Reactor_->Is_Initialized == false)
    {
        std::cout << "[Reactor] Haven't been Initialized. Can not use it." << std::endl;
    };
    return Point_Reactor_;
};

void Reactor::Initialize()
{
    Is_Initialized = true;
//Histogram
    Get_Default_PDFs();
};

void Reactor::Get_Default_PDFs()
{
//Setup Histogram
    TFile pdf_file_reactor(PDF_REACTOR.c_str());
    Hist_Reactor = (TH1D*) pdf_file_reactor.Get("Reactor_Prompt_Energy");
    Hist_Reactor->Scale(1.0/Hist_Reactor->Integral());
    Hist_Reactor->SetName(Name_Reactor.c_str());
//Show Details
    std::cout << "[Reactor::Get_Default_PDFs] Load Default Reactor PDF. Min:" << Hist_Reactor->GetXaxis()->GetXmin() << ", Max:" << Hist_Reactor->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_Reactor->GetXaxis()->GetNbins() << ", Width:" << Hist_Reactor->GetXaxis()->GetBinWidth(1) << std::endl;
//Test Code
    if(Is_Test == true)
    {
        SavePlot("./Reactor_Default_PDF.jpg", Hist_Reactor, "Reactor", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    }
};

void Reactor::Setup_PDFs(TH1D* Example_Hist)
{
    if(Is_Initialized == false)
    {
        std::cout << "[Reactor] Haven't Load Default PDF. But Load PDFs according Given Histogram." << std::endl;
        Is_Initialized = true;
    };
//Setup Histogram
    Double_t x_min = Example_Hist->GetXaxis()->GetXmin();
    Double_t x_max = Example_Hist->GetXaxis()->GetXmax();
    std::string Selection = Form("PromptEnergy >= %f && PromptEnergy <= %f", x_min, x_max);
//Setup Reactor
    TFile pdf_data_file_reactor(PDF_DATA_REACTOR.c_str());
    TTree *tree_reactor = (TTree*) pdf_data_file_reactor.Get("output");
    Hist_Reactor = (TH1D*) (Example_Hist->Clone(Name_Reactor.c_str()));
    Hist_Reactor->Reset("ICES");

    tree_reactor->Project(Name_Reactor.c_str(), "PromptEnergy", Selection.c_str());
    Hist_Reactor->Scale(1.0/Hist_Reactor->Integral());
//Show Details
    std::cout << "[Reactor::Setup_PDFs] Sucessfully Setup Reactor PDF. Min:" << Hist_Reactor->GetXaxis()->GetXmin() << ", Max:" << Hist_Reactor->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_Reactor->GetXaxis()->GetNbins() << ", Width:" << Hist_Reactor->GetXaxis()->GetBinWidth(1) << std::endl;
//Test Code
    if(Is_Test == true)
    {
        SavePlot("./Reactor_Setup_PDF.jpg", Hist_Reactor, "Reactor", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };
};

void Reactor::Compute_Predictions(std::vector<TH1D*> &Hists)
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Double_t Number_Reactor = Fit_Par->Get_Value(NAME_REACTOR);
//Reactor
    TH1D* temp_hist = (TH1D*) Hist_Reactor->Clone(NAME_REACTOR.c_str());
    temp_hist->Scale(Number_Reactor);
    Hists.push_back(temp_hist);
};

#endif 