//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>

void FitDletaT()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/MC/Reactor_20240403/Reactor_CoincidencePair.root";

    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");

    Long64_t Prompt50MHz, Delayed50MHz;
    intree->SetBranchAddress("Prompt50MHz", &Prompt50MHz);
    intree->SetBranchAddress("Delayed50MHz", &Delayed50MHz);

    TH1D *DeltaT_Hist = new TH1D("DeltaT", "", 500, 0, 1000);//mu s
    
    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        Double_t DeltaT = 20.0 * (Delayed50MHz - Prompt50MHz)/1e3;// mu s
        DeltaT_Hist->Fill(DeltaT);
    };
    DeltaT_Hist->Scale(1.0/DeltaT_Hist->Integral());
    

}

int Check_Muon()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/test/Gold_20R_Muon_0000300000-0000300050.root";
    std::string InFile2 = "/rat/MyCode/Work/Geo-nu-Data/test/Zombie_Gold_20R_Muon_0000300000-0000300050.root";

    TFile *file1 = new TFile(InFile.c_str());
    TTree *intree1 = (TTree*) file1->Get("output");
    TFile *file2 = new TFile(InFile2.c_str());
    TTree *intree2 = (TTree*) file2->Get("output");

    Int_t GTID_Muon, GTID_Zombie;
    intree1->SetBranchAddress("MuonGTID", &GTID_Muon);
    intree2->SetBranchAddress("MuonGTID", &GTID_Zombie);
    Int_t NHits_Muon, NHits_Zombie;
    intree1->SetBranchAddress("MuonNHits", &NHits_Muon);
    intree2->SetBranchAddress("MuonNHits", &NHits_Zombie);
    Long64_t Clock50MHz_Muon, Clock50MHz_Zombie;
    intree1->SetBranchAddress("Muon50MHz", &Clock50MHz_Muon);
    intree2->SetBranchAddress("Muon50MHz", &Clock50MHz_Zombie);
    Long64_t Applied_Muon, Applied_Zombie;
    intree1->SetBranchAddress("MuonDataApplied", &Applied_Muon);
    intree2->SetBranchAddress("MuonDataApplied", &Applied_Zombie);
    Long64_t Flags_Muon, Flags_Zombie;
    intree1->SetBranchAddress("MuonDataFlags", &Flags_Muon);
    intree2->SetBranchAddress("MuonDataFlags", &Flags_Zombie);


    for(int ii1 = 0; ii1 < intree1->GetEntries(); ii1++)
    {
        intree1->GetEntry(ii1);
        intree2->GetEntry(ii1);
        std::cout << GTID_Muon << "," << NHits_Muon << "," << Clock50MHz_Muon << "," << Applied_Muon << "," << Flags_Muon << std::endl;
        if( GTID_Muon != GTID_Zombie){std::cout << GTID_Muon << "," << GTID_Zombie << std::endl;};
        if( NHits_Muon != NHits_Zombie){std::cout << NHits_Muon << "," << NHits_Zombie << std::endl;};
        if( Clock50MHz_Muon != Clock50MHz_Zombie){std::cout << Clock50MHz_Muon << "," << Clock50MHz_Zombie << std::endl;};
        if( Applied_Muon != Applied_Zombie){std::cout << Applied_Muon << "," << Applied_Zombie << std::endl;};
        if( Flags_Muon != Flags_Zombie){std::cout << Flags_Muon << "," << Flags_Zombie << std::endl;};
    };
    

    return 0;
};

void Compare(Int_t v1, Int_t v2)
{
    if(v1 != v2){ std::cout << v1 << "," << v2 << std::endl;};
};
void Compare(Double_t v1, Double_t v2)
{
    if(v1 != v2){ std::cout << v1 << "," << v2 << std::endl;};
};
void Compare(Long64_t v1, Long64_t v2)
{
    if(v1 != v2){ std::cout << v1 << "," << v2 << std::endl;};
};

int Check()
{
    std::string InFile = "./Gold_20R_CoincidencePair_0000300000_s011.root";
    std::string InFile2 = "./test.root";

    TFile *file1 = new TFile(InFile.c_str());
    TTree *intree1 = (TTree*) file1->Get("output");
    TFile *file2 = new TFile(InFile2.c_str());
    TTree *intree2 = (TTree*) file2->Get("output");

    Double_t ZOffSet_CP, ZOffSet_Zombie;
    intree1->SetBranchAddress("ZOffSet", &ZOffSet_CP);
    intree2->SetBranchAddress("ZOffSet", &ZOffSet_Zombie);
    Int_t PromptEntry_CP, PromptEntry_Zombie;
    intree1->SetBranchAddress("PromptEntry", &PromptEntry_CP);
    intree2->SetBranchAddress("PromptEntry", &PromptEntry_Zombie);
    Int_t PromptEv_CP, PromptEv_Zombie;
    intree1->SetBranchAddress("PromptEv", &PromptEv_CP);
    intree2->SetBranchAddress("PromptEv", &PromptEv_Zombie);
    Int_t PromptGTID_CP, PromptGTID_Zombie;
    intree1->SetBranchAddress("PromptGTID", &PromptGTID_CP);
    intree2->SetBranchAddress("PromptGTID", &PromptGTID_Zombie);
    Int_t PromptOWLHits_CP, PromptOWLHits_Zombie;
    intree1->SetBranchAddress("PromptOWLHits", &PromptOWLHits_CP);
    intree2->SetBranchAddress("PromptOWLHits", &PromptOWLHits_Zombie);
    Int_t PromptNHits_CP, PromptNHits_Zombie;
    intree1->SetBranchAddress("PromptNHits", &PromptNHits_CP);
    intree2->SetBranchAddress("PromptNHits", &PromptNHits_Zombie);
    Int_t PromptNHitsCleaned_CP, PromptNHitsCleaned_Zombie;
    intree1->SetBranchAddress("PromptNHitsCleaned", &PromptNHitsCleaned_CP);
    intree2->SetBranchAddress("PromptNHitsCleaned", &PromptNHitsCleaned_Zombie);
    Long64_t Prompt50MHz_CP, Prompt50MHz_Zombie;
    intree1->SetBranchAddress("Prompt50MHz", &Prompt50MHz_CP);
    intree2->SetBranchAddress("Prompt50MHz", &Prompt50MHz_Zombie);
    Long64_t PromptApplied_CP, PromptApplied_Zombie;
    intree1->SetBranchAddress("PromptDataApplied", &PromptApplied_CP);
    intree2->SetBranchAddress("PromptDataApplied", &PromptApplied_Zombie);
    Long64_t PromptFlags_CP, PromptFlags_Zombie;
    intree1->SetBranchAddress("PromptDataFlags", &PromptFlags_CP);
    intree2->SetBranchAddress("PromptDataFlags", &PromptFlags_Zombie);
    Double_t PromptEnergy_CP, PromptEnergy_Zombie;
    intree1->SetBranchAddress("PromptEnergy", &PromptEnergy_CP);
    intree2->SetBranchAddress("PromptEnergy", &PromptEnergy_Zombie);
    Double_t PromptOriginalEnergy_CP, PromptOriginalEnergy_Zombie;
    intree1->SetBranchAddress("PromptOriginalEnergy", &PromptOriginalEnergy_CP);
    intree2->SetBranchAddress("PromptOriginalEnergy", &PromptOriginalEnergy_Zombie);
    Double_t PromptTonyEnergy_CP, PromptTonyEnergy_Zombie;
    intree1->SetBranchAddress("PromptTonyEnergy", &PromptTonyEnergy_CP);
    intree2->SetBranchAddress("PromptTonyEnergy", &PromptTonyEnergy_Zombie); 
    Double_t PromptPosX_CP, PromptPosX_Zombie;
    intree1->SetBranchAddress("PromptPosX", &PromptPosX_CP);
    intree2->SetBranchAddress("PromptPosX", &PromptPosX_Zombie);
    Double_t PromptPosY_CP, PromptPosY_Zombie;
    intree1->SetBranchAddress("PromptPosY", &PromptPosY_CP);
    intree2->SetBranchAddress("PromptPosY", &PromptPosY_Zombie);
    Double_t PromptPosZ_CP, PromptPosZ_Zombie;
    intree1->SetBranchAddress("PromptPosZ", &PromptPosZ_CP);
    intree2->SetBranchAddress("PromptPosZ", &PromptPosZ_Zombie);

    Int_t DelayedEntry_CP, DelayedEntry_Zombie;
    intree1->SetBranchAddress("DelayedEntry", &DelayedEntry_CP);
    intree2->SetBranchAddress("DelayedEntry", &DelayedEntry_Zombie);
    Int_t DelayedEv_CP, DelayedEv_Zombie;
    intree1->SetBranchAddress("DelayedEv", &DelayedEv_CP);
    intree2->SetBranchAddress("DelayedEv", &DelayedEv_Zombie);
    Int_t DelayedGTID_CP, DelayedGTID_Zombie;
    intree1->SetBranchAddress("DelayedGTID", &DelayedGTID_CP);
    intree2->SetBranchAddress("DelayedGTID", &DelayedGTID_Zombie);
    Int_t DelayedOWLHits_CP, DelayedOWLHits_Zombie;
    intree1->SetBranchAddress("DelayedOWLHits", &DelayedOWLHits_CP);
    intree2->SetBranchAddress("DelayedOWLHits", &DelayedOWLHits_Zombie);
    Int_t DelayedNHits_CP, DelayedNHits_Zombie;
    intree1->SetBranchAddress("DelayedNHits", &DelayedNHits_CP);
    intree2->SetBranchAddress("DelayedNHits", &DelayedNHits_Zombie);
    Int_t DelayedNHitsCleaned_CP, DelayedNHitsCleaned_Zombie;
    intree1->SetBranchAddress("DelayedNHitsCleaned", &DelayedNHitsCleaned_CP);
    intree2->SetBranchAddress("DelayedNHitsCleaned", &DelayedNHitsCleaned_Zombie);
    Long64_t Delayed50MHz_CP, Delayed50MHz_Zombie;
    intree1->SetBranchAddress("Delayed50MHz", &Delayed50MHz_CP);
    intree2->SetBranchAddress("Delayed50MHz", &Delayed50MHz_Zombie);
    Long64_t DelayedApplied_CP, DelayedApplied_Zombie;
    intree1->SetBranchAddress("DelayedDataApplied", &DelayedApplied_CP);
    intree2->SetBranchAddress("DelayedDataApplied", &DelayedApplied_Zombie);
    Long64_t DelayedFlags_CP, DelayedFlags_Zombie;
    intree1->SetBranchAddress("DelayedDataFlags", &DelayedFlags_CP);
    intree2->SetBranchAddress("DelayedDataFlags", &DelayedFlags_Zombie);
    Double_t DelayedEnergy_CP, DelayedEnergy_Zombie;
    intree1->SetBranchAddress("DelayedEnergy", &DelayedEnergy_CP);
    intree2->SetBranchAddress("DelayedEnergy", &DelayedEnergy_Zombie);
    Double_t DelayedOriginalEnergy_CP, DelayedOriginalEnergy_Zombie;
    intree1->SetBranchAddress("DelayedOriginalEnergy", &DelayedOriginalEnergy_CP);
    intree2->SetBranchAddress("DelayedOriginalEnergy", &DelayedOriginalEnergy_Zombie);
    Double_t DelayedTonyEnergy_CP, DelayedTonyEnergy_Zombie;
    intree1->SetBranchAddress("DelayedTonyEnergy", &DelayedTonyEnergy_CP);
    intree2->SetBranchAddress("DelayedTonyEnergy", &DelayedTonyEnergy_Zombie); 
    Double_t DelayedPosX_CP, DelayedPosX_Zombie;
    intree1->SetBranchAddress("DelayedPosX", &DelayedPosX_CP);
    intree2->SetBranchAddress("DelayedPosX", &DelayedPosX_Zombie);
    Double_t DelayedPosY_CP, DelayedPosY_Zombie;
    intree1->SetBranchAddress("DelayedPosY", &DelayedPosY_CP);
    intree2->SetBranchAddress("DelayedPosY", &DelayedPosY_Zombie);
    Double_t DelayedPosZ_CP, DelayedPosZ_Zombie;
    intree1->SetBranchAddress("DelayedPosZ", &DelayedPosZ_CP);
    intree2->SetBranchAddress("DelayedPosZ", &DelayedPosZ_Zombie);


    for(int ii1 = 0; ii1 < intree1->GetEntries(); ii1++)
    {
        intree1->GetEntry(ii1);
        intree2->GetEntry(ii1);
        Compare(ZOffSet_CP, ZOffSet_Zombie);
        Compare(PromptEntry_CP, PromptEntry_Zombie);
        Compare(PromptEv_CP, PromptEv_Zombie);
        Compare(PromptOWLHits_CP, PromptOWLHits_Zombie);
        Compare(PromptNHits_CP, PromptNHits_Zombie);
        Compare(PromptNHitsCleaned_CP, PromptNHitsCleaned_Zombie);
        Compare(Prompt50MHz_CP, Prompt50MHz_Zombie);
        Compare(PromptApplied_CP, PromptApplied_Zombie);
        // Compare(PromptEnergy_CP, PromptEnergy_Zombie);
        // Compare(PromptOriginalEnergy_CP, PromptOriginalEnergy_Zombie);
        // Compare(PromptTonyEnergy_CP, PromptTonyEnergy_Zombie);
        Compare(PromptPosX_CP, PromptPosX_Zombie);
        Compare(PromptPosY_CP, PromptPosY_Zombie);
        Compare(PromptPosZ_CP, PromptPosZ_Zombie);

        Compare(DelayedEntry_CP, DelayedEntry_Zombie);
        Compare(DelayedEv_CP, DelayedEv_Zombie);
        Compare(DelayedOWLHits_CP, DelayedOWLHits_Zombie);
        Compare(DelayedNHits_CP, DelayedNHits_Zombie);
        Compare(DelayedNHitsCleaned_CP, DelayedNHitsCleaned_Zombie);
        Compare(Delayed50MHz_CP, Delayed50MHz_Zombie);
        Compare(DelayedApplied_CP, DelayedApplied_Zombie);
        Compare(DelayedEnergy_CP, DelayedEnergy_Zombie);
        Compare(DelayedOriginalEnergy_CP, DelayedOriginalEnergy_Zombie);
        Compare(DelayedTonyEnergy_CP, DelayedTonyEnergy_Zombie);
        Compare(DelayedPosX_CP, DelayedPosX_Zombie);
        Compare(DelayedPosY_CP, DelayedPosY_Zombie);
        Compare(DelayedPosZ_CP, DelayedPosZ_Zombie);
    };
    

    return 0;
}
