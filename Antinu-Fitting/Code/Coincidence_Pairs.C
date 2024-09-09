//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
//RAT
//Self-Defined
#include "./HeadFiles/Result.hh"
#include "./HeadFiles/Constants_Setting.hh"
#include "./HeadFiles/Coincidence_Pairs.hh"

int Coincidence_Pairs()
{
    std::string LooseCut_Files = "/rat/MyCode/Work/Geo-nu-Data/LooseCut/Data";
    std::string Muon_File = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000309277.root";
    std::string Out_File = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Data/20240529/Gold_20R_0000300000-0000309277.root";
    Find_Coincidence_Pairs(Out_File, LooseCut_Files, Muon_File);
    Plot_IBD_Candidate(Out_File, "./Pic");
    return 0;
}