//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
//RAT
//Self-Defined
#include "./HeadFile/Result.hh"
#include "./HeadFile/Constant_Setting.hh"
#include "./HeadFile/CoincidencePairs.hh"

int CoincidencePairs()
{
    std::string LooseCut_Files = "/rat/MyCode/Work/Geo-nu-Data/LooseCut/300000-307612";
    std::string Muon_File = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000307612.root";
    std::string Out_File = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Data/20240529/Gold_20R_0000300000-0000306498.root";
    Find_Coincidence_Pairs(Out_File, LooseCut_Files, Muon_File);
    Plot_IBD_Candidate(Out_File, "./Pic");
    return 0;
}