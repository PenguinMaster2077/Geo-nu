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
#include <RAT/DS/Run.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DS/DataQCFlags.hh>
#include <RAT/DS/BitMask.hh>
#include <RAT/DS/FitResult.hh>
#include <RAT/DS/FitVertex.hh>
#include <RAT/DS/UniversalTime.hh>
#include <RAT/DS/PMTSet.hh>
#include <RAT/DB.hh>
#include <RAT/DBLink.hh>
#include <RAT/DBTable.hh>
#include <RAT/DU/Utility.hh>
#include <RAT/DU/ReconCalibrator.hh>
#include <RAT/DU/Point3D.hh>
#include <RAT/DU/DetectorStateCorrection.hh>
//Self-defined
//////////////////////////////////////////////////////////////////////////////
const Double_t RADIUS_MAX = 6000.0;//mm
// const Double_t DELTA_R_MAX = 12000.0;//mm
// const Double_t DELTA_T_MIN = 400.0, DELTA_T_MAX = 1.0e7;//ns
const Double_t ENERGY_PROMPT_MIN = 0.3, ENERGY_PROMPT_MAX = 5.0;//MeV
const Double_t ENERGY_DELAYED_MIN = 0.3, ENERGY_DELAYED_MAX = 2.0; //MeV
const int NHITS_MIN = 60;
const int MUON_NHITS_MIN = 2500;
//////////////////////////////////////////////////////////////////////////////

RAT::DU::ReconCalibrator recon_cali;
RAT::DU::DetectorStateCorrection detector_state_correct;
Double_t Energy[2] = {0, 0}, Original_Energy[2] = {0, 0}, Tony_Energy[2] = {0, 0};
Int_t Entry[2] = {9999, 9999}, EV[2] = {9999, 9999}, GTID[2] = {9999, 9999}, OWL[2] = {9999, 9999};
Int_t NHits[2] = {9999, 9999}, NHits_Cleaned[2] = {9999, 9999}, NHits_Corrected[2] = {9999, 9999};
ULong64_t Count_50MHz[2] = {9999, 9999}, Data_Applied[2] = {9999, 9999}, Data_Flags[2] = {9999, 9999};
TVector3 Position[2], ZShift;
bool data_mc = 0;

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

        C_FitValid = 0;
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
    Int_t C_FitValid;
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
        C_Begin_Count = 0;
        C_End_Count = 0;
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
//Begin and End Count
    ULong64_t C_Begin_Count, C_End_Count;
};

bool PromptCut(const Double_t Energy, const TVector3 Positon);

bool DelayedCut(const Double_t Energy, const TVector3 Positon);

// int LooseCut_AC_Method_I()
int main(int argc, char** argv)
{
    // std::string RATFile = "/home/shuaioy/scratch/Geo/Gold/300000-306498/ratds/300050/Data/Analysis20R_r0000300050_s000_p000.root";
    // std::string NTUPLEFile = "/home/shuaioy/scratch/Geo/Gold/300000-306498/ntuple/300050/Data/Analysis20R_r0000300050_s013_p000.ntuple.root";
    // std::string OutFile_Prompt = "./test_prompt.root";
    // std::string OutFIle_Delayed = "./test_dealyed.root";
    // TString sub_run = "13";
    // TString Data_MC = "1";

    std::string RATFile = argv[1];
    std::string NTUPLEFile = argv[2];
    std::string OutFile_Prompt = argv[3];
    std::string OutFIle_Delayed = argv[4];
    TString sub_run = argv[5];
    TString Data_MC = argv[6];
    std::cout << "RAT File:" << RATFile << std::endl;
    std::cout << "NTUPLE File:" << NTUPLEFile << std::endl;
    std::cout << "Prompt File:" << OutFile_Prompt << std::endl;
    std::cout << "Delayed File:" << OutFIle_Delayed << std::endl;
    std::cout << "SubRun:" << sub_run << std::endl;
    std::cout << "Data_MC:" << Data_MC << "," << Data_MC.Atoi() << std::endl;
//Create New Root File
    TFile *out_prompt = new TFile(OutFile_Prompt.c_str(), "recreate");
    TTree *outtree_prompt = new TTree("output","");
    TFile *out_delayed = new TFile(OutFIle_Delayed.c_str(), "recreate");
    TTree *outtree_delayed = new TTree("output","");
//Create Wanted Info Class
    myresult res;
//Create Branch
    //Prompt
    outtree_prompt->Branch("RunID", &res.C_RunID);
    outtree_prompt->Branch("SubRunID", &res.C_SubRunID);
    outtree_prompt->Branch("Date", &res.C_Date);
    outtree_prompt->Branch("RunType", &res.C_RunType);
    outtree_prompt->Branch("ZOffSet", &res.C_ZOffSet);
    outtree_prompt->Branch("BeginCount", &res.C_Begin_Count);
    outtree_prompt->Branch("EndCount", &res.C_End_Count);
    
    outtree_prompt->Branch("PromptEntry", &res.C_Prompt.C_Entry);
    outtree_prompt->Branch("PromptEv", &res.C_Prompt.C_EV);
    outtree_prompt->Branch("PromptGTID", &res.C_Prompt.C_GTID);
    outtree_prompt->Branch("PromptOWLHits", &res.C_Prompt.C_OWLHits);
    outtree_prompt->Branch("PromptFitValid", &res.C_Prompt.C_FitValid);
    outtree_prompt->Branch("PromptNHits", &res.C_Prompt.C_Nhits);
    outtree_prompt->Branch("PromptNHitsCleaned", &res.C_Prompt.C_Nhits_Cleaned);
    outtree_prompt->Branch("PromptNHitsCorrected", &res.C_Prompt.C_Nhits_Corrected);
    outtree_prompt->Branch("Prompt50MHz", &res.C_Prompt.C_50MHZ);
    outtree_prompt->Branch("PromptDataApplied", &res.C_Prompt.C_Data_Applied);
    outtree_prompt->Branch("PromptDataFlags", &res.C_Prompt.C_Data_Flags);
    outtree_prompt->Branch("PromptEnergy", &res.C_Prompt.C_Energy);
    outtree_prompt->Branch("PromptOriginalEnergy", &res.C_Prompt.C_Original_Energy);
    outtree_prompt->Branch("PromptTonyEnergy", &res.C_Prompt.C_Tony_Energy);
    outtree_prompt->Branch("PromptPosX", &res.C_Prompt.C_Pos_X);
    outtree_prompt->Branch("PromptPosY", &res.C_Prompt.C_Pos_Y);
    outtree_prompt->Branch("PromptPosZ", &res.C_Prompt.C_Pos_Z);
    //Delayed
    outtree_delayed->Branch("RunID", &res.C_RunID);
    outtree_delayed->Branch("SubRunID", &res.C_SubRunID);
    outtree_delayed->Branch("Date", &res.C_Date);
    outtree_delayed->Branch("RunType", &res.C_RunType);
    outtree_delayed->Branch("ZOffSet", &res.C_ZOffSet);
    outtree_delayed->Branch("BeginCount", &res.C_Begin_Count);
    outtree_delayed->Branch("EndCount", &res.C_End_Count);

    outtree_delayed->Branch("DelayedEntry", &res.C_Delayed.C_Entry);
    outtree_delayed->Branch("DelayedEv", &res.C_Delayed.C_EV);
    outtree_delayed->Branch("DelayedGTID", &res.C_Delayed.C_GTID);
    outtree_delayed->Branch("DelayedOWLHits", &res.C_Delayed.C_OWLHits);
    outtree_delayed->Branch("DelayedFitValid", &res.C_Delayed.C_FitValid);
    outtree_delayed->Branch("DelayedNHits", &res.C_Delayed.C_Nhits);
    outtree_delayed->Branch("DelayedNHitsCleaned", &res.C_Delayed.C_Nhits_Cleaned);
    outtree_delayed->Branch("DelayedNHitsCorrected", &res.C_Delayed.C_Nhits_Corrected);
    outtree_delayed->Branch("Delayed50MHz", &res.C_Delayed.C_50MHZ);
    outtree_delayed->Branch("DelayedDataApplied", &res.C_Delayed.C_Data_Applied);
    outtree_delayed->Branch("DelayedDataFlags", &res.C_Delayed.C_Data_Flags);
    outtree_delayed->Branch("DelayedEnergy", &res.C_Delayed.C_Energy);
    outtree_delayed->Branch("DelayedOriginalEnergy", &res.C_Delayed.C_Original_Energy);
    outtree_delayed->Branch("DelayedTonyEnergy", &res.C_Delayed.C_Tony_Energy);
    outtree_delayed->Branch("DelayedPosX", &res.C_Delayed.C_Pos_X);
    outtree_delayed->Branch("DelayedPosY", &res.C_Delayed.C_Pos_Y);
    outtree_delayed->Branch("DelayedPosZ", &res.C_Delayed.C_Pos_Z);
//Read File
    data_mc = Data_MC.Atoi();
    if(data_mc == 1){std::cout << "Processing Data" << std::endl;}
    else if(data_mc == 0){std::cout << "Processing MC" << std::endl;};
    //Read RATDS File
    // RAT::DU::DSReader dsreader(RATDSFile);
    // recon_cali = RAT::DU::Utility::Get()->GetReconCalibrator();
    // detector_state_correct = RAT::DU::Utility::Get()->GetDetectorStateCorrection();
    //Read NTUPLE File
    TFile *ntuplefile = new TFile(NTUPLEFile.c_str());
    TTree *ntupletree = (TTree*)ntuplefile->Get("output");
    //Set Branch Address
    Int_t ntuple_runID;
    ntupletree->SetBranchAddress("runID", &ntuple_runID);
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
//Record Run Info
    ntupletree->GetEntry(0);
    res.C_RunID = ntuple_runID;
    res.C_SubRunID = sub_run.Atoi();
    res.C_Begin_Count = ntuple_50MHz;
    int Len = ntupletree->GetEntries();
    ntupletree->GetEntry(Len - 1);
    res.C_End_Count = ntuple_50MHz;
//Loop
    TVector3 Pos;
    for(int ii1 = 0; ii1 < ntupletree->GetEntries(); ii1++)
    {
        if(ii1 % 5000 == 0){std::cout << "已处理:" << double (100 * ii1/Len)  <<"%，具体:"<< ii1 << "，还剩" << Len - ii1 << std::endl;};
        ntupletree->GetEntry(ii1);
        if(ntuple_fitValid != 1) {continue;};
        Pos.SetXYZ(ntuple_PosX, ntuple_PosY, ntuple_PosZ);
        //Prompt
        if(PromptCut(ntuple_Original_Energy, Pos) == true)
        {
            res.C_Prompt.C_Entry = ntuple_Entry;
            res.C_Prompt.C_EV = ntuple_EV;
            res.C_Prompt.C_GTID = ntuple_GTID;

            res.C_Prompt.C_Nhits = ntuple_NHits;
            res.C_Prompt.C_Nhits_Cleaned = ntuple_NHitsCleaned;
            res.C_Prompt.C_OWLHits = ntuple_OWLHits;
            res.C_Prompt.C_50MHZ = ntuple_50MHz;
            res.C_Prompt.C_Data_Applied = ntuple_Data_Applied;
            res.C_Prompt.C_Data_Flags = ntuple_Data_Flags;
            res.C_Prompt.C_FitValid = ntuple_fitValid;

            res.C_Prompt.C_Energy = ntuple_Original_Energy;
            res.C_Prompt.C_Original_Energy = ntuple_Original_Energy;
            res.C_Prompt.C_Pos_X = ntuple_PosX;
            res.C_Prompt.C_Pos_Y = ntuple_PosY;
            res.C_Prompt.C_Pos_Z = ntuple_PosZ;

            outtree_prompt->Fill();
        };
        //Delayed
        if(DelayedCut(ntuple_Original_Energy, Pos) == true)
        {
            res.C_Delayed.C_Entry = ntuple_Entry;
            res.C_Delayed.C_EV = ntuple_EV;
            res.C_Delayed.C_GTID = ntuple_GTID;

            res.C_Delayed.C_Nhits = ntuple_NHits;
            res.C_Delayed.C_Nhits_Cleaned = ntuple_NHitsCleaned;
            res.C_Delayed.C_OWLHits = ntuple_OWLHits;
            res.C_Delayed.C_50MHZ = ntuple_50MHz;
            res.C_Delayed.C_Data_Applied = ntuple_Data_Applied;
            res.C_Delayed.C_Data_Flags = ntuple_Data_Flags;
            res.C_Delayed.C_FitValid = ntuple_fitValid;

            res.C_Delayed.C_Energy = ntuple_Original_Energy;
            res.C_Delayed.C_Original_Energy = ntuple_Original_Energy;
            res.C_Delayed.C_Pos_X = ntuple_PosX;
            res.C_Delayed.C_Pos_Y = ntuple_PosY;
            res.C_Delayed.C_Pos_Z = ntuple_PosZ;

            outtree_delayed->Fill();
        };
    };
    out_prompt->Write();
    out_prompt->Close();
    out_delayed->Write();
    out_delayed->Close();
    std::cout << "Compete" << std::endl;
    return 0;
};

bool PromptCut(const Double_t Energy, const TVector3 Position)
{
    if(Energy < ENERGY_PROMPT_MIN || Energy > ENERGY_PROMPT_MAX){return false;};
    if(Position.Mag() > RADIUS_MAX){return false;};
    return true;
};

bool DelayedCut(const Double_t Energy, const TVector3 Positon)
{
    if(Energy < ENERGY_DELAYED_MIN || Energy > ENERGY_DELAYED_MAX){return false;};
    if(Positon.Mag() > RADIUS_MAX){return false;};
    return true;
};