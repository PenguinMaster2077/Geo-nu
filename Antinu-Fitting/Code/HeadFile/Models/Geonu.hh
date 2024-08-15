#ifndef GEONU_HH
#define GEONU_HH
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

class Geonu
{
public:
    //Constructor and Destructor
    Geonu(): Is_Initialized(false), Is_Test(false), Is_Load_PDF(false) {};
    ~Geonu() { std::cout << "[Geonu] Delete Geonu" << std::endl;};
    //Global Point
    static Geonu *Point_Geonu_;
    //Member Functions
    static Geonu *Get_Global_Point();
    void Setup_Oscillation(Double_t Sin_Square_Theta_12, Double_t Sin_Square_Theta_13);
    bool Is_Initialize(){return Is_Initialized;};
    void Compute_Survival_Probability();
    void Show_Oscillation_Parameters();
    void Setup_PDFs( TH1D *Example_Hist );
    void Get_Predictions( std::vector<TH1D*> &Hists);
    void Compute_Predictions();
    TH1D *Get_Total_Prediction() { return Hist_Prediction; };
    void Open_Test_Mode() { Is_Test = true;};
    void Close_Test_Mode() { Is_Test = false;};
    TH1D *Get_Predictions();
private:
    Bool_t Is_Initialized, Is_Oscillated, Is_Load_PDF;
    Double_t C_Sin_Square_Theta_12, C_Cos_Square_Theta_12;
    Double_t C_Sin_Square_Theta_13, C_Cos_Square_Theta_13;
    Double_t C_Survival_Probability;
    TH1D *Hist_U, *Hist_Th, *Hist_Prediction;
    std::string Name_U = "Hist_Geo_U", Name_Th = "Hist_Geo_Th";
    Bool_t Is_Test;
};

Geonu *Geonu::Point_Geonu_ = new Geonu();

Geonu *Geonu::Get_Global_Point()
{
    if(Point_Geonu_->Is_Load_PDF == false)
    {
        std::cout << "[Geonu] Haven't Loaded PDFs." << std::endl;
    }
    return Point_Geonu_;
};

void Geonu::Compute_Survival_Probability()
{
    Double_t Part1 = pow(C_Cos_Square_Theta_13, 2);
    Double_t Part2 = 1 - 2 * pow(C_Sin_Square_Theta_12, 2) * pow(C_Cos_Square_Theta_12, 2);
    Double_t Part3 = pow(C_Sin_Square_Theta_12, 2);
    C_Survival_Probability = Part1 * Part2 + Part3;
}

void Geonu::Show_Oscillation_Parameters()
{
    std::cout << "[Geonu::Show_Oscillation_Parameters] Sin Square Theta 12:" << C_Sin_Square_Theta_12 << ", Sin Square Theta 13:" << C_Sin_Square_Theta_13 << std::endl;
    std::cout << "[Geonu::Show_Oscillation_Parameters] Survival Probability:" << C_Survival_Probability << std::endl;
};

void Geonu::Setup_Oscillation(Double_t Sin_Square_Theta_12, Double_t Sin_Square_Theta_13)
{
    Is_Oscillated = true;
//Oscillation Parameters
    C_Sin_Square_Theta_12 = Sin_Square_Theta_12;
    C_Cos_Square_Theta_12 = 1 - C_Sin_Square_Theta_12;
    C_Sin_Square_Theta_13 = Sin_Square_Theta_13;
    C_Cos_Square_Theta_13 = 1 - C_Sin_Square_Theta_13;
    Compute_Survival_Probability();
    Show_Oscillation_Parameters();
}

void Geonu::Setup_PDFs(TH1D *Example_Hist)
{
    Is_Initialized = true;
    Is_Load_PDF = true;
//Setup Histogram
    Double_t x_min = Example_Hist->GetXaxis()->GetXmin();
    Double_t x_max = Example_Hist->GetXaxis()->GetXmax();
    Int_t NBinx = Example_Hist->GetNbinsX();
    std::string Selection = Form("PromptEnergy >= %f && PromptEnergy <= %f", x_min, x_max);
//Setup U
    Hist_U = (TH1D*) (Example_Hist->Clone(Name_U.c_str()));
    Hist_U->Reset("ICES");

    TFile pdf_data_file_U(PDF_DATA_GEO_U.c_str());
    TTree *tree_U = (TTree*) pdf_data_file_U.Get("output");
    Double_t PromptEnergy;
    tree_U->SetBranchAddress("PromptEnergy", &PromptEnergy);
    for(int ii1 = 0; ii1 < tree_U->GetEntries(); ii1++)
    {
        tree_U->GetEntry(ii1);
        if(PromptEnergy < x_min || PromptEnergy > x_max){continue;};
        Hist_U->Fill(PromptEnergy);
    };
    pdf_data_file_U.Close();

    Hist_U->Scale(1.0/Hist_U->Integral());

//Setup Th
    Hist_Th = (TH1D*) (Example_Hist->Clone(Name_Th.c_str()));
    Hist_Th->Reset("ICES");

    TFile pdf_data_file_Th(PDF_DATA_GEO_TH.c_str());
    TTree *tree_Th = (TTree*) pdf_data_file_Th.Get("output");
    tree_Th->SetBranchAddress("PromptEnergy", &PromptEnergy);
    for(int ii1 = 0; ii1 < tree_Th->GetEntries(); ii1++)
    {
        tree_Th->GetEntry(ii1);
        if(PromptEnergy < x_min || PromptEnergy > x_max){continue;};
        Hist_Th->Fill(PromptEnergy);
    };
    pdf_data_file_Th.Close();

    Hist_Th->Scale(1.0/Hist_Th->Integral());
//Show Details
    std::cout << "[Geonu::Setup_PDFs] Sucessfully Setup Geo-U PDF. Min:" << Hist_U->GetXaxis()->GetXmin() << ", Max:" << Hist_U->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_U->GetXaxis()->GetNbins() << ", Width:" << Hist_U->GetXaxis()->GetBinWidth(1) << std::endl;
    std::cout << "[Geonu::Setup_PDFs] Sucessfully Setup Geo-Th PDF. Min:" << Hist_Th->GetXaxis()->GetXmin() << ", Max:" << Hist_Th->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_Th->GetXaxis()->GetNbins() << ", Width:" << Hist_Th->GetXaxis()->GetBinWidth(1) << std::endl;
//Test Code
    if(Is_Test == true)
    {
        SavePlot("./Geo_U_Setup_PDF.jpg", Hist_U, "U", "E_{Prompt}/MeV", "U", 0, 0, 0); //For Testing
        SavePlot("./Geo_Th_Setup_PDF.jpg", Hist_Th, "Th", "E_{Prompt}/MeV", "U", 0, 0, 0); //For Testing
    };
};

// void Geonu::Get_Predictions( std::vector<TH1D*> &Hists)
// {
//     FitParameters *Fit_Par = FitParameters::Get_Global_Point();
//     if(Is_Load_PDF == false)
//     {
//         std::cout << "[Geonu::Get_PDFs] Haven't Load PDFs." << std::endl;
//     }
//     else
//     {
//         FitParameters *Fit_Par = FitParameters::Get_Global_Point();
//         Double_t Number_U = Fit_Par->Get_Value(NAME_GEO_U);
//         Double_t Number_Th = Fit_Par->Get_Value(NAME_GEO_TH);
//         TH1D *temp = (TH1D*) (Hist_U->Clone());
//     };
// };

void Geonu::Compute_Predictions()
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Double_t Number_Geo = Fit_Par->Get_Value(NAME_GEO);
    Double_t Geo_ratio = Fit_Par->Get_Value(NAME_GEO_RATIO);
    Double_t Number_U = Number_Geo * Geo_ratio/ (Geo_ratio + 1);
    Double_t Number_Th = Number_Geo * 1/ (Geo_ratio + 1);
//U
    Hist_Prediction = (TH1D*) (Hist_U->Clone(NAME_FITTER_GEO.c_str()));
    Hist_Prediction->Scale(Number_U);
//Th
    Hist_Prediction->Add( Hist_Th, Number_Th);
//Test Code
    if(Is_Test == true)
    {
        SavePlot("./Geo_Prediction.jpg", Hist_Prediction, "Prediction", "E_{Prompt}/MeV", "U", 0, 0, 0); //For Testing
    };
};

TH1D *Geonu::Get_Predictions()
{
    Compute_Predictions();
    return Hist_Prediction;
}


#endif
