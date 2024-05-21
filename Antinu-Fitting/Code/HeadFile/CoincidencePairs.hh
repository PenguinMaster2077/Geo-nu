//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
//RAT
//Self-Defined
#include "./Result.hh"
#include "./Constant_Setting.hh"
#include "./HeadFile/GeneratingPDFs.hh"

void Find_Coincidence_Pairs()
{
    std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/LooseCut/300000-307612";
//Add Files
    TChain *chain = new TChain("output");
    chain->Add((InPWD + "/*").c_str());
//Set Branch Address
    //Run Info
    UInt_t run_id, subrun_id, date, time, run_type;
    chain->SetBranchAddress("RunID", &run_id);
    chain->SetBranchAddress("SubRunID", &subrun_id);
    chain->SetBranchAddress("Date", &date);
    chain->SetBranchAddress("Time", &time);
    chain->SetBranchAddress("RunType", &run_type);
    Double_t z_offset;
    chain->SetBranchAddress("ZOffSet", &z_offset);
    //Prompt
    Int_t prompt_entry, prompt_ev;
    chain->SetBranchAddress("PromptEntry", &prompt_entry);
    chain->SetBranchAddress("PromptEv", &prompt_ev);
    Int_t prompt_gtid, prompt_owlhits, prompt_nhits, prompt_nhits_cleaned, prompt_nhits_corrected;
    chain->SetBranchAddress("PromptGTID", &prompt_gtid);
    chain->SetBranchAddress("PromptOWLHits", &prompt_owlhits);
    chain->SetBranchAddress("PromptNHits", &prompt_nhits);
    chain->SetBranchAddress("PromptNHitsCleaned", &prompt_nhits_cleaned);
    chain->SetBranchAddress("PromptNHitsCorrected", &prompt_nhits_corrected);
    ULong64_t prompt_50mhz, prompt_data_applied, prompt_data_flags;
    chain->SetBranchAddress("Prompt50MHz", &prompt_50mhz);
    chain->SetBranchAddress("PromptDataApplied", &prompt_data_applied);
    chain->SetBranchAddress("PromptDataFlags", &prompt_data_flags);
    Double_t prompt_energy, prompt_original_energy, prompt_tony_energy;
    chain->SetBranchAddress("PromptEnergy", &prompt_energy);
    chain->SetBranchAddress("PromptOriginalEnergy", &prompt_original_energy);
    chain->SetBranchAddress("PromptTonyEnergy", &prompt_tony_energy);
    Double_t prompt_posx, prompt_posy, prompt_posz;
    chain->SetBranchAddress("PromptPosX", &prompt_posx);
    chain->SetBranchAddress("PromptPosY", &prompt_posy);
    chain->SetBranchAddress("PromptPosZ", &prompt_posz);
    //Delayed
    Int_t delayed_entry, delayed_ev;
    chain->SetBranchAddress("DelayedEntry", &delayed_entry);
    chain->SetBranchAddress("DelayedEv", &delayed_ev);
    Int_t delayed_gtid, delayed_owlhits, delayed_nhits, delayed_nhits_cleaned, delayed_nhits_corrected;
    chain->SetBranchAddress("DelayedGTID", &delayed_gtid);
    chain->SetBranchAddress("DelayedOWLHits", &delayed_owlhits);
    chain->SetBranchAddress("DelayedNHits", &delayed_nhits);
    chain->SetBranchAddress("DelayedNHitsCleaned", &delayed_nhits_cleaned);
    chain->SetBranchAddress("DelayedNHitsCorrected", &delayed_nhits_corrected);
    ULong64_t delayed_50mhz, delayed_data_applied, delayed_data_flags;
    chain->SetBranchAddress("Delayed50MHz", &delayed_50mhz);
    chain->SetBranchAddress("DelayedDataApplied", &delayed_data_applied);
    chain->SetBranchAddress("DelayedDataFlags", &delayed_data_flags);
    Double_t delayed_energy, delayed_original_energy, delayed_tony_energy;
    chain->SetBranchAddress("DelayedEnergy", &delayed_energy);
    chain->SetBranchAddress("DelayedOriginalEnergy", &delayed_original_energy);
    chain->SetBranchAddress("DelayedTonyEnergy", &delayed_tony_energy);
    Double_t delayed_posx, delayed_posy, delayed_posz;
    chain->SetBranchAddress("DelayedPosX", &delayed_posx);
    chain->SetBranchAddress("DelayedPosY", &delayed_posy);
    chain->SetBranchAddress("DelayedPosZ", &delayed_posz);
//Loop the Data
    Double_t Delta_T = 0;
    TVector3 Pos[2], Delta_R;
    Int_t Number_CoincidencePair = 0, Number_DataClean = 0, Number_Muon = 0;
    for(int ii1 = 0; ii1 < chain->GetEntries(); ii1++)
    {
        chain->GetEntry(ii1);
    //Energy Cut
        if(Prompt_Energy_Cut(prompt_energy) == false) {continue;};
        if(Delayed_Energy_Cut(delayed_energy) == false) {continue;};
    //Radius Cut
        Pos[0].SetXYZ(prompt_posx, prompt_posy, prompt_posz);
        Pos[1].SetXYZ(delayed_posx, delayed_posy, delayed_posz);
        if(Pos[0].Mag() > RADIUS_MAX){continue;};
        if(Pos[1].Mag() > RADIUS_MAX) {continue;};
    //Delta T Cut
        Delta_T = ComputeDelta_T(prompt_50mhz, delayed_50mhz);
        if(DeltaT_Cut(Delta_T) == false){continue;};
    //Delta R Cut
        Delta_R = Pos[1] - Pos[0];
        if(Delta_R.Mag() > DELTA_R_MAX){continue;};
        Number_CoincidencePair++;
    //Data Clean Cut
        if((prompt_data_flags & DATA_CLEANING_MASK) == DATA_CLEANING_MASK){continue;};
        if((delayed_data_flags & DATA_CLEANING_MASK) == DATA_CLEANING_MASK){continue;};        
        Number_DataClean++;
    //Muon Cut
    };
    std::cout << "After CP Cut:" << Number_CoincidencePair << ", After Data Cleaning:" << Number_DataClean << std::endl;
}


/* //Back Code for Testing
    std::cout << "Run:" << run_id << ", SubRun:" << subrun_id << ", Date:" << date << ", Time:" << time << ", Run Type:" << run_type << ", Z Offset:" << z_offset << std::endl;
    std::cout << "[Prompt Info]" << std::endl;
    std::cout << "Entry:" << prompt_entry << ", EV:" << prompt_ev << ", GTID:" << prompt_gtid << ", OWH:" << prompt_owlhits << std::endl;
    std::cout << "NHits:" << prompt_nhits << ", NHits Cleaned:" << prompt_nhits_cleaned << ", NHits Corrected:" << prompt_nhits_corrected << std::endl;
    std::cout << "50MHz:" << prompt_50mhz << ", Applied:" << prompt_data_applied << ", Flags:" << prompt_data_flags << std::endl;
    std::cout << "Energy:" << prompt_energy << ", Original Energy:" << prompt_original_energy << ", Tony Energy:" << prompt_tony_energy << std::endl;
    std::cout << "X:" << prompt_posx << ", Y:" << prompt_posy << ", Z:" << prompt_posz << std::endl << std::endl;
    std::cout << "[Delayed Info]" << std::endl;
    std::cout << "Entry:" << delayed_entry << ", EV:" << delayed_ev << ", GTID:" << delayed_gtid << ", OWH:" << delayed_owlhits << std::endl;
    std::cout << "NHits:" << delayed_nhits << ", NHits Cleaned:" << delayed_nhits_cleaned << ", NHits Corrected:" << delayed_nhits_corrected << std::endl;
    std::cout << "50MHz:" << delayed_50mhz << ", Applied:" << delayed_data_applied << ", Flags:" << delayed_data_flags << std::endl;
    std::cout << "Energy:" << delayed_energy << ", Original Energy:" << delayed_original_energy << ", Tony Energy:" << delayed_tony_energy << std::endl;
    std::cout << "X:" << delayed_posx << ", Y:" << delayed_posy << ", Z:" << delayed_posz << std::endl << std::endl;

*/