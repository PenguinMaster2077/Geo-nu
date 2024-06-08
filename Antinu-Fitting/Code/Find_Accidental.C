//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./HeadFile/Constant_Setting.hh"
#include "./HeadFile/Base_Functions.hh"
#include "./HeadFile/Find_Accidental.hh"

int Find_Accidental()
{
    std::string Prompt_Files = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/RawData/20240530/Prompt";
    std::string Delayed_Files = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/RawData/20240530/Delayed";
    std::string OutPWD = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/DataCleaning/20240530";
    // Data_Cleaning_Cut(OutPWD, Prompt_Files, Delayed_Files);
    Compute_Rate("", OutPWD);
    return 0;
}