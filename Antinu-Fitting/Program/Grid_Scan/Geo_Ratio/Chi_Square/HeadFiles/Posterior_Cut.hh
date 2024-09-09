#ifndef POSTERIOR_CUT_HH
#define POSTERIOR_CUT_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TVector3.h>
//RAT
//Self-Defined
#include "./Base_Functions.hh"
#include "./Plot_Code.hh"

const std::string U_FILE = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/U_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
const std::string TH_FILE = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/Th_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
const std::string AC_FILE = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/Generated_Events/20240530/Gold_AC_Generated_CP.root";

class PDFs
{
public:
    //Constructor and Destructor
    PDFs(std::string name):Is_Test(false) { Name = name;};
    ~PDFs(){};
    //Setup Histogram
    void Setup_Histogram( TH1D *Example_TH1D_Delayed, TH1D *Example_TH1D_Posterior, TH2D *Example_TH2D );
    void Setup_TH1D_Delayed( TH1D *Example_Hist );
    void Setup_TH1D_Posterior( TH1D *Example_Hist );
    void Setup_TH2D( TH2D *Example_Hist );
    void Normalize_TH1D_Delayed();
    void Normalize_TH1D_Posterior();
    void Normalize_TH2D();
    void Open_Test_Mode() { Is_Test = true; };
    void Close_Test_Mode() { Is_Test = false; };
//Members
    std::string Name;
    TH1D *Delayed_Energy, *Posterior;
    TH2D *DeltaR_DeltaT;
    std::vector<Double_t> Rate;
    Bool_t Is_Test;
};

void PDFs::Setup_Histogram( TH1D *Example_TH1D_Delayed, TH1D *Example_TH1D_Posterior, TH2D *Example_TH2D )
{
    Setup_TH1D_Delayed(Example_TH1D_Delayed);
    Setup_TH1D_Posterior(Example_TH1D_Posterior);
    Setup_TH2D(Example_TH2D);
};

void PDFs::Setup_TH1D_Delayed( TH1D *Example_Hist )
{
    Delayed_Energy = (TH1D*) Example_Hist->Clone((Name + "_Delayed_Energy").c_str());
    Delayed_Energy->Reset("ICES");
};

void PDFs::Setup_TH1D_Posterior( TH1D *Example_Hist )
{
    Posterior = (TH1D*) Example_Hist->Clone((Name + "_Posterior").c_str());
    Posterior->Reset("ICES");
};

void PDFs::Setup_TH2D( TH2D*Example_Hist )
{
    DeltaR_DeltaT = (TH2D*) Example_Hist->Clone((Name + "_DeltaT_DeltaR").c_str());
    DeltaR_DeltaT->Reset("ICES");
};

void PDFs::Normalize_TH1D_Delayed()
{
    Delayed_Energy->Scale( 1.0/Delayed_Energy->Integral());
};

void PDFs::Normalize_TH1D_Posterior()
{
    Posterior->Scale( 1.0/Posterior->Integral());
};

void PDFs::Normalize_TH2D()
{
    DeltaR_DeltaT->Scale( 1.0/DeltaR_DeltaT->Integral());
}

PDFs geo("Geo");
PDFs AC("AC");

void Get_Geo_PDF( PDFs &pdf)
{
    std::string InFile_U = U_FILE;
    std::string InFile_Th = TH_FILE;
//Read Files
    TFile *infile_U = new TFile(InFile_U.c_str());
    TTree *intree_U = (TTree*) infile_U->Get("output");

    TFile *infile_Th = new TFile(InFile_Th.c_str());
    TTree *intree_Th = (TTree*) infile_Th->Get("output");
    std::cout << "[Posterior_Cut::Get_Geo_PDF] Open U File:" << InFile_U << std::endl;
    std::cout << "[Posterior_Cut::Get_Geo_PDF] Open Th File:" << InFile_Th << std::endl;
//Set Branch Address
    Double_t Delayed_Energy;
    intree_U->SetBranchAddress("DelayedEnergy", &Delayed_Energy);

    intree_Th->SetBranchAddress("DelayedEnergy", &Delayed_Energy);
    ULong64_t Prompt_50, Delayed_50;
    intree_U->SetBranchAddress("Prompt50MHz", &Prompt_50);
    intree_U->SetBranchAddress("Delayed50MHz", &Delayed_50);

    intree_Th->SetBranchAddress("Prompt50MHz", &Prompt_50);
    intree_Th->SetBranchAddress("Delayed50MHz", &Delayed_50);
    Double_t Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z;
    intree_U->SetBranchAddress("PromptPosX", &Prompt_Pos_X);
    intree_U->SetBranchAddress("PromptPosY", &Prompt_Pos_Y);
    intree_U->SetBranchAddress("PromptPosZ", &Prompt_Pos_Z);

    intree_Th->SetBranchAddress("PromptPosX", &Prompt_Pos_X);
    intree_Th->SetBranchAddress("PromptPosY", &Prompt_Pos_Y);
    intree_Th->SetBranchAddress("PromptPosZ", &Prompt_Pos_Z);
    Double_t Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z;
    intree_U->SetBranchAddress("DelayedPosX", &Delayed_Pos_X);
    intree_U->SetBranchAddress("DelayedPosY", &Delayed_Pos_Y);
    intree_U->SetBranchAddress("DelayedPosZ", &Delayed_Pos_Z);

    intree_Th->SetBranchAddress("DelayedPosX", &Delayed_Pos_X);
    intree_Th->SetBranchAddress("DelayedPosY", &Delayed_Pos_Y);
    intree_Th->SetBranchAddress("DelayedPosZ", &Delayed_Pos_Z);
    std::cout << "[Posterior_Cut::Get_Geo_PDF] Setup Branch Address" << std::endl;
//Compute Number of U and Th
    Int_t Len_U = intree_U->GetEntries();
    Int_t Len_Th = intree_Th->GetEntries();
    if( 4 * Len_Th > Len_U)
    {
        Len_Th = Len_U/4;
    }
    else
    {
        Len_U = 4 * Len_Th;
    };
    std::cout << "[Posterior_Cut::Get_Geo_PDF] U Events:" << Len_U << ", Th Events:" << Len_Th << ", Total Events:" << Len_U + Len_Th << std::endl;
//Loop U Data
    Double_t Delta_T;
    TVector3 Prompt_Pos, Delayed_Pos, Delta_R;
    std::cout << "[Posterior_Cut::Get_Geo_PDF] Begin to Loop U Data" << std::endl;
    for(int ii1 = 0; ii1 < Len_U; ii1++)
    {
        intree_U->GetEntry(ii1);
        Delta_T = Compute_Delta_T(Prompt_50, Delayed_50)/1e3;//us
        Prompt_Pos.SetXYZ(Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z);
        Delayed_Pos.SetXYZ(Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z);
        Delta_R = Delayed_Pos - Prompt_Pos;
        pdf.Delayed_Energy->Fill(Delayed_Energy);
        pdf.DeltaR_DeltaT->Fill(Delta_R.Mag(), Delta_T);
    };
//Loop Th Data
    std::cout << "[Posterior_Cut::Get_Geo_PDF] Begin to Loop Th Data" << std::endl;
    for(int ii1 = 0; ii1 < Len_Th; ii1++)
    {
        intree_Th->GetEntry(ii1);
        Delta_T = Compute_Delta_T(Prompt_50, Delayed_50)/1e3;//us
        Prompt_Pos.SetXYZ(Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z);
        Delayed_Pos.SetXYZ(Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z);
        Delta_R = Delayed_Pos - Prompt_Pos;
        pdf.Delayed_Energy->Fill(Delayed_Energy);
        pdf.DeltaR_DeltaT->Fill(Delta_R.Mag(), Delta_T);
    };
//Close Files
    infile_U->Close();
    infile_Th->Close();
//Normalize
    pdf.Normalize_TH1D_Delayed();
    pdf.Normalize_TH2D();
//Show Messages
    std::cout << "[Posterior_Cut::Get_Geo_PDF] Complete" << std::endl;
//Test Code
    if( pdf.Is_Test == true)
    {
        SavePlot("./Test/Posterior_Geo_Delayed_Energy.jpg", pdf.Delayed_Energy, "Geo", "E_{Delayed}/MeV", "Geo Delayed Energy", 0, 0, 0);
        SavePlot("./Test/Posterior_Geo_DeltaT_DeltaR.jpg", pdf.DeltaR_DeltaT, "Geo", "#Delta R/mm", "#Delta T/us", "Geo #Delta T vs #Delta R");
    };
};

void Get_AC_PDF( PDFs &pdf)
{
    std::string InFile = AC_FILE;
//Read File
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
    std::cout << "[Posterior_Cut::Get_AC_PDF] Open AC File:" << InFile << std::endl;
//Set Branch Address
    Double_t Delayed_Energy;
    intree->SetBranchAddress("DelayedEnergy", &Delayed_Energy);

    ULong64_t Prompt_50, Delayed_50;
    intree->SetBranchAddress("Prompt50MHz", &Prompt_50);
    intree->SetBranchAddress("Delayed50MHz", &Delayed_50);

    Double_t Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z;
    intree->SetBranchAddress("PromptPosX", &Prompt_Pos_X);
    intree->SetBranchAddress("PromptPosY", &Prompt_Pos_Y);
    intree->SetBranchAddress("PromptPosZ", &Prompt_Pos_Z);

    Double_t Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z;
    intree->SetBranchAddress("DelayedPosX", &Delayed_Pos_X);
    intree->SetBranchAddress("DelayedPosY", &Delayed_Pos_Y);
    intree->SetBranchAddress("DelayedPosZ", &Delayed_Pos_Z);

    Double_t Delta_T;
    intree->SetBranchAddress("DeltaT", &Delta_T);
    std::cout << "[Posterior_Cut::Get_AC_PDF] Setup Branch Address" << std::endl;
//Count Events
    const Int_t Len_AC = intree->GetEntries();
    std::cout << "[Posterior_Cut::Get_AC_PDF] AC Events:" << Len_AC << std::endl;
//Loop Data
    TVector3 Prompt_Pos, Delayed_Pos, Delta_R;
    std::cout << "[Posterior_Cut::Get_AC_PDF] Begin to Loop AC Data" << std::endl;
    for(int ii1 = 0; ii1 < Len_AC; ii1++)
    {
        intree->GetEntry(ii1);
        Delta_T = Delta_T/1e3;
        Prompt_Pos.SetXYZ(Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z);
        Delayed_Pos.SetXYZ(Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z);
        Delta_R = Delayed_Pos - Prompt_Pos;
        pdf.Delayed_Energy->Fill(Delayed_Energy);
        pdf.DeltaR_DeltaT->Fill(Delta_R.Mag(), Delta_T );
    };
//Close Files
    infile->Close();
//Normalize
    pdf.Normalize_TH1D_Delayed();
    pdf.Normalize_TH2D();
//Show Messages
    std::cout << "[Posterior_Cut::Get_AC_PDF] Complete" << std::endl;
//Test Code
    if( pdf.Is_Test == true)
    {
        SavePlot("./Test/Posterior_AC_Delayed_Energy.jpg", pdf.Delayed_Energy, "AC", "E_{Delayed}/MeV", "AC Delayed Energy", 0, 0, 0);
        SavePlot("./Test/Posterior_AC_DeltaT_DeltaR.jpg", pdf.DeltaR_DeltaT, "AC", "#Delta R/mm", "#Delta T/us", "AC #Delta T vs #Delta R");
    };
};

Double_t Compute_Posterior( Double_t Delayed_Energy, Double_t Delta_R, Double_t Delta_T )
{
    Double_t P = 0;
//Delayed Energy
    Int_t Index = AC.Delayed_Energy->FindBin(Delayed_Energy);
    Double_t Geo_Delayed = geo.Delayed_Energy->GetBinContent(Index);
    Double_t AC_Delayed = AC.Delayed_Energy->GetBinContent(Index);
    if(Geo_Delayed == 0)
    {
        Geo_Delayed = geo.Delayed_Energy->GetMinimum(1e-20);
    };
    if(AC_Delayed == 0)
    {
        AC_Delayed = AC.Delayed_Energy->GetMinimum(1e-20);
    }
    std::cout << "Delayed:" << Geo_Delayed << "," << AC_Delayed << ", Ratio:" << log(Geo_Delayed / AC_Delayed) << std::endl;
    P = P + log(Geo_Delayed / AC_Delayed);
//TH2D
    Index = AC.DeltaR_DeltaT->FindBin(Delta_R, Delta_T);
    Double_t Geo_2D = geo.DeltaR_DeltaT->GetBinContent(Index);
    Double_t AC_2D = AC.DeltaR_DeltaT->GetBinContent(Index);
    if(Geo_2D == 0)
    {
        Geo_2D = geo.DeltaR_DeltaT->GetMinimum(1e-20);
    };
    if(AC_2D == 0)
    {
        AC_2D = AC.DeltaR_DeltaT->GetMinimum(1e-20);
    };
    std::cout << "2D:" << Geo_2D << "," << AC_2D << ", Ratio:" << log(Geo_2D / AC_2D) << std::endl;
    P = P + log(Geo_2D / AC_2D);
//Test Codes
    return P;
};

void Test_Posterior()
{
    std::string U_File = U_FILE;
    std::string Th_File = TH_FILE;
    std::string AC_File = AC_FILE;
//Read Files
    TFile *infile_U = new TFile(U_File.c_str());
    TTree *intree_U = (TTree*) infile_U->Get("output");
    TFile *infile_Th = new TFile(Th_File.c_str());
    TTree *intree_Th = (TTree*) infile_Th->Get("output");
    TFile *infile_AC = new TFile(AC_File.c_str());
    TTree *intree_AC = (TTree*) infile_AC->Get("output");
//Set Branch Address
    Double_t Delayed_Energy;
    intree_U->SetBranchAddress("DelayedEnergy", &Delayed_Energy);

    intree_Th->SetBranchAddress("DelayedEnergy", &Delayed_Energy);

    intree_AC->SetBranchAddress("DelayedEnergy", &Delayed_Energy);
    ULong64_t Prompt_50, Delayed_50;
    intree_U->SetBranchAddress("Prompt50MHz", &Prompt_50);
    intree_U->SetBranchAddress("Delayed50MHz", &Delayed_50);

    intree_Th->SetBranchAddress("Prompt50MHz", &Prompt_50);
    intree_Th->SetBranchAddress("Delayed50MHz", &Delayed_50);
    Double_t Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z;
    intree_U->SetBranchAddress("PromptPosX", &Prompt_Pos_X);
    intree_U->SetBranchAddress("PromptPosY", &Prompt_Pos_Y);
    intree_U->SetBranchAddress("PromptPosZ", &Prompt_Pos_Z);

    intree_Th->SetBranchAddress("PromptPosX", &Prompt_Pos_X);
    intree_Th->SetBranchAddress("PromptPosY", &Prompt_Pos_Y);
    intree_Th->SetBranchAddress("PromptPosZ", &Prompt_Pos_Z);

    intree_AC->SetBranchAddress("PromptPosX", &Prompt_Pos_X);
    intree_AC->SetBranchAddress("PromptPosY", &Prompt_Pos_Y);
    intree_AC->SetBranchAddress("PromptPosZ", &Prompt_Pos_Z);
    Double_t Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z;
    intree_U->SetBranchAddress("DelayedPosX", &Delayed_Pos_X);
    intree_U->SetBranchAddress("DelayedPosY", &Delayed_Pos_Y);
    intree_U->SetBranchAddress("DelayedPosZ", &Delayed_Pos_Z);

    intree_Th->SetBranchAddress("DelayedPosX", &Delayed_Pos_X);
    intree_Th->SetBranchAddress("DelayedPosY", &Delayed_Pos_Y);
    intree_Th->SetBranchAddress("DelayedPosZ", &Delayed_Pos_Z);

    intree_AC->SetBranchAddress("DelayedPosX", &Delayed_Pos_X);
    intree_AC->SetBranchAddress("DelayedPosY", &Delayed_Pos_Y);
    intree_AC->SetBranchAddress("DelayedPosZ", &Delayed_Pos_Z);

    Double_t Delta_T;
    intree_AC->SetBranchAddress("DeltaT", &Delta_T);
//Compute Number of U and Th
    Int_t Len_U = intree_U->GetEntries();
    Int_t Len_Th = intree_Th->GetEntries();
    if( 4 * Len_Th > Len_U)
    {
        Len_Th = Len_U/4;
    }
    else
    {
        Len_U = 4 * Len_Th;
    };
    std::cout << "[Posterior_Cut::Test_Posterior] U Events:" << Len_U << ", Th Events:" << Len_Th << ", Total Events:" << Len_U + Len_Th << std::endl;
//Loop U Data
    Double_t temp_DeltaT, temp_Posterior;
    TVector3 Prompt_Pos, Delayed_Pos, Delta_R;
    for(int ii1 = 0; ii1 < Len_U; ii1++)
    {
        intree_U->GetEntry(ii1);
        temp_DeltaT = Compute_Delta_T(Prompt_50, Delayed_50);
        Prompt_Pos.SetXYZ(Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z);
        Delayed_Pos.SetXYZ(Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z);
        Delta_R = Delayed_Pos - Prompt_Pos;
        temp_Posterior = Compute_Posterior(Delayed_Energy, Delta_R.Mag(), temp_DeltaT);
        geo.Posterior->Fill(temp_Posterior);
    };
//Loop Th Data
    for(int ii1 = 0; ii1 < Len_Th; ii1++)
    {
        intree_Th->GetEntry(ii1);
        temp_DeltaT = Compute_Delta_T(Prompt_50, Delayed_50);
        Prompt_Pos.SetXYZ(Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z);
        Delayed_Pos.SetXYZ(Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z);
        Delta_R = Delayed_Pos - Prompt_Pos;
        temp_Posterior = Compute_Posterior(Delayed_Energy, Delta_R.Mag(), temp_DeltaT);
        geo.Posterior->Fill(temp_Posterior);
    };
//Loop AC Data
    Int_t Len_AC = intree_AC->GetEntries();
    for(int ii1 = 0; ii1 < Len_AC; ii1++)
    {
        intree_AC->GetEntry(ii1);
        temp_DeltaT = Delta_T;
        Prompt_Pos.SetXYZ(Prompt_Pos_X, Prompt_Pos_Y, Prompt_Pos_Z);
        Delayed_Pos.SetXYZ(Delayed_Pos_X, Delayed_Pos_Y, Delayed_Pos_Z);
        Delta_R = Delayed_Pos - Prompt_Pos;
        temp_Posterior = Compute_Posterior(Delayed_Energy, Delta_R.Mag(), temp_DeltaT);

        AC.Posterior->Fill(temp_Posterior);
    };
//Draw
    Save_Two_Plots("./Test/Posterior_Geo_AC.jpg", geo.Posterior, "Geo", AC.Posterior, "AC", "Posterior", "Posterior", 1, 0, 0);
};

#endif