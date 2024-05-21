//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
//RAT
//Self-Defined
#include "./HeadFile/Result.hh"
#include "./HeadFile/Constant_Setting.hh"
#include "./HeadFile/CoincidencePairs.hh"
#include "./HeadFile/GeneratingPDFs.hh"

int CoincidencePairs()
{
    Find_Coincidence_Pairs();
    return 0;
}