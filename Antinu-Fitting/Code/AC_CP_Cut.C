//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TVector3.h>
//RAT
//Self-Defined

//Coincidence Pair Cut
const Double_t PROMPT_ENERGY_MIN = 0.9, PROMPT_ENERGY_MAX = 8.0;//MeV
const Double_t DELAYED_ENERGY_MIN = 1.85, DELAYED_ENERGY_MAX = 2.5;//MeV
const Double_t RADIUS_MAX = 5700.0;//mm
const Double_t DELTA_T_MIN = 400, DELTA_T_MAX = 2.0e6;//ns
const Double_t DELTA_R_MAX = 2500.0;//mm
const ULong64_t DATA_CLEANING_MASK = 0x2100000042C2;

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

bool PromptCut(const Double_t Energy, const TVector3 Position)
{
    if(Energy < PROMPT_ENERGY_MIN || Energy > PROMPT_ENERGY_MAX){return false;};
    if(Position.Mag() > RADIUS_MAX){return false;};
    return true;
};

bool DelayedCut(const Double_t Energy, const TVector3 Positon)
{
    if(Energy < DELAYED_ENERGY_MIN || Energy > DELAYED_ENERGY_MAX){return false;};
    if(Positon.Mag() > RADIUS_MAX){return false;};
    return true;
};

// int AC_CP_Cut()
int main(int argc, char** argv)
{
    // std::string Prompt_Files = "/home/shuaioy/projects/def-ckrauss/shuaioy/work/Geo/Ingrida/Code/ExperimentData/300050/Accidental/LooseCut_I/OUTPUT/prompt/Gold_20R_AC_LooseCut_0000300000_s005_prompt.root";
    // std::string Delayed_Files = "/home/shuaioy/projects/def-ckrauss/shuaioy/work/Geo/Ingrida/Code/ExperimentData/300050/Accidental/LooseCut_I/OUTPUT/delayed/Gold_20R_AC_LooseCut_0000300000_s005_delayed.root";
    // std::string OutFile_Prompt = "./test_prompt.root";
    // std::string OutFile_Delayed = "./test_delayed.root";

    std::string Prompt_Files = argv[1];
    std::string Delayed_Files = argv[2];
    std::string OutFile_Prompt = argv[3];
    std::string OutFile_Delayed = argv[4];
    
    std::cout << "Prompt Files:" << Prompt_Files << std::endl;
    std::cout << "Delayed Files:" << Delayed_Files << std::endl;
    std::cout << "OutPut Prompt:" << OutFile_Prompt << std::endl;
    std::cout << "OutPut Delayed:" << OutFile_Delayed << std::endl;
//Read Files
    TChain *chain_prompt = new TChain("output");
    TChain *chain_delayed = new TChain("output");
    chain_prompt->Add(Prompt_Files.c_str());
    std::cout << "Prompt文件加载成功" << std::endl;
    chain_delayed->Add(Delayed_Files.c_str());
    std::cout << "Delayed文件加载成功" << std::endl;
//Set Branch Address
    UInt_t run_id, subrun_id, date, run_type;
    chain_prompt->SetBranchAddress("RunID", &run_id);
    chain_prompt->SetBranchAddress("SubRunID", &subrun_id);
    chain_prompt->SetBranchAddress("Date", &date);
    chain_prompt->SetBranchAddress("RunType", &run_type);

    chain_delayed->SetBranchAddress("RunID", &run_id);
    chain_delayed->SetBranchAddress("SubRunID", &subrun_id);
    chain_delayed->SetBranchAddress("Date", &date);
    chain_delayed->SetBranchAddress("RunType", &run_type);
    Double_t z_offset;
    chain_prompt->SetBranchAddress("ZOffSet", &z_offset);

    chain_delayed->SetBranchAddress("ZOffSet", &z_offset);
    ULong64_t begin_count, end_count;
    chain_prompt->SetBranchAddress("BeginCount", &begin_count);
    chain_prompt->SetBranchAddress("EndCount", &end_count);

    chain_delayed->SetBranchAddress("BeginCount", &begin_count);
    chain_delayed->SetBranchAddress("EndCount", &end_count);    
    Int_t entry, ev, gtid, owlhits, fitvalid, nhits, nhits_cleaned, nhits_corrected;
    chain_prompt->SetBranchAddress("PromptEntry", &entry);
    chain_prompt->SetBranchAddress("PromptEv", &ev);
    chain_prompt->SetBranchAddress("PromptGTID", &gtid);
    chain_prompt->SetBranchAddress("PromptOWLHits", &owlhits);
    chain_prompt->SetBranchAddress("PromptFitValid", &fitvalid);
    chain_prompt->SetBranchAddress("PromptNHits", &nhits);
    chain_prompt->SetBranchAddress("PromptNHitsCleaned", &nhits_cleaned);
    chain_prompt->SetBranchAddress("PromptNHitsCorrected", &nhits_corrected);

    chain_delayed->SetBranchAddress("DelayedEntry", &entry);
    chain_delayed->SetBranchAddress("DelayedEv", &ev);
    chain_delayed->SetBranchAddress("DelayedGTID", &gtid);
    chain_delayed->SetBranchAddress("DelayedOWLHits", &owlhits);
    chain_delayed->SetBranchAddress("DelayedFitValid", &fitvalid);
    chain_delayed->SetBranchAddress("DelayedNHits", &nhits);
    chain_delayed->SetBranchAddress("DelayedNHitsCleaned", &nhits_cleaned);
    chain_delayed->SetBranchAddress("DelayedNHitsCorrected", &nhits_corrected);
    ULong64_t clock50, data_applied, data_flags;
    chain_prompt->SetBranchAddress("Prompt50MHz", &clock50);
    chain_prompt->SetBranchAddress("PromptDataApplied", &data_applied);
    chain_prompt->SetBranchAddress("PromptDataFlags", &data_flags);

    chain_delayed->SetBranchAddress("Delayed50MHz", &clock50);
    chain_delayed->SetBranchAddress("DelayedDataApplied", &data_applied);
    chain_delayed->SetBranchAddress("DelayedDataFlags", &data_flags);
    Double_t energy, pos_x, pos_y, pos_z, original_energy, tony_energy;
    chain_prompt->SetBranchAddress("PromptEnergy", &energy);
    chain_prompt->SetBranchAddress("PromptPosX", &pos_x);
    chain_prompt->SetBranchAddress("PromptPosY", &pos_y);
    chain_prompt->SetBranchAddress("PromptPosZ", &pos_z);
    chain_prompt->SetBranchAddress("PromptOriginalEnergy", &original_energy);
    chain_prompt->SetBranchAddress("PromptTonyEnergy", &tony_energy);

    chain_delayed->SetBranchAddress("DelayedEnergy", &energy);
    chain_delayed->SetBranchAddress("DelayedPosX", &pos_x);
    chain_delayed->SetBranchAddress("DelayedPosY", &pos_y);
    chain_delayed->SetBranchAddress("DelayedPosZ", &pos_z);
    chain_delayed->SetBranchAddress("DelayedOriginalEnergy", &original_energy);
    chain_delayed->SetBranchAddress("DelayedTonyEnergy", &tony_energy);
//Creat OutPut File
    myresult res;
    res.ClearAll();
    TFile *outfile_prompt = new TFile(OutFile_Prompt.c_str(), "recreate");
    TTree *outtree_prompt;
    if( chain_prompt->GetEntries() != 0)
    {
        outtree_prompt= chain_prompt->CloneTree(0);
    }
    else
    {
        outtree_prompt = new TTree("output", "");
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
    };
    std::cout << "Prompt输出文件创建成功" << std::endl;
    TFile *outfile_delayed = new TFile(OutFile_Delayed.c_str(), "recreate");
    TTree *outtree_delayed;
    if( chain_delayed->GetEntries() != 0)
    {
        outtree_delayed= chain_delayed->CloneTree(0);
    }
    else
    {
        outtree_delayed = new TTree("output", "");
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
    };
    std::cout << "Delayed输出文件创建成功" << std::endl;
//Loop of Prompt
    std::cout << "开始循环" << std::endl;
    TVector3 Pos;
    int len_prompt = chain_prompt->GetEntries();
    if(len_prompt == 0)
    {
        std::cout << "[Prompt Loop] Prompt File is empty. Fill Zero." << std::endl;
        outtree_prompt->Fill();
    }
    else
    {
        for(int ii1 = 0; ii1 < chain_prompt->GetEntries(); ii1++)
        {
            if(ii1 % 10000 == 0){std::cout << "[Prompt Loop] 已处理" << double(100.0 * ii1/len_prompt) << "%，具体：" << ii1 << "，还剩" << len_prompt - ii1 << std::endl;};
            chain_prompt->GetEntry(ii1);
            if(fitvalid != 1){continue;};
            Pos.SetXYZ(pos_x, pos_y, pos_z);
            if(PromptCut(energy, Pos) == true)
            {
                outtree_prompt->Fill();
            };
        };
    };

//Loop of Delayed
    int len_delayed = chain_delayed->GetEntries();
    if(len_delayed == 0)
    {
        std::cout << "[Delayed Loop] Delayed File is empty. Fill Zero." << std::endl;
        outtree_delayed->Fill();
    }
    else
    {
        for(int ii1 = 0; ii1 < chain_prompt->GetEntries(); ii1++)
        {
            if(ii1 % 10000 == 0){std::cout << "[Delayed Loop] 已处理" << double(100.0 * ii1/len_delayed) << "%，具体：" << ii1 << "，还剩" << len_delayed - ii1 << std::endl;};
            chain_delayed->GetEntry(ii1);
            if(fitvalid != 1){continue;};
            Pos.SetXYZ(pos_x, pos_y, pos_z);
            if(DelayedCut(energy, Pos) == true)
            {
                outtree_delayed->Fill();
            };
        };
    };
//Close the Files
    outfile_prompt->Write();
    outfile_prompt->Close();
    outfile_delayed->Write();
    outfile_delayed->Close();
    std::cout << "Complete" << std::endl;


    return 0;
}
