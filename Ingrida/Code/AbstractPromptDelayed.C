//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TVector3.h>

Double_t GetTriggeredEventTime(const Int_t uTDays, const Int_t uTSecs, const Int_t uTNSecs)
{
    // Int_t EventTime = uTDays*24*3600*1e9 + uTSecs*1e9 + uTNSecs;
    return uTDays*24*3600*1e9 + uTSecs*1e9 + uTNSecs;
};

const int Max = 10000;
class Struct
{
public:
    Struct()
    {
        ClearAll();
    };
    void ClearAll()
    {
        ClearRun();
        ClearMC();
        ClearEV();
        ClearFitVertex();
        ClearHitPMT();
        CleartResidual();
    };
    void ClearRun()
    {
        C_RunID = 0;
        C_SubRunID = 0;
        
    };
    void ClearMC()
    {
        C_MCFlag = 0;
        C_MCID = 0;
    };
    void ClearEV()
    {
        C_GTID = 0;
        C_Nhits = 0;
        C_NhitsCleaned = 0;
        C_Days = 0;
        C_Seconds = 0;
        C_NanoSeconds = 0;
    };
    void ClearFitVertex()
    {
        C_FitVertexTime = 0;
        C_FitVertexPosX = 0;
        C_FitVertexPosY = 0;
        C_FitVertexPosZ = 0;
        C_FitVertexEnergy = 0;
    };
    void ClearHitPMT()
    {
        C_LenHit = 0;
    };
    void CleartResidual()
    {
        for(int ii1 = 0; ii1 < Max; ii1++)
        {
            C_tResidual[ii1] = 0;
        };
    };
    
    //Run等信息
        UInt_t C_RunID, C_SubRunID;
    //MC等信息
        UInt_t C_MCFlag;
        Int_t C_MCID;
    //EV的信息
        Int_t C_GTID;
        UInt_t C_Nhits, C_NhitsCleaned;
        Int_t C_Days, C_Seconds;
        Double_t C_NanoSeconds;
    //FitVertex信息
        Double_t C_FitVertexTime, C_FitVertexPosX, C_FitVertexPosY, C_FitVertexPosZ;
        Double_t C_FitVertexEnergy;
    //HitPMT信息
        int C_LenHit;
    //tResidual信息
        Double_t C_tResidual[Max];
};

class Result
{
public:
    void ClearAll()
    {   
        Prompt.ClearAll();
        Delayed.ClearAll();
    };

    Struct Prompt, Delayed;
    
};





const double ZOffSet = 186;//mm
int AbstractPromptDelayed()
{   
//测试专用
//集成
    std::string InFiles = "/rat/MyCode/Work/Geo-nu/Ingrida/Code/test.root";
    std::string OutFile = "/rat/MyCode/Work/Geo-nu/Ingrida/Code/test_2.root";

    TFile *infile = new TFile(InFiles.c_str());
    TTree *chain = (TTree*) infile->Get("output");
    //chain->Print();

    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *tree = new TTree("output", "output");

//Meta
    int RunID, SubRunID, MCFlag;
    chain->SetBranchAddress("RunID", &RunID);
    chain->SetBranchAddress("SubRunID", &SubRunID);
    chain->SetBranchAddress("MCFlag", &MCFlag);
//MC
    int MCID;
    chain->SetBranchAddress("MCID", &MCID);
//EV
    int GTID, Nhits, NhitsCleaned;
    chain->SetBranchAddress("GTID", &GTID);
    chain->SetBranchAddress("Nhits", &Nhits);
    chain->SetBranchAddress("NhitsCleaned", &NhitsCleaned);
    Int_t Days, Seconds;
    Double_t NanoSeconds;
    chain->SetBranchAddress("Days", &Days);
    chain->SetBranchAddress("Seconds", &Seconds);
    chain->SetBranchAddress("NanoSeconds", &NanoSeconds);
//FitVertex
    Double_t FitVertexTime, FitVertexPosX, FitVertexPosY, FitVertexPosZ, FitVertexEnergy;
    chain->SetBranchAddress("FitVertexTime", &FitVertexTime);
    chain->SetBranchAddress("FitVertexPosX", &FitVertexPosX);
    chain->SetBranchAddress("FitVertexPosY", &FitVertexPosY);
    chain->SetBranchAddress("FitVertexPosZ", &FitVertexPosZ);
    chain->SetBranchAddress("FitVertexEnergy", &FitVertexEnergy);
//PMT
    int LenHit;
    chain->SetBranchAddress("LenHit", &LenHit);
//Residual Time
    Double_t tResidual[Max];
    chain->SetBranchAddress("tResidual", tResidual);
    ULong64_t Clock50;
    chain->SetBranchAddress("50MHz", &Clock50);

    Result res;

    tree->Branch("PromptRunID", &res.Prompt.C_RunID, "PromptRunID/I");
    tree->Branch("PromptSubRunID", &res.Prompt.C_SubRunID, "PromptSubRunID/I");
    tree->Branch("PromptMCFlag", &res.Prompt.C_MCFlag, "PromptMCFlag/I");

    tree->Branch("PromptMCID", &res.Prompt.C_MCID, "PromptMCID/I");

    tree->Branch("PromptGTID", &res.Prompt.C_GTID, "PromptGTID/I");
    tree->Branch("PromptNhits", &res.Prompt.C_Nhits, "PromptNhits/I");
    tree->Branch("PromptNhitsCleaned", &res.Prompt.C_NhitsCleaned, "PromptNhitsCleaned/I");

    tree->Branch("PromptFitVertexTime", &res.Prompt.C_FitVertexTime, "PromptFitVertexTime/D");
    tree->Branch("PromptFitVertexPosX", &res.Prompt.C_FitVertexPosX, "PromptFitVertexPosX/D");
    tree->Branch("PromptFitVertexPosY", &res.Prompt.C_FitVertexPosY, "PromptFitVertexPosY/D");
    tree->Branch("PromptFitVertexPosZ", &res.Prompt.C_FitVertexPosZ, "PromptFitVertexPosZ/D");
    tree->Branch("PromptFitVertexEnergy", &res.Prompt.C_FitVertexEnergy, "PromptFitVertexEnergy/D");
    tree->Branch("PromptDays", &res.Prompt.C_Days, "PromptDays/I");
    tree->Branch("PromptSeconds", &res.Prompt.C_Seconds, "PromptSeconds/I");
    tree->Branch("PromptNanoSeconds", &res.Prompt.C_NanoSeconds, "PromptNanoSeconds/D");

    tree->Branch("PromptLenHit", &res.Prompt.C_LenHit, "PromptLenHit/I");
    tree->Branch("PrompttResidual", res.Prompt.C_tResidual, "PrompttResidual[10000]/D");
    
    tree->Branch("DelayedRunID", &res.Delayed.C_RunID, "DelayedRunID/I");
    tree->Branch("DelayedSubRunID", &res.Delayed.C_SubRunID, "DelayedSubRunID/I");
    tree->Branch("DelayedMCFlag", &res.Delayed.C_MCFlag, "DelayedMCFlag/I");

    tree->Branch("DelayedMCID", &res.Delayed.C_MCID, "DelayedMCID/I");

    tree->Branch("DelayedGTID", &res.Delayed.C_GTID, "DelayedGTID/I");
    tree->Branch("DelayedNhits", &res.Delayed.C_Nhits, "DelayedNhits/I");
    tree->Branch("DelayedNhitsCleaned", &res.Delayed.C_NhitsCleaned, "DelayedNhitsCleaned/I");

    tree->Branch("DelayedFitVertexTime", &res.Delayed.C_FitVertexTime, "DelayedFitVertexTime/D");
    tree->Branch("DelayedFitVertexPosX", &res.Delayed.C_FitVertexPosX, "DelayedFitVertexPosX/D");
    tree->Branch("DelayedFitVertexPosY", &res.Delayed.C_FitVertexPosY, "DelayedFitVertexPosY/D");
    tree->Branch("DelayedFitVertexPosZ", &res.Delayed.C_FitVertexPosZ, "DelayedFitVertexPosZ/D");
    tree->Branch("DelayedFitVertexEnergy", &res.Delayed.C_FitVertexEnergy, "DelayedFitVertexEnergy/D");
    tree->Branch("DelayedDays", &res.Delayed.C_Days, "DelayedDays/I");
    tree->Branch("DelayedSeconds", &res.Delayed.C_Seconds, "DelayedSeconds/I");
    tree->Branch("DelayedNanoSeconds", &res.Delayed.C_NanoSeconds, "DelayedNanoSeconds/D");

    tree->Branch("DelayedLenHit", &res.Delayed.C_LenHit, "DelayedLenHit/I");
    tree->Branch("DelayedtResidual", res.Delayed.C_tResidual, "DelayedtResidual[10000]/D");

    TVector3 PromptPos, DelayedPos, RelativePos;
    Double_t PromptTime, DelayedTime, RelativeTime;
    bool FindPrompt, FindDelayed;
    int tempEVIndex = -99;
    const int Len = chain->GetEntries();
    ULong64_t Clock_Prompt, Clock_Delayed;
    for(int ii1 = 0; ii1 < Len; ii1++)
    {
        chain->GetEntry(ii1);
        res.ClearAll();
        if(ii1 % 1000 == 0){std::cout <<"已处理" << ii1 << ",还剩" << Len - ii1 << std::endl;};
        FindPrompt = false;
        FindDelayed = false;
        //std::cout << ii1 << ", " << res.Prompt.C_tResidual << ", " << tResidual << std::endl;
    //寻找Promot
        if(FitVertexEnergy < 0.9 || FitVertexEnergy > 3.5){continue;};
        PromptPos = TVector3(FitVertexPosX, FitVertexPosY, FitVertexPosZ - ZOffSet);
        if(PromptPos.Mag() > 5700){continue;};
        PromptTime = GetTriggeredEventTime(Days, Seconds, NanoSeconds);
        Clock_Prompt = Clock50;
    //排除重复找到prompt的情况
        if(MCID != tempEVIndex){FindPrompt = true; tempEVIndex = MCID;};
        //std::cout << ii1 << ", 开始寻找中子, 还剩" << Len - ii1 << std::endl;
    //寻找Delayed
        for(int ii2 = ii1 + 1; ii2 < ii1 + 4; ii2++)//4来自于实际观察数据，基本上一个Event对应4个数据
        {
            chain->GetEntry(ii2);
            //至少不是Prompt
            if(MCID != tempEVIndex) {continue;};
            if(FitVertexEnergy < 1.8 || FitVertexEnergy > 2.4){continue;};
            DelayedPos = TVector3(FitVertexPosX, FitVertexPosY, FitVertexPosZ - ZOffSet);
            if(DelayedPos.Mag() > 5700){continue;};
            RelativePos = (DelayedPos - PromptPos);
            if(RelativePos.Mag() > 1500){continue;};
            DelayedTime = GetTriggeredEventTime(Days, Seconds, NanoSeconds);
            RelativeTime = DelayedTime - PromptTime;
            Clock_Delayed = Clock50;
            if(RelativeTime < 400 || RelativeTime > 8*pow(10,5)){continue;};
            //std::cout << ii2 << ", " << MCID << ", " << FitVertexEnergy << std::endl;
            FindDelayed = true;
            if(FindDelayed)
            {
            //Meta
                res.Delayed.C_RunID = RunID;
                res.Delayed.C_SubRunID = SubRunID;
                res.Delayed.C_MCFlag = MCFlag;
            //MC
                res.Delayed.C_MCID = MCID;
            //EV
                res.Delayed.C_GTID = GTID;
                res.Delayed.C_Nhits = Nhits;
                res.Delayed.C_NhitsCleaned = NhitsCleaned;
            //FitVertex
                res.Delayed.C_FitVertexTime = FitVertexTime;
                res.Delayed.C_FitVertexPosX = FitVertexPosX;
                res.Delayed.C_FitVertexPosY = FitVertexPosY;
                res.Delayed.C_FitVertexPosZ = FitVertexPosZ;
                res.Delayed.C_FitVertexEnergy = FitVertexEnergy;
                res.Delayed.C_Days = Days;
                res.Delayed.C_Seconds = Seconds;
                res.Delayed.C_NanoSeconds = NanoSeconds;
            //HitPMT
                res.Delayed.C_LenHit = LenHit;
            //Residual Time
                for(int ii3 = 0; ii3 < LenHit; ii3++)
                {
                    res.Delayed.C_tResidual[ii3] = tResidual[ii3];
                };
                tempEVIndex = MCID;
            };
            //std::cout << ii2 << ", 找到中子" << std::endl;
        };
        if(FindPrompt == false || FindDelayed == false){continue;};
        chain->GetEntry(ii1);
        //Meta
            res.Prompt.C_RunID = RunID;
            res.Prompt.C_SubRunID = SubRunID;
            res.Prompt.C_MCFlag = MCFlag;
        //MC
            res.Prompt.C_MCID = MCID;
        //EV
            res.Prompt.C_GTID = GTID;
            res.Prompt.C_Nhits = Nhits;
            res.Prompt.C_NhitsCleaned = NhitsCleaned;
        //FitVertex
            res.Prompt.C_FitVertexTime = FitVertexTime;
            res.Prompt.C_FitVertexPosX = FitVertexPosX;
            res.Prompt.C_FitVertexPosY = FitVertexPosY;
            res.Prompt.C_FitVertexPosZ = FitVertexPosZ;
            res.Prompt.C_FitVertexEnergy = FitVertexEnergy;
            res.Prompt.C_Days = Days;
            res.Prompt.C_Seconds = Seconds;
            res.Prompt.C_NanoSeconds = NanoSeconds;
        //HitPMT
            res.Prompt.C_LenHit = LenHit;

            std::cout << "Prompt GTID:" << res.Prompt.C_GTID << ", Prompt Energy:" << res.Prompt.C_FitVertexEnergy
            << ", Delayed GTID:" << res.Delayed.C_GTID << ", Delayed Energy:" << res.Delayed.C_FitVertexEnergy << std::endl;
        //Residual Time
            for(int ii2 = 0; ii2 < LenHit; ii2++)
            {
                res.Prompt.C_tResidual[ii2] = tResidual[ii2];
                //if(ii1 == 50 || ii1 == 51){std::cout << ii2 << ", " << res.Prompt.C_tResidual[ii2] << std::endl;};
            };
        // std::cout << ii1 << ", " << res.Prompt.C_MCID << ", " << res.Prompt.C_FitVertexEnergy << std::endl;
        // std::cout << ii1 << ", " << res.Delayed.C_MCID << ", " << res.Delayed.C_FitVertexEnergy << std::endl;
        tree->Fill();
    };
    
    outfile->Write();
    delete tree; tree = NULL;
    outfile->Close();
    delete outfile; outfile = NULL;
    delete chain; chain = NULL;
    

    // return 0;
};