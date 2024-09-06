#ifndef test_HH
#define test_HH
//CPP
#include <iostream>
//ROOT
#include <TH1D.h>
#include <TFile.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "../HeadFile/Constant_Setting.hh"

class Geonu
{
public:
    //Constructor and Destructor
    Geonu(): Is_Initialized(false), Is_Test(false) {};
    ~Geonu() { std::cout << "[Geonu] Delete Geonu" << std::endl;};
    //Global Point
    static Geonu *Point_Geonu_;
    static Geonu *Get_Global_Point();
    void Setup(TH1D *Example_Hist);
    void Show_Integral();
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
    return Point_Geonu_;
};

void Geonu::Setup(TH1D *Example_Hist)
{
//Setup Histogram
    Double_t x_min = Example_Hist->GetXaxis()->GetXmin();
    Double_t x_max = Example_Hist->GetXaxis()->GetXmax();
    Int_t NBinx = Example_Hist->GetNbinsX();
    std::string Selection = Form("PromptEnergy >= %f && PromptEnergy <= %f", x_min, x_max);
    std::cout << NBinx << "," << x_min << "," << x_max << std::endl;
//Setup U
    // Hist_U = new TH1D(Name_U.c_str(), "", NBinx, x_min, x_max);
    // Hist_U->Reset("ICES");
    Hist_U = (TH1D*) (Example_Hist->Clone(Name_U.c_str()));
    TFile pdf_data_file_U(PDF_DATA_GEO_U.c_str());
    TTree *tree_U = (TTree*) pdf_data_file_U.Get("output");
    
    // Hist_U->Reset("ICES");
    
    // tree_U->Project(Hist_U->GetName(), "PromptEnergy", Selection.c_str());
    Double_t PromptEnergy;
    tree_U->SetBranchAddress("PromptEnergy", &PromptEnergy);
    for(int ii1 = 0; ii1 < tree_U->GetEntries(); ii1++)
    {
        tree_U->GetEntry(ii1);
        if(PromptEnergy< x_min || PromptEnergy > x_max){continue;};
        Hist_U->Fill(PromptEnergy);
    };


    std::cout << Hist_U->Integral() << std::endl;
    pdf_data_file_U.Close();
    std::cout << "[Setup] " << Hist_U->Integral() << std::endl;
};

void Geonu::Show_Integral()
{
    std::cout << Hist_U->Integral() << std::endl;
};

#endif