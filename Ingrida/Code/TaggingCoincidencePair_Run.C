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
#include <RAT/DB.hh>
#include <RAT/DBLink.hh>
#include <RAT/DBTable.hh>
#include <RAT/DU/Utility.hh>
#include <RAT/DU/ReconCalibrator.hh>
#include <RAT/DU/Point3D.hh>
#include <RAT/DU/DetectorStateCorrection.hh>
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
        std::cout << "Data Applied:" << C_Data_Applied << std::endl;
        std::cout << "Data Flags:" << C_Data_Flags << std::endl;
    };
    void ShowPosTime()
    {
        std::cout << "E:" << C_Energy << ",X:" << C_Pos_X << ",Y:" << C_Pos_Y << ",Z:" << C_Pos_Z << std::endl;
    };
//EV Info
    unsigned int C_Entry, C_EV;
    Int_t C_GTID;
    UInt_t C_Nhits, C_Nhits_Cleaned, C_Nhits_Corrected;
    ULong64_t C_50MHZ, C_Data_Applied, C_Data_Flags;
//Energy and Position
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

bool GetReconInfo(TVector3 ZShift, Double_t &Energy, TVector3 &Position, RAT::DS::EV &ev,
Int_t &GTID, UInt_t &NHits, UInt_t &NHits_Cleaned, UInt_t &NHits_Corrected, ULong64_t &Count50MHz,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct);

bool PromptCut(const Double_t Energy, const TVector3 Positon);

bool DelayedCut(const Double_t Energy, const TVector3 Positon);

bool SearchDelayed(RAT::DU::DSReader &dsreader, TVector3 ZShift,unsigned int &iEntry,
unsigned int &iEV, Double_t &Delta_T, TVector3 &Delta_R,Double_t &PromptEnergy,
TVector3 &PromptPos, Double_t &DelayedEnergy, TVector3 &DelayedPos, unsigned int &DelayedEntry,
unsigned int &DelayedEV, Int_t &DelayedGTID, UInt_t &DelayedNhits, UInt_t &DelayedNhitsCleaned,
UInt_t &DelayedNhitsCorrected, ULong64_t &Delayed50MHz, ULong64_t &DelayedDataApplied,
ULong64_t &DelayedDataFlags,RAT::DU::ReconCalibrator &recon_cali,
RAT::DU::DetectorStateCorrection &detector_state_correct);

void RecordCoincidencePair(myresult &res, const Double_t Energy[], const unsigned int Entry[],
const unsigned int EV[], const Int_t GTID[], const UInt_t NHits[], const UInt_t NHitsCleaned[],
const UInt_t NHitsCorrected[], const ULong64_t Count50MHz[], const ULong64_t DataApplied[],
const ULong64_t DataFlags[],
const TVector3 PromptPos, const TVector3 DelayedPos);

// int TaggingCoincidencePair()
int main(int argc, char** argv)
{   
    std::string InFile = argv[1];
    std::string OutFile = argv[2];

    std::vector<std::string> RunFile = Run(InFile);//一定要修改RAT::DU::DSReader

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
    outtree->Branch("PromptNHits", &res.C_Prompt.C_Nhits, "PromptNHits/I");
    outtree->Branch("PromptNHitsCleaned", &res.C_Prompt.C_Nhits_Cleaned, "PromptNHitsCleaned/I");
    outtree->Branch("PromptNHitsCorrected", &res.C_Prompt.C_Nhits_Corrected, "PromptNHitsCorrected/I");
    outtree->Branch("Prompt50MHz", &res.C_Prompt.C_50MHZ, "Prompt50MHz/L");
    outtree->Branch("PromptDataApplied", &res.C_Prompt.C_Data_Applied, "PromptDataApplied/L");
    outtree->Branch("PromptDataFlags", &res.C_Prompt.C_Data_Flags, "PromptDataFlags/L");
    outtree->Branch("PromptEnergy", &res.C_Prompt.C_Energy, "PromptEnergy/D");
    outtree->Branch("PromptPosX", &res.C_Prompt.C_Pos_X, "PromptPosX/D");
    outtree->Branch("PromptPosY", &res.C_Prompt.C_Pos_Y, "PromptPosY/D");
    outtree->Branch("PromptPosZ", &res.C_Prompt.C_Pos_Z, "PromptPosZ/D");
    //Delayed
    outtree->Branch("DelayedEntry", &res.C_Delayed.C_Entry, "DelayedEntry/I");
    outtree->Branch("DelayedEv", &res.C_Delayed.C_EV, "DelayedEv/I");
    outtree->Branch("DelayedGTID", &res.C_Delayed.C_GTID, "DelayedGTID/I");
    outtree->Branch("DelayedNHits", &res.C_Delayed.C_Nhits, "DelayedNHits/I");
    outtree->Branch("DelayedNHitsCleaned", &res.C_Delayed.C_Nhits_Cleaned, "DelayedNHitsCleaned/I");
    outtree->Branch("DelayedNHitsCorrected", &res.C_Delayed.C_Nhits_Corrected, "DelayedNHitsCorrected/I");
    outtree->Branch("Delayed50MHz", &res.C_Delayed.C_50MHZ, "Delayed50MHz/L");
    outtree->Branch("DelayedDataApplied", &res.C_Delayed.C_Data_Applied, "DelayedDataApplied/L");
    outtree->Branch("DelayedDataFlags", &res.C_Delayed.C_Data_Flags, "DelayedDataFlags/L");
    outtree->Branch("DelayedEnergy", &res.C_Delayed.C_Energy, "DelayedEnergy/D");
    outtree->Branch("DelayedPosX", &res.C_Delayed.C_Pos_X, "DelayedPosX/D");
    outtree->Branch("DelayedPosY", &res.C_Delayed.C_Pos_Y, "DelayedPosY/D");
    outtree->Branch("DelayedPosZ", &res.C_Delayed.C_Pos_Z, "DelayedPosZ/D");
//Read File
    RAT::DU::DSReader dsreader(RunFile);
    RAT::DU::ReconCalibrator recon_cali = RAT::DU::Utility::Get()->GetReconCalibrator();
    RAT::DU::Utility::Get()->BeginOfRun();
    RAT::DU::DetectorStateCorrection detector_state_correct = RAT::DU::Utility::Get()->GetDetectorStateCorrection();
//Recording Run Info
    RecordRunInfo(res, dsreader);
    Double_t ZOffSet = res.C_ZOffSet;
    TVector3 ZShift(0, 0, ZOffSet);
//Loop
    RAT::DS::Entry entry;
    RAT::DS::EV ev;
    RAT::DS::DataQCFlags data;
    //Coincidence Pair:{Prompt, Delayed}
    int Prompt = 0; //If there isn't Coincidence pair, let root record a zero result.
    Double_t Energy[2] = {-99.0, -99.0};
    TVector3 Position[2] = {TVector3(-99.0, -99.0, -99.0), TVector3(-99.0, -99.0, -99.0)};
    unsigned int Entry[2] = {99999, 99999}, EV[2] = {99999, 99999};
    Int_t GTID[2] = {-99, -99};
    UInt_t NHits[2] = {99999, 99999}, NHits_Cleaned[2] = {99999, 99999}, NHits_Corrected[2] = {99999, 99999};
    ULong64_t Count_50MHz[2] = {99999, 99999}, Data_Applied[2] = {99999, 99999}, Data_Flags[2] = {99999, 99999};
    Double_t Delta_T = -999.0;
    TVector3 Delta_R(-99.0, -99.0, -99.0);
    std::cout << "Begin to Search Coincidence Pair" << std::endl;
    for(unsigned int iEntry = 0; iEntry < dsreader.GetEntryCount(); iEntry++)
    {
        if(iEntry % 500 == 0){std::cout << "已处理" << iEntry << ",还剩" << dsreader.GetEntryCount() - iEntry << std::endl;};
        entry = dsreader.GetEntry(iEntry);
        for(unsigned int iEv = 0; iEv < entry.GetEVCount(); iEv++)
        {
            ev = entry.GetEV(iEv);
        //Initialization
            Energy[0] = Energy[1]= -99.0;
            Position[0] = Position[1]  = TVector3(-99.0, -99.0, -99.0);
            Entry[0] = Entry[1] = 0;
            EV[0] = EV[1] = 0;
            GTID[0] = GTID[1] = 0;
            NHits[0] = NHits[1] = 0;
            NHits_Cleaned[0] = NHits_Cleaned[1] = 0;
            NHits_Corrected[0] = NHits_Corrected[1] = 0;
            Count_50MHz[0] = Count_50MHz[1] = 0;
            Data_Applied[0] = Data_Applied[1] = 0;
            Data_Flags[0] = Data_Flags[1] = 0;
        //Find Prompt Event
            if(ev.GetNhits() < 30){continue;};//Boost Finding
            if(GetReconInfo(ZShift, Energy[0], Position[0], ev, GTID[0], NHits[0], NHits_Cleaned[0], NHits_Corrected[0], Count_50MHz[0], recon_cali, detector_state_correct) == false){continue;};//skip the events that doesn't have reconcted info
            if(PromptCut(Energy[0], Position[0]) == false){continue;};
            Entry[0] = iEntry;
            EV[0] = iEv;
            //DataCleaning
            data = ev.GetDataCleaningFlags();
            Int_t last_pass = data.GetLatestPass();
            if(last_pass >= 0)
            {
                Data_Applied[0] = data.GetApplied(last_pass).GetULong64_t(0);
                Data_Flags[0] = data.GetFlags(last_pass).GetULong64_t(0);
            };
            //std::cout << "Find Prompt Event!!!" << std::endl;
            //std::cout << "Entry:" << iEntry << ", EV:" << iEv << ", GTID:" << ev.GetGTID() << ", Energy:" << Energy[0] << std::endl;
            //ShowVector(Position[0]);
        //Find Delayed Event
            if(SearchDelayed(dsreader, ZShift, iEntry, iEv, Delta_T, Delta_R, Energy[0], Position[0], Energy[1], Position[1], Entry[1], EV[1], GTID[1], NHits[1], NHits_Cleaned[1], NHits_Corrected[1], Count_50MHz[1], Data_Applied[1], Data_Flags[1], recon_cali, detector_state_correct) == false){continue;};
        //Record Coincidence pair
            std::cout << "Find Coincidence Pair!!!Recording!!!" << std::endl;
            RecordCoincidencePair(res, Energy, Entry, EV, GTID, NHits, NHits_Cleaned, NHits_Corrected, Count_50MHz, Data_Applied, Data_Flags, Position[0], Position[1]);
            Prompt = 1;
            res.ShowPrompt();
            res.ShowDelayed();
        //Write into ROOT File
            outtree->Fill();
        };
    };
    if(Prompt == 0){outtree->Fill();};
    outfile->Write();
    //delete outtree; outfile = NULL;
    outfile->Close();
    //delete outfile; outfile = NULL;

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

bool GetReconInfo(TVector3 ZShift, Double_t &Energy, TVector3 &Position, RAT::DS::EV &ev,
Int_t &GTID, UInt_t &NHits, UInt_t &NHits_Cleaned, UInt_t &NHits_Corrected, ULong64_t &Count50MHz,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct)
{
try{
//Valid
    std::string fit_name = ev.GetDefaultFitName();
    if(fit_name == "") {return false;};
    RAT::DS::FitResult fit_result = ev.GetFitResult(fit_name);
    if(!fit_result.GetValid()) {return false;};
    RAT::DS::FitVertex fit_vertex = fit_result.GetVertex(0);
    if(!(fit_vertex.ValidPosition() && fit_vertex.ValidTime() && fit_vertex.ValidEnergy())){return false;};
//Get Info
    Energy = fit_vertex.GetEnergy();
    Position = fit_vertex.GetPosition() - ZShift;
//Logan's Energy Correction----DocDB: 7730
//Detector Status Correction will be integrated in reconstruction later, thus it should be firstly applied
    RAT::DU::Point3D tempPoint3D(0, Position);
    Energy = Energy * detector_state_correct.GetCorrection(9394, 0.75058) / detector_state_correct.GetCorrectionPos(tempPoint3D, 0, 0);
//Tony's Energy Correction-----DocDB: 7895
    Energy = recon_cali.CalibrateEnergyRTF(true, Energy, Position.Perp(), Position.Z());
//NHits
    GTID = ev.GetGTID();
    NHits = ev.GetNhits();
    NHits_Cleaned = ev.GetNhitsCleaned();
    NHits_Corrected = (NHits)/detector_state_correct.GetCorrectionPos(tempPoint3D, 0, 0);
//50MHz
    Count50MHz = ev.GetClockCount50();
    }
    catch(const RAT::DS::DataNotFound&) {return false;}
    catch(const RAT::DS::FitCollection::NoResultError&) {return false;}
    catch(const RAT::DS::FitResult::NoVertexError&) {return false;}
    catch(const RAT::DS::FitVertex::NoValueError&) {return false;}
    return true;
};

bool PromptCut(const Double_t Energy, const TVector3 Position)
{
    if(Energy < ENERGY_PROMPT_MIN || Energy > ENERGY_PROMPT_MAX){return false;};
    if(Position.Mag() > RADIUS_MAX){return false;};
    return true;
};
bool SearchDelayed(RAT::DU::DSReader &dsreader, TVector3 ZShift, unsigned int &iEntry,
unsigned int &iEV, Double_t &Delta_T, TVector3 &Delta_R,Double_t &PromptEnergy,
TVector3 &PromptPos, Double_t &DelayedEnergy, TVector3 &DelayedPos, unsigned int &DelayedEntry,
unsigned int &DelayedEV, Int_t &DelayedGTID, UInt_t &DelayedNhits, UInt_t &DelayedNhitsCleaned,
UInt_t &DelayedNhitsCorrected, ULong64_t &Delayed50MHz, ULong64_t &DelayedDataApplied,
ULong64_t &DelayedDataFlags,RAT::DU::ReconCalibrator &recon_cali,
RAT::DU::DetectorStateCorrection &detector_state_correct)
{
    bool TimeOut = false, FindDelayed = false;
    RAT::DS::Entry prompt_entry = dsreader.GetEntry(iEntry);
    RAT::DS::EV prompt_ev = prompt_entry.GetEV(iEV);
    ULong64_t prompt_50MHz_time = prompt_ev.GetClockCount50();

    for(unsigned int iDEntry = iEntry ; iDEntry < dsreader.GetEntryCount(); iDEntry++)
    {
        RAT::DS::Entry delayed_entry = dsreader.GetEntry(iDEntry);
        for(unsigned int iDEv = iEV; iDEv < delayed_entry.GetEVCount(); iDEv++)
        {
            RAT::DS::EV delayed_ev = delayed_entry.GetEV(iDEv);
            if(!(GetReconInfo(ZShift, DelayedEnergy, DelayedPos, delayed_ev, DelayedGTID, DelayedNhits, DelayedNhitsCleaned, DelayedNhitsCorrected, Delayed50MHz, recon_cali, detector_state_correct))){continue;};
            Delta_T = ((delayed_ev.GetClockCount50()- prompt_50MHz_time) & 0x7FFFFFFFFFF) * 20.0;//ns
            if(Delta_T < DELTA_T_MIN){continue;};
            if(Delta_T > DELTA_T_MAX){TimeOut = true; break;};
            if(DelayedCut(DelayedEnergy, DelayedPos) == false){continue;};
            Delta_R = DelayedPos - PromptPos;
            if(Delta_R.Mag() > DELTA_R_MAX) {continue;};
            FindDelayed = true;
            //std::cout << "Find Dealyed Event!!!" << std::endl;
            //std::cout << "Entry:" << iDEntry << ", EV:" << iDEv << ", GTID:" << delayed_ev.GetGTID() << ", Energy:" << DelayedEnergy << std::endl;
            if(FindDelayed == true) 
            {  
                RAT::DS::DataQCFlags tempdata = delayed_ev.GetDataCleaningFlags();
                Int_t last_pass = tempdata.GetLatestPass();
                if(last_pass >= 0)
                {
                    DelayedDataApplied = tempdata.GetApplied(last_pass).GetULong64_t(0);
                    DelayedDataFlags = tempdata.GetFlags(last_pass).GetULong64_t(0);
                };
                DelayedEntry = iDEntry;
                DelayedEV = iDEv;
                break;
            };
        };
        if(TimeOut == true || FindDelayed == true){break;};
    };
    if(FindDelayed == true && TimeOut == false) 
    {
        //std::cout << "Find Dealyed Event" << std::endl;
        return true;
    }
    else
    {
        //std::cout << "Timeout, doesn't Find Delayed Event" << std::endl;
        return false;
    };
};

bool DelayedCut(const Double_t Energy, const TVector3 Positon)
{
    if(Energy < ENERGY_DELAYED_MIN || Energy > ENERGY_DELAYED_MAX){return false;};
    if(Positon.Mag() > RADIUS_MAX){return false;};
    return true;
};

void RecordCoincidencePair(myresult &res, const Double_t Energy[], const unsigned int Entry[],
const unsigned int EV[], const Int_t GTID[], const UInt_t NHits[], const UInt_t NHitsCleaned[],
const UInt_t NHitsCorrected[], const ULong64_t Count50MHz[], const ULong64_t DataApplied[],
const ULong64_t DataFlags[],
const TVector3 PromptPos, const TVector3 DelayedPos)
{
//Prompt
    res.ClearPrompt();
    int index = 0;
    res.C_Prompt.C_Entry = Entry[index];
    res.C_Prompt.C_EV = EV[index];
    res.C_Prompt.C_GTID = GTID[index];
    res.C_Prompt.C_Nhits = NHits[index];
    res.C_Prompt.C_Nhits_Cleaned = NHitsCleaned[index];
    res.C_Prompt.C_Nhits_Corrected = NHitsCorrected[index];
    res.C_Prompt.C_50MHZ = Count50MHz[index];
    res.C_Prompt.C_Energy = Energy[index];
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
    res.C_Delayed.C_Nhits = NHits[index];
    res.C_Delayed.C_Nhits_Cleaned = NHitsCleaned[index];
    res.C_Delayed.C_Nhits_Corrected = NHitsCorrected[index];
    res.C_Delayed.C_50MHZ = Count50MHz[index];
    res.C_Delayed.C_Data_Applied = DataApplied[index];
    res.C_Delayed.C_Data_Flags = DataFlags[index];
    res.C_Delayed.C_Energy = Energy[index];
    res.C_Delayed.C_Pos_X = DelayedPos.X();
    res.C_Delayed.C_Pos_Y = DelayedPos.Y();
    res.C_Delayed.C_Pos_Z = DelayedPos.Z();
}