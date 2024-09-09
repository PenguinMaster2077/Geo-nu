#ifndef BASE_FUNCTIONS_HH
#define BASE_FUNCTIONS_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"
//Delta T
Double_t Compute_Delta_T(ULong64_t &Last_50MHz, ULong64_t &Present_50MHz, std::ofstream &logfile)
{
    
    if(Last_50MHz > Present_50MHz && Last_50MHz > MIN_CARE_NUMBER && Present_50MHz < MIN_CARE_NUMBER)
    {
        ULong64_t Part1 = MAX_NUMBER - Last_50MHz + 1;
        ULong64_t ClockGap = (Part1 + Present_50MHz);
        // logfile << "[Compute_Delta_T] 注意Clock反转！！！！！！！Max Clock:" << MAX_NUMBER << ",Last Clock:" << Last_50MHz << ", Present Clock:" << Present_50MHz << ", Delta T(s):" << 20.0 * ClockGap / 1e9 << ",返回 false"<< std::endl;
        return 20.0 * ClockGap;
    }
    else
    {
        ULong64_t ClockGap = (Present_50MHz - Last_50MHz);
        return 20.0 *ClockGap;
    };
};

Double_t Compute_Delta_T(ULong64_t &Last_50MHz, ULong64_t &Present_50MHz)
{
    
    if(Last_50MHz > Present_50MHz && Last_50MHz > MIN_CARE_NUMBER && Present_50MHz < MIN_CARE_NUMBER)
    {
        ULong64_t Part1 = MAX_NUMBER - Last_50MHz + 1;
        ULong64_t ClockGap = (Part1 + Present_50MHz);
        //std::cout << "[Compute_Delta_T] 注意Clock反转！！！！！！！Max Clock:" << MAX_NUMBER << ",Last Clock:" << Last_50MHz << ", Present Clock:" << Present_50MHz << ", Delta T(s):" << 20.0 * ClockGap / 1e9 << ",返回 false"<< std::endl;
        return 20.0 * ClockGap;
    }
    else
    {
        ULong64_t ClockGap = (Present_50MHz - Last_50MHz);
        return 20.0 *ClockGap;
    };
};

//IBD Energy Cut
bool Prompt_Energy_Cut(Double_t energy)
{
    if(energy >= PROMPT_ENERGY_MIN && energy <= PROMPT_ENERGY_MAX){return true;}
    else {return false;}
};

bool Delayed_Energy_Cut(Double_t energy)
{
    if(energy >= DELAYED_ENERGY_MIN && energy <= DELAYED_ENERGY_MAX){return true;}
    else {return false;}
};

bool DeltaT_Cut(Double_t DeltaT)
{
    if(DeltaT >= DELTA_T_MIN && DeltaT <= DELTA_T_MAX){return true;}
    else {return false;};
};

//BiPo Energy Cut
bool Prompt_Energy_BiPo_Cut(Double_t energy)
{
    if(energy >= PROMPT_BIPO_ENERGY_MIN && energy <= PROMPT_BIPO_ENERGY_MAX){return true;}
    else {return false;};
};

bool Delayed_Energy_BiPo_Cut(Double_t energy)
{
    if(energy >= DELAYED_BIPO_ENERGY_MIN && energy <= DELAYED_BIPO_ENERGY_MAX){return true;}
    else {return false;};
}


//Muon Cut
bool PassMuonCut(int &Index_Muon, const int Muon_Len, ULong64_t Muon_50MHz[], Int_t Muon_Run[], Int_t Muon_SubRun[], ULong64_t Prompt_50MHz, Int_t Prompt_Run, Int_t Prompt_SubRun)
{
    Double_t Delta_T = Compute_Delta_T(Muon_50MHz[Index_Muon - 1], Prompt_50MHz);
    std::cout << "[Muon Cut] 前一个Muon检测信息。Run:" << Muon_Run[Index_Muon - 1] << ",SubRun:" << Muon_SubRun[Index_Muon - 1] << ",50MHz:" << Muon_50MHz[Index_Muon - 1] << ",Prompt Run:" << Prompt_Run << ",Prompt SubRun:" << Prompt_SubRun << ",Delta T(s):" << Delta_T/1e9 << std::endl;
    int FindMuon = 0;
    if(Delta_T >= 0 && Delta_T <= 20e9){return false;};
    if(Delta_T > 20e9 || Delta_T < 0)
    {
        for(int ii2 = Index_Muon; ii2 < Muon_Len; ii2++)
        {
            if(Muon_Run[ii2] > Prompt_Run){ break;};
            Delta_T = Compute_Delta_T(Muon_50MHz[ii2], Prompt_50MHz);
            std::cout << "[Muon Cut] Run:" << Muon_Run[ii2] << ",SubRun:" << Muon_SubRun[ii2] << ",50MHz:" << Muon_50MHz[ii2] << ",Prompt Run:" << Prompt_Run  << ",Prompt SubRun:" << Prompt_SubRun << ",Delta T(s):" << Delta_T/1e9 << std::endl;
            if(Delta_T >=0 && Delta_T<=20e9)
            {
                std::cout << "[Muon Cut] 找到对应的Muon。Run:" << Muon_Run[ii2] << ",SubRun:" << Muon_SubRun[ii2] << ",Prompt Run:" << Prompt_Run << ",Prompt SubRun:" << Prompt_SubRun << ",Delta T(s):" << Delta_T/1e9  << std::endl << std::endl;
                FindMuon = 1; 
                break;
            };
        };
    };
    if(FindMuon == 1){return false;}
    else
    {
        std::cout << "[Muon Cut] 并没有找到对应的Muon，通过Muon Cut" << std::endl;
        return true;
    };
};
#endif