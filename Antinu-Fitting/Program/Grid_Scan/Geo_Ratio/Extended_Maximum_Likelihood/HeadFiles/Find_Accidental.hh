#ifndef FIND_ACCIDENTAL_HH
#define FIND_ACCIDENTAL_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH1D.h>
#include <TVector3.h>
#include <TRandom.h>
#include <TDatime.h>
//RAT
//Self-Defined
#include "./Constants_Setting.hh"
#include "./Base_Functions.hh"
#include "./Result.hh"
#include "./Plot_Code.hh"

void Data_Cleaning_Cut(std::string OutPWD, std::string Prompt_PWD, std::string Delayed_PWD)
{
//Load Prompt Files
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Load Prompt Files" << std::endl;
    TChain *chain_prompt = new TChain("output");
    chain_prompt->Add((Prompt_PWD + "/*").c_str());
//Set Branch Address;
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Set up Branch Address" << std::endl;
    ULong64_t Prompt_Flags;
    chain_prompt->SetBranchAddress("PromptDataFlags", &Prompt_Flags);
//Set Prompt Output File
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Create Prompt Output File" << std::endl;
    TFile *outfile_prompt = new TFile((OutPWD + "/Prompt/Gold_AC_Loose_CP_Data_Prompt.root").c_str(), "recreate");
    TTree *outtree_prompt = chain_prompt->CloneTree(0);
//Loop Prompt Data
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Loop Prompt Data" << std::endl;
    for(int iPrompt = 0; iPrompt < chain_prompt->GetEntries(); iPrompt++)
    {
        chain_prompt->GetEntry(iPrompt);
        if( (Prompt_Flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK ){continue;};
        outtree_prompt->Fill();
    };
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Total:" << chain_prompt->GetEntries() << ", After Data Cleaning:" << outtree_prompt->GetEntries() << std::endl;
    outfile_prompt->Write();
    outfile_prompt->Close();
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Complete Processing Prompt Files" << std::endl;

//Load Delayed Files
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Load Delayed Files" << std::endl;
    TChain *chain_delayed = new TChain("output");
    chain_delayed->Add((Delayed_PWD + "/*").c_str());
//Set Branch Address
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Set up Branch Address" << std::endl;
    ULong64_t Delayed_Flags;
    chain_delayed->SetBranchAddress("DelayedDataFlags", &Delayed_Flags);
//Set Delyaed Output File
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Create Delayed Output File" << std::endl;
    TFile *outfile_delayed = new TFile( (OutPWD + "/Delayed/Gold_AC_Loose_CP_Data_Delayed.root").c_str(), "recreate");
    TTree *outtree_delayed = chain_delayed->CloneTree(0);
//Loop Delayed Data
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Loop Delayed Data" << std::endl;
    for(int iDelayed = 0; iDelayed < chain_delayed->GetEntries(); iDelayed++)
    {
        chain_delayed->GetEntry(iDelayed);
        if( (Delayed_Flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK) {continue;};
        outtree_delayed->Fill();
    };
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Total:" << chain_delayed->GetEntries() << ", After Data Cleaning:" << outtree_delayed->GetEntries() << std::endl;
    outfile_delayed->Write();
    outfile_delayed->Close();
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Complete Processing Delayed Files" << std::endl;
};

void Randomly_Generate_AC_Pairs(std::string OutFile, std::string InPWD)
{
    std::string Prompt_Files = InPWD + "/Prompt/Gold_AC_Loose_CP_Data_Prompt.root";
    std::string Delayed_Files = InPWD + "/Delayed/Gold_AC_Loose_CP_Data_Delayed.root";
    // std::string OutFile = "./test.root";
//Read Prompt Files
    TFile *infile_prompt = new TFile(Prompt_Files.c_str());
    TTree *intree_prompt = (TTree*) infile_prompt->Get("output");
    //Set Branch Address
    UInt_t prompt_run_id, prompt_subrun_id, prompt_date, prompt_run_type;
    intree_prompt->SetBranchAddress("RunID", &prompt_run_id);
    intree_prompt->SetBranchAddress("SubRunID", &prompt_subrun_id);
    intree_prompt->SetBranchAddress("Date", &prompt_date);
    intree_prompt->SetBranchAddress("RunType", &prompt_run_type);
    Double_t prompt_Z_offset;
    intree_prompt->SetBranchAddress("ZOffSet", &prompt_Z_offset);
    ULong64_t prompt_begin_count, prompt_end_count;
    intree_prompt->SetBranchAddress("BeginCount", &prompt_begin_count);
    intree_prompt->SetBranchAddress("EndCount", &prompt_end_count);
    ULong64_t prompt_50, prompt_applied, prompt_flags;
    intree_prompt->SetBranchAddress("Prompt50MHz", &prompt_50);
    intree_prompt->SetBranchAddress("PromptDataApplied", &prompt_applied);
    intree_prompt->SetBranchAddress("PromptDataFlags", &prompt_flags);
    Int_t prompt_entry, prompt_ev, prompt_gtid, prompt_owlhits, prompt_nhits, prompt_nhits_cleaned, prompt_nhits_corrected;
    intree_prompt->SetBranchAddress("PromptEntry", &prompt_entry);
    intree_prompt->SetBranchAddress("PromptEv", &prompt_ev);
    intree_prompt->SetBranchAddress("PromptGTID", &prompt_gtid);
    intree_prompt->SetBranchAddress("PromptOWLHits", &prompt_owlhits);
    intree_prompt->SetBranchAddress("PromptNHits", &prompt_nhits);
    intree_prompt->SetBranchAddress("PromptNHitsCleaned", &prompt_nhits_cleaned);
    intree_prompt->SetBranchAddress("PromptNHitsCorrected", &prompt_nhits_corrected);
    Double_t prompt_energy, prompt_pos_x, prompt_pos_y, prompt_pos_z, prompt_original_energy, prompt_tony_energy;
    intree_prompt->SetBranchAddress("PromptEnergy", &prompt_energy);
    intree_prompt->SetBranchAddress("PromptPosX", &prompt_pos_x);
    intree_prompt->SetBranchAddress("PromptPosY", &prompt_pos_y);
    intree_prompt->SetBranchAddress("PromptPosZ", &prompt_pos_z);
    intree_prompt->SetBranchAddress("PromptOriginalEnergy", &prompt_original_energy);
    intree_prompt->SetBranchAddress("PromptTonyEnergy", &prompt_tony_energy);

    std::cout << "[Find_Accidental::Randomly_Generate_AC_Pairs] Load Prompt Files" << std::endl;
    //Record Index and Runs
    std::vector<Int_t> prompt_index, prompt_run;
    std::vector<ULong64_t> prompt_begin, prompt_end;
    Int_t Index = 0;
    Int_t Last_Prompt_Run;
    for(Int_t iPrompt = 0; iPrompt < intree_prompt->GetEntries(); iPrompt++)
    {
        intree_prompt->GetEntry(iPrompt);
        if(iPrompt == 0)
        {
            prompt_index.push_back(iPrompt);
            prompt_run.push_back(prompt_run_id);
            prompt_begin.push_back(prompt_begin_count);
            prompt_end.push_back(prompt_end_count);
            Last_Prompt_Run = prompt_run_id;

            continue;
        };
        if( Last_Prompt_Run == prompt_run_id)
        {
            prompt_end.at(Index) = prompt_end_count;
            continue;
        }
        else if( Last_Prompt_Run != prompt_run_id)
        {
            prompt_index.push_back(iPrompt);
            prompt_run.push_back(prompt_run_id);
            prompt_begin.push_back(prompt_begin_count);
            prompt_end.push_back(prompt_end_count);
            Last_Prompt_Run = prompt_run_id;

            Index++;
        };
    };
    prompt_index.push_back(intree_prompt->GetEntries());
    //Prompt Rate
    std::vector<Double_t> prompt_rate;
    Double_t temp_duration;
    Int_t temp_total;
    for(int ii1 = 0; ii1 < prompt_run.size(); ii1++)
    {
        temp_duration = Compute_Delta_T(prompt_begin.at(ii1), prompt_end.at(ii1))/1e9;
        temp_total = prompt_index.at(ii1 + 1) - prompt_index.at(ii1) + 1;
        prompt_rate.push_back(1.0 * temp_total / temp_duration);
    };
    // for(Int_t index = 0; index < prompt_run.size(); index++)
    // {
    //     std::cout << "Run:" << prompt_run.at(index) << ", Index:" << prompt_index.at(index) << ", Duration:"<< Compute_Delta_T(prompt_begin.at(index), prompt_end.at(index))/1e9 << ", Rate:" << prompt_rate.at(index) << std::endl;
    // };
//Read Delayed Files
    TFile *infile_delayed = new TFile(Delayed_Files.c_str());
    TTree *intree_delayed = (TTree*) infile_delayed->Get("output");
    //Set Branch Address
    UInt_t delayed_run_id, delayed_subrun_id, delayed_date, delayed_run_type;
    intree_delayed->SetBranchAddress("RunID", &delayed_run_id);
    intree_delayed->SetBranchAddress("SubRunID", &delayed_subrun_id);
    intree_delayed->SetBranchAddress("Date", &delayed_date);
    intree_delayed->SetBranchAddress("RunType", &delayed_run_type);
    Double_t delayed_Z_offset;
    intree_delayed->SetBranchAddress("ZOffSet", &delayed_Z_offset);
    ULong64_t delayed_begin_count, delayed_end_count;
    intree_delayed->SetBranchAddress("BeginCount", &delayed_begin_count);
    intree_delayed->SetBranchAddress("EndCount", &delayed_end_count);
    ULong64_t delayed_50, delayed_applied, delayed_flags;
    intree_delayed->SetBranchAddress("Delayed50MHz", &delayed_50);
    intree_delayed->SetBranchAddress("DelayedDataApplied", &delayed_applied);
    intree_delayed->SetBranchAddress("DelayedDataFlags", &delayed_flags);
    Int_t delayed_entry, delayed_ev, delayed_gtid, delayed_owlhits, delayed_nhits, delayed_nhits_cleaned, delayed_nhits_corrected;
    intree_delayed->SetBranchAddress("DelayedEntry", &delayed_entry);
    intree_delayed->SetBranchAddress("DelayedEv", &delayed_ev);
    intree_delayed->SetBranchAddress("DelayedGTID", &delayed_gtid);
    intree_delayed->SetBranchAddress("DelayedOWLHits", &delayed_owlhits);
    intree_delayed->SetBranchAddress("DelayedNHits", &delayed_nhits);
    intree_delayed->SetBranchAddress("DelayedNHitsCleaned", &delayed_nhits_cleaned);
    intree_delayed->SetBranchAddress("DelayedNHitsCorrected", &delayed_nhits_corrected);
    Double_t delayed_energy, delayed_pos_x, delayed_pos_y, delayed_pos_z, delayed_original_energy, delayed_tony_energy;
    intree_delayed->SetBranchAddress("DelayedEnergy", &delayed_energy);
    intree_delayed->SetBranchAddress("DelayedPosX", &delayed_pos_x);
    intree_delayed->SetBranchAddress("DelayedPosY", &delayed_pos_y);
    intree_delayed->SetBranchAddress("DelayedPosZ", &delayed_pos_z);
    intree_delayed->SetBranchAddress("DelayedOriginalEnergy", &delayed_original_energy);
    intree_delayed->SetBranchAddress("DelayedTonyEnergy", &delayed_tony_energy);

    //Record Index and Runs
    std::vector<Int_t> delayed_index, delayed_run;
    std::vector<ULong64_t> delayed_begin, delayed_end;
    Int_t Last_Delayed_Run;
    Index = 0;
    for(Int_t iDelayed = 0; iDelayed < intree_delayed->GetEntries(); iDelayed++)
    {
        intree_delayed->GetEntry(iDelayed);
        if(iDelayed == 0)
        {
            delayed_index.push_back(iDelayed);
            delayed_run.push_back(delayed_run_id);
            delayed_begin.push_back(delayed_begin_count);
            delayed_end.push_back(delayed_end_count);
            Last_Delayed_Run = delayed_run_id;

            continue;
        }
        else if( Last_Delayed_Run == delayed_run_id)
        {
            delayed_end.at(Index) = delayed_end_count;
            continue;
        }
        else if( Last_Delayed_Run != delayed_run_id)
        {
            delayed_index.push_back(iDelayed);
            delayed_run.push_back(delayed_run_id);
            delayed_begin.push_back(delayed_begin_count);
            delayed_end.push_back(delayed_end_count);
            Last_Delayed_Run = delayed_run_id;

            Index++;
        };
    };
    delayed_index.push_back(intree_delayed->GetEntries());
    //Delayed Rate
    std::vector<Double_t> delayed_rate;
    for(int ii1 = 0; ii1 < delayed_run.size(); ii1++)
    {
        temp_duration = Compute_Delta_T(delayed_begin.at(ii1), delayed_end.at(ii1))/1e9;
        temp_total = delayed_index.at(ii1 + 1) - delayed_index.at(ii1) + 1;
        delayed_rate.push_back(1.0 * temp_total / temp_duration);
    };
    // for(Int_t index = 0; index < delayed_run.size(); index++)
    // {
    //     std::cout << "Run:" << delayed_run.at(index) << ", Index:" << delayed_index.at(index) << ", Duration:"<< Compute_Delta_T(delayed_begin.at(index), delayed_end.at(index))/1e9 << ", Rate:" << delayed_rate.at(index) << std::endl;
    // };
    std::cout << "[Find_Accidental::Randomly_Generate_AC_Pairs] Load Delayed Files" << std::endl;
// AC Events (3600 s)
    std::vector<Int_t> ac_events;
    for(int ii1 = 0; ii1 < prompt_run.size(); ii1++)
    {
        ac_events.push_back(prompt_rate.at(ii1) * delayed_rate.at(ii1) * 3600 + 1);
    };
//Create Output File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "");
    //Create Branch
    outtree->Branch("RunID", &delayed_run_id);
    outtree->Branch("SubRunID", &delayed_subrun_id);
    outtree->Branch("Date", &delayed_date);
    outtree->Branch("RunType", &delayed_run_type);
    outtree->Branch("ZOffSet", &prompt_Z_offset);
    //Prompt
    outtree->Branch("PromptEntry", &prompt_entry);
    outtree->Branch("PromptEv", &prompt_ev);
    outtree->Branch("PromptGTID", &prompt_gtid);
    outtree->Branch("PromptOWLHits", &prompt_owlhits);
    outtree->Branch("PromptNHits", &prompt_nhits);
    outtree->Branch("PromptNHitsCleaned", &prompt_nhits_cleaned);
    outtree->Branch("PromptNHitsCorrected", &prompt_nhits_corrected);
    outtree->Branch("Prompt50MHz", &prompt_50);
    outtree->Branch("PromptDataApplied", &prompt_applied);
    outtree->Branch("PromptDataFlags", &prompt_flags);

    outtree->Branch("PromptEnergy", &prompt_energy);
    outtree->Branch("PromptPosX", &prompt_pos_x);
    outtree->Branch("PromptPosY", &prompt_pos_y);
    outtree->Branch("PromptPosZ", &prompt_pos_z);
    outtree->Branch("PromptOriginalEnergy", &prompt_original_energy);
    outtree->Branch("PromptTonyEnergy", &prompt_tony_energy);
    //Delayed
    outtree->Branch("DelayedEntry", &delayed_entry);
    outtree->Branch("DelayedEv", &delayed_ev);
    outtree->Branch("DelayedGTID", &delayed_gtid);
    outtree->Branch("DelayedOWLHits", &delayed_owlhits);
    outtree->Branch("DelayedNHits", &delayed_nhits);
    outtree->Branch("DelayedNHitsCleaned", &delayed_nhits_cleaned);
    outtree->Branch("DelayedNHitsCorrected", &delayed_nhits_corrected);
    outtree->Branch("Delayed50MHz", &delayed_50);
    outtree->Branch("DelayedDataApplied", &delayed_applied);
    outtree->Branch("DelayedDataFlags", &delayed_flags);

    outtree->Branch("DelayedEnergy", &delayed_energy);
    outtree->Branch("DelayedPosX", &delayed_pos_x);
    outtree->Branch("DelayedPosY", &delayed_pos_y);
    outtree->Branch("DelayedPosZ", &delayed_pos_z);
    outtree->Branch("DelayedOriginalEnergy", &delayed_original_energy);
    outtree->Branch("DelayedTonyEnergy", &delayed_tony_energy);
    //Other
    Double_t Delta_T;
    outtree->Branch("DeltaT", &Delta_T);//ns
    Int_t Seed;
    outtree->Branch("Seed", &Seed);//ns
//Uniform Generator
    TDatime now;
    Seed = now.Get();
    TRandom random(Seed);
    std::cout << "[Find_Accidental::Randomly_Generate_AC_Pairs] Set up Random Generator" << std::endl;
//Loop the Data
    std::cout << "[Find_Accidental::Randomly_Generate_AC_Pairs] Begin to Loop" << std::endl;
    const Double_t Len_prompt = intree_prompt->GetEntries();
    const Double_t Len_delayed = intree_delayed->GetEntries();
    Int_t Count = 0;
    Int_t temp_prompt_index, temp_delayed_index;
    const int len = prompt_run.size();
    for(Int_t Index = 0; Index < prompt_run.size(); Index++)
    {
        std::cout << "[Find_Accidental::Randomly_Generate_AC_Pairs] Processing Run:" << prompt_run.at(Index) << "，已完成：" << 100.0 * Index / len << "%，已处理：" << Index << "，还剩：" << len - Index << std::endl;
        for(int ii1 = 0; ii1 < NUMBER_GENERATED; ii1++)
        {
            // if(ii1 % 5000 == 0)
            // {
            //     std::cout << "[Find_Accidental::Randomly_Generate_AC_Pairs] 已处理" << 100.0 * ii1 / NUMBER_GENERATED << "%，具体：" << ii1 << "，还剩" << NUMBER_GENERATED - ii1 << std::endl;
            // };
            temp_prompt_index = random.Uniform(prompt_index.at(Index), prompt_index.at(Index + 1)) + 1;
            temp_delayed_index = random.Uniform(delayed_index.at(Index), delayed_index.at(Index + 1)) + 1;
            intree_prompt->GetEntry(temp_prompt_index);
            intree_delayed->GetEntry(temp_delayed_index);
            Delta_T = random.Uniform(0, TIME_WINDOWS);
            outtree->Fill();
        };
    };
//Close
    outfile->Write();
    outfile->Close();
    std::cout << "[Find_Accidental::Randomly_Generate_AC_Pairs] Complete" << std::endl;
//Test Code
    // for(int ii1 = 0; ii1 < 10; ii1++)
    // {
    //     intree_prompt->GetEntry(ii1);
    //     std::cout << ii1 << ", Run:" << prompt_run_id << ", Subrun:" << prompt_subrun_id << ", Date:" << prompt_date << ", Run Type:" << prompt_run_type << ", Z Offset:" << prompt_Z_offset << std::endl;
    //     std::cout << ii1 << ", Entry:" << prompt_entry << ", EV:" << prompt_ev << ", GTID:" << prompt_gtid << ", OWL:" << prompt_owlhits << ", Nhits:" << prompt_nhits << ", NHits Cleaned:" << prompt_nhits_cleaned << ", Nhits Corrected:" << prompt_nhits_corrected << std::endl;
    //     std::cout << ii1 << ", Ennergy:" << prompt_energy << ", Original Energy:" << prompt_original_energy << ", Tony Energy:" << prompt_tony_energy << ", X:" << prompt_pos_x << ", Y:" << prompt_pos_y << ", Z:" << prompt_pos_z << std::endl;
    // };
    // for(int ii1 = 0; ii1 < 10; ii1++)
    // {
    //     intree_delayed->GetEntry(ii1);
    //     std::cout << ii1 << ", Run:" << delayed_run_id << ", Subrun:" << delayed_subrun_id << ", Date:" << delayed_date << ", Run Type:" << delayed_run_type << ", Z Offset:" << delayed_Z_offset << std::endl;
    //     std::cout << ii1 << ", Entry:" << delayed_entry << ", EV:" << delayed_ev << ", GTID:" << delayed_gtid << ", OWL:" << delayed_owlhits << ", Nhits:" << delayed_nhits << ", NHits Cleaned:" << delayed_nhits_cleaned << ", Nhits Corrected:" << delayed_nhits_corrected << std::endl;
    //     std::cout << ii1 << ", Energy:" << delayed_energy << ", Original Energy:" << delayed_original_energy << ", Tony Energy:" << delayed_tony_energy << ", X:" << delayed_pos_x << ", Y:" << delayed_pos_y << ", Z:" << delayed_pos_z << std::endl;
    // };
};

void Apply_CP_Cut(std::string OutFile, std::string InFile)
{
    // std::string InFile = "./test.root";
    // std::string OutFile = "./test_out.root";
//Read File
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
    //Set Branch Address
    UInt_t run_id, subrun_id, date, run_type;
    intree->SetBranchAddress("RunID", &run_id);
    intree->SetBranchAddress("SubRunID", &subrun_id);
    intree->SetBranchAddress("Date", &date);
    intree->SetBranchAddress("RunType", &run_type);
    Double_t prompt_Z_offset;
    intree->SetBranchAddress("ZOffSet", &prompt_Z_offset);
    //Prompt
    ULong64_t prompt_50, prompt_applied, prompt_flags;
    intree->SetBranchAddress("Prompt50MHz", &prompt_50);
    intree->SetBranchAddress("PromptDataApplied", &prompt_applied);
    intree->SetBranchAddress("PromptDataFlags", &prompt_flags);
    Int_t prompt_entry, prompt_ev, prompt_gtid, prompt_owlhits, prompt_nhits, prompt_nhits_cleaned, prompt_nhits_corrected;
    intree->SetBranchAddress("PromptEntry", &prompt_entry);
    intree->SetBranchAddress("PromptEv", &prompt_ev);
    intree->SetBranchAddress("PromptGTID", &prompt_gtid);
    intree->SetBranchAddress("PromptOWLHits", &prompt_owlhits);
    intree->SetBranchAddress("PromptNHits", &prompt_nhits);
    intree->SetBranchAddress("PromptNHitsCleaned", &prompt_nhits_cleaned);
    intree->SetBranchAddress("PromptNHitsCorrected", &prompt_nhits_corrected);
    Double_t prompt_energy, prompt_pos_x, prompt_pos_y, prompt_pos_z, prompt_original_energy, prompt_tony_energy;
    intree->SetBranchAddress("PromptEnergy", &prompt_energy);
    intree->SetBranchAddress("PromptPosX", &prompt_pos_x);
    intree->SetBranchAddress("PromptPosY", &prompt_pos_y);
    intree->SetBranchAddress("PromptPosZ", &prompt_pos_z);
    intree->SetBranchAddress("PromptOriginalEnergy", &prompt_original_energy);
    intree->SetBranchAddress("PromptTonyEnergy", &prompt_tony_energy);
    //Delayed
    ULong64_t delayed_50, delayed_applied, delayed_flags;
    intree->SetBranchAddress("Delayed50MHz", &delayed_50);
    intree->SetBranchAddress("DelayedDataApplied", &delayed_applied);
    intree->SetBranchAddress("DelayedDataFlags", &delayed_flags);
    Int_t delayed_entry, delayed_ev, delayed_gtid, delayed_owlhits, delayed_nhits, delayed_nhits_cleaned, delayed_nhits_corrected;
    intree->SetBranchAddress("DelayedEntry", &delayed_entry);
    intree->SetBranchAddress("DelayedEv", &delayed_ev);
    intree->SetBranchAddress("DelayedGTID", &delayed_gtid);
    intree->SetBranchAddress("DelayedOWLHits", &delayed_owlhits);
    intree->SetBranchAddress("DelayedNHits", &delayed_nhits);
    intree->SetBranchAddress("DelayedNHitsCleaned", &delayed_nhits_cleaned);
    intree->SetBranchAddress("DelayedNHitsCorrected", &delayed_nhits_corrected);
    Double_t delayed_energy, delayed_pos_x, delayed_pos_y, delayed_pos_z, delayed_original_energy, delayed_tony_energy;
    intree->SetBranchAddress("DelayedEnergy", &delayed_energy);
    intree->SetBranchAddress("DelayedPosX", &delayed_pos_x);
    intree->SetBranchAddress("DelayedPosY", &delayed_pos_y);
    intree->SetBranchAddress("DelayedPosZ", &delayed_pos_z);
    intree->SetBranchAddress("DelayedOriginalEnergy", &delayed_original_energy);
    intree->SetBranchAddress("DelayedTonyEnergy", &delayed_tony_energy);
    //Other
    Double_t Delta_T;
    intree->SetBranchAddress("DeltaT", &Delta_T);
//Create Output File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = intree->CloneTree(0);
//Histogram
    TH1D *Hist_Delta_T = new TH1D("AC Delta T", "", 20, 0, 2000);//us
    TH1D *Hist_Delta_R = new TH1D("AC Delta R", "", 20, 0, 3000);//mm
    TGraph *TGraph_prompt_pos = new TGraph;
    TGraph *TGraph_delayed_pos = new TGraph;
//Loop
    TVector3 prompt_pos, delayed_pos, Delta_R;
    const int len = intree->GetEntries();
    Int_t Number_Data_Energy_Radius_DeltaT_DeltaR = 0;
    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        if(ii1 % 50000 == 0)
        {
            std::cout << "[Find_Accidental::Apply_CP_Cut] 已处理" << 100.0 * ii1 / len << "%，具体：" << ii1 << "，还剩：" << len - ii1 << std::endl;
        };
        intree->GetEntry(ii1);
        //Different Events
        if( prompt_gtid == delayed_gtid){continue;};
        //Data Cleaning
        if( (prompt_flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK){continue;};
        if( (delayed_flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK){continue;};
        //Energy Cut
        if(Prompt_Energy_Cut(prompt_energy) == false){continue;};
        if(Delayed_Energy_Cut(delayed_energy) == false){continue;};
        //Radius Cut
        prompt_pos.SetXYZ(prompt_pos_x, prompt_pos_y, prompt_pos_z);
        delayed_pos.SetXYZ(delayed_pos_x, delayed_pos_y, delayed_pos_z);
        if(prompt_pos.Mag() > RADIUS_MAX){continue;};
        if(delayed_pos.Mag() > RADIUS_MAX){continue;};
        //Delta T Cut
        if(DeltaT_Cut(Delta_T) == false){continue;};
        //Delta R Cut
        Delta_R = delayed_pos - prompt_pos;
        if(Delta_R.Mag() > DELTA_R_MAX) {continue;};
        //Other Cut
        //Fill in Histogram
        Hist_Delta_T->Fill(Delta_T);
        Hist_Delta_R->Fill(Delta_R.Mag());
        TGraph_prompt_pos->SetPoint(Number_Data_Energy_Radius_DeltaT_DeltaR, prompt_pos.Perp2(), prompt_pos.Z());
        TGraph_delayed_pos->SetPoint(Number_Data_Energy_Radius_DeltaT_DeltaR, delayed_pos.Perp2(), delayed_pos.Z());
        //Record
        outtree->Fill();
        Number_Data_Energy_Radius_DeltaT_DeltaR++;
    };
//Draw Histogram
    std::string Delta_T_PWD = "./Pic/AC_Delta_T_CP.jpg";
    std::string Delta_R_PWD = "./Pic/AC_Delta_R_CP.jpg";
    std::string Position_PWD = "./Pic/AC_Position_CP.jpg";
    SavePlot(Delta_T_PWD, Hist_Delta_T, "AC #Delta T", "#Delta T/#mu s", "Counts", 0, 0, 0);
    SavePlot(Delta_R_PWD, Hist_Delta_R, "AC #Delta R", "#Delta R/mm", "Counts", 0, 0, 0);
    SaveTwoPlots(Position_PWD, TGraph_prompt_pos, "Prompt", TGraph_delayed_pos, "Delayed", "Postion of AC Pairs", "#rho^{2}/mm^{2}", "Z/mm");
//Write in Files
    outfile->Write();
    outfile->Close();
    std::cout << "[Find_Accidental::Apply_CP_Cut] Complete" << std::endl;
};

void Compute_Rate(std::string OutFile, std::string InPWD)
{
//Load Prompt and Delayed Files
    TFile *infile_prompt = new TFile((InPWD + "/Prompt/Gold_AC_Loose_CP_Data_Prompt.root").c_str());
    TTree *intree_prompt = (TTree*) infile_prompt->Get("output");
    
    TFile *infile_delayed = new TFile((InPWD + "/Delayed/Gold_AC_Loose_CP_Data_Delayed.root").c_str());
    TTree *intree_delayed = (TTree*) infile_delayed->Get("output");
//Set Branch Address
    UInt_t Run_ID;
    intree_prompt->SetBranchAddress("RunID", &Run_ID);
    ULong64_t Begin_Count, End_Count;
    intree_prompt->SetBranchAddress("BeginCount", &Begin_Count);
    intree_prompt->SetBranchAddress("EndCount", &End_Count);
//Loop Prompt Data
    UInt_t Index_Vector, Event_Prompt, Event_Delayed;
    std::vector<UInt_t> Runs, Events_Prompt, Events_Delayed;
    std::vector<ULong64_t> Begin_Counts, End_Counts;
    std::string Selection_Prompt, Selection_Delayed;
    Int_t Len = intree_prompt->GetEntries();
    for(int ii1 = 0; ii1 < intree_prompt->GetEntries(); ii1++)
    {
        if( ii1 % 50000 == 0) { std::cout << "[Find_Accidental::Compute_Rate] 已处理" << 100.0 * ii1/Len << "%，具体:" << ii1 << "，还剩" << Len - ii1 << std::endl;};
        intree_prompt->GetEntry(ii1);
        //Initialization
        if(ii1 == 0)
        {
            
            Index_Vector = 0;
            Runs.push_back(Run_ID);
            Begin_Counts.push_back(Begin_Count);
            End_Counts.push_back(End_Count);

            Selection_Prompt = Form("RunID == %u", Run_ID);
            Selection_Delayed = Form("RunID == %u", Run_ID);
            Event_Prompt = intree_prompt->GetEntries(Selection_Prompt.c_str());
            Event_Delayed = intree_delayed->GetEntries(Selection_Delayed.c_str());
            Events_Prompt.push_back(Event_Prompt);
            Events_Delayed.push_back(Event_Delayed);
            continue;
        };
        //Same Run
        if( Runs.at(Index_Vector) == Run_ID)
        {
            // End_Counts.at(Index_Vector) = End_Count;
            End_Counts.at(Index_Vector) = End_Count;
            continue;
        }
        else if( Runs.at(Index_Vector) != Run_ID)
        {
            Index_Vector++;
            Runs.push_back(Run_ID);
            Begin_Counts.push_back(Begin_Count);
            End_Counts.push_back(End_Count);

            Selection_Prompt = Form("RunID == %u", Run_ID);
            Selection_Delayed = Form("RunID == %u", Run_ID);
            Event_Prompt = intree_prompt->GetEntries(Selection_Prompt.c_str());
            Event_Delayed = intree_delayed->GetEntries(Selection_Delayed.c_str());            
            Events_Prompt.push_back(Event_Prompt);
            Events_Delayed.push_back(Event_Delayed);
        };
    };
//Compute Prompt and Delayed Rate
    Double_t Time = 0;
    std::vector<Double_t> Rates_Prompt, Rates_Delayed, Rates_AC;
    for(int ii1 = 0; ii1 < Runs.size(); ii1++)
    {
        Time = Compute_Delta_T(Begin_Counts.at(ii1), End_Counts.at(ii1))/1e9;
        Rates_Prompt.push_back( 1.0 * Events_Prompt.at(ii1)/ Time);// Events/s
        Rates_Delayed.push_back( 1.0 * Events_Delayed.at(ii1)/ Time);// Events/s
        Rates_AC.push_back(Rates_Prompt.at(ii1) * Rates_Delayed.at(ii1) * 2.0e-3);
    };
//Compute Average AC Rate
    Double_t Sum = 0, Rate_Average;
    for(int ii1 = 0; ii1 < Rates_AC.size(); ii1++)
    {
        Sum = Sum + Rates_AC.at(ii1);
    };
    Rate_Average = 1.0 * Sum / Rates_AC.size();
//Show Details
    for(int ii1 = 0; ii1 < Runs.size(); ii1++)
    {
        std::cout << "Runs:" << Runs.at(ii1) << ", Prompt Events:" << Events_Prompt.at(ii1) << ", Delayed Events:" << Events_Delayed.at(ii1) << ", Begin Count:" << Begin_Counts.at(ii1) << ", End Count:" << End_Counts.at(ii1)  << ", Prompt Rate:" << Rates_Prompt.at(ii1) << ", Delayed Rate:" << Rates_Delayed.at(ii1) << ", AC Rate:" << Rates_AC.at(ii1) << std::endl;
    };
    std::cout << Rate_Average << std::endl;
}

void Apply_Other_Cut()
{
    std::string InFile
}

#endif