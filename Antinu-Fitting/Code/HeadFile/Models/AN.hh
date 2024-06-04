#ifndef AN_HH
#define AN_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "../Constant_Setting.hh"
#include "../Fit_Parameters.hh"
#include "../Plot_Code.hh"

class AN
{
public:
    //Constructor and Destructor
    AN(): Is_Initialized(false), Is_Test(false), Is_Load_PDF(false) {};
    //Global Point
    static AN *Point_AN_;
    //Member Functions
    static AN *Get_Global_Point();
    bool Is_Initialize() {return Is_Initialized;};
    void Setup_PDFs(TH1D *Example_Hist);
    void Compute_Predictions();
    TH1D *Get_Total_Prediction() { return Hist_Prediction; };
    TH1D *Get_Ground() { return Hist_Ground; };
    TH1D *Get_Exicted() { return Hist_Exicted; };
    TH1D *Get_Proton_Recoil() { return Hist_Proton; };
    TH1D *Get_C12() { return Hist_C12; };
    TH1D *Get_O16() { return Hist_O16; };
    void Open_Test_Mode() { Is_Test = true; };
    void Close_Test_Mode() { Is_Test = false; };
private:
    Bool_t Is_Initialized, Is_Load_PDF;
    TH1D *Hist_Proton, *Hist_C12, *Hist_O16;
    TH1D *Hist_Ground, *Hist_Exicted, *Hist_Prediction;
    std::string Name_Proton = "Hist_Proton", Name_C12 = "Hist_C12", Name_O16 = "Hist_O16";
    Bool_t Is_Test;
    Double_t Pro_Proton, Pro_C12, Pro_O16;
};

AN *AN::Point_AN_ = new AN();

AN *AN::Get_Global_Point()
{
    if(Point_AN_->Is_Initialized == false)
    {
        std::cout << "[Geonu] Haven't been Initialized. Can not use it." << std::endl;
    };
    return Point_AN_;
};

void AN::Setup_PDFs(TH1D *Example_Hist)
{
    Is_Initialized = true;
    Is_Load_PDF = true;
//Setup Histogram
    Double_t x_min = Example_Hist->GetXaxis()->GetXmin();
    Double_t x_max = Example_Hist->GetXaxis()->GetXmax();
    Int_t NBinx = Example_Hist->GetNbinsX();
    std::string Selection = Form("PromptEnergy >= %f && PromptEnergy <= %f", x_min, x_max);
//Setup Full PDF
    TH1D* Full_PDF = (TH1D*) Example_Hist->Clone("Full_PDF"); 
    Full_PDF->Reset("ICES");
    //Pronton Recoil
    Hist_Proton = (TH1D*) (Example_Hist->Clone(Name_Proton.c_str()));
    Hist_Proton->Reset("ICES");
    //C12
    Hist_C12 = (TH1D*) (Example_Hist->Clone(Name_C12.c_str()));
    Hist_C12->Reset("ICES");
    //O16
    Hist_O16 = (TH1D*) (Example_Hist->Clone(Name_O16.c_str()));
    Hist_O16->Reset("ICES");
    //Fill Data
    TFile pdf_data_file_an(PDF_DATA_AN.c_str());
    TTree *tree_an = (TTree*) pdf_data_file_an.Get("output");
    Double_t PromptEnergy;
    tree_an->SetBranchAddress("PromptEnergy", &PromptEnergy);
    for(int ii1 = 0; ii1 < tree_an->GetEntries(); ii1++)
    {
        tree_an->GetEntry(ii1);
        if(PromptEnergy < x_min || PromptEnergy > x_max){continue;};
        Full_PDF->Fill(PromptEnergy);
        if(PromptEnergy >= 0 && PromptEnergy < AN_ENERGY_PROTON_RECOIL)
        {
            Hist_Proton->Fill(PromptEnergy);
        };
        if(PromptEnergy >= AN_ENERGY_PROTON_RECOIL && PromptEnergy <= AN_ENERGY_C12)
        {
            Hist_C12->Fill(PromptEnergy);
        };
        if(PromptEnergy > AN_ENERGY_C12)
        {
            Hist_O16->Fill(PromptEnergy);
        }
    };
    pdf_data_file_an.Close();

    Double_t ToTal = Full_PDF->Integral();
//Setup Proton Recoil
    if( ToTal >= 0.0)
    {
        Pro_Proton = Hist_Proton->Integral() / Full_PDF->Integral();
    }
    else
    {
        std::cout << "[AN::Setup_PDFs] Total Area is 0. Quit." << std::endl;
        exit(1);
    };
    Hist_Proton->Scale( 1.0 / Hist_Proton->Integral());
//Setup C12
    if( ToTal >= 0.0)
    {
        Pro_C12 = Hist_C12->Integral() / Full_PDF->Integral();
    }
    else
    {
        std::cout << "[AN::Setup_PDFs] Total Area is 0. Quit." << std::endl;
        exit(1);
    };
    Hist_C12->Scale( 1.0 / Hist_C12->Integral());
//Setup O16
    if( ToTal >= 0.0)
    {
        Pro_O16 = Hist_O16->Integral() / Full_PDF->Integral();
    }
    else
    {
        std::cout << "[AN::Setup_PDFs] Total Area is 0. Quit." << std::endl;
        exit(1);
    };
    Hist_O16->Scale( 1.0 / Hist_O16->Integral());
//Show Details
    std::cout << "[AN::Setup_PDFs] Sucessfully Setup AN Proton Recoil PDF. Min:" << Hist_Proton->GetXaxis()->GetXmin() << ", Max:" << Hist_Proton->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_Proton->GetXaxis()->GetNbins() << ", Width:" << Hist_Proton->GetXaxis()->GetBinWidth(1) << std::endl;
    std::cout << "[AN::Setup_PDFs] Sucessfully Setup AN C12 PDF. Min:" << Hist_C12->GetXaxis()->GetXmin() << ", Max:" << Hist_C12->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_C12->GetXaxis()->GetNbins() << ", Width:" << Hist_C12->GetXaxis()->GetBinWidth(1) << std::endl;
    std::cout << "[AN::Setup_PDFs] Sucessfully Setup AN O16 PDF. Min:" << Hist_O16->GetXaxis()->GetXmin() << ", Max:" << Hist_O16->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_O16->GetXaxis()->GetNbins() << ", Width:" << Hist_O16->GetXaxis()->GetBinWidth(1) << std::endl;
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./AN_Proton_Setup_PDF.jpg", Hist_Proton, "Proton Recoil", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
        SavePlot("./AN_C12_Setup_PDF.jpg", Hist_C12, "{}^{12}C", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
        SavePlot("./AN_O16_Setup_PDF.jpg", Hist_O16, "{}^{16}O", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };
};

void AN::Compute_Predictions()
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Double_t Number_Ground = Fit_Par->Get_Value(NAME_AN_GROUND);
    Double_t Number_Exicted = Fit_Par->Get_Value(NAME_AN_EXICTED);

    Double_t Number_Proton = Number_Ground * Pro_Proton / (Pro_Proton + Pro_C12);
    Double_t Number_C12 = Number_Ground * Pro_C12 / (Pro_Proton + Pro_C12);
    Double_t Number_O16 = Number_Exicted;
//Ground States
    Hist_Ground = (TH1D*) Hist_Proton->Clone(NAME_FITTER_AN_GROUND.c_str());
    Hist_Ground->Scale(Number_Proton);
    Hist_Ground->Add(Hist_C12, Number_C12);
//1st Exicted States
    Hist_Exicted = (TH1D*) Hist_O16->Clone(NAME_FITTER_AN_EXICTED.c_str());
    Hist_Exicted->Scale(Number_Exicted);
//Total AN Histogram
    Hist_Prediction = (TH1D*) Hist_Ground->Clone(NAME_FITTER_AN.c_str());
    Hist_Prediction->Add(Hist_Exicted);
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./AN_Prediction_Ground_PDF.jpg", Hist_Ground, "Ground", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
        SavePlot("./AN_Prediction_Exicted_PDF.jpg", Hist_Exicted, "Exicted", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
        SavePlot("./AN_Prediction_PDF.jpg", Hist_Prediction, "Prediction", "E_{Prompt}/MeV", "Reactor", 0, 0, 0);
    };
};

#endif