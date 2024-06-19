#ifndef COINCIDENCEPAIRS_HH
#define COINCIDENCEPAIRS_HH
//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TVector3.h>
//RAT
//Self-Defined
#include "./Result.hh"
#include "./Constant_Setting.hh"
#include "./Base_Functions.hh"
#include "./Plot_Code.hh"


void Find_Coincidence_Pairs(std::string Out_File, std::string LooseCut_Files, std::string Muon_File)
{
    // std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/LooseCut/300000-307612";
    std::string Log_CP = "./Log/Coincidence_Pairs.txt";
    std::string Log_CP_Energy = "./Log/Coincidence_Pairs_Energy.txt";
    std::string Log_CP_Energy_Radius = "./Log/Coincidence_Pairs_Energy_Radius.txt";
    std::string Log_CP_Energy_Radius_DeltaT = "./Log/Coincidence_Pairs_Energy_DeltaT.txt";
    std::string Log_CP_Energy_Radius_DeltaT_DeltaR = "./Log/Coincidence_Pairs_Energy_DeltaT_DeltaR.txt";
    std::string Log_CP_Energy_Radius_DeltaT_DeltaR_Data = "./Log/Coincidence_Pairs_Energy_DeltaT_DeltaR_Data.txt";
    std::string Log_CP_Energy_Radius_DeltaT_DeltaR_Data_Muon = "./Log/Coincidence_Pairs_Energy_DeltaT_DeltaR_Data_Muon.txt";
//Create OutPut File
    TFile *outfile = new TFile(Out_File.c_str(), "recreate");
    TTree *outtree = new TTree("output", "");
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] 输出文件创建成功" << std::endl;
    //Set Branch Address
    myresult res;
    outtree->Branch("RunID", &res.C_RunID);
    outtree->Branch("SubRunID", &res.C_SubRunID);
    outtree->Branch("Date", &res.C_Date);
    outtree->Branch("Time", &res.C_Time);
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
//Read Loose Cut Files
    TChain *chain = new TChain("output");
    chain->Add((LooseCut_Files + "/*").c_str());
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] LooseCut文件加载成功" << std::endl;
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
//Read Muon Files
    TFile muonfile(Muon_File.c_str());
    TTree *muontree = (TTree*) muonfile.Get("output");
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] Muon文件加载成功" << std::endl;
    //Set Branch Addres
    muontree->SetBranchAddress("RunID", &run_id);
    muontree->SetBranchAddress("SubRunID", &subrun_id);
    muontree->SetBranchAddress("Date", &date);
    muontree->SetBranchAddress("Time", &time);
    muontree->SetBranchAddress("RunType", &run_type);
    Int_t Muon_Entry, Muon_EV;
    muontree->SetBranchAddress("MuonEntry", &Muon_Entry);
    muontree->SetBranchAddress("MuonEv", &Muon_EV);
    Int_t Muon_GTID, Muon_OWLHits;
    muontree->SetBranchAddress("MuonGTID", &Muon_GTID);
    muontree->SetBranchAddress("MuonOWLHits", &Muon_OWLHits);
    Int_t Muon_Nhits, Muon_Nhits_Cleaned;
    muontree->SetBranchAddress("MuonNHits", &Muon_Nhits);
    muontree->SetBranchAddress("MuonNHitsCleaned", &Muon_Nhits_Cleaned);
    ULong64_t Muon_Clock50MHz, Muon_Data_Applied, Muon_Data_Flags;
    muontree->SetBranchAddress("Muon50MHz", &Muon_Clock50MHz);
    muontree->SetBranchAddress("MuonDataApplied", &Muon_Data_Applied);
    muontree->SetBranchAddress("MuonDataFlags", &Muon_Data_Flags);
    //Load Muon Data
    int tempInt = muontree->GetEntries();
    const int Muon_Len = tempInt;
    Int_t Muon_Run_Array[Muon_Len], Muon_SubRun_Array[Muon_Len];
    Int_t Muon_GTID_Array[Muon_Len];
    ULong64_t Muon_50MHz_Array[Muon_Len];
    for(int ii1 = 0; ii1 < Muon_Len; ii1++)
    {
        muontree->GetEntry(ii1);
        Muon_Run_Array[ii1] = run_id;
        Muon_SubRun_Array[ii1] = subrun_id;
        Muon_GTID_Array[ii1] = Muon_GTID;
        Muon_50MHz_Array[ii1] = Muon_Clock50MHz;
    };
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] Muon数据加载成功" << std::endl;
//Set up Log
    std::ofstream cp_energy(Log_CP_Energy.c_str(), std::ios::out);
    std::ofstream cp_energy_radius(Log_CP_Energy_Radius.c_str(), std::ios::out);
    std::ofstream cp_energy_radius_deltat(Log_CP_Energy_Radius_DeltaT.c_str(), std::ios::out);
    std::ofstream cp_energy_radius_deltat_deltar(Log_CP_Energy_Radius_DeltaT_DeltaR.c_str(), std::ios::out);
    std::ofstream cp_energy_radius_deltat_deltar_data(Log_CP_Energy_Radius_DeltaT_DeltaR_Data.c_str(), std::ios::out);
    std::ofstream cp_energy_radius_deltat_deltar_data_muon(Log_CP_Energy_Radius_DeltaT_DeltaR_Data_Muon.c_str(), std::ios::out);


    cp_energy << "Run" << "\t" << "SubRun" << "\t" << "Prompt Entry" << "\t" << "Prompt EV" << "\t" << "Prompt GTID" << "\t" << "Prompt Energy" << "\t" << "Prompt Radius" << "\t" << "Prompt OWLHits" << "\t" << "Prompt NHits" << "\t" << "Prompt 50MHz" << "\t" << "Delayed Entry" << "\t" << "Delayed EV" << "\t" << "Delayed GTID" << "\t" << "Delayed Energy" << "\t" << "Delayed Radius" << "\t" << "Delayed OWLHits" << "\t" << "Delayed NHits" << "\t" << "Delayed 50MHz" << "\t" << "Delta T(ms)" << "\t" << "Delta R(m)"<< std::endl;
    cp_energy_radius << "Run" << "\t" << "SubRun" << "\t" << "Prompt Entry" << "\t" << "Prompt EV" << "\t" << "Prompt GTID" << "\t" << "Prompt Energy" << "\t" << "Prompt Radius" << "\t" << "Prompt OWLHits" << "\t" << "Prompt NHits" << "\t" << "Prompt 50MHz" << "\t" << "Delayed Entry" << "\t" << "Delayed EV" << "\t" << "Delayed GTID" << "\t" << "Delayed Energy" << "\t" << "Delayed Radius" << "\t" << "Delayed OWLHits" << "\t" << "Delayed NHits" << "\t" << "Delayed 50MHz" << "\t" << "Delta T(ms)" << "\t" << "Delta R(m)"<< std::endl;
    cp_energy_radius_deltat << "Run" << "\t" << "SubRun" << "\t" << "Prompt Entry" << "\t" << "Prompt EV" << "\t" << "Prompt GTID" << "\t" << "Prompt Energy" << "\t" << "Prompt Radius" << "\t" << "Prompt OWLHits" << "\t" << "Prompt NHits" << "\t" << "Prompt 50MHz" << "\t" << "Delayed Entry" << "\t" << "Delayed EV" << "\t" << "Delayed GTID" << "\t" << "Delayed Energy" << "\t" << "Delayed Radius" << "\t" << "Delayed OWLHits" << "\t" << "Delayed NHits" << "\t" << "Delayed 50MHz" << "\t" << "Delta T(ms)" << "\t" << "Delta R(m)"<< std::endl;
    cp_energy_radius_deltat_deltar << "Run" << "\t" << "SubRun" << "\t" << "Prompt Entry" << "\t" << "Prompt EV" << "\t" << "Prompt GTID" << "\t" << "Prompt Energy" << "\t" << "Prompt Radius" << "\t" << "Prompt OWLHits" << "\t" << "Prompt NHits" << "\t" << "Prompt 50MHz" << "\t" << "Delayed Entry" << "\t" << "Delayed EV" << "\t" << "Delayed GTID" << "\t" << "Delayed Energy" << "\t" << "Delayed Radius" << "\t" << "Delayed OWLHits" << "\t" << "Delayed NHits" << "\t" << "Delayed 50MHz" << "\t" << "Delta T(ms)" << "\t" << "Delta R(m)"<< std::endl;
    cp_energy_radius_deltat_deltar_data << "Run" << "\t" << "SubRun" << "\t" << "Prompt Entry" << "\t" << "Prompt EV" << "\t" << "Prompt GTID" << "\t" << "Prompt Energy" << "\t" << "Prompt Radius" << "\t" << "Prompt OWLHits" << "\t" << "Prompt NHits" << "\t" << "Prompt 50MHz" << "\t" << "Delayed Entry" << "\t" << "Delayed EV" << "\t" << "Delayed GTID" << "\t" << "Delayed Energy" << "\t" << "Delayed Radius" << "\t" << "Delayed OWLHits" << "\t" << "Delayed NHits" << "\t" << "Delayed 50MHz" << "\t" << "Delta T(ms)" << "\t" << "Delta R(m)"<< std::endl;
    cp_energy_radius_deltat_deltar_data_muon << "Run" << "\t" << "SubRun" << "\t" << "Prompt Entry" << "\t" << "Prompt EV" << "\t" << "Prompt GTID" << "\t" << "Prompt Energy" << "\t" << "Prompt Radius" << "\t" << "Prompt OWLHits" << "\t" << "Prompt NHits" << "\t" << "Prompt 50MHz" << "\t" << "Delayed Entry" << "\t" << "Delayed EV" << "\t" << "Delayed GTID" << "\t" << "Delayed Energy" << "\t" << "Delayed Radius" << "\t" << "Delayed OWLHits" << "\t" << "Delayed NHits" << "\t" << "Delayed 50MHz" << "\t" << "Delta T(ms)" << "\t" << "Delta R(m)"<< std::endl;

//Loop the Data
    Double_t Delta_T = 0;
    TVector3 Pos[2], Delta_R;
    Int_t Number_CoincidencePair = 0, Number_CoincidencePair_Energy = 0, Number_CoincidencePair_Energy_Radius = 0;
    Int_t Number_CoincidencePair_Energy_Radius_DeltaT = 0, Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR = 0;
    Int_t Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR_Data = 0, Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR_Data_Muon = 0;
    Int_t Number_Muon = 0;
    Int_t Index_Muon_Array = 0, Change_Run_SubRun = 0;
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] 开始处理" << std::endl;
    int Len = chain->GetEntries();
    for(int ii1 = 0; ii1 < chain->GetEntries(); ii1++)
    {
        if(ii1 % 5000 == 0){std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] 已处理：" << double(100.0 * ii1/Len) << "%，具体：" << ii1 << "，还剩" << Len - ii1 << std::endl;};
        chain->GetEntry(ii1);
        Pos[0].SetXYZ(prompt_posx, prompt_posy, prompt_posz);
        Pos[1].SetXYZ(delayed_posx, delayed_posy, delayed_posz);
        Delta_R = Pos[1] - Pos[0];
        Delta_T = Compute_Delta_T(prompt_50mhz, delayed_50mhz);
    //Energy Cut
        if(Prompt_Energy_Cut(prompt_energy) == false) {continue;};
        if(Delayed_Energy_Cut(delayed_energy) == false) {continue;};
        Number_CoincidencePair_Energy++;
        cp_energy << run_id << "\t" << subrun_id << "\t" << prompt_entry << "\t" << prompt_ev << "\t" << prompt_gtid << "\t" << prompt_energy << "\t" << Pos[0].Mag() << "\t" << prompt_owlhits << "\t" << prompt_nhits << "\t" << prompt_50mhz << "\t" << delayed_entry << "\t" << delayed_ev << "\t" << delayed_gtid << "\t" << delayed_energy << "\t" << Pos[1].Mag() << "\t" << delayed_owlhits << "\t" << delayed_nhits << "\t" << delayed_50mhz << "\t" << Delta_T/1e6 << "\t" << Delta_R.Mag() << std::endl; 
    //Radius Cut
        if(Pos[0].Mag() > RADIUS_MAX){continue;};
        if(Pos[1].Mag() > RADIUS_MAX) {continue;};
        Number_CoincidencePair_Energy_Radius++;
        cp_energy_radius << run_id << "\t" << subrun_id << "\t" << prompt_entry << "\t" << prompt_ev << "\t" << prompt_gtid << "\t" << prompt_energy << "\t" << Pos[0].Mag() << "\t" << prompt_owlhits << "\t" << prompt_nhits << "\t" << prompt_50mhz << "\t" << delayed_entry << "\t" << delayed_ev << "\t" << delayed_gtid << "\t" << delayed_energy << "\t" << Pos[1].Mag() << "\t" << delayed_owlhits << "\t" << delayed_nhits << "\t" << delayed_50mhz << "\t" << Delta_T/1e6 << "\t" << Delta_R.Mag() << std::endl; 
    //Delta T Cut
        if(DeltaT_Cut(Delta_T) == false){continue;};
        Number_CoincidencePair_Energy_Radius_DeltaT++;
        cp_energy_radius_deltat << run_id << "\t" << subrun_id << "\t" << prompt_entry << "\t" << prompt_ev << "\t" << prompt_gtid << "\t" << prompt_energy << "\t" << Pos[0].Mag() << "\t" << prompt_owlhits << "\t" << prompt_nhits << "\t" << prompt_50mhz << "\t" << delayed_entry << "\t" << delayed_ev << "\t" << delayed_gtid << "\t" << delayed_energy << "\t" << Pos[1].Mag() << "\t" << delayed_owlhits << "\t" << delayed_nhits << "\t" << delayed_50mhz << "\t" << Delta_T/1e6 << "\t" << Delta_R.Mag() << std::endl; 
    //Delta R Cut
        if(Delta_R.Mag() > DELTA_R_MAX){continue;};
        Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR++;
        cp_energy_radius_deltat_deltar << run_id << "\t" << subrun_id << "\t" << prompt_entry << "\t" << prompt_ev << "\t" << prompt_gtid << "\t" << prompt_energy << "\t" << Pos[0].Mag() << "\t" << prompt_owlhits << "\t" << prompt_nhits << "\t" << prompt_50mhz << "\t" << delayed_entry << "\t" << delayed_ev << "\t" << delayed_gtid << "\t" << delayed_energy << "\t" << Pos[1].Mag() << "\t" << delayed_owlhits << "\t" << delayed_nhits << "\t" << delayed_50mhz << "\t" << Delta_T/1e6 << "\t" << Delta_R.Mag() << std::endl; 
    //Data Clean Cut
        if((prompt_data_flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK){continue;};
        if((delayed_data_flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK){continue;};
        //std::cout << "Prompt:" << prompt_data_flags << ", Mask:" << DATA_CLEANING_MASK << ", After:" << (prompt_data_flags & DATA_CLEANING_MASK) << std::endl;  
        Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR_Data++;
        cp_energy_radius_deltat_deltar_data << run_id << "\t" << subrun_id << "\t" << prompt_entry << "\t" << prompt_ev << "\t" << prompt_gtid << "\t" << prompt_energy << "\t" << Pos[0].Mag() << "\t" << prompt_owlhits << "\t" << prompt_nhits << "\t" << prompt_50mhz << "\t" << delayed_entry << "\t" << delayed_ev << "\t" << delayed_gtid << "\t" << delayed_energy << "\t" << Pos[1].Mag() << "\t" << delayed_owlhits << "\t" << delayed_nhits << "\t" << delayed_50mhz << "\t" << Delta_T/1e6 << "\t" << Delta_R.Mag() << std::endl; 
    //Muon Cut
        //Find the Closest Muon Event
        if( Muon_Run_Array[Index_Muon_Array] < run_id)
        {
            for(int ii2 = Index_Muon_Array; ii2 < Muon_Len; ii2++)
            {
                if(Muon_Run_Array[ii2] == run_id)
                {
                    Index_Muon_Array = ii2;
                    break;
                };
            };
            for(int ii2 = Index_Muon_Array; ii2 < Muon_Len; ii2++)
            {
                if(Muon_SubRun_Array[ii2] >= subrun_id && Muon_Run_Array[ii2] == run_id)
                {
                    Index_Muon_Array = ii2 - 1;
                    break;
                };
            };
            std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs::Muon Cut] 更新Muon：" << Muon_Run_Array[Index_Muon_Array] << ",SubRun:" << Muon_SubRun_Array[Index_Muon_Array] << ",Prompt Run:" << run_id << ", Prompt SubRun:" << subrun_id << std::endl;
        };
        if( PassMuonCut(Index_Muon_Array, Muon_Len, Muon_50MHz_Array, Muon_Run_Array, Muon_SubRun_Array, prompt_50mhz, run_id, subrun_id) == false){continue;};
        Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR_Data_Muon++;
        cp_energy_radius_deltat_deltar_data_muon << run_id << "\t" << subrun_id << "\t" << prompt_entry << "\t" << prompt_ev << "\t" << prompt_gtid << "\t" << prompt_energy << "\t" << Pos[0].Mag() << "\t" << prompt_owlhits << "\t" << prompt_nhits << "\t" << prompt_50mhz << "\t" << delayed_entry << "\t" << delayed_ev << "\t" << delayed_gtid << "\t" << delayed_energy << "\t" << Pos[1].Mag() << "\t" << delayed_owlhits << "\t" << delayed_nhits << "\t" << delayed_50mhz << "\t" << Delta_T/1e6 << "\t" << Delta_R.Mag() << std::endl; 
    //Record Info
        res.C_RunID = run_id;
        res.C_SubRunID = subrun_id;
        res.C_Date = date;
        res.C_Time = time;
        res.C_RunType = run_type;
        res.C_ZOffSet = z_offset;
        //Prompt
        res.C_Prompt.C_Entry = prompt_entry;
        res.C_Prompt.C_EV = prompt_ev;
        res.C_Prompt.C_GTID = prompt_gtid;
        res.C_Prompt.C_OWLHits = prompt_owlhits;
        res.C_Prompt.C_Nhits = prompt_nhits;
        res.C_Prompt.C_Nhits_Cleaned = prompt_nhits_cleaned;
        res.C_Prompt.C_Nhits_Corrected = prompt_nhits_corrected;
        res.C_Prompt.C_50MHZ = prompt_50mhz;
        res.C_Prompt.C_Data_Applied = prompt_data_applied;
        res.C_Prompt.C_Data_Flags = prompt_data_flags;
        res.C_Prompt.C_Energy = prompt_energy;
        res.C_Prompt.C_Pos_X = prompt_posx;
        res.C_Prompt.C_Pos_Y = prompt_posy;
        res.C_Prompt.C_Pos_Z = prompt_posz;
        res.C_Prompt.C_Original_Energy = prompt_original_energy;
        res.C_Prompt.C_Tony_Energy = prompt_tony_energy;
        //Delayed
        res.C_Delayed.C_Entry = delayed_entry;
        res.C_Delayed.C_EV = delayed_ev;
        res.C_Delayed.C_GTID = delayed_gtid;
        res.C_Delayed.C_OWLHits = delayed_owlhits;
        res.C_Delayed.C_Nhits = delayed_nhits;
        res.C_Delayed.C_Nhits_Cleaned = delayed_nhits_cleaned;
        res.C_Delayed.C_Nhits_Corrected = delayed_nhits_corrected;
        res.C_Delayed.C_50MHZ = delayed_50mhz;
        res.C_Delayed.C_Data_Applied = delayed_data_applied;
        res.C_Delayed.C_Data_Flags = delayed_data_flags;
        res.C_Delayed.C_Energy = delayed_energy;
        res.C_Delayed.C_Pos_X = delayed_posx;
        res.C_Delayed.C_Pos_Y = delayed_posy;
        res.C_Delayed.C_Pos_Z = delayed_posz;
        res.C_Delayed.C_Original_Energy = delayed_original_energy;
        res.C_Delayed.C_Tony_Energy = delayed_tony_energy;
        //Write into ROOT File
        outtree->Fill();
    };
//Close Log Files
    cp_energy.close();
    cp_energy_radius.close();
    cp_energy_radius_deltat.close();
    cp_energy_radius_deltat_deltar.close();
    cp_energy_radius_deltat_deltar_data.close();
//Close OutPut File
    outfile->Write();
    outfile->Close();
//Show Details
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] After Energy Cut:" << Number_CoincidencePair_Energy << std::endl;
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] After Radius Cut:" << Number_CoincidencePair_Energy_Radius << std::endl;
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] After Delta T Cut:" << Number_CoincidencePair_Energy_Radius_DeltaT << std::endl;
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] After Delta R Cut:" << Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR << std::endl;
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] After Data Clean Mask Cut:" << Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR_Data << std::endl;
    std::cout << "[Coincidence_Pairs::Find_Coincidence_Pairs] After Muon Cut:" << Number_CoincidencePair_Energy_Radius_DeltaT_DeltaR_Data_Muon << std::endl;
}

void Plot_IBD_Candidate(std::string IBD_Files, std::string OutPWD)
{
    // std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/IBDCandidate/Gold_20R_IBD_Candidate_0000300000-0000307612.root";
    // std::string OutPWD = "/rat/MyCode/Work/Geo-nu/Ingrida/Pic/";
    std::string OutPut_Prompt_Energy = OutPWD + "/Prompt_Energy.png";
    std::string OutPut_Delayed_Energy = OutPWD + "/Delayed_Energy.png";
    std::string OutPut_Delta_T = OutPWD + "/Delta_T.png";
    std::string OutPut_Position = OutPWD + "/Position.png";
//Read File    
    TFile *infile = new TFile(IBD_Files.c_str());
    TTree *intree = (TTree*) infile->Get("output");
    //Set Branch Address
    //Prompt
    Int_t PromptEntry, PromptEv;
    intree->SetBranchAddress("PromptEntry", &PromptEntry);
    intree->SetBranchAddress("PromptEv", &PromptEv);
    Int_t PromptGTID, PromptOWLHits;
    intree->SetBranchAddress("PromptGTID", &PromptGTID);
    intree->SetBranchAddress("PromptOWLHits", &PromptOWLHits);
    Int_t PromptNHits, PromptNHitsCleaned, PromptNHitsCorrected;
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
    Int_t DelayedEntry, DelayedEv;
    intree->SetBranchAddress("DelayedEntry", &DelayedEntry);
    intree->SetBranchAddress("DelayedEv", &DelayedEv);
    Int_t DelayedGTID, DelayedOWLHits;
    intree->SetBranchAddress("DelayedGTID", &DelayedGTID);
    intree->SetBranchAddress("DelayedOWLHits", &DelayedOWLHits);
    Int_t DelayedNHits, DelayedNHitsCleaned, DelayedNHitsCorrected;
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
    TH1D *delta_T = new TH1D("Delta T", "", 20, 0, 2000);//\mu s
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
        Delta_T = Compute_Delta_T(Prompt50MHz, Delayed50MHz)/1e3;
        delta_T->Fill(Delta_T);
        //Position
        Prompt_Pos.SetXYZ(PromptPosX, PromptPosY, PromptPosZ);
        Delayed_Pos.SetXYZ(DelayedPosX, DelayedPosY, DelayedPosZ);
        prompt_pos->SetPoint(ii1, Prompt_Pos.Perp2(), Prompt_Pos.Z());
        delayed_pos->SetPoint(ii1, Delayed_Pos.Perp2(), Delayed_Pos.Z());
    };

    //Save Plots
    SavePlot(OutPut_Prompt_Energy, prompt_energy, "Prompt", "E_{prompt}/MeV", "Prompt Energy", 0, 0, 0, 0.8, 0.7, 0.9, 0.8);
    FitSavePlot_G(OutPut_Delayed_Energy, delayed_energy, "Data", "E_{delayed}/MeV", "Delayed Energy", 0, 0, 0, 0.13, 0.7, 0.3, 0.85);
    FitSavePlot_E(OutPut_Delta_T, delta_T, "Data", "#Delta T/#mu s", "#Delta T", 0, 0, 0, 0.5, 0.7, 0.8, 0.8);
    //SavePlot(OutPut_Position, delayed_pos, "Prompt", "z/mm", "#rho^2/mm^2");
    Save_Two_Plots(OutPut_Position, prompt_pos, "Prompt", delayed_pos, "Delayed", "Position of Coincidence Pair", "#rho^{2}/mm^{2}", "z/mm", 0.8, 0.8, 0.9, 0.9);
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

#endif