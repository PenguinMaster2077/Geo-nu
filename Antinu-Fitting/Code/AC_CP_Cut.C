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
    TFile *outfile_prompt = new TFile(OutFile_Prompt.c_str(), "recreate");
    TTree *outtree_prompt = chain_prompt->CloneTree(0);
    std::cout << "Prompt输出文件创建成功" << std::endl;
    TFile *outfile_delayed = new TFile(OutFile_Delayed.c_str(), "recreate");
    TTree *outtree_delayed = chain_delayed->CloneTree(0);
    std::cout << "Delayed输出文件创建成功" << std::endl;
//Loop of Prompt
    std::cout << "开始循环" << std::endl;
    TVector3 Pos;
    int len_prompt = chain_prompt->GetEntries();
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

//Loop of Delayed
    int len_delayed = chain_delayed->GetEntries();
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
//Close the Files
    outfile_prompt->Write();
    outfile_prompt->Close();
    outfile_delayed->Write();
    outfile_delayed->Close();
    std::cout << "Complete" << std::endl;


    return 0;
}
