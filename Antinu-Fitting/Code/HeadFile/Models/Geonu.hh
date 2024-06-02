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
    Geonu(): Is_Initialized(false), Is_Test(false) {};
    //Global Point
    static Geonu *Point_Geonu_;
    //Member Functions
    static Geonu *Get_Global_Point();
    void Initialize(Double_t Sin_Square_Theta_12, Double_t Sin_Square_Theta_13);
    bool Is_Initialize(){return Is_Initialized;};
    void Compute_Survival_Probability();
    void Show_Oscillation_Parameters();
    void Get_Default_PDFs();
    void Setup_PDFs(TH1D *Example_Hist);
    void Compute_Predictions(std::vector<TH1D*> &Hists);
    void Open_Test_Mode() { Is_Test = true;};
    void Close_Test_Mode() { Is_Test = false;};

private:
    Bool_t Is_Initialized;
    Double_t C_Sin_Square_Theta_12, C_Cos_Square_Theta_12;
    Double_t C_Sin_Square_Theta_13, C_Cos_Square_Theta_13;
    Double_t C_Survival_Probability;
    TH1D *Hist_U, *Hist_Th;
    std::string Name_U = "Hist_Geo_U", Name_Th = "Hist_Geo_Th";
    Bool_t Is_Test;
};

Geonu *Geonu::Point_Geonu_ = new Geonu();

Geonu *Geonu::Get_Global_Point()
{
    if(Point_Geonu_->Is_Initialized == false)
    {
        std::cout << "[Geonu] Haven't been Initialized. Can not use it." << std::endl;
    };
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

void Geonu::Initialize(Double_t Sin_Square_Theta_12, Double_t Sin_Square_Theta_13)
{
    Is_Initialized = true;
//Histogram
    Get_Default_PDFs();
//Oscillation Parameters
    C_Sin_Square_Theta_12 = Sin_Square_Theta_12;
    C_Cos_Square_Theta_12 = 1 - C_Sin_Square_Theta_12;
    C_Sin_Square_Theta_13 = Sin_Square_Theta_13;
    C_Cos_Square_Theta_13 = 1 - C_Sin_Square_Theta_13;
    Compute_Survival_Probability();
    Show_Oscillation_Parameters();
}

void Geonu::Get_Default_PDFs()
{
//Setup Histogram
    TFile pdf_file_U(PDF_GEO_U.c_str()), pdf_file_Th(PDF_GEO_TH.c_str());
    Hist_U = (TH1D*) pdf_file_U.Get("U_Prompt_Energy");
    Hist_Th = (TH1D*) pdf_file_Th.Get("Th_Prompt_Energy");

    Hist_U->Scale(1.0/Hist_U->Integral());
    Hist_Th->Scale(1.0/Hist_Th->Integral());

    Hist_U->SetName(Name_U.c_str());
    Hist_Th->SetName(Name_Th.c_str());
//Show Details
    std::cout << "[Geonu::Get_Default_PDFs] Load Default Geo-U PDFs. Min:" << Hist_U->GetXaxis()->GetXmin() << ", Max:" << Hist_U->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_U->GetXaxis()->GetNbins() << ", Width:" << Hist_U->GetXaxis()->GetBinWidth(1) << std::endl;
    std::cout << "[Geonu::Get_Default_PDFs] Load Default Setup Geo-Yh PDFs. Min:" << Hist_Th->GetXaxis()->GetXmin() << ", Max:" << Hist_Th->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_Th->GetXaxis()->GetNbins() << ", Width:" << Hist_Th->GetXaxis()->GetBinWidth(1) << std::endl;
//Test Code
    if(Is_Test == true)
    {
        SavePlot("./Geo_U_Default_PDF.jpg", Hist_U, "U", "E_{Prompt}/MeV", "U", 0, 0, 0); //For Testing
        SavePlot("./Geo_Th_Default_PDF.jpg", Hist_Th, "Th", "E_{Prompt}/MeV", "Th", 0, 0, 0); //For Testing
    };
};

void Geonu::Setup_PDFs(TH1D *Example_Hist)
{
    if(Is_Initialized == false)
    {
        std::cout << "[Geonu] Haven't Setup Oscillation Parameters. Can not use it." << std::endl;
    };
//Setup Histogram
    Double_t x_min = Example_Hist->GetXaxis()->GetXmin();
    Double_t x_max = Example_Hist->GetXaxis()->GetXmax();
    std::string Selection = Form("PromptEnergy >= %f && PromptEnergy <= %f", x_min, x_max);
//Setup U
    TFile pdf_data_file_U(PDF_DATA_GEO_U.c_str());
    TTree *tree_U = (TTree*) pdf_data_file_U.Get("output");
    Hist_U = (TH1D*) (Example_Hist->Clone(Name_U.c_str()));
    Hist_U->Reset("ICES");

    tree_U->Project(Name_U.c_str(), "PromptEnergy", Selection.c_str());
    Hist_U->Scale(1.0/Hist_U->Integral());
//Setup Th
    TFile pdf_data_file_Th(PDF_DATA_GEO_TH.c_str());
    TTree *tree_Th = (TTree*) pdf_data_file_Th.Get("output");
    Hist_Th = (TH1D*) (Example_Hist->Clone(Name_Th.c_str()));
    Hist_Th->Reset("ICES");

    tree_Th->Project(Name_Th.c_str(), "PromptEnergy", Selection.c_str());
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
}

void Geonu::Compute_Predictions(std::vector<TH1D*> &Hists)
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Double_t Number_U = Fit_Par->Get_Value(NAME_GEO_U);
    Double_t Number_Th = Fit_Par->Get_Value(NAME_GEO_TH);
//U
    TH1D* temp_hist = (TH1D*) Hist_U->Clone(NAME_GEO_U.c_str());
    temp_hist->Scale(Number_U);
    Hists.push_back(temp_hist);
//Th
    temp_hist = (TH1D*) Hist_Th->Clone(NAME_GEO_TH.c_str());
    temp_hist->Scale(Number_Th);
    Hists.push_back(temp_hist);
};


#endif
