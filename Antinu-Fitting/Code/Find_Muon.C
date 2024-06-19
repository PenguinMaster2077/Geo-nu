//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./HeadFile/Result.hh"
#include "./HeadFile/Constant_Setting.hh"
#include "./HeadFile/Find_Muon.hh"

int Find_Muon()
{
    std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/Muon/20240412/300000-307612";
    std::string OutFile = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000307612.root";
    Find_Muons(InPWD, OutFile);
    // Find_Muon_Spallation(InPWD, "./test.root");
    return 0;
}