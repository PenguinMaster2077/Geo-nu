/*
上传到Cedar，进行初步处理数据
*/
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
#include <RAT/DU/DSReader.hh>
// #include <RAT/DU/Utility.hh>
// #include <RAT/DU/PMTInfo.hh>
// #include <RAT/DU/PMTCalStatus.hh>
// #include <RAT/DU/EffectiveVelocity.hh>
// #include <RAT/DS/Entry.hh>
// #include <RAT/DS/EV.hh>
// #include <RAT/DS/PMT.hh>
// #include <RAT/DS/Run.hh>
// #include <RAT/DS/PMTSet.hh>
// #include <RAT/DS/FitVertex.hh>
// #include <RAT/DS/MC.hh>
// #include <RAT/DS/UniversalTime.hh>

const int Max = 10000;
struct Result
{
    void ClearMeta()
    {
        C_RunID = 0;
        C_SubRunID = 0;
        C_MCFlag = 0;
    };
    void ClearMC()
    {
        C_MCID = 0;
    }
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
        for(int ii1 = 0; ii1 < Max; ii1++)
        {
            C_HitTime[ii1] = 0;
            C_HitPosX[ii1] = 0;
            C_HitPosY[ii1] = 0;
            C_HitPosZ[ii1] = 0;
        };
    };
    void CleartResidual()
    {
        for(int ii1 = 0; ii1 < Max; ii1++)
        {
            C_tResidual[ii1] = 0;
        }
    }

//Run等信息
    UInt_t C_RunID, C_SubRunID, C_MCFlag;
//MC等信息
    Int_t C_MCID;
//EV的信息
    Int_t C_GTID;
    UInt_t C_Nhits, C_NhitsCleaned;
    UInt_t C_Days, C_Seconds;
    Double_t C_NanoSeconds;
//FitVertex信息
    Double_t C_FitVertexTime, C_FitVertexPosX, C_FitVertexPosY, C_FitVertexPosZ;
    Double_t C_FitVertexEnergy;
//HitPMT信息
    int C_LenHit;
    Double_t C_HitTime[Max], C_HitPosX[Max], C_HitPosY[Max], C_HitPosZ[Max];
//tResidual信息
    Double_t C_tResidual[Max];

    ULong64_t C_50MHz;
};

std::string scintFitter = "scintFitter";
int Shearing()
{
    std::string InFile = "/home/shuaioy/scratch/Geo/Reactor/Fisher/20240219/ROOT/Reactor_Fisher.306499.root";
    std::string OutFile = "/home/shuaioy/projects/def-ckrauss/shuaioy/work/Geo/Ingrida/Code/test.root";

    RAT::DU::DSReader dsreader(InFile);
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *tree = new TTree("output", "output");

    Result res;

//Meta信息
    tree->Branch("RunID", &res.C_RunID, "RunID/I");
    tree->Branch("SubRunID", &res.C_SubRunID, "SubRunID/I");
    tree->Branch("MCFlag", &res.C_MCFlag, "MCFlag/I");
//MC信息
    tree->Branch("MCID", &res.C_MCID, "MCID/I");
    tree->Branch("Days", &res.C_Days, "Days/I");
    tree->Branch("Seconds", &res.C_Seconds, "Seconds/I");
    tree->Branch("NanoSeconds", &res.C_NanoSeconds, "NanoSeconds/D");
//EV信息
    tree->Branch("GTID", &res.C_GTID, "GTID/I");
    tree->Branch("Nhits", &res.C_Nhits, "Nhits/I");
    tree->Branch("NhitsCleaned", &res.C_NhitsCleaned, "NhitsCleaned/I");
//FitVertex信息
    tree->Branch("FitVertexTime", &res.C_FitVertexTime, "FitVertexTime/D");
    tree->Branch("FitVertexPosX", &res.C_FitVertexPosX, "FitVertexPosX/D");
    tree->Branch("FitVertexPosY", &res.C_FitVertexPosY, "FitVertexPosY/D");
    tree->Branch("FitVertexPosZ", &res.C_FitVertexPosZ, "FitVertexPosZ/D");
    tree->Branch("FitVertexEnergy", &res.C_FitVertexEnergy, "FitVertexEnergy/D");
//HitPMT信息
    tree->Branch("LenHit", &res.C_LenHit, "LenHit/I");
    tree->Branch("HitTime", res.C_HitTime, "HitTime[LenHit]/D");
    tree->Branch("HitPosX", res.C_HitPosX, "HitPosX[LenHit]/D");
    tree->Branch("HitPosY", res.C_HitPosY, "HitPosY[LenHit]/D");
    tree->Branch("HitPosZ", res.C_HitPosZ, "HitPosZ[LenHit]/D");
//tResidual信息
    tree->Branch("tResidual", res.C_tResidual, "tResidual[LenHit]/D");
    
    tree->Branch("50MHz", &res.C_50MHz);

    
    RAT::DS::Entry entry;
    RAT::DS::MC mc;
    RAT::DS::UniversalTime universaltime;
    RAT::DS::EV ev;
    RAT::DS::FitVertex fitvertex;
    RAT::DS::CalPMTs calpmts;
    RAT::DS::PMTCal pmtcal;
    TVector3 tempTVector3, FitVertexPos, HitPos[Max];
    Double_t FitTime, HitTime[Max], tResidual[Max], DistInInnerAV, DistInAV, DistInWater;
    const RAT::DU::PMTInfo &pmtinfo = RAT::DU::Utility::Get()->GetPMTInfo();
    const RAT::DU::PMTCalStatus &pmtcalstatus = RAT::DU::Utility::Get()->GetPMTCalStatus();
    const RAT::DU::EffectiveVelocity &effectiveVelocity = RAT::DU::Utility::Get()->GetEffectiveVelocity();
    RAT::DU::LightPathCalculator lightpathcalculator = RAT::DU::Utility::Get()->GetLightPathCalculator();
    RAT::DU::Point3D tempPoint3D1, tempPoint3D2;
    int tempint;
    UInt_t tempUInt_t;
    Double_t tempDouble_t;
    const int Len = dsreader.GetEntryCount();
    for(int ii1 = 0; ii1 < Len; ii1++)
    {
        std::cout << ii1 << ", Entry开始，还剩" << Len - ii1 << std::endl;
        entry = dsreader.GetEntry(ii1);
    //Meta信息
        res.ClearMeta();
        res.C_RunID = entry.GetRunID();
        res.C_SubRunID = entry.GetSubRunID();
        res.C_MCFlag = entry.MCExists();
    //MC信息
        mc = entry.GetMC();
        res.ClearMC();
        res.C_MCID = mc.GetMCID();
    //loop the Events
        for(size_t ii2 = 0; ii2 < entry.GetEVCount(); ii2++)
        {
            //std::cout << ii1 << "，Entry中的第" << ii2 <<"，EV开始，还剩" << entry.GetEVCount() - ii2 << std::endl;
        //EV
            ev = entry.GetEV(ii2);
            universaltime = ev.GetUniversalTime();

            res.C_50MHz = ev.GetClockCount50();
            std::cout << "GTID:"<< ev.GetGTID() << ",Entry:" << ii1 << ",EV:" << ii2<< std::endl;
            std::cout << res.C_50MHz << std::endl;
            // std::cout << (universaltime.GetDays()) * 24 * 3600 *1e9 + (universaltime.GetSeconds()) * 1e9 + universaltime.GetNanoSeconds() << std::endl;
            //EV信息
            res.ClearEV();
            res.C_GTID = ev.GetGTID();
            res.C_Nhits = ev.GetNhits();
            res.C_NhitsCleaned = ev.GetNhitsCleaned();
            res.C_Days = universaltime.GetDays();
            res.C_Seconds = universaltime.GetSeconds();
            res.C_NanoSeconds = universaltime.GetNanoSeconds();
        //FitResult
            if( ev.FitResultExists(scintFitter) == 0){continue;};
            const RAT::DS::FitResult fitresult = ev.GetFitResult(scintFitter);
            if(fitresult.GetValid() == 0){continue;};
        //FitVertex
            fitvertex = fitresult.GetVertex(0);
            if(fitvertex.ContainsEnergy() == 0){continue;};
            if(fitvertex.ValidEnergy() == 0){continue;};
            if(fitvertex.ContainsPosition() == 0){continue;};
            if(fitvertex.ValidPosition() == 0){continue;};
            if(fitvertex.ContainsTime() == 0 ){continue;};
            if(fitvertex.ValidTime() == 0){continue;};
            FitVertexPos = fitvertex.GetPosition();
            FitTime = fitvertex.GetTime();
            //FitVertex信息
            res.ClearFitVertex();
            res.C_FitVertexEnergy = fitvertex.GetEnergy();
            res.C_FitVertexTime = fitvertex.GetTime();
            tempTVector3 = fitvertex.GetPosition();
            res.C_FitVertexPosX = tempTVector3.X();
            res.C_FitVertexPosY = tempTVector3.Y();
            res.C_FitVertexPosZ = tempTVector3.Z();
        //HitPMTs
            calpmts = ev.GetCalPMTs();
            const int LenCalPMTs = calpmts.GetCount();
            res.ClearHitPMT();
            tempint = 0;
            for(int ii3 = 0 ; ii3 < LenCalPMTs; ii3++)
            {
                pmtcal = calpmts.GetPMT(ii3);
                tempUInt_t = pmtcalstatus.GetHitStatus(pmtcal);
                if( tempUInt_t != 0) {continue;}; //0表示是一个clean hit，需要保留下来。
                HitTime[tempint] = pmtcal.GetTime();
                tempUInt_t = pmtcal.GetID();
                HitPos[tempint] = pmtinfo.GetPosition(tempUInt_t);
                //记录信息
                tempTVector3 = pmtinfo.GetPosition(tempUInt_t);
                res.C_HitPosX[tempint] = tempTVector3.X();
                res.C_HitPosY[tempint] = tempTVector3.Y();
                res.C_HitPosZ[tempint] = tempTVector3.Z();
                res.C_HitTime[tempint] = pmtcal.GetTime();
                tempint++;
            };
                res.C_LenHit = tempint;
        tree->Fill();
        };
    };
    outfile->Write();
    delete tree; tree = NULL;
    outfile->Close();
    delete outfile; outfile = NULL;

    return 0;
}