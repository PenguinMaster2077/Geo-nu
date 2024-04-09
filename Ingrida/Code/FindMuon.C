//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TTree.h>

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

int FindMuon()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_0000300000-0000300050.root";
    std::string OutFile = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000300050.root";
    std::string MuonFile = "./Muon.txt";
//Read File
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
    //Set Branch Address
    Int_t RunID, SubRunID, Date, Time, RunType;
    intree->SetBranchAddress("RunID", &RunID);
    intree->SetBranchAddress("SubRunID", &SubRunID);
    intree->SetBranchAddress("Date", &Date);
    intree->SetBranchAddress("Time", &Time);
    intree->SetBranchAddress("RunType", &RunType);
    Int_t Entry, EV;
    intree->SetBranchAddress("MuonEntry", &Entry);
    intree->SetBranchAddress("MuonEv", &EV);
    Int_t GTID, OWLHits;
    intree->SetBranchAddress("MuonGTID", &GTID);
    intree->SetBranchAddress("MuonOWLHits", &OWLHits);
    Int_t Nhits, Nhits_Cleaned;
    intree->SetBranchAddress("MuonNHits", &Nhits);
    intree->SetBranchAddress("MuonNHitsCleaned", &Nhits_Cleaned);
    Long64_t Clock50MHz, Data_Applied, Data_Flags;
    intree->SetBranchAddress("Muon50MHz", &Clock50MHz);
    intree->SetBranchAddress("MuonDataApplied", &Data_Applied);
    intree->SetBranchAddress("MuonDataFlags", &Data_Flags);
//Creat File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "output");
    //Create Branch
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
//Begin to Find
    std::ofstream Muon(MuonFile.c_str());

    Int_t Last_GTID = 0, Last_Run = 0, Last_SubRun = 0;
    Long64_t Last_50MHz = 0;
    int MuonNumber = 0;
    for( int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        intree->GetEntry(ii1);
        if(GTID == 0){continue;};//排除0结果
        if(ii1 == 0){ Last_GTID = GTID; Last_50MHz = Clock50MHz; Last_Run = RunID; Last_SubRun = SubRunID;};//初始化
        if(Nhits < 2500){continue;};
        if( Last_GTID == GTID) {continue;}//后期可以删除，只是排除此时的一个bug
        Last_GTID = GTID;      
        Double_t Delta_T = 20.0 *(Clock50MHz - Last_50MHz);
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
        res.C_Muon.C_50MHz = Clock50MHz;
        res.C_Muon.C_Data_Applied = Data_Applied;
        res.C_Muon.C_Data_Flags = Data_Flags;
        outtree->Fill();
        Muon << RunID << " " << SubRunID << " " << Entry << " " << EV << " " << GTID << " " << OWLHits << " " << Nhits << " " << Clock50MHz << std::endl;
        //std::cout << ii1 << ",Run:" << RunID << ",SubRun:" << SubRunID << ",Entry:" << Entry << ",EV:"
        //<< EV << ",GTID:" << GTID << ",OWLHits:" << OWLHits << ",Nhits:" << Nhits << ",Nhits Cleaned:"
        //<< Nhits_Cleaned << ",Delta T(s):" << Delta_T/1e9 << std::endl;
    };
    outfile->Write();
    outfile->Close();
    std::cout << "Find Muon:" << MuonNumber << std::endl;
    return 0;
}