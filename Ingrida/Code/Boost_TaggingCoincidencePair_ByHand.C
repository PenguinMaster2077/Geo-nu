//This code is desinged to tagging prompt and delayed pair
//CPP
#include <iostream>
#include <fstream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>
//RAT
#include <RAT/DU/DSReader.hh>
// #include <RAT/DS/Run.hh>
// #include <RAT/DS/Entry.hh>
// #include <RAT/DS/EV.hh>
// #include <RAT/DS/DataQCFlags.hh>
// #include <RAT/DS/BitMask.hh>
// #include <RAT/DS/FitResult.hh>
// #include <RAT/DS/FitVertex.hh>
// #include <RAT/DS/UniversalTime.hh>
// #include <RAT/DS/PMTSet.hh>
// #include <RAT/DB.hh>
// #include <RAT/DBLink.hh>
// #include <RAT/DBTable.hh>
// #include <RAT/DU/Utility.hh>
// #include <RAT/DU/ReconCalibrator.hh>
// #include <RAT/DU/Point3D.hh>
// #include <RAT/DU/DetectorStateCorrection.hh>
//Self-defined
//////////////////////////////////////////////////////////////////////////////
const Double_t RADIUS_MAX = 5700.0;//mm
const Double_t DELTA_R_MAX = 1500.0;//mm
const Double_t DELTA_T_MIN = 400.0, DELTA_T_MAX = 8.0E5;//ns
const Double_t ENERGY_PROMPT_MIN = 0.9, ENERGY_PROMPT_MAX = 8.0;//MeV
const Double_t ENERGY_DELAYED_MIN = 1.8, ENERGY_DELAYED_MAX = 2.4; //MeV
const int NHITS_MIN = 60;
const int MUON_NHITS_MIN = 2500;
//////////////////////////////////////////////////////////////////////////////

void ShowVector(const TVector3 v);

void ShowVector(const RAT::DU::Point3D v);

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

std::vector<std::string> Run(std::string InFile);

void RecordRunInfo(myresult &res, RAT::DU::DSReader &dsreader);

Double_t GiveTonyEnergy(Double_t Energy, TVector3 Position, RAT::DU::ReconCalibrator &recon_cali, 
RAT::DU::DetectorStateCorrection &detector_state_correct);

Double_t GiveCorrectedEnergy(Double_t Original_Energy, TVector3 Position,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct);

Int_t GiveNHitsCorrected(Int_t Original_NHits, TVector3 Position,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct);

bool PromptCut(const Double_t Energy, const TVector3 Positon);

bool DelayedCut(const Double_t Energy, const TVector3 Positon);

void RecordCoincidencePair(myresult &res, const Double_t Energy[], const Double_t Original_Energy[],
const Double_t Tony_Energy[], Int_t Entry[], Int_t EV[],
const Int_t GTID[], const Int_t OWL[], const UInt_t NHits[], const UInt_t NHitsCleaned[],
const UInt_t NHitsCorrected[], const ULong64_t Count50MHz[], const ULong64_t DataApplied[],
const ULong64_t DataFlags[], const TVector3 PromptPos, const TVector3 DelayedPos);

// int Zombie_CoincidencePair()
int Boost_TaggingCoincidencePair_ByHand()
{
    std::string RATDSFile = "/home/shuaioy/scratch/Geo/Gold/300000-306498/ratds/302000/Data/Analysis20R_r0000301910_s000_p000.root";
    std::string NTUPLEFile = "/home/shuaioy/scratch/Geo/Gold/MC/Th/302000/Data/ScintFit_2p2Geoibd_ThRun_r301910_s0_p0.ntuple.root";
    std::string OutFile = "/project/6001358/shuaioy/work/Geo/Ingrida/Code/MCData/Th/300000-307612/302000/OUTPUT/MC_Th_301910.root";
    TString sub_run = "0";
//Create New Root File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "output");
//Create Wanted Info Class
    myresult res;
//Create Branch
    outtree->Branch("RunID", &res.C_RunID, "RunID/I");
    outtree->Branch("SubRunID", &res.C_SubRunID, "RunID/I");
    outtree->Branch("Date", &res.C_Date, "RunID/I");
    outtree->Branch("Time", &res.C_Time, "RunID/I");
    outtree->Branch("RunType", &res.C_RunType, "RunID/I");
    outtree->Branch("ZOffSet", &res.C_ZOffSet, "ZOffSet/D");
    //Prompt
    outtree->Branch("PromptEntry", &res.C_Prompt.C_Entry, "PromptEntry/I");
    outtree->Branch("PromptEv", &res.C_Prompt.C_EV, "PromptEv/I");
    outtree->Branch("PromptGTID", &res.C_Prompt.C_GTID, "PromptGTID/I");
    outtree->Branch("PromptOWLHits", &res.C_Prompt.C_OWLHits, "PromptOWLHits/I");
    outtree->Branch("PromptNHits", &res.C_Prompt.C_Nhits, "PromptNHits/I");
    outtree->Branch("PromptNHitsCleaned", &res.C_Prompt.C_Nhits_Cleaned, "PromptNHitsCleaned/I");
    outtree->Branch("PromptNHitsCorrected", &res.C_Prompt.C_Nhits_Corrected, "PromptNHitsCorrected/I");
    outtree->Branch("Prompt50MHz", &res.C_Prompt.C_50MHZ, "Prompt50MHz/L");
    outtree->Branch("PromptDataApplied", &res.C_Prompt.C_Data_Applied, "PromptDataApplied/L");
    outtree->Branch("PromptDataFlags", &res.C_Prompt.C_Data_Flags, "PromptDataFlags/L");
    outtree->Branch("PromptEnergy", &res.C_Prompt.C_Energy, "PromptEnergy/D");
    outtree->Branch("PromptOriginalEnergy", &res.C_Prompt.C_Original_Energy, "PromptOriginalEnergy/D");
    outtree->Branch("PromptTonyEnergy", &res.C_Prompt.C_Tony_Energy, "PromptTonyEnergy/D");
    outtree->Branch("PromptPosX", &res.C_Prompt.C_Pos_X, "PromptPosX/D");
    outtree->Branch("PromptPosY", &res.C_Prompt.C_Pos_Y, "PromptPosY/D");
    outtree->Branch("PromptPosZ", &res.C_Prompt.C_Pos_Z, "PromptPosZ/D");
    //Delayed
    outtree->Branch("DelayedEntry", &res.C_Delayed.C_Entry, "DelayedEntry/I");
    outtree->Branch("DelayedEv", &res.C_Delayed.C_EV, "DelayedEv/I");
    outtree->Branch("DelayedGTID", &res.C_Delayed.C_GTID, "DelayedGTID/I");
    outtree->Branch("DelayedOWLHits", &res.C_Delayed.C_OWLHits, "DelayedOWLHits/I");
    outtree->Branch("DelayedNHits", &res.C_Delayed.C_Nhits, "DelayedNHits/I");
    outtree->Branch("DelayedNHitsCleaned", &res.C_Delayed.C_Nhits_Cleaned, "DelayedNHitsCleaned/I");
    outtree->Branch("DelayedNHitsCorrected", &res.C_Delayed.C_Nhits_Corrected, "DelayedNHitsCorrected/I");
    outtree->Branch("Delayed50MHz", &res.C_Delayed.C_50MHZ, "Delayed50MHz/L");
    outtree->Branch("DelayedDataApplied", &res.C_Delayed.C_Data_Applied, "DelayedDataApplied/L");
    outtree->Branch("DelayedDataFlags", &res.C_Delayed.C_Data_Flags, "DelayedDataFlags/L");
    outtree->Branch("DelayedEnergy", &res.C_Delayed.C_Energy, "DelayedEnergy/D");
    outtree->Branch("DelayedOriginalEnergy", &res.C_Delayed.C_Original_Energy, "DelayedOriginalEnergy/D");
    outtree->Branch("DelayedTonyEnergy", &res.C_Delayed.C_Tony_Energy, "DelayedTonyEnergy/D");
    outtree->Branch("DelayedPosX", &res.C_Delayed.C_Pos_X, "DelayedPosX/D");
    outtree->Branch("DelayedPosY", &res.C_Delayed.C_Pos_Y, "DelayedPosY/D");
    outtree->Branch("DelayedPosZ", &res.C_Delayed.C_Pos_Z, "DelayedPosZ/D");
//Read File
    //Read RATDS File
    RAT::DU::DSReader dsreader(RATDSFile);
    RAT::DU::ReconCalibrator recon_cali = RAT::DU::Utility::Get()->GetReconCalibrator();
    RAT::DU::Utility::Get()->BeginOfRun();
    RAT::DU::DetectorStateCorrection detector_state_correct = RAT::DU::Utility::Get()->GetDetectorStateCorrection();
    //Read NTUPLE File
    TFile *ntuplefile = new TFile(NTUPLEFile.c_str());
    TTree *ntupletree = (TTree*)ntuplefile->Get("output");
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
//Recording Run Info
    RecordRunInfo(res, dsreader);
    res.C_SubRunID = sub_run.Atoi();
    Double_t ZOffSet = res.C_ZOffSet;
    TVector3 ZShift(0, 0, ZOffSet);
    // TVector3 ZShift(0, 0, 183);
//Loop
    //Coincidence Pair:{Prompt, Delayed}
    int Prompt = 0; //If there isn't Coincidence pair, let root record a zero result.
    int index = 0;
    Double_t Energy[2] = {-99.0, -99.0};
    Double_t Original_Energy[2] = {-99.0, -99.0}, Tony_Energy[2] = {-99.0, -99.0};
    TVector3 Position[2] = {TVector3(-99.0, -99.0, -99.0), TVector3(-99.0, -99.0, -99.0)};
    Int_t Entry[2] = {99999, 99999}, EV[2] = {99999, 99999};
    Int_t GTID[2] = {-99, -99}, OWL[2] = {-99, -99};
    UInt_t NHits[2] = {99999, 99999}, NHits_Cleaned[2] = {99999, 99999}, NHits_Corrected[2] = {99999, 99999};
    ULong64_t Count_50MHz[2] = {99999, 99999}, Data_Applied[2] = {99999, 99999}, Data_Flags[2] = {99999, 99999};
    Double_t Delta_T = -999.0;
    TVector3 Delta_R(-99.0, -99.0, -99.0);
    std::cout << "Begin to Search Coincidence Pair" << std::endl;
    for(int iEntry = 0; iEntry < ntupletree->GetEntries(); iEntry++)
    {
        if(iEntry % 500 == 0){std::cout << "已处理" << iEntry << ",还剩" << ntupletree->GetEntries() - iEntry << std::endl;};
        ntupletree->GetEntry(iEntry);
        //Initialization
            Energy[0] = Energy[1]= 0;
            Original_Energy[0] = Original_Energy[1] = 0;
            Tony_Energy[0] = Tony_Energy[1] = 0;
            Position[0] = Position[1]  = TVector3(-99.0, -99.0, -99.0);
            Entry[0] = Entry[1] = 0;
            EV[0] = EV[1] = 0;
            GTID[0] = GTID[1] = 0;
            OWL[0] = OWL[1] = 0;
            NHits[0] = NHits[1] = 0;
            NHits_Cleaned[0] = NHits_Cleaned[1] = 0;
            NHits_Corrected[0] = NHits_Corrected[1] = 0;
            Count_50MHz[0] = Count_50MHz[1] = 0;
            Data_Applied[0] = Data_Applied[1] = 0;
            Data_Flags[0] = Data_Flags[1] = 0;
    //Search Prompt Event
        if( ntuple_NHits <= 30 ){continue;};//Boost Searching
        if( ntuple_fitValid != 1){continue;};
        index = 0;
        Position[index] = TVector3(ntuple_PosX, ntuple_PosY, ntuple_PosZ) - ZShift;
        Energy[index] = GiveCorrectedEnergy(ntuple_Original_Energy, Position[index], recon_cali, detector_state_correct);        
        if( PromptCut(Energy[index], Position[index]) == false){continue;};
        //Record Prompt Info
            index = 0;
            Entry[index] = ntuple_Entry;
            EV[index] = ntuple_EV;
            GTID[index] = ntuple_GTID;
            OWL[index] = ntuple_OWLHits;
            NHits[index] = ntuple_NHits;
            NHits_Cleaned[index] = ntuple_NHitsCleaned;
            NHits_Corrected[index] = GiveNHitsCorrected(ntuple_NHits, Position[index], recon_cali, detector_state_correct);
            Count_50MHz[index] = ntuple_50MHz;
            Data_Applied[index] = ntuple_Data_Applied;
            Data_Flags[index] = ntuple_Data_Flags;
            Original_Energy[index] = ntuple_Original_Energy;
            Tony_Energy[index] = GiveTonyEnergy(ntuple_Original_Energy, Position[index], recon_cali, detector_state_correct);
    //Search Delayed Event
        bool TimeOut = false, FindDelayed = false;
        for(int iDEntry = iEntry + 1; iDEntry < ntupletree->GetEntries(); iDEntry++)
        {   
            ntupletree->GetEntry(iDEntry);
            if( ntuple_NHits <= 30 ){continue;};//Boost Searching
            if( ntuple_fitValid != 1){continue;};
            Delta_T = 20.0 * ((ntuple_50MHz - Count_50MHz[0]) & 0x7FFFFFFFFFF);
            if(Delta_T < DELTA_T_MIN){continue;};
            if(Delta_T > DELTA_T_MAX){TimeOut = true; break;};
            index = 1;
            Position[index] = TVector3(ntuple_PosX, ntuple_PosY, ntuple_PosZ) - ZShift;
            Energy[index] = GiveCorrectedEnergy(ntuple_Original_Energy, Position[index], recon_cali, detector_state_correct);         
            if( DelayedCut(Energy[index], Position[index]) == false){continue;};
            Delta_R = Position[1] - Position[0];
            if( Delta_R.Mag() > DELTA_R_MAX) {continue;};
            FindDelayed = true;
            if(FindDelayed == true)
            {
                index = 1;
                Entry[index] = ntuple_Entry;
                EV[index] = ntuple_EV;
                GTID[index] = ntuple_GTID;
                OWL[index] = ntuple_OWLHits;
                NHits[index] = ntuple_NHits;
                NHits_Cleaned[index] = ntuple_NHitsCleaned;
                NHits_Corrected[index] = GiveNHitsCorrected(ntuple_NHits, Position[index], recon_cali, detector_state_correct);
                Count_50MHz[index] = ntuple_50MHz;
                Data_Applied[index] = ntuple_Data_Applied;
                Data_Flags[index] = ntuple_Data_Flags;
                Original_Energy[index] = ntuple_Original_Energy;
                Tony_Energy[index] = GiveTonyEnergy(ntuple_Original_Energy, Position[index], recon_cali, detector_state_correct);
                break;
            };
        };
        if( TimeOut == true) {continue;};
        //Record Coincidence Pair
        if( TimeOut == false && FindDelayed == true)
        {
            std::cout << "Find Coincidence Pair!!!Recording!!!" << std::endl;
            RecordCoincidencePair(res, Energy, Original_Energy, Tony_Energy, Entry, EV, GTID, OWL, NHits, NHits_Cleaned, NHits_Corrected, Count_50MHz, Data_Applied, Data_Flags, Position[0], Position[1]);
            Prompt = 1;
            res.ShowPrompt();
            res.ShowDelayed();
        //Write into ROOT File
            outtree->Fill();
        };
    };
    if(Prompt == 0){outtree->Fill();};
    outfile->Write();
    delete outtree; outtree = NULL;
    outfile->Close();
    delete outfile; outfile = NULL;

    std::cout << "Complete" << std::endl;
    return 0;
};

////////////////////////////////////////////////////////////////////////////////////////
void ShowVector(const TVector3 v)
{
    std::cout << "X:" << v.X() << ",Y:" << v.Y() << ",Z:" << v.Z() << ",R:" << v.Mag()<< std::endl;
};

void ShowVector(const RAT::DU::Point3D v)
{
    std::cout << "X:" << v.X() << ",Y:" << v.Y() << ",Z:" << v.Z() << ",R:" << v.Mag()<< std::endl;
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

bool PromptCut(const Double_t Energy, const TVector3 Position)
{
    if(Energy < ENERGY_PROMPT_MIN || Energy > ENERGY_PROMPT_MAX){return false;};
    if(Position.Mag() > RADIUS_MAX){return false;};
    return true;
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

Int_t GiveNHitsCorrected(Int_t Original_NHits, TVector3 Position,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct)
{
    RAT::DU::Point3D tempPoint3D(0, Position);
    Int_t NHits_Corrected = (Original_NHits)/detector_state_correct.GetCorrectionPos(tempPoint3D, 0, 0);
    return NHits_Corrected;
};

bool DelayedCut(const Double_t Energy, const TVector3 Positon)
{
    if(Energy < ENERGY_DELAYED_MIN || Energy > ENERGY_DELAYED_MAX){return false;};
    if(Positon.Mag() > RADIUS_MAX){return false;};
    return true;
};

void RecordCoincidencePair(myresult &res, const Double_t Energy[], const Double_t Original_Energy[],
const Double_t Tony_Energy[], Int_t Entry[], Int_t EV[],
const Int_t GTID[], const Int_t OWL[], const UInt_t NHits[], const UInt_t NHitsCleaned[],
const UInt_t NHitsCorrected[], const ULong64_t Count50MHz[], const ULong64_t DataApplied[],
const ULong64_t DataFlags[], const TVector3 PromptPos, const TVector3 DelayedPos)
{
//Prompt
    res.ClearPrompt();
    int index = 0;
    res.C_Prompt.C_Entry = Entry[index];
    res.C_Prompt.C_EV = EV[index];
    res.C_Prompt.C_GTID = GTID[index];
    res.C_Prompt.C_OWLHits = OWL[index];
    res.C_Prompt.C_Nhits = NHits[index];
    res.C_Prompt.C_Nhits_Cleaned = NHitsCleaned[index];
    res.C_Prompt.C_Nhits_Corrected = NHitsCorrected[index];
    res.C_Prompt.C_50MHZ = Count50MHz[index];
    res.C_Prompt.C_Energy = Energy[index];
    res.C_Prompt.C_Original_Energy = Original_Energy[index];
    res.C_Prompt.C_Tony_Energy = Tony_Energy[index];
    res.C_Prompt.C_Data_Applied = DataApplied[index];
    res.C_Prompt.C_Data_Flags = DataFlags[index];
    res.C_Prompt.C_Pos_X = PromptPos.X();
    res.C_Prompt.C_Pos_Y = PromptPos.Y();
    res.C_Prompt.C_Pos_Z = PromptPos.Z();
//Delayed
    res.ClearDelayed();
    index = 1;
    res.C_Delayed.C_Entry = Entry[index];
    res.C_Delayed.C_EV = EV[index];
    res.C_Delayed.C_GTID = GTID[index];
    res.C_Delayed.C_OWLHits = OWL[index];
    res.C_Delayed.C_Nhits = NHits[index];
    res.C_Delayed.C_Nhits_Cleaned = NHitsCleaned[index];
    res.C_Delayed.C_Nhits_Corrected = NHitsCorrected[index];
    res.C_Delayed.C_50MHZ = Count50MHz[index];
    res.C_Delayed.C_Data_Applied = DataApplied[index];
    res.C_Delayed.C_Data_Flags = DataFlags[index];
    res.C_Delayed.C_Energy = Energy[index];
    res.C_Delayed.C_Original_Energy = Original_Energy[index];
    res.C_Delayed.C_Tony_Energy = Tony_Energy[index];
    res.C_Delayed.C_Pos_X = DelayedPos.X();
    res.C_Delayed.C_Pos_Y = DelayedPos.Y();
    res.C_Delayed.C_Pos_Z = DelayedPos.Z();
}