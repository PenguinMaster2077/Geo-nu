//CPP
#include <iostream>
#include <fstream>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./HeadFiles/Result.hh"
#include "./HeadFiles/Constants_Setting.hh"
#include "./HeadFiles/Find_Muon.hh"

int Find_Muon()
{
    std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/Muon/20240412/";
    std::string OutFile = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000309277.root";
    Find_Muons(InPWD, OutFile);
    // Find_Muon_Spallation(InPWD, "./test.root");
    return 0;
}