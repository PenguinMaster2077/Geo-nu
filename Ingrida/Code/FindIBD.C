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

bool PassMuonCut(int &Change_Run_SubRun, const int Muon_Len, int &Index_Muon_Array, Long64_t Muon_50MHz[], Int_t Muon_Run[], Int_t Muon_SubRun[], Long64_t Prompt_50MHz);

int FindIBD()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Data/Gold_20R_CoincidencePair_0000300000-0000301000.root";
    std::string InMuonFile = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000300050.root";
    std::string OutFile = "./CoincidencePair.txt";
    std::string OutFile2 = "./CoincidencePair_DataFlags.txt";
    std::string OutFile3 = "./CoincidencePair_DataFlags_Muon.txt";
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
    Int_t PromptGTID;
    intree->SetBranchAddress("PromptGTID", &PromptGTID);
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
    Long64_t DelayedDataApplied, DelayedDataFlags;
    intree->SetBranchAddress("DelayedDataApplied", &DelayedDataApplied);
    intree->SetBranchAddress("DelayedDataFlags", &DelayedDataFlags);
    Double_t DelayedPosX, DelayedPosY, DelayedPosZ, DelayedEnergy;
    intree->SetBranchAddress("DelayedPosX", &DelayedPosX);
    intree->SetBranchAddress("DelayedPosY", &DelayedPosY);
    intree->SetBranchAddress("DelayedPosZ", &DelayedPosZ);
    intree->SetBranchAddress("DelayedEnergy", &DelayedEnergy);
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
    Long64_t Muon_50MHz_Array[Muon_Len];
    for(int ii1 = 0; ii1 < Muon_Len; ii1++)
    {
        muontree->GetEntry(ii1);
        Muon_Run_Array[ii1] = RunID;
        Muon_SubRun_Array[ii1] = SubRunID;
        Muon_GTID_Array[ii1] = Muon_GTID;
        Muon_50MHz_Array[ii1] = Muon_Clock50MHz;
    };
//Begin to Find Coincidence Pair
    std::ofstream coincidence_pair(OutFile.c_str());
    std::ofstream coincidence_pair_dataflags(OutFile2.c_str());
    std::ofstream coincidence_pair_dataflags_muon(OutFile3.c_str());

    int Coin_Pair_Numer = 0, After_Flags_Numer = 0, After_Flags_Muon_Numer = 0;
    int Index_Muon_Array = 0, Change_Run_SubRun = 0;
    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        intree->GetEntry(ii1);
        if(PromptGTID == 0) {continue;};
        if(PromptNHits >= 2500){continue;};
        TVector3 PromptPos = TVector3(PromptPosX, PromptPosY, PromptPosZ);
        TVector3 DelayedPos = TVector3(DelayedPosX, DelayedPosY, DelayedPosZ);
        TVector3 Delta_R = DelayedPos - PromptPos;
        Double_t Delta_T = (Delayed50MHz - Prompt50MHz) * 20;//ns;
        Coin_Pair_Numer++;
    //Recording
        coincidence_pair << RunID << " " << SubRunID << " " << PromptGTID << " " << PromptEnergy << " "
        << PromptPos.Mag() << " " << DelayedGTID << " " << DelayedEnergy << " " << DelayedPos.Mag() 
        << " " << Delta_T << " " << Delta_R.Mag() << std::endl;
    //Data Clean Cut
        ULong64_t temp = PromptDataFlags;
        if( (temp &0x2100000042C2) != 0x2100000042C2){ continue;};
        temp = DelayedDataFlags;
        if( (temp &0x2100000042C2) != 0x2100000042C2){ continue;};
    //Recording
        After_Flags_Numer++;
        //if( (DelayedDataFlags &0x2100000042C2) != 0x2100000042C2){ continue;};
        coincidence_pair_dataflags << RunID << " " << SubRunID << " " << PromptGTID << " " << PromptEnergy << " "
        << PromptPos.Mag() << " " << DelayedGTID << " " << DelayedEnergy << " " << DelayedPos.Mag() 
        << " " << Delta_T << " " << Delta_R.Mag() << std::endl;
    //Muon Cut
        //Same Run and SubRun
        if(Muon_Run_Array[Index_Muon_Array] < RunID)//Same Run
        {
            for(int ii2 = Index_Muon_Array; ii2 < Muon_Len; ii2++)
            {
                if(Muon_Run_Array[ii2] == RunID) 
                {
                    Index_Muon_Array = ii2;
                    break;
                };
            };
        };
        if(Muon_SubRun_Array[Index_Muon_Array] < SubRunID)//Same SubRun
        {
            for(int ii2 = Index_Muon_Array; ii2 < Muon_Len; ii2++)
            {
                if(Muon_SubRun_Array[ii2] == SubRunID)
                {
                    Index_Muon_Array = ii2;
                    break;
                };
            }  
        };
        if(Muon_Run_Array[Index_Muon_Array - 1] != Muon_Run_Array[Index_Muon_Array] || Muon_SubRun_Array[Index_Muon_Array - 1] != Muon_SubRun_Array[Index_Muon_Array]){Change_Run_SubRun = 1;};
        if(PassMuonCut(Change_Run_SubRun, Muon_Len, Index_Muon_Array, Muon_50MHz_Array, Muon_Run_Array, Muon_SubRun_Array, Prompt50MHz) == false){continue;};
        After_Flags_Muon_Numer++;
        //Recording
        coincidence_pair_dataflags_muon << RunID << " " << SubRunID << " " << PromptGTID << " " << PromptEnergy << " "
        << PromptPos.Mag() << " " << DelayedGTID << " " << DelayedEnergy << " " << DelayedPos.Mag() 
        << " " << Delta_T << " " << Delta_R.Mag() << std::endl;

        std::cout << "通过Muon Cut，输出Coincidence Pair的信息：" << std::endl;
        std::cout << "Prompt Info:" << std::endl;
        std::cout << "Run:" << RunID << ",SubRun:" << SubRunID << ",GTID:" << PromptGTID  << ",Entry:" << PromptEntry << ",EV:" << PromptEv  << ",NHits:" << PromptNHits << ",50MHz:" << Prompt50MHz << std::endl;
        std::cout << "Delayed Info:" << std::endl;
        std::cout << "Run:" << RunID << ",SubRun:" << SubRunID << ",GTID" << DelayedGTID << ",Entry:" << DelayedEntry << ",EV:" << DelayedEv << ",NHits:" << DelayedNHits << ",50MHz:" << Delayed50MHz <<  std::endl << std::endl;

    };
    coincidence_pair.close();
    coincidence_pair_dataflags.close();

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

bool PassMuonCut(int &Change_Run_SubRun, const int Muon_Len, int &Index_Muon_Array, Long64_t Muon_50MHz[], Int_t Muon_Run[], Int_t Muon_SubRun[], Long64_t Prompt_50MHz)
{
    Double_t Delta_T = 20.0 *(Prompt_50MHz - Muon_50MHz[Index_Muon_Array]);
    bool PassMuonCut = false;
    bool In_Next_Muon = false;
    std::cout << "开始检测，此时Muon信息为：" << Muon_Run[Index_Muon_Array] << ",SubRun:" << Muon_SubRun[Index_Muon_Array] << ", 50MHz:" << Muon_50MHz[Index_Muon_Array] << std::endl;
    if(Delta_T < 0 && Change_Run_SubRun == 1)
    { 
        std::cout << "通过Muon Cut，此时Muon信息为:" << Muon_Run[Index_Muon_Array] << ",SubRun:" << Muon_SubRun[Index_Muon_Array] << ", 50MHz:" << Muon_50MHz[Index_Muon_Array] << std::endl;
        std::cout << "此时Delta T(s):" << 20.0 * (Prompt_50MHz - Muon_50MHz[Index_Muon_Array]) / 1e9 << std::endl;
        Change_Run_SubRun = 0;
        return true;
    };//因为Run或者SubRun改变了，所以时间会比20s长。
    if( Delta_T >= 0.0 && Delta_T <= 20.0e9){ Change_Run_SubRun == 0; return false;}
    else
    {
        std::cout << "计算的Delta T:" << Delta_T << ",开始寻找下一个Muon" << std::endl;
        Change_Run_SubRun = 0;
        for(int ii1 = Index_Muon_Array + 1; ii1 < Muon_Len; ii1++)
        {   
            if((Muon_Run[ii1] != Muon_Run[Index_Muon_Array]) || (Muon_SubRun[ii1] != Muon_SubRun[Index_Muon_Array]))
            {
                std::cout << "Run or SubRun 发生改变，通过Muon Cut, 此时Muon的信息为：" << Muon_Run[ii1] << ",SubRun:" << Muon_SubRun[ii1] << ", 50MHz:" << Muon_50MHz[ii1] << std::endl;
                Double_t tempDelta_T = 20.0 * (Prompt_50MHz - Muon_50MHz[ii1]) / 1e9;
                std::cout << "此时的Delta T(s)：" << tempDelta_T << std::endl;
                Index_Muon_Array = ii1; 
                In_Next_Muon = false;
                PassMuonCut = true;
                break;
            };
            Double_t Delta_T_Next = 20.0 * (Prompt_50MHz - Muon_50MHz[ii1]);
            if(Delta_T_Next >=0 && Delta_T_Next <= 20e9)
            { 
                std::cout << "找到对应的Muon:" << Muon_Run[ii1] << ",SubRun:" << Muon_SubRun[ii1] << ",Muon 50MHz:" << Muon_50MHz[ii1] << std::endl; 
                std::cout << "此时的Delta T(s):" << 20.0 * (Prompt_50MHz - Muon_50MHz[ii1]) / 1e9 << std::endl << std::endl;
                Index_Muon_Array = ii1; 
                In_Next_Muon = true;
                PassMuonCut = false;
                break;
            };
            if(Delta_T_Next < 0)
            { 
                std::cout << "没有找到对应的Muon，通过Muon Cut" << std::endl;
                Index_Muon_Array = ii1; 
                In_Next_Muon = false; 
                PassMuonCut = true; 
                break;
            };
            if(Delta_T_Next > 20e9){continue;};
        };
    }; 
    if(PassMuonCut == true && In_Next_Muon == false) { return true;}
    else
    {
        return false;
    };
}; 