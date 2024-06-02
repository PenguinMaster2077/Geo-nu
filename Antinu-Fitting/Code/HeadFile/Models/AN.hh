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
    AN(): Is_Initialized(false), Is_Test(false) {};
    //Global Point
    static AN *Point_AN_;
    //Member Functions
    static AN *Get_Global_Point();
    void Initialize();
    bool Is_Initialize() {return Is_Initialized;};
    void Get_Default_PDFs();
    void Setup_PDFs(TH1D *Example_Hist);
    void Compute_Predictions(std::vector<TH1D*> &Hists);
    void Open_Test_Mode() { Is_Test = true; };
    void Close_Test_Mode() { Is_Test = false; };
private:
    Bool_t Is_Initialized;
    TH1D *Hist_Proton, *Hist_C12, *Hist_O16;
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

void AN::Initialize()
{
    Is_Initialized = true;
//Histogram
    Get_Default_PDFs();
};

void AN::Get_Default_PDFs()
{
//Setup Histogram
    TFile pdf_file_an(PDF_AN.c_str());
    TH1D* Full_PDF = (TH1D*) pdf_file_an.Get("AN_Prompt_Energy");
    Hist_Proton = (TH1D*) pdf_file_an.Get("AN_Proton_Recoil");
    Hist_C12 = (TH1D*) pdf_file_an.Get("AN_C12");
    Hist_O16 = (TH1D*) pdf_file_an.Get("AN_O16");

    Pro_Proton = Hist_Proton->Integral() / Full_PDF->Integral();
    Pro_C12 = Hist_C12->Integral() / Full_PDF->Integral();
    Pro_O16 = Hist_O16->Integral() / Full_PDF->Integral();

    Hist_Proton->Scale( 1.0 / Hist_Proton->Integral() );
    Hist_C12->Scale( 1.0 / Hist_C12->Integral() );
    Hist_O16->Scale( 1.0 / Hist_O16->Integral() );

    Hist_Proton->SetName(NAME_AN_PROTON_RECOIL.c_str());
    Hist_C12->SetName(NAME_AN_C12.c_str());
    Hist_O16->SetName(NAME_AN_O16.c_str());
//Show Details
    std::cout << "[Geonu::Get_Default_PDFs] Load Default AN Proton Recoil PDF. Min:" << Hist_Proton->GetXaxis()->GetXmin() << ", Max:" << Hist_Proton->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_Proton->GetXaxis()->GetNbins() << ", Width:" << Hist_Proton->GetXaxis()->GetBinWidth(1) << std::endl;
    std::cout << "[Geonu::Get_Default_PDFs] Load Default AN C12 PDF. Min:" << Hist_C12->GetXaxis()->GetXmin() << ", Max:" << Hist_C12->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_C12->GetXaxis()->GetNbins() << ", Width:" << Hist_C12->GetXaxis()->GetBinWidth(1) << std::endl;
    std::cout << "[Geonu::Get_Default_PDFs] Load Default AN O16 PDF. Min:" << Hist_O16->GetXaxis()->GetXmin() << ", Max:" << Hist_O16->GetXaxis()->GetXmax() << ", Bin Number:" << Hist_O16->GetXaxis()->GetNbins() << ", Width:" << Hist_O16->GetXaxis()->GetBinWidth(1) << std::endl;
//Test Code
    if( Is_Test == true)
    {
        SavePlot("./AN_Proton_Default_PDF.jpg", Hist_Proton, "PR", "E_{Prompt}/MeV", "U", 0, 0, 0); //For Testing
        SavePlot("./AN_C12_Default_PDF.jpg", Hist_C12, "{}^{12}C", "E_{Prompt}/MeV", "Th", 0, 0, 0); //For Testing
        SavePlot("./AN_O16_Default_PDF.jpg", Hist_O16, "{}^{16}O", "E_{Prompt}/MeV", "U", 0, 0, 0); //For Testing
    };
};

void AN::Setup_PDFs(TH1D *Example_Hist)
{
    if( Is_Initialized == true)
    {
        std::cout << "[AN] Haven't Load Default PDFs. But Load PDFs according Given Histogram." << std::endl;
    };
    TFile pdf_data_file_an(PDF_DATA_AN.c_str());
    TTree *tree_an = (TTree*) pdf_data_file_an.Get("output");
//Setup Histogram
    Double_t x_min = Example_Hist->GetXaxis()->GetXmin();
    Double_t x_max = Example_Hist->GetXaxis()->GetXmax();
    std::string Selection = Form("PromptEnergy >= %f && PromptEnergy <= %f", x_min, x_max);
//Setup Full PDF
    TH1D* Full_PDF = (TH1D*) Example_Hist->Clone("Full_PDF"); 
    Full_PDF->Reset("ICES");
    tree_an->Project(Full_PDF->GetName(), "PromptEnergy", Selection.c_str());
    Double_t ToTal = Full_PDF->Integral();
//Setup Proton Recoil
    Hist_Proton = (TH1D*) (Example_Hist->Clone(Name_Proton.c_str()));
    Hist_Proton->Reset("ICES");
    Selection= Form("PromptEnergy >= %f && PromptEnergy < %f", 0.0, AN_ENERGY_PROTON_RECOIL);
    tree_an->Project(Name_Proton.c_str(), "PromptEnergy", Selection.c_str());
    if( ToTal >= 0.0)
    {
        Pro_Proton = Hist_Proton->Integral() / Full_PDF->Integral();
    }
    else
    {
        std::cout << "[AN::Setup_PDFs] Total Area is 0. Probability of Proton Recoil is assigned to 0" << std::endl;
        Pro_Proton = 0;
    };
    Hist_Proton->Scale( 1.0 / ToTal);
//Setup C12
    Hist_C12 = (TH1D*) (Example_Hist->Clone(Name_C12.c_str()));
    Hist_C12->Reset("ICES");
    Selection= Form("PromptEnergy >= %f && PromptEnergy <= %f", AN_ENERGY_PROTON_RECOIL, AN_ENERGY_C12);
    tree_an->Project(Name_C12.c_str(), "PromptEnergy", Selection.c_str());
    Pro_C12 = Hist_C12->Integral() / Full_PDF->Integral();
    if( ToTal >= 0.0)
    {
        Pro_C12 = Hist_C12->Integral() / Full_PDF->Integral();
    }
    else
    {
        std::cout << "[AN::Setup_PDFs] Total Area is 0. Probability of Proton Recoil is assigned to 0" << std::endl;
        Pro_C12 = 0;
    };
    Hist_C12->Scale( 1.0 / Hist_C12->Integral());
//Setup O16
    Hist_O16 = (TH1D*) (Example_Hist->Clone(Name_O16.c_str()));
    Hist_O16->Reset("ICES");
    Selection= Form("PromptEnergy > %f", AN_ENERGY_C12);
    tree_an->Project(Name_O16.c_str(), "PromptEnergy", Selection.c_str());
    if( ToTal >= 0.0)
    {
        Pro_O16 = Hist_O16->Integral() / Full_PDF->Integral();
    }
    else
    {
        std::cout << "[AN::Setup_PDFs] Total Area is 0. Probability of Proton Recoil is assigned to 0" << std::endl;
        Pro_O16 = 0;
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

void AN::Compute_Predictions(std::vector<TH1D*> &Hists)
{
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Double_t Number_Ground = Fit_Par->Get_Value(NAME_AN_GROUND);
    Double_t Number_Exicted = Fit_Par->Get_Value(NAME_AN_EXICTED);

    Double_t Number_Proton = Number_Ground * Pro_Proton / (Pro_Proton + Pro_C12);
    Double_t Number_C12 = Number_Ground * Pro_C12 / (Pro_Proton + Pro_C12);
    Double_t Number_O16 = Number_Exicted;
//Ground States
    TH1D *temp_hist = (TH1D*) Hist_Proton->Clone(NAME_AN_GROUND.c_str());
    temp_hist->Scale(Number_Proton);
    temp_hist->Add(Hist_C12, Number_C12);
    Hists.push_back(temp_hist);
//1st Exicted States
    temp_hist = (TH1D*) Hist_O16->Clone(NAME_AN_EXICTED.c_str());
    temp_hist->Scale(Number_Exicted);
    Hists.push_back(temp_hist);
};

#endif