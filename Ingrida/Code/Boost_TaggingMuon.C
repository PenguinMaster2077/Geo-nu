//CPP
#include <iostream>
#include <fstream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
//RAT
#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Run.hh>

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

void RecordEV(Myresult &res, const Int_t Entry, const Int_t EV, const Int_t GTID,
const Int_t OWLHits, const Int_t NHits, const Int_t NhitsCleaned, const Long64_t Clock50MHz,
const Long64_t Applied, const Long64_t Flags);

// int Zombie_Muon()
int main(int argc, char** argv)
{
    std::string RATDSFile = argv[1];
    std::string NTUPLEFile = argv[2];
    std::string OutFile = argv[3];
    TString sub_run = argv[4];
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
    //Read RATDS File
    RAT::DU::DSReader dsreader(RATDSFile);
    //Read NTUPLE File
    TFile *ntuplefile = new TFile(NTUPLEFile.c_str());
    TTree *ntupletree = (TTree*)ntuplefile->Get("output");
    //Set Wanted Branch
    Int_t ntuple_Entry, ntuple_EV;
    ntupletree->SetBranchAddress("mcIndex", &ntuple_Entry);
    ntupletree->SetBranchAddress("evIndex", &ntuple_EV);
    Int_t ntuple_GTID;
    ntupletree->SetBranchAddress("eventID", &ntuple_GTID);
    Int_t ntuple_OWLHits, ntuple_NHits, ntuple_NHitsCleaned;
    ntupletree->SetBranchAddress("owlnhits", &ntuple_OWLHits);
    ntupletree->SetBranchAddress("nhits", &ntuple_NHits);
    ntupletree->SetBranchAddress("nhitsCleaned", &ntuple_NHitsCleaned);
    ULong64_t ntuple_50MHz, ntuple_Data_Applied, ntuple_Data_Flags;
    ntupletree->SetBranchAddress("clockCount50", &ntuple_50MHz);
    ntupletree->SetBranchAddress("dcApplied", &ntuple_Data_Applied);
    ntupletree->SetBranchAddress("dcFlagged", &ntuple_Data_Flags);
//Record Run Info
    RecordRunInfo(res, dsreader);
    res.C_SubRunID = sub_run.Atoi();
//Loop
    int LargeNhits = 0, LargeOWPMT = 0, MuonNumber = 0;
    std::cout << "Begin to Research" << std::endl;
    for(int iEntry = 0; iEntry < ntupletree->GetEntries(); iEntry++)
    {
        LargeNhits = 0;
        LargeOWPMT = 0;
        if(iEntry % 1000 ==0) { std::cout << "已处理" << iEntry << ",还剩" << ntupletree->GetEntries() - iEntry << std::endl;};
        ntupletree->GetEntry(iEntry);
        if(ntuple_NHits >= 2000){LargeNhits = 1;};
        if(ntuple_OWLHits >= 3){LargeOWPMT = 1;};
        if( LargeNhits == 0 && LargeOWPMT == 0){continue;};
        std::cout << "Find Muon Event!!!!!!" << std::endl;
        MuonNumber++;
        RecordEV(res, ntuple_Entry, ntuple_EV, ntuple_GTID, ntuple_OWLHits, ntuple_NHits, ntuple_NHitsCleaned, ntuple_50MHz, ntuple_Data_Applied, ntuple_Data_Flags);
        res.ShowMuon();
        outtree->Fill();
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

void RecordEV(Myresult &res, const Int_t Entry, const Int_t EV, const Int_t GTID,
const Int_t OWLHits, const Int_t NHits, const Int_t NhitsCleaned, const Long64_t Clock50MHz,
const Long64_t Applied, const Long64_t Flags)
{   
    res.ClearMuon();
    res.C_Muon.C_Entry = Entry;
    res.C_Muon.C_EV = EV;
    res.C_Muon.C_GTID = GTID;
    res.C_Muon.C_OWLHits = OWLHits;
    res.C_Muon.C_OWLHits = OWLHits;
    res.C_Muon.C_Nhits = NHits;
    res.C_Muon.C_Nhits_Cleaned = NhitsCleaned;
    res.C_Muon.C_50MHz = Clock50MHz;
    res.C_Muon.C_Data_Applied = Applied;
    res.C_Muon.C_Data_Flags = Flags;
};