//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
#include <TString.h>
//RAT
#include <RAT/DU/DSReader.hh>
#include <RAT/DB.hh>
#include <RAT/DU/Utility.hh>
#include <RAT/DU/Point3D.hh>
#include <RAT/DU/ReconCalibrator.hh>
#include <RAT/DU/DetectorStateCorrection.hh>
//Self-Defined

const Double_t MIN_MULTI_ENERGY = 0.4;//MeV;
const Double_t MIN_MULTI_R = 0.0;
const Double_t MAX_MULTI_R = 2000.0;//mm
const Double_t MIN_MULTI_DELTAT = 0;
const Double_t MAX_MULTI_DELTAT = 2.0e-3;//s

const Long64_t MAX_NUMBER = 8796093022207;//2^43 - 1; DocDB-481
const Long64_t MIN_CARE_NUMBER = 8596093022207;

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
        std::cout << "NhitsCorre:" << C_Nhits_Corrected << std::endl;
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
    UInt_t C_Nhits, C_Nhits_Cleaned, C_Nhits_Corrected;
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
        ClearMulti();
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
    void ClearMulti()  {C_MultiEvent.ClearAll();};
    void ShowMultiEvent()   {std::cout << "Details of Multi Event" << std::endl; C_MultiEvent.ShowDetals();};
//Run Information
    UInt_t C_RunID, C_SubRunID, C_Date, C_Time, C_RunType;
    Double_t C_ZOffSet;
//Prompt and Delayed
    Event C_MultiEvent;
};

Double_t ComputeDelta_T(Long64_t Last_50MHz, Long64_t Present_50MHz)//ns
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

void RecordRunInfo(myresult &res, RAT::DU::DSReader &dsreader);

Double_t GiveTonyEnergy(Double_t Energy, TVector3 Position, RAT::DU::ReconCalibrator &recon_cali, 
RAT::DU::DetectorStateCorrection &detector_state_correct);

Double_t GiveCorrectedEnergy(Double_t Original_Energy, TVector3 Position,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct);

// int TaggingMultiEvent()
int main(int argc, char** argv)
{
    std::string RATDSFile = argv[1];
    std::string NTUPLEFile = argv[2];
    std::string OutFile = argv[3];
    TString sub_run = argv[4];
    TString PromptEntry = argv[5];
    TString DelayedEntry = argv[6];
    
    //Read Ratds File
    RAT::DU::DSReader dsreader(RATDSFile);
    RAT::DU::ReconCalibrator recon_cali = RAT::DU::Utility::Get()->GetReconCalibrator();
    RAT::DU::Utility::Get()->BeginOfRun();
    RAT::DU::DetectorStateCorrection detector_state_correct = RAT::DU::Utility::Get()->GetDetectorStateCorrection();
    //Read Ntuple File
    TFile *ntuple = new TFile(NTUPLEFile.c_str());
    TTree *ntupletree = (TTree*) ntuple->Get("output");
    //Set Branch Address
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
    Bool_t ntuple_fitValid;
    ntupletree->SetBranchAddress("fitValid", &ntuple_fitValid);
    Double_t ntuple_Original_Energy, ntuple_PosX, ntuple_PosY, ntuple_PosZ;
    ntupletree->SetBranchAddress("energy", &ntuple_Original_Energy);
    ntupletree->SetBranchAddress("posx", &ntuple_PosX);
    ntupletree->SetBranchAddress("posy", &ntuple_PosY);
    ntupletree->SetBranchAddress("posz", &ntuple_PosZ);
    //Create Output File
    myresult res;
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "output");
    //Set Branch Address
    outtree->Branch("RunID", &res.C_RunID, "RunID/I");
    outtree->Branch("SubRunID", &res.C_SubRunID, "RunID/I");
    outtree->Branch("Date", &res.C_Date, "RunID/I");
    outtree->Branch("Time", &res.C_Time, "RunID/I");
    outtree->Branch("RunType", &res.C_RunType, "RunID/I");
    outtree->Branch("ZOffSet", &res.C_ZOffSet, "ZOffSet/D");
    //Multi Event
    outtree->Branch("MultiEntry", &res.C_MultiEvent.C_Entry, "MultiEntry/I");
    outtree->Branch("MultiEv", &res.C_MultiEvent.C_EV, "MultiEv/I");
    outtree->Branch("MultiGTID", &res.C_MultiEvent.C_GTID, "MultiGTID/I");
    outtree->Branch("MultiOWLHits", &res.C_MultiEvent.C_OWLHits, "MultiOWLHits/I");
    outtree->Branch("MultiNHits", &res.C_MultiEvent.C_Nhits, "MultiNHits/I");
    outtree->Branch("MultiNHitsCleaned", &res.C_MultiEvent.C_Nhits_Cleaned, "MultiNHitsCleaned/I");
    outtree->Branch("Multi50MHz", &res.C_MultiEvent.C_50MHZ, "Multi50MHz/L");
    outtree->Branch("MultiDataApplied", &res.C_MultiEvent.C_Data_Applied, "MultiDataApplied/L");
    outtree->Branch("MultiDataFlags", &res.C_MultiEvent.C_Data_Flags, "MultiDataFlags/L");
    std::cout << "Recording Run Info" << std::endl;
//Record RunInfo
    RecordRunInfo(res, dsreader);
    res.C_SubRunID = sub_run.Atoi();
    Double_t ZOffSet = res.C_ZOffSet;
    // TVector3 ZShift(0, 0, 183);
//Load Coincidence Pair Info
    std::cout << "Load Pair Info" << std::endl;
    TVector3 Prompt_Pos, Delayed_Pos;
    Long64_t Prompt_50MHz, Delayed_50MHz;
    ntupletree->GetEntry(PromptEntry.Atoi());
    Prompt_Pos.SetXYZ(ntuple_PosX, ntuple_PosY, ntuple_PosZ - ZOffSet);
    Prompt_50MHz = ntuple_50MHz;
    ntupletree->GetEntry(DelayedEntry.Atoi());
    Delayed_Pos.SetXYZ(ntuple_PosX, ntuple_PosY, ntuple_PosZ - ZOffSet);
    Delayed_50MHz = ntuple_50MHz;
//Begin to Search Multi Event
    int Find_Multi_Event = 0;
    int StartIndex, EndIndex;
    if(PromptEntry.Atoi() - 10000 <= 0)
    {
        StartIndex = 0;
    }
    else
    {
        StartIndex = PromptEntry.Atoi() - 10000;
    };
    if(DelayedEntry.Atoi() + 10000 >= ntupletree->GetEntries())
    {
        EndIndex = ntupletree->GetEntries();
    }
    else
    {
        EndIndex = DelayedEntry.Atoi() + 10000;
    };
    std::cout << "Start:" << StartIndex << ",End:" << EndIndex << ",Entries:" << ntupletree->GetEntries() << std::endl;
    Double_t Corrected_Energy, Tony_Energy;
    TVector3 Delta_R_Event_Prompt, Delta_R_Event_Delayed, Event_Pos;
    Double_t Delta_T_Event_Prompt, Delta_T_Event_Delayed;
    std::cout << "Begin to Research" << std::endl;
    for(int ii1 = StartIndex; ii1 <= EndIndex; ii1++)
    {
        if(ii1 % 500 == 0) {std::cout << "已处理" << ii1 << ",还剩" << EndIndex - ii1 << std::endl;};
        if(ii1 == PromptEntry.Atoi() || ii1 == DelayedEntry.Atoi()){continue;};
        ntupletree->GetEntry(ii1);
        if(ntuple_fitValid == 0){continue;};
        //Compute Delta T
        if(ii1 < PromptEntry.Atoi())
        {
            Delta_T_Event_Prompt = ComputeDelta_T(ntuple_50MHz, Prompt_50MHz);//ns
            Delta_T_Event_Delayed = ComputeDelta_T(ntuple_50MHz, Delayed_50MHz);//ns
        };
        if(ii1 >= PromptEntry.Atoi() && ii1 <= DelayedEntry.Atoi())
        {
            Delta_T_Event_Prompt = ComputeDelta_T(Prompt_50MHz, ntuple_50MHz);//ns
            Delta_T_Event_Delayed = ComputeDelta_T(ntuple_50MHz, Delayed_50MHz);//ns
        };
        if(ii1 > DelayedEntry.Atoi())
        {
            Delta_T_Event_Prompt = ComputeDelta_T(Prompt_50MHz, ntuple_50MHz);//ns
            Delta_T_Event_Delayed = ComputeDelta_T(Delayed_50MHz, ntuple_50MHz);//ns
        };
        if(Delta_T_Event_Prompt > 2e6 && Delta_T_Event_Delayed > 2e6)
        {
            continue;
        };
        std::cout << "Find Possible Event!!!!!!!!! Check it!!!!!!!!!" << std::endl;
        std::cout << "Prompt Entry:" << PromptEntry.Atoi() << ", Delayed Entry:" << DelayedEntry.Atoi() << std::endl;
        std::cout << "Event Entry:" << ntuple_Entry << ", Event GTID:" << ntuple_GTID << std::endl;
        std::cout << "Event 50MHz:" << ntuple_50MHz << ", Prompt 50MHz:" << Prompt_50MHz << ", Delayed 50MHz:" << Delayed_50MHz << std::endl;
        std::cout << ii1 << ",Pass Time Cut(ms):" << Delta_T_Event_Prompt/1e6 << "," << Delta_T_Event_Delayed/1e6 << std::endl;
        std::cout << "Time Check: Pass!!!!!!!!!!!!" << std::endl;
        //Energy Check
        Event_Pos.SetXYZ(ntuple_PosX, ntuple_PosY, ntuple_PosZ - ZOffSet);
        Corrected_Energy = GiveCorrectedEnergy(ntuple_Original_Energy, Event_Pos, recon_cali, detector_state_correct);
        Tony_Energy = GiveTonyEnergy(ntuple_Original_Energy, Event_Pos, recon_cali, detector_state_correct);
        std::cout << ii1 << ", Event Energy:" << Corrected_Energy << std::endl;
        if(Corrected_Energy < 0.4)
        {
            std::cout << "Energy Check: Fail!!!!!!!!!!" << std::endl;
            continue;
        };
        std::cout << "Energy Check: Pass!!!!!!!!!!" << std::endl;
        //Compute Position
        Delta_R_Event_Prompt = Event_Pos - Prompt_Pos;
        Delta_R_Event_Delayed = Event_Pos - Delayed_Pos;
        std::cout << "Event: X=" << Event_Pos.X() << ",Y=" << Event_Pos.Y() << ",Z=" << Event_Pos.Z() << std::endl;
        std::cout << "Prompt: X=" << Prompt_Pos.X() << ",Y=" << Prompt_Pos.Y() << ",Z=" << Prompt_Pos.Z() << std::endl;
        std::cout << "Delayed: X=" << Delayed_Pos.X() << ",Y=" << Delayed_Pos.Y() << ",Z=" << Delayed_Pos.Z() << std::endl; 
        std::cout << ii1 << ",Prompt Dis(mm):" << Delta_R_Event_Prompt.Mag() << ",Delayed Dis(mm):" << Delta_R_Event_Delayed.Mag() << std::endl;
        if(Delta_R_Event_Prompt.Mag() > MAX_MULTI_R && Delta_R_Event_Delayed.Mag() > MAX_MULTI_R) 
        {
            std::cout << "Distance Check: Fail!!!!!!!!!!!!" << std::endl;
            continue;
        };
        std::cout << "Distance Check: Pass!!!!!!!!!!!!" << std::endl;
        //Record Info
        std::cout << "Find Multi Event!!!!!!!!! Recording!!!!!!!!!" << std::endl;
        res.C_MultiEvent.C_Entry = ntuple_Entry;
        res.C_MultiEvent.C_EV = ntuple_EV;
        res.C_MultiEvent.C_GTID = ntuple_GTID;
        res.C_MultiEvent.C_OWLHits = ntuple_OWLHits;
        res.C_MultiEvent.C_Nhits = ntuple_NHits;
        res.C_MultiEvent.C_Nhits_Cleaned = ntuple_NHitsCleaned;
        res.C_MultiEvent.C_Nhits_Corrected = 0;
        res.C_MultiEvent.C_50MHZ = ntuple_50MHz;
        res.C_MultiEvent.C_Data_Applied = ntuple_Data_Applied;
        res.C_MultiEvent.C_Data_Flags = ntuple_Data_Flags;
        res.C_MultiEvent.C_Energy = Corrected_Energy;
        res.C_MultiEvent.C_Original_Energy = ntuple_Original_Energy;
        res.C_MultiEvent.C_Tony_Energy = Tony_Energy;
        res.C_MultiEvent.C_Pos_X = Event_Pos.X();
        res.C_MultiEvent.C_Pos_Y = Event_Pos.Y();
        res.C_MultiEvent.C_Pos_Z = Event_Pos.Z();

        res.ShowMultiEvent();
        Find_Multi_Event = 1;
        outtree->Fill();
    };
    if(Find_Multi_Event == 0){outtree->Fill();};
    outfile->Write();
    outfile->Close();

    std::cout << "Complete" << std::endl;

    return 0;
};

void RecordRunInfo(myresult &res, RAT::DU::DSReader &dsreader)
{
    RAT::DS::Run run = dsreader.GetRun();
    RAT::DB *db = RAT::DB::Get();
    db->BeginOfRun(run);
    std::vector<double> AVPos;
    AVPos = db->GetLink("AV_OFFSET_RUNTIME")->GetDArray("position");
    std::cout << run.GetRunID() << ": Z-OffSet is " << AVPos[2] << std::endl;
    res.ClearRunInfo();
    res.C_RunID = run.GetRunID();
    res.C_SubRunID = run.GetSubRunID();
    res.C_Date = run.GetDate();//Start Date
    res.C_Time = run.GetTime();//Start Time
    res.C_RunType = run.GetRunType();
    res.C_ZOffSet = AVPos[2];
};

Double_t GiveTonyEnergy(Double_t Original_Energy, TVector3 Position, RAT::DU::ReconCalibrator &recon_cali, 
RAT::DU::DetectorStateCorrection &detector_state_correct)
{
    Double_t Tony_Energy = recon_cali.CalibrateEnergyRTF(true, Original_Energy, Position.Perp(), Position.Z());
    return Tony_Energy;
};

Double_t GiveCorrectedEnergy(Double_t Original_Energy, TVector3 Position,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct)
{
//Logan's Energy Correction----DocDB: 7730
//Detector Status Correction will be integrated in reconstruction later, thus it should be firstly applied
    RAT::DU::Point3D tempPoint3D(0, Position);
    Double_t Energy = Original_Energy * detector_state_correct.GetCorrection(9394, 0.75058) / detector_state_correct.GetCorrectionPos(tempPoint3D, 0, 0);
//Tony's Energy Correction-----DocDB: 7895
    Energy = recon_cali.CalibrateEnergyRTF(true, Energy, Position.Perp(), Position.Z());
    return Energy;
};