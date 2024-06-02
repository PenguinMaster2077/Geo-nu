#ifndef FIND_ACCIDENTAL_HH
#define FIND_ACCIDENTAL_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TVector3.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"
#include "./Base_Functions.hh"
#include "./Result.hh"

void Select_AC(std::string OutFile, std::string Prompt_PWD, std::string Delayed_PWD)
{
//Load Files
    TChain *chain_prompt = new TChain("output");
    TChain *chain_delayed = new TChain("output");
    chain_prompt->Add((Prompt_PWD + "/*").c_str());
    std::cout << "Prompt文件加载成功" << std::endl;
    chain_delayed->Add((Delayed_PWD + "/*").c_str());
    std::cout << "Delayed文件加载成功" << std::endl;
//Set Branch Address
    UInt_t run_id, subrun_id, date, run_type;
    chain_prompt->SetBranchAddress("RunID", &run_id);
    chain_prompt->SetBranchAddress("SubRunID", &subrun_id);
    chain_prompt->SetBranchAddress("Date", &date);
    
    chain_prompt->SetBranchAddress("RunType", &run_type);

    chain_delayed->SetBranchAddress("RunID", &run_id);
    chain_delayed->SetBranchAddress("SubRunID", &subrun_id);
    chain_delayed->SetBranchAddress("Date", &date);
    chain_delayed->SetBranchAddress("RunType", &run_type);
    Double_t z_offset;
    chain_prompt->SetBranchAddress("ZOffSet", &z_offset);

    chain_delayed->SetBranchAddress("ZOffSet", &z_offset);
    ULong64_t prompt_begin_count, prompt_end_count;
    chain_prompt->SetBranchAddress("BeginCount", &prompt_begin_count);
    chain_prompt->SetBranchAddress("EndCount", &prompt_end_count);
    Int_t prompt_entry, prompt_ev, prompt_gtid, prompt_owlhits, prompt_fitvalid, prompt_nhits, prompt_nhits_cleaned, prompt_nhits_corrected;
    chain_prompt->SetBranchAddress("PromptEntry", &prompt_entry);
    chain_prompt->SetBranchAddress("PromptEv", &prompt_ev);
    chain_prompt->SetBranchAddress("PromptGTID", &prompt_gtid);
    chain_prompt->SetBranchAddress("PromptOWLHits", &prompt_owlhits);
    chain_prompt->SetBranchAddress("PromptFitValid", &prompt_fitvalid);
    chain_prompt->SetBranchAddress("PromptNHits", &prompt_nhits);
    chain_prompt->SetBranchAddress("PromptNHitsCleaned", &prompt_nhits_cleaned);
    chain_prompt->SetBranchAddress("PromptNHitsCorrected", &prompt_nhits_corrected);
    ULong64_t prompt_clock50, prompt_data_applied, prompt_data_flags;
    chain_prompt->SetBranchAddress("Prompt50MHz", &prompt_clock50);
    chain_prompt->SetBranchAddress("PromptDataApplied", &prompt_data_applied);
    chain_prompt->SetBranchAddress("PromptDataFlags", &prompt_data_flags);
    Double_t prompt_energy, prompt_pos_x, prompt_pos_y, prompt_pos_z, prompt_original_energy, prompt_tony_energy;
    chain_prompt->SetBranchAddress("PromptEnergy", &prompt_energy);
    chain_prompt->SetBranchAddress("PromptPosX", &prompt_pos_x);
    chain_prompt->SetBranchAddress("PromptPosY", &prompt_pos_y);
    chain_prompt->SetBranchAddress("PromptPosZ", &prompt_pos_z);
    chain_prompt->SetBranchAddress("PromptOriginalEnergy", &prompt_original_energy);
    chain_prompt->SetBranchAddress("PromptTonyEnergy", &prompt_tony_energy);

    ULong64_t delayed_begin_count, delayed_end_count;
    chain_delayed->SetBranchAddress("BeginCount", &delayed_begin_count);
    chain_delayed->SetBranchAddress("EndCount", &delayed_end_count);    
    Int_t delayed_entry, delayed_ev, delayed_gtid, delayed_owlhits, delayed_fitvalid, delayed_nhits, delayed_nhits_cleaned, delayed_nhits_corrected;
    chain_delayed->SetBranchAddress("DelayedEntry", &delayed_entry);
    chain_delayed->SetBranchAddress("DelayedEv", &delayed_ev);
    chain_delayed->SetBranchAddress("DelayedGTID", &delayed_gtid);
    chain_delayed->SetBranchAddress("DelayedOWLHits", &delayed_owlhits);
    chain_delayed->SetBranchAddress("DelayedFitValid", &delayed_fitvalid);
    chain_delayed->SetBranchAddress("DelayedNHits", &delayed_nhits);
    chain_delayed->SetBranchAddress("DelayedNHitsCleaned", &delayed_nhits_cleaned);
    chain_delayed->SetBranchAddress("DelayedNHitsCorrected", &delayed_nhits_corrected);
    ULong64_t delayed_clock50, delayed_data_applied, delayed_data_flags;
    chain_delayed->SetBranchAddress("Delayed50MHz", &delayed_clock50);
    chain_delayed->SetBranchAddress("DelayedDataApplied", &delayed_data_applied);
    chain_delayed->SetBranchAddress("DelayedDataFlags", &delayed_data_flags);
    Double_t delayed_energy, delayed_pos_x, delayed_pos_y, delayed_pos_z, delayed_original_energy, delayed_tony_energy;
    chain_delayed->SetBranchAddress("DelayedEnergy", &delayed_energy);
    chain_delayed->SetBranchAddress("DelayedPosX", &delayed_pos_x);
    chain_delayed->SetBranchAddress("DelayedPosY", &delayed_pos_y);
    chain_delayed->SetBranchAddress("DelayedPosZ", &delayed_pos_z);
    chain_delayed->SetBranchAddress("DelayedOriginalEnergy", &delayed_original_energy);
    chain_delayed->SetBranchAddress("DelayedTonyEnergy", &delayed_tony_energy);
//Create OutPut File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "");
    std::cout << "输出文件创建成功" << std::endl;
    //Set Branch Address
    myresult res;
    outtree->Branch("RunID", &res.C_RunID);
    outtree->Branch("SubRunID", &res.C_SubRunID);
    outtree->Branch("Date", &res.C_Date);
    // outtree->Branch("Time", &res.C_Time);
    outtree->Branch("RunType", &res.C_RunType);
    outtree->Branch("ZOffSet", &res.C_ZOffSet);
    //Prompt
    outtree->Branch("PromptEntry", &res.C_Prompt.C_Entry);
    outtree->Branch("PromptEv", &res.C_Prompt.C_EV);
    outtree->Branch("PromptGTID", &res.C_Prompt.C_GTID);
    outtree->Branch("PromptOWLHits", &res.C_Prompt.C_OWLHits);
    outtree->Branch("PromptNHits", &res.C_Prompt.C_Nhits);
    outtree->Branch("PromptNHitsCleaned", &res.C_Prompt.C_Nhits_Cleaned);
    outtree->Branch("PromptNHitsCorrected", &res.C_Prompt.C_Nhits_Corrected);
    outtree->Branch("Prompt50MHz", &res.C_Prompt.C_50MHZ);
    outtree->Branch("PromptDataApplied", &res.C_Prompt.C_Data_Applied);
    outtree->Branch("PromptDataFlags", &res.C_Prompt.C_Data_Flags);
    outtree->Branch("PromptEnergy", &res.C_Prompt.C_Energy);
    outtree->Branch("PromptOriginalEnergy", &res.C_Prompt.C_Original_Energy);
    outtree->Branch("PromptTonyEnergy", &res.C_Prompt.C_Tony_Energy);
    outtree->Branch("PromptPosX", &res.C_Prompt.C_Pos_X);
    outtree->Branch("PromptPosY", &res.C_Prompt.C_Pos_Y);
    outtree->Branch("PromptPosZ", &res.C_Prompt.C_Pos_Z);
    //Delayed
    outtree->Branch("DelayedEntry", &res.C_Delayed.C_Entry);
    outtree->Branch("DelayedEv", &res.C_Delayed.C_EV);
    outtree->Branch("DelayedGTID", &res.C_Delayed.C_GTID);
    outtree->Branch("DelayedOWLHits", &res.C_Delayed.C_OWLHits);
    outtree->Branch("DelayedNHits", &res.C_Delayed.C_Nhits);
    outtree->Branch("DelayedNHitsCleaned", &res.C_Delayed.C_Nhits_Cleaned);
    outtree->Branch("DelayedNHitsCorrected", &res.C_Delayed.C_Nhits_Corrected);
    outtree->Branch("Delayed50MHz", &res.C_Delayed.C_50MHZ);
    outtree->Branch("DelayedDataApplied", &res.C_Delayed.C_Data_Applied);
    outtree->Branch("DelayedDataFlags", &res.C_Delayed.C_Data_Flags);
    outtree->Branch("DelayedEnergy", &res.C_Delayed.C_Energy);
    outtree->Branch("DelayedOriginalEnergy", &res.C_Delayed.C_Original_Energy);
    outtree->Branch("DelayedTonyEnergy", &res.C_Delayed.C_Tony_Energy);
    outtree->Branch("DelayedPosX", &res.C_Delayed.C_Pos_X);
    outtree->Branch("DelayedPosY", &res.C_Delayed.C_Pos_Y);
    outtree->Branch("DelayedPosZ", &res.C_Delayed.C_Pos_Z);
//Loop the Delayed
    std::cout << "开始循环" << std::endl;
    TVector3 Pos_Prompt, Pos_Delayed;
    int len_delayed = chain_delayed->GetEntries();
    Int_t Number_Delayed_Final = 0;
    for(int ii1= 0; ii1 < chain_delayed->GetEntries(); ii1++)
    {
        if(ii1 % 10000 == 0){std::cout << "[Find Delayed-Like Loop] 已处理" << double(100.0 * ii1/len_delayed) << "%，具体：" << ii1 << "，还剩" << len_delayed - ii1 << std::endl;};
        chain_delayed->GetEntry(ii1);
    //Fit Valid
        if(delayed_fitvalid != 1){continue;};
        Pos_Delayed.SetXYZ(delayed_pos_x, delayed_pos_y, delayed_pos_z);
    //Energy Cut
        if(Delayed_Energy_Cut(delayed_energy) == false){continue;};
    //Radius Cut
        if(Pos_Delayed.Mag() > RADIUS_MAX){continue;};
    //Data Clean Mask
        if( (delayed_data_flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK){continue;};
        Number_Delayed_Final++;
    };
//Loop the Prompt-like and Find CP
    int len_prompt = chain_prompt->GetEntries();
    Int_t Number_Prompt_Final = 0, Number_IBD_Final = 0;
    Int_t Index_Last_Delayed, Run_Prompt, SubRun_Prompt;
    ULong64_t Prompt_50MHz;
    Double_t Delta_T;
    TVector3 Delta_R;
    for(int ii1 = 0; ii1 < chain_prompt->GetEntries(); ii1++)
    {
        if(ii1 % 10000 == 0){std::cout << "[Find Prompt-Like Loop] 已处理" << double(100.0 * ii1/len_prompt) << "%，具体：" << ii1 << "，还剩" << len_prompt - ii1 << std::endl;};
        chain_prompt->GetEntry(ii1);
    //Fit Valid
        if(prompt_fitvalid != 1){continue;};
        Pos_Prompt.SetXYZ(prompt_pos_x, prompt_pos_y, prompt_pos_z);
    //Energy Cut
        if(Prompt_Energy_Cut(prompt_energy) == false){continue;};
    //Radius Cut
        if(Pos_Prompt.Mag() > RADIUS_MAX){continue;};
    //Data Clean Mask
        if( (prompt_data_flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK){continue;};
        Number_Prompt_Final++;
    //Record Prompt Info
        Prompt_50MHz = prompt_clock50;
        Run_Prompt = run_id;
        SubRun_Prompt = subrun_id;

        res.C_RunID = run_id;
        res.C_SubRunID = subrun_id;
        res.C_Date = date;
        // res.C_Time = ;//?????????????
        res.C_RunType = run_type;
        res.C_ZOffSet = z_offset;
        //Prompt
        res.C_Prompt.C_Entry = prompt_entry;
        res.C_Prompt.C_EV = prompt_ev;
        res.C_Prompt.C_GTID = prompt_gtid;
        res.C_Prompt.C_OWLHits = prompt_owlhits;
        res.C_Prompt.C_Nhits = prompt_nhits;
        res.C_Prompt.C_Nhits_Cleaned = prompt_nhits_cleaned;
        res.C_Prompt.C_Nhits_Corrected = prompt_nhits_cleaned;
        res.C_Prompt.C_50MHZ = prompt_clock50;
        res.C_Prompt.C_Data_Applied = prompt_data_applied;
        res.C_Prompt.C_Data_Flags = prompt_data_flags;
        res.C_Prompt.C_Energy = prompt_energy;
        res.C_Prompt.C_Pos_X = prompt_pos_x;
        res.C_Prompt.C_Pos_Y = prompt_pos_y;
        res.C_Prompt.C_Pos_Z = prompt_pos_z;
        res.C_Prompt.C_Original_Energy = prompt_original_energy;
        res.C_Prompt.C_Tony_Energy = prompt_tony_energy;
    //Find Delayed Event
        for(int iDelayed = Index_Last_Delayed; iDelayed < chain_delayed->GetEntries(); iDelayed++)
        {
            chain_delayed->GetEntry(iDelayed);
        //Boost to Same Run and SubRun
            if(run_id < Run_Prompt){continue;};
            if(subrun_id < SubRun_Prompt){continue;}
            else if(subrun_id > SubRun_Prompt)
            {
                Index_Last_Delayed = iDelayed - 1;
                break;
            };
        //Fit Valid
            if(delayed_fitvalid != 1){continue;};
            Pos_Delayed.SetXYZ(delayed_pos_x, delayed_pos_y, delayed_pos_z);
        //Energy Cut
            if(Delayed_Energy_Cut(delayed_energy) == false){continue;};
        //Radius Cut
            if(Pos_Delayed.Mag() > RADIUS_MAX){continue;};
        //Delta T Cut
            Delta_T = ComputeDelta_T(Prompt_50MHz, delayed_clock50);
            if(Delta_T < 0){continue;};
            if(Delta_T > 2e6)
            {
                Index_Last_Delayed = iDelayed;
                // std::cout << "超时" << std::endl;
                //std::cout << "超时, Delta T(s)" << Delta_T/1e9 << ", Delayed Index:" << Index_Last_Delayed << std::endl;
                //std::cout << "Prompt: Run:" << Run_Prompt << ", SubRun:" << SubRun_Prompt << ", 50MHz:" << Prompt_50MHz << std::endl;
                //std::cout << "Delayed: Run:" << run_id << ", SubRun:" << subrun_id << ", 50MHz:" << clock50 << std::endl << std::endl;
                break;
            };
            if(DeltaT_Cut(Delta_T) == false){continue;};
        //Delta R Cut
            Delta_R = Pos_Delayed - Pos_Prompt;
            if(Delta_R.Mag() > DELTA_R_MAX){continue;};
            Index_Last_Delayed = iDelayed;
            Number_IBD_Final++;
        //Record Delayed Info
            res.C_Delayed.C_Entry = delayed_entry;
            res.C_Delayed.C_EV = delayed_ev;
            res.C_Delayed.C_GTID = delayed_gtid;
            res.C_Delayed.C_OWLHits = delayed_owlhits;
            res.C_Delayed.C_Nhits = delayed_nhits;
            res.C_Delayed.C_Nhits_Cleaned = delayed_nhits_cleaned;
            res.C_Delayed.C_Nhits_Corrected = delayed_nhits_cleaned;
            res.C_Delayed.C_50MHZ = delayed_clock50;
            res.C_Delayed.C_Data_Applied = delayed_data_applied;
            res.C_Delayed.C_Data_Flags = delayed_data_flags;
            res.C_Delayed.C_Energy = delayed_energy;
            res.C_Delayed.C_Pos_X = delayed_pos_x;
            res.C_Delayed.C_Pos_Y = delayed_pos_y;
            res.C_Delayed.C_Pos_Z = delayed_pos_z;
            res.C_Delayed.C_Original_Energy = delayed_original_energy;
            res.C_Delayed.C_Tony_Energy = delayed_tony_energy;
            std::cout << std::endl << "[Find Coincidence Pairs] 找到IBD对" << std::endl << std::endl;
            // res.C_Prompt.ShowDetals();
            // res.C_Delayed.ShowDetals();
            // std::cout << "Delta T(ms):" << Delta_T/1e6 << std::endl;
            outtree->Fill();
        };
    };
    outfile->Write();
    outfile->Close();
    std::cout << "Final Prompt-Like:" << Number_Prompt_Final << ", Total:" << len_prompt << std::endl;
    std::cout << "Final Delayed-Like:" << Number_Delayed_Final << ", Total:" << len_delayed << std::endl;
    std::cout << "Final Coincidence Pairs:" << Number_IBD_Final << std::endl;
}
#endif