//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TGraph.h>
#include <TChain.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
//Self-Defined
// #include "/home/penguin/Rat/rat-7.0.14-ROOT5/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

#include "/rat/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

int Plot()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/IBDCandidate/Gold_20R_IBD_Candidate_0000300000-0000307612.root";
    std::string OutPWD = "/rat/MyCode/Work/Geo-nu/Ingrida/Pic/";
    std::string OutPut_Prompt_Energy = OutPWD + "Prompt_Energy.png";
    std::string OutPut_Delayed_Energy = OutPWD + "Delayed_Energy.png";
    std::string OutPut_Delta_T = OutPWD + "Delta_T.png";
    std::string OutPut_Position = OutPWD + "Position.png";
//Read File    
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
    //Set Branch Address
    //Prompt
    UInt_t PromptEntry, PromptEv;
    intree->SetBranchAddress("PromptEntry", &PromptEntry);
    intree->SetBranchAddress("PromptEv", &PromptEv);
    Int_t PromptGTID, PromptOWLHits;
    intree->SetBranchAddress("PromptGTID", &PromptGTID);
    intree->SetBranchAddress("PromptOWLHits", &PromptOWLHits);
    UInt_t PromptNHits, PromptNHitsCleaned, PromptNHitsCorrected;
    intree->SetBranchAddress("PromptNHits", &PromptNHits);
    intree->SetBranchAddress("PromptNHitsCleaned", &PromptNHitsCleaned);
    intree->SetBranchAddress("PromptNHitsCorrected", &PromptNHitsCorrected);
    ULong64_t Prompt50MHz;
    intree->SetBranchAddress("Prompt50MHz", &Prompt50MHz);
    ULong64_t PromptDataApplied, PromptDataFlags;
    intree->SetBranchAddress("PromptDataApplied", &PromptDataApplied);
    intree->SetBranchAddress("PromptDataFlags", &PromptDataFlags);
    Double_t PromptPosX, PromptPosY, PromptPosZ, PromptEnergy;
    intree->SetBranchAddress("PromptPosX", &PromptPosX);
    intree->SetBranchAddress("PromptPosY", &PromptPosY);
    intree->SetBranchAddress("PromptPosZ", &PromptPosZ);
    intree->SetBranchAddress("PromptEnergy", &PromptEnergy);
    Double_t Prompt_Original_Energy, Prompt_Tony_Energy;
    intree->SetBranchAddress("PromptOriginalEnergy", &Prompt_Original_Energy);
    intree->SetBranchAddress("PromptTonyEnergy", &Prompt_Tony_Energy);
//Delayed
    UInt_t DelayedEntry, DelayedEv;
    intree->SetBranchAddress("DelayedEntry", &DelayedEntry);
    intree->SetBranchAddress("DelayedEv", &DelayedEv);
    Int_t DelayedGTID, DelayedOWLHits;
    intree->SetBranchAddress("DelayedGTID", &DelayedGTID);
    intree->SetBranchAddress("DelayedOWLHits", &DelayedOWLHits);
    UInt_t DelayedNHits, DelayedNHitsCleaned, DelayedNHitsCorrected;
    intree->SetBranchAddress("DelayedNHits", &DelayedNHits);
    intree->SetBranchAddress("DelayedNHitsCleaned", &DelayedNHitsCleaned);
    intree->SetBranchAddress("DelayedNHitsCorrected", &DelayedNHitsCorrected);
    ULong64_t Delayed50MHz;
    intree->SetBranchAddress("Delayed50MHz", &Delayed50MHz);
    ULong64_t DelayedDataApplied, DelayedDataFlags;
    intree->SetBranchAddress("DelayedDataApplied", &DelayedDataApplied);
    intree->SetBranchAddress("DelayedDataFlags", &DelayedDataFlags);
    Double_t DelayedPosX, DelayedPosY, DelayedPosZ, DelayedEnergy;
    intree->SetBranchAddress("DelayedPosX", &DelayedPosX);
    intree->SetBranchAddress("DelayedPosY", &DelayedPosY);
    intree->SetBranchAddress("DelayedPosZ", &DelayedPosZ);
    intree->SetBranchAddress("DelayedEnergy", &DelayedEnergy);
    Double_t Delayed_Original_Energy, Delayed_Tony_Energy;
    intree->SetBranchAddress("DelayedOriginalEnergy", &Delayed_Original_Energy);
    intree->SetBranchAddress("DelayedTonyEnergy", &Delayed_Tony_Energy);
    //intree->Print();
//Fill Data

    TH1D *prompt_energy = new TH1D("prompt energy", "", 30, 0.8, 8.0);
    TH1D *delayed_energy = new TH1D("delayed energy", "", 10, 1.8, 2.4);
    TH1D *delta_T = new TH1D("Delta T", "", 20, 0, 800);//\mu s
    TGraph *prompt_pos = new TGraph;
    TGraph *delayed_pos = new TGraph;
    TVector3 Prompt_Pos, Delayed_Pos;
    Double_t Delta_T;
    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        intree->GetEntry(ii1);
        //Energy
        prompt_energy->Fill(PromptEnergy);
        delayed_energy->Fill(DelayedEnergy);
        //Delta T
        Delta_T = 20.0*(Delayed50MHz - Prompt50MHz)/1e3;
        delta_T->Fill(Delta_T);
        //Position
        Prompt_Pos.SetXYZ(PromptPosX, PromptPosY, PromptPosZ);
        Delayed_Pos.SetXYZ(DelayedPosX, DelayedPosY, DelayedPosZ);
        prompt_pos->SetPoint(ii1, Prompt_Pos.Perp2(), Prompt_Pos.Z());
        delayed_pos->SetPoint(ii1, Delayed_Pos.Perp2(), Delayed_Pos.Z());
    };

    //Save Plots
    //SavePlot(OutPut_Prompt_Energy, prompt_energy, "Prompt", "E_{prompt}/MeV", "Prompt Energy", 0.8, 0.7, 0.9, 0.8);
    FitSavePlot_G(OutPut_Delayed_Energy, delayed_energy, "Data", "E_{delayed}/MeV", "Delayed Energy", 0.13, 0.7, 0.3, 0.85);
    FitSavePlot_E(OutPut_Delta_T, delta_T, "Data", "#Delta T/#mu s", "#Delta T", 0.5, 0.7, 0.8, 0.8);
    // SavePlot(OutPut_Position, delayed_pos, "Prompt", "z/mm", "#rho^2/mm^2");
    //SaveTwoPlots(OutPut_Position, prompt_pos, "Prompt", delayed_pos, "Delayed", "Position of Coincidence Pair", "#rho^{2}/mm^{2}", "z/mm", 0.8, 0.8, 0.9, 0.9);
    return 0;
}
