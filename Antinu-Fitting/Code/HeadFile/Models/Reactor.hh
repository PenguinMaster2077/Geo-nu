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
    Reactor(): Is_Initialized(false), Is_Test(false), Is_Load_PDF(false) {};
    //Global Point
    static Reactor *Point_Reactor_;
    //Member Functions
    static Reactor *Get_Global_Point();
    bool Is_Initialize() { return Is_Initialized; };
    void Setup_PDFs(TH1D *Example_Hist);
    void Compute_Predictions();
    TH1D *Get_Total_Prediction() {return Hist_Prediction; };
    void Open_Test_Mode() { Is_Test = true; };
    void Close_Test_Mode() { Is_Test = false; };
private:
    Bool_t Is_Initialized, Is_Load_PDF;
    TH1D *Hist_Reactor, *Hist_Prediction;
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

void Reactor::Setup_PDFs(TH1D* Example_Hist)
{
    Is_Initialized = true;
    Is_Load_PDF = true;
//Setup Histogram
   Double_t x_min = Example_Hist->GetXaxis()->GetXmin();
    Double_t x_max = Example_Hist->GetXaxis()->GetXmax();
    Int_t NBinx = Example_Hist->GetNbinsX();
    std::string Selection = Form("PromptEnergy >= %f && PromptEnergy <= %f", x_min, x_max);
//Setup Reactor
    Hist_Reactor = (TH1D*) (Example_Hist->Clone(Name_Reactor.c_str()));
    Hist_Reactor->Reset("ICES");
    TFile pdf_data_file_reactor(PDF_DATA_REACTOR.c_str());
    TTree *tree_reactor = (TTree*) pdf_data_file_reactor.Get("output");
    Double_t PromptEnergy;
    tree_reactor->SetBranchAddress("PromptEnergy", &PromptEnergy);
    for(int ii1 = 0; ii1 < tree_reactor->GetEntries(); ii1++)
    {
        tree_reactor->GetEntry(ii1);
        if(PromptEnergy < x_min || PromptEnergy > x_max){continue;};
        Hist_Reactor->Fill(PromptEnergy);
    };
    pdf_data_file_reactor.Close();

    Hist_Reactor->Scale(1.0/Hist_Reactor->Integral());
//Show Details
    std::cout << "[Reactor::Setup_PDFs] Sucessfully Setup Reactor PDF. Min:" << Hist_Reactor->GetXaxis()->GetXmin() << ", Max:" << Hist_Reactor->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_Reactor->GetXaxis()->GetNbins() << ", Width:" << Hist_Reactor->GetXaxis()->GetBinWidth(1) << std::endl;
//Test Code
    if(Is_Test == true)
    {
        SavePlot("./Reactor_Setup_PDF.jpg", Hist_Reactor, "Reactor", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };
};

void Reactor::Compute_Predictions()
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Double_t Number_Reactor = Fit_Par->Get_Value(NAME_REACTOR);
//Reactor
    Hist_Prediction = (TH1D*) Hist_Reactor->Clone(NAME_FITTER_REACTOR.c_str());
    Hist_Prediction->Scale(Number_Reactor);
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./Reactor_Prediction.jpg", Hist_Prediction, "Prediction", "E_{Prompt}/MeV", "U", 0, 0, 0); //For Testing
    };
};

#endif 