//CPP
#include <iostream>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined

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

        C_OWLHits = 0;
        C_Nhits = 0;
        C_Nhits_Cleaned = 0;
        C_Nhits_Corrected = 0;
        C_50MHZ = 0;
        C_Data_Applied = 0;
        C_Data_Flags = 0;
    };
    void ClearPosEnergy()
    {
        C_Energy = 0;
        C_Pos_X = 0;
        C_Pos_Y = 0;
        C_Pos_Z = 0;

        C_Original_Energy = 0;
        C_Tony_Energy = 0;
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
        std::cout << "OWLHits:" << C_OWLHits << std::endl;
        std::cout << "Nhits:" << C_Nhits << std::endl;
        std::cout << "NhitsCleaned:" << C_Nhits_Cleaned << std::endl;
        std::cout << "NhitsCorrect:" << C_Nhits_Corrected << std::endl;
        std::cout << "50MHz:" << C_50MHZ << std::endl;
        std::cout << "Data Applied:" << C_Data_Applied << std::endl;
        std::cout << "Data Flags:" << C_Data_Flags << std::endl;
    };
    void ShowPosTime()
    {
        std::cout << "Energy:" << C_Energy << ",X:" << C_Pos_X << ",Y:" << C_Pos_Y << ",Z:" << C_Pos_Z << std::endl;
        std::cout << "Original Energy:" << C_Original_Energy <<",Tony Energy:" << C_Tony_Energy << std::endl;
    };
//EV Info
    Int_t C_Entry, C_EV;
    Int_t C_GTID;
    Int_t C_OWLHits;
    Int_t C_Nhits, C_Nhits_Cleaned, C_Nhits_Corrected;
    ULong64_t C_50MHZ, C_Data_Applied, C_Data_Flags;
//Energy and Position
    Double_t C_Energy, C_Pos_X, C_Pos_Y, C_Pos_Z;
    Double_t C_Original_Energy, C_Tony_Energy;//Tony Energy: Original + Tony Correction
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
        C_ZOffSet = 0;
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
    Double_t C_ZOffSet;
//Prompt and Delayed
    Event C_Prompt, C_Delayed;
};

class myresult_muon
{
public:
    myresult_muon() { ClearAll();};
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