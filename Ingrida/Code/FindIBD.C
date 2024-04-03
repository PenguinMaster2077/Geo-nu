//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
//Self-defined

const double ZOffset = 186;//mm
class Event;
class myresult;

int FindIBD()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Data_20240329/Gold_20R_CoincidencePair_0000300000-0000300400.root";
    std::string OutFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Data_20240329/Gold_20R_CoincidencePair_0000300000-0000300400.txt";

    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
    
    //intree->Print();
    //return 0;
    Int_t RunID, SubRunID, Date, Time, RunType;
    intree->SetBranchAddress("RunID", &RunID);
    intree->SetBranchAddress("SubRunID", &SubRunID);
    intree->SetBranchAddress("Date", &Date);
    intree->SetBranchAddress("Time", &Time);
    intree->SetBranchAddress("RunType", &RunType);
//Prompt
    int PromptEntry, PromptEv;
    intree->SetBranchAddress("PromptEntry", &PromptEntry);
    intree->SetBranchAddress("PromptEv", &PromptEv);
    Int_t PromptGTID;
    intree->SetBranchAddress("PromptGTID", &PromptGTID);
    Int_t PromptNHits, PromptNHitsCleaned, PromptNHitsCorrected;
    intree->SetBranchAddress("PromptNHits", &PromptNHits);
    intree->SetBranchAddress("PromptNHitsCleaned", &PromptNHitsCleaned);
    intree->SetBranchAddress("PromptNHitsCorrected", &PromptNHitsCorrected);
    Long64_t Prompt50MHz;
    intree->SetBranchAddress("Prompt50MHz", &Prompt50MHz);
    Double_t PromptPosX, PromptPosY, PromptPosZ, PromptEnergy;
    intree->SetBranchAddress("PromptPosX", &PromptPosX);
    intree->SetBranchAddress("PromptPosY", &PromptPosY);
    intree->SetBranchAddress("PromptPosZ", &PromptPosZ);
    intree->SetBranchAddress("PromptEnergy", &PromptEnergy);
//Delayed
    int DelayedEntry, DelayedEv;
    intree->SetBranchAddress("DelayedEntry", &DelayedEntry);
    intree->SetBranchAddress("DelayedEv", &DelayedEv);
    Int_t DelayedGTID;
    intree->SetBranchAddress("DelayedGTID", &DelayedGTID);
    Int_t DelayedNHits, DelayedNHitsCleaned, DelayedNHitsCorrected;
    intree->SetBranchAddress("DelayedNHits", &DelayedNHits);
    intree->SetBranchAddress("DelayedNHitsCleaned", &DelayedNHitsCleaned);
    intree->SetBranchAddress("DelayedNHitsCorrected", &DelayedNHitsCorrected);
    Long64_t Delayed50MHz;
    intree->SetBranchAddress("Delayed50MHz", &Delayed50MHz);
    Double_t DelayedPosX, DelayedPosY, DelayedPosZ, DelayedEnergy;
    intree->SetBranchAddress("DelayedPosX", &DelayedPosX);
    intree->SetBranchAddress("DelayedPosY", &DelayedPosY);
    intree->SetBranchAddress("DelayedPosZ", &DelayedPosZ);
    intree->SetBranchAddress("DelayedEnergy", &DelayedEnergy);

    std::ofstream file(OutFile.c_str());

    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        intree->GetEntry(ii1);
        if(PromptGTID == 0) {continue;};
        TVector3 PromptPos = TVector3(PromptPosX, PromptPosY, PromptPosZ);
        TVector3 DelayedPos = TVector3(DelayedPosX, DelayedPosY, DelayedPosZ);
        TVector3 Delta_R = DelayedPos - PromptPos;
        Long64_t Delta_T = (Delayed50MHz - Prompt50MHz) * 20;//ns;
        file << RunID << " " << SubRunID << " " << PromptGTID << " " << PromptEnergy << " "
        << PromptPos.Mag() << " " << DelayedGTID << " " << DelayedEnergy << " " << DelayedPos.Mag() 
        << " " << Delta_T << " " << Delta_R.Mag() << std::endl;
    };
    file.close();

    return 0;
};

class Event
{
public:
    Event(){ClearAll();};
    void ClearAll()
    {
        ClearEV();
        ClearPosEnergy();
    };
    void ClearEV()
    {
        C_Entry = 0;
        C_EV = 0;
        C_GTID = 0;
        C_Nhits = 0;
        C_Nhits_Cleaned = 0;
        C_Nhits_Corrected = 0;
        C_50MHZ = 0;
    };
    void ClearPosEnergy()
    {
        C_Energy = 0;
        C_Pos_X = 0;
        C_Pos_Y = 0;
        C_Pos_Z = 0;
    };
    void ShowDetals()
    {
        ShowEVInfo();
        ShowPosTime();
    };
    void ShowEVInfo()
    {
        std::cout << "Entry:" << C_Entry << std::endl;
        std::cout << "EV:" << C_EV << std::endl;
        std::cout << "GTID:" << C_GTID << std::endl;
        std::cout << "Nhits:" << C_Nhits << std::endl;
        std::cout << "NhitsCleaned:" << C_Nhits_Cleaned << std::endl;
        std::cout << "NhitsCorre:" << C_Nhits_Corrected << std::endl;
        std::cout << "50MHz:" << C_50MHZ << std::endl;
    };
    void ShowPosTime()
    {
        std::cout << "E:" << C_Energy << ",X:" << C_Pos_X << ",Y:" << C_Pos_Y << ",Z:" << C_Pos_Z << std::endl;
    };
//EV Info
    unsigned int C_Entry, C_EV;
    Int_t C_GTID;
    UInt_t C_Nhits, C_Nhits_Cleaned, C_Nhits_Corrected;
    ULong64_t C_50MHZ;
//4D Position and Time
    Double_t C_Energy, C_Pos_X, C_Pos_Y, C_Pos_Z;
};

class myresult
{
public:
    myresult()
    {
        ClearAll();
    }
    void ClearAll()
    {
        ClearRunInfo();
        ClearPrompt();
        ClearDelayed();
    };
    void ClearRunInfo()
    {
        C_RunID = 0;
        C_SubRunID = 0;
        C_Date = 0;
        C_Time = 0;
        C_RunType = 0;
    };
    void ShowRunInfo()
    {
        std::cout << "RunID:" << C_RunID << std::endl;
        std::cout << "SubRunID:" << C_SubRunID << std::endl;
        std::cout << "Date:" << C_Date << std::endl;
        std::cout << "Time:" << C_Time << std::endl;
        std::cout << "RunType:" << C_RunType << std::endl;
    };
    void ClearPrompt()  {C_Prompt.ClearAll();};
    void ClearDelayed() {C_Delayed.ClearAll();};
    void ShowPrompt()   {std::cout << "Details of Prompt" << std::endl; C_Prompt.ShowDetals();};
    void ShowDelayed()  {std::cout << "Details of Dealyed" << std::endl; C_Delayed.ShowDetals();};
//Run Information
    UInt_t C_RunID, C_SubRunID, C_Date, C_Time, C_RunType;
//Prompt and Delayed
    Event C_Prompt, C_Delayed;
};