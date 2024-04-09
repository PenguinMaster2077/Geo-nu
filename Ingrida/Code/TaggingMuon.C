//CPP
#include <iostream>
#include <fstream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DS/DataQCFlags.hh>
#include <RAT/DS/BitMask.hh>
#include <RAT/DS/PMT.hh>
#include <RAT/DS/PMTSet.hh>
#include <RAT/DU/PMTCalStatus.hh>
#include <RAT/DU/PMTInfo.hh>
#include <RAT/DU/Utility.hh>

class Event
{
public:
    Event(){ClearAll();};
    void ClearAll()
    {
        ClearEVInfo();
    }
    void ClearEVInfo()
    {
        C_Entry = 0;
        C_EV = 0;
        C_GTID = 0;
        C_OWLHits = 0;
        C_Nhits = 0;
        C_Nhits_Cleaned = 0;
        C_50MHz = 0;
        C_Data_Applied = 0;
        C_Data_Flags = 0;
    };
    void ShowEVInfo()
    {
        std::cout << "Entry:" << C_Entry << std::endl;
        std::cout << "EV:" << C_EV << std::endl;
        std::cout << "GTID:" << C_GTID << std::endl;
        std::cout << "OWLHits:" << C_OWLHits << std::endl;
        std::cout << "NHits:" << C_Nhits << std::endl;
        std::cout << "NHits Cleaned:" << C_Nhits_Cleaned << std::endl;
        std::cout << "Clock 50MHz:" << C_50MHz << std::endl;
        std::cout << "Data Applied:" << C_Data_Applied << std::endl;
        std::cout << "Data Flags:" << C_Data_Flags << std::endl;
    };
//EV Info
    unsigned int C_Entry, C_EV;
    Int_t C_GTID, C_OWLHits;
    UInt_t C_Nhits, C_Nhits_Cleaned;
    ULong64_t C_50MHz, C_Data_Applied, C_Data_Flags;
};

class Myresult
{
public:
    Myresult() { ClearAll();};
    void ClearAll() { ClearRun(); ClearMuon();};
    void ClearRun()
    {
        C_RunID = 0;
        C_SubRunID = 0;
        C_Date = 0;
        C_RunType = 0;
        C_ZOffSet = 0;
    };
    void ShowRun()
    {
        std::cout << "Run:" << C_RunID << std::endl;
        std::cout << "SubRun:" << C_SubRunID << std::endl;
        std::cout << "Date:" << C_Date << std::endl;
        std::cout << "RunType:" << C_RunType << std::endl;
        std::cout << "Z-Offset:" << C_ZOffSet << std::endl;
    };
    void ClearMuon() {C_Muon.ClearAll();};
    void ShowMuon()
    { 
        C_Muon.ShowEVInfo();
    }
//Run Info
    UInt_t C_RunID, C_SubRunID, C_Date, C_Time, C_RunType;
    Double_t C_ZOffSet;
//Event
    Event C_Muon;
};

std::vector<std::string> Run(std::string InFile);

void RecordRunInfo(Myresult &res, RAT::DU::DSReader &dsreader);

void RecordEV(Myresult &res, RAT::DU::DSReader &dsreader, const unsigned int iEntry, const unsigned iEv);

// int TaggingMuon()
int main(int argc, char** argv)
{
    // std::string InFile = "/home/shuaioy/scratch/Geo/Gold/300000-306498/ratds/300050/Data/Analysis20R_r0000300043_s011_p000.root";
    // std::string OutFile = "./test.root";
    std::string InFile = argv[1];
    std::string OutFile = argv[2];

//Create New Root File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "output");
//Create Wanted Info Class
    Myresult res;
//Create Branch
    outtree->Branch("RunID", &res.C_RunID, "RunID/I");
    outtree->Branch("SubRunID", &res.C_SubRunID, "RunID/I");
    outtree->Branch("Date", &res.C_Date, "RunID/I");
    outtree->Branch("Time", &res.C_Time, "RunID/I");
    outtree->Branch("RunType", &res.C_RunType, "RunID/I");
    //EV Info
    outtree->Branch("MuonEntry", &res.C_Muon.C_Entry, "MuonEntry/I");
    outtree->Branch("MuonEv", &res.C_Muon.C_EV, "MuonEv/I");
    outtree->Branch("MuonGTID", &res.C_Muon.C_GTID, "MuonGTID/I");
    outtree->Branch("MuonOWLHits", &res.C_Muon.C_OWLHits, "MuonOWLHits/I");
    outtree->Branch("MuonNHits", &res.C_Muon.C_Nhits, "MuonNHits/I");
    outtree->Branch("MuonNHitsCleaned", &res.C_Muon.C_Nhits_Cleaned, "MuonNHitsCleaned/I");
    outtree->Branch("Muon50MHz", &res.C_Muon.C_50MHz, "Muon50MHz/L");
    outtree->Branch("MuonDataApplied", &res.C_Muon.C_Data_Applied, "MuonDataApplied/L");
    outtree->Branch("MuonDataFlags", &res.C_Muon.C_Data_Flags, "MuonDataFlags/L");
//Read File
    // std::vector<std::string> InRun = Run(InFile);

    RAT::DU::DSReader dsreader(InFile);
//Record Run Info
    RecordRunInfo(res, dsreader);
//Loop
    RAT::DS::Entry entry;
    RAT::DS::EV ev;
    RAT::DS::CalPMTs cal_pmts;
    RAT::DS::PMTCal owl_pmt;
    int LargeNhits = 0, LargeOWPMT = 0, MuonNumber = 0;
    std::cout << "Begin to Research" << std::endl;
    for(unsigned int iEntry = 0; iEntry < dsreader.GetEntryCount(); iEntry++)
    {   
        if(iEntry % 1000 ==0) { std::cout << "已处理" << iEntry << ",还剩" << dsreader.GetEntryCount() - iEntry << std::endl;};
        entry = dsreader.GetEntry(iEntry);
        for(unsigned int iEv = 0; iEv < entry.GetEVCount(); iEv++)
        {   
            //Initialization
            LargeNhits = 0;
            LargeOWPMT = 0;
            //Begin to find
            ev = entry.GetEV(iEv);
            if( ev.GetNhits() >= 2000){ LargeNhits = 1; };
            if( ev.GetUncalPMTs().GetOWLCount() >= 3) { LargeOWPMT = 1;};
            if( LargeNhits == 0 && LargeOWPMT == 0) {continue;};
            //Record and Show Info for LargeNhits- or LargeOWPMT-event
            std::cout << "Find Muon Event!!!!!!" << std::endl;
            MuonNumber++;
            RecordEV(res, dsreader, iEntry, iEv);
            res.ShowMuon();
            outtree->Fill();
        };
    };
    if(MuonNumber == 0){outtree->Fill();};//If doesn't find Muon, record 0.
    outfile->Write();
    outfile->Close();

    std::cout << "Complete" << std::endl;
    return 0;
};

std::vector<std::string> Run(std::string InFile)
{
    std::ifstream file(InFile.c_str());

    std::vector<std::string> Run;
    
    if(file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            Run.push_back(line);
        };
    };

    return Run;
};

void RecordRunInfo(Myresult &res, RAT::DU::DSReader &dsreader)
{
    RAT::DS::Run run = dsreader.GetRun();

    res.ClearRun();
    res.C_RunID = run.GetRunID();
    res.C_SubRunID = run.GetSubRunID();
    res.C_Date = run.GetDate();//Start Date
    res.C_Time = run.GetTime();//Start Time
    res.C_RunType = run.GetRunType();
};

void RecordEV(Myresult &res, RAT::DU::DSReader &dsreader, const unsigned int iEntry, const unsigned iEv)
{   
    RAT::DS::Entry entry = dsreader.GetEntry(iEntry);
    RAT::DS::EV ev = entry.GetEV(iEv);
    RAT::DS::DataQCFlags data = ev.GetDataCleaningFlags();
    Int_t last_pass = data.GetLatestPass();

    res.ClearMuon();
    res.C_Muon.C_Entry = iEntry;
    res.C_Muon.C_EV = iEv;
    res.C_Muon.C_GTID = ev.GetGTID();
    res.C_Muon.C_OWLHits = ev.GetUncalPMTs().GetOWLCount();
    res.C_Muon.C_Nhits = ev.GetNhits();
    res.C_Muon.C_Nhits_Cleaned = ev.GetNhitsCleaned();
    res.C_Muon.C_50MHz = ev.GetClockCount50();
    
    if(last_pass >= 0)
    {
        res.C_Muon.C_Data_Applied = data.GetApplied(last_pass).GetULong64_t(0);
        res.C_Muon.C_Data_Flags = data.GetFlags(last_pass).GetULong64_t(0);
    };
};