//CPP
#include <iostream>
#include <fstream>
#include <iomanip>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
//Self-defined
const Long64_t MAX_NUMBER = 8796093022207;//2^43 - 1; DocDB-481
const Long64_t MIN_CARE_NUMBER = 8596093022207;

const double ZOffset = 186;//mm

class Event;
class myresult;

Double_t ComputeDelta_T(Long64_t Last_50MHz, Long64_t Present_50MHz)
{
    
    if(Last_50MHz > Present_50MHz && Last_50MHz > MIN_CARE_NUMBER && Present_50MHz < MIN_CARE_NUMBER)
    {
        Long64_t Part1 = MAX_NUMBER - Last_50MHz;
        Long64_t ClockGap = (Part1 + Present_50MHz);
        std::cout << "注意Clock反转！！！！！！！, Max Clock:" << MAX_NUMBER << ",Last Clock:" << Last_50MHz << ", Present Clock:" << Present_50MHz << ", Delta T(s):" << 20.0 * ClockGap / 1e9 << std::endl;
        return 20.0 * ClockGap;
    }
    else
    {
        Long64_t ClockGap = (Present_50MHz - Last_50MHz);
        return 20.0 *ClockGap;
    };
};

bool PassMuonCut(int &Index_Muon, const int Muon_Len, ULong64_t Muon_50MHz[], Int_t Muon_Run[], Int_t Muon_SubRun[], ULong64_t Prompt_50MHz, Int_t Prompt_Run, Int_t Prompt_SubRun);

int FindIBD()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Data/Gold_20R_CoincidencePair_0000300000-0000307612.root";
    std::string InMuonFile = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000307612.root";
    std::string OutRootFile = "/rat/MyCode/Work/Geo-nu-Data/IBDCandidate/Gold_20R_IBD_Candidate_0000300000-0000307612.root";
    std::string OutFile = "./CoincidencePair.txt";
    std::string OutFile2 = "./CoincidencePair_DataFlags.txt";
    std::string OutFile3 = "./CoincidencePair_DataFlags_Muon.txt";
    std::string OutFile4 = "./CoincidencePair_Find_Multi_Event.txt";
//Read Coincidence Pair Files
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
    Double_t ZOffSet;
    intree->SetBranchAddress("ZOffSet", &ZOffSet);
//Prompt
    int PromptEntry, PromptEv;
    intree->SetBranchAddress("PromptEntry", &PromptEntry);
    intree->SetBranchAddress("PromptEv", &PromptEv);
    Int_t PromptGTID, PromptOWLHits;
    intree->SetBranchAddress("PromptGTID", &PromptGTID);
    intree->SetBranchAddress("PromptOWLHits", &PromptOWLHits);
    Int_t PromptNHits, PromptNHitsCleaned, PromptNHitsCorrected;
    intree->SetBranchAddress("PromptNHits", &PromptNHits);
    intree->SetBranchAddress("PromptNHitsCleaned", &PromptNHitsCleaned);
    intree->SetBranchAddress("PromptNHitsCorrected", &PromptNHitsCorrected);
    Long64_t Prompt50MHz;
    intree->SetBranchAddress("Prompt50MHz", &Prompt50MHz);
    Long64_t PromptDataApplied, PromptDataFlags;
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
    int DelayedEntry, DelayedEv;
    intree->SetBranchAddress("DelayedEntry", &DelayedEntry);
    intree->SetBranchAddress("DelayedEv", &DelayedEv);
    Int_t DelayedGTID, DelayedOWLHits;
    intree->SetBranchAddress("DelayedGTID", &DelayedGTID);
    intree->SetBranchAddress("DelayedOWLHits", &DelayedOWLHits);
    Int_t DelayedNHits, DelayedNHitsCleaned, DelayedNHitsCorrected;
    intree->SetBranchAddress("DelayedNHits", &DelayedNHits);
    intree->SetBranchAddress("DelayedNHitsCleaned", &DelayedNHitsCleaned);
    intree->SetBranchAddress("DelayedNHitsCorrected", &DelayedNHitsCorrected);
    Long64_t Delayed50MHz;
    intree->SetBranchAddress("Delayed50MHz", &Delayed50MHz);
    Long64_t DelayedDataApplied, DelayedDataFlags;
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
//Read and Load Muon File
    TFile *muonfile = new TFile(InMuonFile.c_str());
    TTree *muontree = (TTree*) muonfile->Get("output");
    //Set Branch Addres
    muontree->SetBranchAddress("RunID", &RunID);
    muontree->SetBranchAddress("SubRunID", &SubRunID);
    muontree->SetBranchAddress("Date", &Date);
    muontree->SetBranchAddress("Time", &Time);
    muontree->SetBranchAddress("RunType", &RunType);
    Int_t Muon_Entry, Muon_EV;
    muontree->SetBranchAddress("MuonEntry", &Muon_Entry);
    muontree->SetBranchAddress("MuonEv", &Muon_EV);
    Int_t Muon_GTID, Muon_OWLHits;
    muontree->SetBranchAddress("MuonGTID", &Muon_GTID);
    muontree->SetBranchAddress("MuonOWLHits", &Muon_OWLHits);
    Int_t Muon_Nhits, Muon_Nhits_Cleaned;
    muontree->SetBranchAddress("MuonNHits", &Muon_Nhits);
    muontree->SetBranchAddress("MuonNHitsCleaned", &Muon_Nhits_Cleaned);
    Long64_t Muon_Clock50MHz, Muon_Data_Applied, Muon_Data_Flags;
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
        Muon_Run_Array[ii1] = RunID;
        Muon_SubRun_Array[ii1] = SubRunID;
        Muon_GTID_Array[ii1] = Muon_GTID;
        Muon_50MHz_Array[ii1] = Muon_Clock50MHz;
    };
//Create Out Root File
    TFile *outfile = new TFile(OutRootFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "output");
    //Create Branch
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
    outtree->Branch("PromptPosX", &res.C_Prompt.C_Pos_X);
    outtree->Branch("PromptPosY", &res.C_Prompt.C_Pos_Y);
    outtree->Branch("PromptPosZ", &res.C_Prompt.C_Pos_Z);
    outtree->Branch("PromptEnergy", &res.C_Prompt.C_Energy);
    outtree->Branch("PromptOriginalEnergy", &res.C_Prompt.C_Original_Energy);
    outtree->Branch("PromptTonyEnergy", res.C_Prompt.C_Tony_Energy);
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
    outtree->Branch("DelayedPosX", &res.C_Delayed.C_Pos_X);
    outtree->Branch("DelayedPosY", &res.C_Delayed.C_Pos_Y);
    outtree->Branch("DelayedPosZ", &res.C_Delayed.C_Pos_Z);
    outtree->Branch("DelayedEnergy", &res.C_Delayed.C_Energy);
    outtree->Branch("DelayedOriginalEnergy", &res.C_Delayed.C_Original_Energy);
    outtree->Branch("DelayedTonyEnergy", res.C_Delayed.C_Tony_Energy);
//Begin to Find Coincidence Pair
    std::ofstream coincidence_pair(OutFile.c_str());
    std::ofstream coincidence_pair_dataflags(OutFile2.c_str());
    std::ofstream coincidence_pair_dataflags_muon(OutFile3.c_str());
    std::ofstream coincidence_pair_multi(OutFile4.c_str());

    int Coin_Pair_Numer = 0, After_Flags_Numer = 0, After_Flags_Muon_Numer = 0;
    int Index_Muon_Array = 0, Change_Run_SubRun = 0;
    ULong64_t PromptFlags = 0, DelayedFlags = 0;
    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        intree->GetEntry(ii1);
        if(PromptGTID == 0) {continue;};
        if(PromptNHits >= 2500){continue;};
        TVector3 PromptPos = TVector3(PromptPosX, PromptPosY, PromptPosZ);
        TVector3 DelayedPos = TVector3(DelayedPosX, DelayedPosY, DelayedPosZ);
        TVector3 Delta_R = DelayedPos - PromptPos;
        ULong64_t prompt_50MHz = Prompt50MHz, delayed_50MHz = Delayed50MHz;
        Double_t Delta_T = ComputeDelta_T(prompt_50MHz, delayed_50MHz);//ns;
        Coin_Pair_Numer++;
    //Recording
        coincidence_pair << RunID << " " << SubRunID << " " << PromptGTID << " " << PromptEnergy << " "
        << PromptPos.Mag() << " " << DelayedGTID << " " << DelayedEnergy << " " << DelayedPos.Mag() 
        << " " << Delta_T << " " << Delta_R.Mag() << std::endl;
    //Data Clean Cut
        PromptFlags = PromptDataFlags;
        DelayedFlags = DelayedDataFlags;
        if( (PromptFlags &0x2100000042C2) != 0x2100000042C2){ continue;};
        if( (DelayedFlags &0x2100000042C2) != 0x2100000042C2){ continue;};
    //Recording
        After_Flags_Numer++;
        //if( (DelayedDataFlags &0x2100000042C2) != 0x2100000042C2){ continue;};
        coincidence_pair_dataflags << RunID << " " << SubRunID << " " << PromptGTID << " " << PromptEnergy << " "
        << PromptPos.Mag() << " " << DelayedGTID << " " << DelayedEnergy << " " << DelayedPos.Mag() 
        << " " << Delta_T << " " << Delta_R.Mag() << std::endl;
    //Muon Cut
        //Find the Closest Muon Event
        if( Muon_Run_Array[Index_Muon_Array] < RunID)
        {
            for(int ii2 = Index_Muon_Array; ii2 < Muon_Len; ii2++)
            {
                if(Muon_Run_Array[ii2] == RunID)
                {
                    Index_Muon_Array = ii2;
                    break;
                };
            };
            for(int ii2 = Index_Muon_Array; ii2 < Muon_Len; ii2++)
            {
                if(Muon_SubRun_Array[ii2] >= SubRunID && Muon_Run_Array[ii2] == RunID)
                {
                    Index_Muon_Array = ii2 - 1;
                    break;
                };
            };
            std::cout << "更新Muon:" << Muon_Run_Array[Index_Muon_Array] << ",SubRun:" << Muon_SubRun_Array[Index_Muon_Array] << ",Prompt Run:" << RunID << ", Prompt SubRun:" << SubRunID << std::endl;
        };
        if( PassMuonCut(Index_Muon_Array, Muon_Len, Muon_50MHz_Array, Muon_Run_Array, Muon_SubRun_Array, Prompt50MHz, RunID, SubRunID) == false){continue;};
        After_Flags_Muon_Numer++;
        //Recording
        coincidence_pair_dataflags_muon << RunID << " " << SubRunID << " " << PromptGTID << " " << PromptEnergy << " "
        << PromptPos.Mag() << " " << DelayedGTID << " " << DelayedEnergy << " " << DelayedPos.Mag() 
        << " " << Delta_T << " " << Delta_R.Mag() << std::endl;

        //Recording for Multi Cut
        coincidence_pair_multi << RunID << " "  << std::setw(3) << std::setfill('0') << SubRunID << " " << PromptEntry << " " << DelayedEntry << std::endl;

        std::cout << "通过Muon Cut，输出Coincidence Pair的信息：" << std::endl;
        std::cout << "Prompt Info:" << std::endl;
        std::cout << "Run:" << RunID << ",SubRun:" << SubRunID << ",GTID:" << PromptGTID  << ",Entry:" << PromptEntry << ",EV:" << PromptEv  << ",Energy:" << PromptEnergy << ",Original Energ:" << Prompt_Original_Energy << ", Tony Energy:" << Prompt_Tony_Energy << ",NHits:" << PromptNHits <<  ",OWL:" << PromptOWLHits << ",50MHz:" << Prompt50MHz << ",Applied:" << PromptDataApplied << ",Flags:" << PromptFlags << ", Flags&Mask:" << ( (PromptFlags & 0x2100000042C2) == 0x2100000042C2) << std::endl;
        std::cout << "Delayed Info:" << std::endl;
        std::cout << "Run:" << RunID << ",SubRun:" << SubRunID << ",GTID:" << DelayedGTID << ",Entry:" << DelayedEntry << ",EV:" << DelayedEv << ",Energy:" << DelayedEnergy << ",Original Energy:" << Delayed_Original_Energy << ", Tony Energy:" << Delayed_Tony_Energy << ",NHits:" << DelayedNHits << ",OWL:" << DelayedOWLHits << ",50MHz:" << Delayed50MHz << ",Applied:" << DelayedDataApplied << ",Flags:" << DelayedFlags << ", Flags&Mask:" << ((DelayedFlags & 0x2100000042C2) == 0x2100000042C2)  <<  std::endl << std::endl;
        //Recording Info
            //Run Info
            res.C_RunID = RunID;
            res.C_SubRunID = SubRunID;
            res.C_Date = Date;
            res.C_Time = Time;
            res.C_RunType = RunType;
            res.C_ZOffSet = ZOffSet;
            //Prompt
            res.C_Prompt.C_Entry = PromptEntry;
            res.C_Prompt.C_EV = PromptEv;
            res.C_Prompt.C_GTID = PromptGTID;
            res.C_Prompt.C_OWLHits = PromptOWLHits;
            res.C_Prompt.C_Nhits = PromptNHits;
            res.C_Prompt.C_Nhits_Cleaned = PromptNHitsCleaned;
            res.C_Prompt.C_Nhits_Corrected = PromptNHitsCorrected;
            res.C_Prompt.C_50MHZ = Prompt50MHz;
            res.C_Prompt.C_Data_Applied = PromptDataApplied;
            res.C_Prompt.C_Data_Flags = PromptDataFlags;
            res.C_Prompt.C_Pos_X = PromptPosX;
            res.C_Prompt.C_Pos_Y = PromptPosY;
            res.C_Prompt.C_Pos_Z = PromptPosZ;
            res.C_Prompt.C_Energy = PromptEnergy;
            res.C_Prompt.C_Original_Energy = Prompt_Original_Energy;
            res.C_Prompt.C_Tony_Energy = Prompt_Tony_Energy;
            //Delayed
            res.C_Delayed.C_Entry = DelayedEntry;
            res.C_Delayed.C_EV = DelayedEv;
            res.C_Delayed.C_GTID = DelayedGTID;
            res.C_Delayed.C_OWLHits = DelayedOWLHits;
            res.C_Delayed.C_Nhits = DelayedNHits;
            res.C_Delayed.C_Nhits_Cleaned = DelayedNHitsCleaned;
            res.C_Delayed.C_Nhits_Corrected = DelayedNHitsCorrected;
            res.C_Delayed.C_50MHZ = Delayed50MHz;
            res.C_Delayed.C_Data_Applied = DelayedDataApplied;
            res.C_Delayed.C_Data_Flags = DelayedDataFlags;
            res.C_Delayed.C_Pos_X = DelayedPosX;
            res.C_Delayed.C_Pos_Y = DelayedPosY;
            res.C_Delayed.C_Pos_Z = DelayedPosZ;
            res.C_Delayed.C_Energy = DelayedEnergy;
            res.C_Delayed.C_Original_Energy = Delayed_Original_Energy;
            res.C_Delayed.C_Tony_Energy = Delayed_Tony_Energy;
        outtree->Fill();
    };
    coincidence_pair.close();
    coincidence_pair_dataflags.close();
    coincidence_pair_dataflags_muon.close();
    coincidence_pair_multi.close();

    outfile->Write();
    outfile->Close();
    std::cout << "Coincidence Pair Number:" << Coin_Pair_Numer << ", After Data Flags Number:" << After_Flags_Numer << ", After Muon Cut Number:" << After_Flags_Muon_Numer << std::endl;

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
        std::cout << "OWL:" << C_OWLHits << std::endl;
        std::cout << "Nhits:" << C_Nhits << std::endl;
        std::cout << "NhitsCleaned:" << C_Nhits_Cleaned << std::endl;
        std::cout << "NhitsCorre:" << C_Nhits_Corrected << std::endl;
        std::cout << "50MHz:" << C_50MHZ << std::endl;
        std::cout << "Applied:" << C_Data_Applied << std::endl;
        std::cout << "Flags:" << C_Data_Flags << std::endl;
    };
    void ShowPosTime()
    {
        std::cout << "E:" << C_Energy << ",X:" << C_Pos_X << ",Y:" << C_Pos_Y << ",Z:" << C_Pos_Z << std::endl;
        std::cout << "Original E:" << C_Tony_Energy << ", Tony Energy:" << C_Tony_Energy << std::endl;
    };
//EV Info
    unsigned int C_Entry, C_EV;
    Int_t C_GTID, C_OWLHits;
    UInt_t C_Nhits, C_Nhits_Cleaned, C_Nhits_Corrected;
    ULong64_t C_50MHZ, C_Data_Applied, C_Data_Flags;
//4D Position and Time
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
        std::cout << "Z OffSet:" << C_ZOffSet << std::endl;
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

bool PassMuonCut(int &Index_Muon, const int Muon_Len, ULong64_t Muon_50MHz[], Int_t Muon_Run[], Int_t Muon_SubRun[], ULong64_t Prompt_50MHz, Int_t Prompt_Run, Int_t Prompt_SubRun)
{
    Double_t Delta_T = ComputeDelta_T(Muon_50MHz[Index_Muon - 1], Prompt_50MHz);
    std::cout << "前一个Muon检测信息。Run:" << Muon_Run[Index_Muon - 1] << ",SubRun:" << Muon_SubRun[Index_Muon - 1] << ",50MHz:" << Muon_50MHz[Index_Muon - 1] << ",Prompt Run:" << Prompt_Run << ",Prompt SubRun:" << Prompt_SubRun << ",Delta T(s):" << Delta_T/1e9 << std::endl;
    int FindMuon = 0;
    if(Delta_T >= 0 && Delta_T <= 20e9){return false;};
    if(Delta_T > 20e9 || Delta_T < 0)
    {
        for(int ii2 = Index_Muon; ii2 < Muon_Len; ii2++)
        {
            if(Muon_Run[ii2] > Prompt_Run){ break;};
            Delta_T = ComputeDelta_T(Muon_50MHz[ii2], Prompt_50MHz);
            std::cout << "Run:" << Muon_Run[ii2] << ",SubRun:" << Muon_SubRun[ii2] << ",50MHz:" << Muon_50MHz[ii2] << ",Prompt Run:" << Prompt_Run  << ",Prompt SubRun:" << Prompt_SubRun << ",Delta T(s):" << Delta_T/1e9 << std::endl;
            if(Delta_T >=0 && Delta_T<=20e9)
            {
                std::cout << "找到对应的Muon。Run:" << Muon_Run[ii2] << ",SubRun:" << Muon_SubRun[ii2] << ",Prompt Run:" << Prompt_Run << ",Prompt SubRun:" << Prompt_SubRun << ",Delta T(s):" << Delta_T/1e9  << std::endl << std::endl;
                FindMuon = 1; 
                break;
            };
        };
    };
    if(FindMuon == 1){return false;}
    else
    {
        std::cout << "并没有找到对应的Muon，通过Muon Cut" << std::endl;
        return true;
    };
}; 