#ifndef FINDMUON_HH
#define FINDMUON_HH
//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./Result.hh"
#include "./Constant_Setting.hh"
#include "./Base_Functions.hh"

void Find_Muons(std::string InPWD, std::string OutFile)
{
    std::string Muon_LargeNHits = "./Log/Muon_LargeNHits.txt";
    std::string Muon_LargeNHits_DeltaT = "./Log/Muon_LargeNHits_DeltaT.txt";
//Read File
    TChain *chain = new TChain("output");
    chain->Add((InPWD + "/*").c_str());
    //Set Branch Address
    Int_t RunID, SubRunID, Date, Time, RunType;
    chain->SetBranchAddress("RunID", &RunID);
    chain->SetBranchAddress("SubRunID", &SubRunID);
    chain->SetBranchAddress("Date", &Date);
    chain->SetBranchAddress("Time", &Time);
    chain->SetBranchAddress("RunType", &RunType);
    Int_t Entry, EV;
    chain->SetBranchAddress("MuonEntry", &Entry);
    chain->SetBranchAddress("MuonEv", &EV);
    Int_t GTID, OWLHits;
    chain->SetBranchAddress("MuonGTID", &GTID);
    chain->SetBranchAddress("MuonOWLHits", &OWLHits);
    Int_t Nhits, Nhits_Cleaned;
    chain->SetBranchAddress("MuonNHits", &Nhits);
    chain->SetBranchAddress("MuonNHitsCleaned", &Nhits_Cleaned);
    Long64_t Clock50MHz, Data_Applied, Data_Flags;
    chain->SetBranchAddress("Muon50MHz", &Clock50MHz);
    chain->SetBranchAddress("MuonDataApplied", &Data_Applied);
    chain->SetBranchAddress("MuonDataFlags", &Data_Flags);
//Creat File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "output");
    //Create Branch
    myresult_muon res;
//Create Branch
    outtree->Branch("RunID", &res.C_RunID);
    outtree->Branch("SubRunID", &res.C_SubRunID);
    outtree->Branch("Date", &res.C_Date);
    outtree->Branch("Time", &res.C_Time);
    outtree->Branch("RunType", &res.C_RunType);
    //EV Info
    outtree->Branch("MuonEntry", &res.C_Muon.C_Entry);
    outtree->Branch("MuonEv", &res.C_Muon.C_EV);
    outtree->Branch("MuonGTID", &res.C_Muon.C_GTID);
    outtree->Branch("MuonOWLHits", &res.C_Muon.C_OWLHits);
    outtree->Branch("MuonNHits", &res.C_Muon.C_Nhits);
    outtree->Branch("MuonNHitsCleaned", &res.C_Muon.C_Nhits_Cleaned);
    outtree->Branch("Muon50MHz", &res.C_Muon.C_50MHZ);
    outtree->Branch("MuonDataApplied", &res.C_Muon.C_Data_Applied);
    outtree->Branch("MuonDataFlags", &res.C_Muon.C_Data_Flags);
//Begin to Find
    std::ofstream Muon_NHits(Muon_LargeNHits.c_str());
    std::ofstream Muon_NHits_T(Muon_LargeNHits_DeltaT.c_str());

    Muon_NHits << "Run" << "\t" << "SubRun" << "\t" << "Entry" << "\t" << "EV" << "\t" << "GTID" << "\t" << "OWLHits" << "\t" << "NHits" << "\t" << "Clock50MHz" << "\t" << "DeltaT(s)" << std::endl;
    Muon_NHits_T << "Run" << "\t" << "SubRun" << "\t" << "Entry" << "\t" << "EV" << "\t" << "GTID" << "\t" << "OWLHits" << "\t" << "NHits" << "\t" << "Clock50MHz" << "\t" << "DeltaT(s)" << std::endl;

    Int_t Last_GTID = 0, Last_Run = 0, Last_SubRun = 0;
    ULong64_t Last_50MHz = 0, Temp_50MHz = 0;
    int MuonNumber = 0;
    for( int ii1 = 0; ii1 < chain->GetEntries(); ii1++)
    {
        chain->GetEntry(ii1);
        if(GTID == 0){continue;};//排除0结果
        //初始化
        if(ii1 == 0){ Last_GTID = GTID; Last_50MHz = Clock50MHz; Last_Run = RunID; Last_SubRun = SubRunID;};//初始化
        //Tag Muon Event
        if(Nhits < MIN_NHITS ){continue;};
        Last_GTID = GTID;
        Temp_50MHz = Clock50MHz;
        Double_t Delta_T = ComputeDelta_T(Last_50MHz, Temp_50MHz);

        Muon_NHits << RunID << "\t" << SubRunID << "\t" << Entry << "\t" << EV << "\t" << GTID << "\t" << OWLHits << "\t" << Nhits << "\t" << Clock50MHz  << "\t" << Delta_T/1e9 << std::endl;
        //if( Last_50MHz > Temp_50MHz){std::cout << "发生反转：" << Last_50MHz << "," << Temp_50MHz << "," << Delta_T << std::endl;}
        if(Delta_T <= 0){std::cout << ii1 << ",Present Clock:" << Temp_50MHz << ",Last Clock:" << Last_50MHz << ",Delta T(s):" << Delta_T/1e9 << std::endl;};
        if( Delta_T <= 20e9 && Delta_T >= 0){continue;};//找到第一个Muon Event，忽略此后20秒的所有事件；
        Last_50MHz = Clock50MHz;      
        //std::cout << RunID << "," << SubRunID << "," << Delta_T/1e9 << std::endl;
        MuonNumber++;
        std::cout << ii1 << ",Run:" << RunID << ",SubRun:" << SubRunID << ",Entry:" << Entry << ",EV:"
        << EV << ",GTID:" << GTID << ",OWLHits:" << OWLHits << ",Nhits:" << Nhits << ",50MHz:"
        << Clock50MHz << ",Delta T(s):" << Delta_T/1e9 << std::endl;
        
        //Record Info
        res.C_RunID = RunID;
        res.C_SubRunID = SubRunID;
        res.C_Date = Date;
        res.C_Time = Time;
        res.C_RunType = RunType;
        res.C_Muon.C_Entry = Entry;
        res.C_Muon.C_EV = EV;
        res.C_Muon.C_GTID = GTID;
        res.C_Muon.C_OWLHits = OWLHits;
        res.C_Muon.C_Nhits = Nhits;
        res.C_Muon.C_Nhits_Cleaned = Nhits_Cleaned;
        res.C_Muon.C_50MHZ = Clock50MHz;
        res.C_Muon.C_Data_Applied = Data_Applied;
        res.C_Muon.C_Data_Flags = Data_Flags;
        outtree->Fill();
        Muon_NHits_T << RunID << "\t" << SubRunID << "\t" << Entry << "\t" << EV << "\t" << GTID << "\t" << OWLHits << "\t" << Nhits << "\t" << Clock50MHz  << "\t" << Delta_T/1e9 << std::endl;
        //std::cout << ii1 << ",Run:" << RunID << ",SubRun:" << SubRunID << ",Entry:" << Entry << ",EV:"
        //<< EV << ",GTID:" << GTID << ",OWLHits:" << OWLHits << ",Nhits:" << Nhits << ",Nhits Cleaned:"
        //<< Nhits_Cleaned << ",Delta T(s):" << Delta_T/1e9 << std::endl;
    };
    outfile->Write();
    outfile->Close();
    std::cout << "Find Muon:" << MuonNumber << std::endl;
};

#endif