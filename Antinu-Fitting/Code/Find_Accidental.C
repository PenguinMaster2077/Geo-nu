//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./HeadFiles/Constants_Setting.hh"
#include "./HeadFiles/Base_Functions.hh"
#include "./HeadFiles/Find_Accidental.hh"

int Find_Accidental()
{
    std::string Prompt_Files = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/RawData/20240530/Prompt";
    std::string Delayed_Files = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/RawData/20240530/Delayed";
    std::string Data_Clean_PWD = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/DataCleaning/20240530";
    std::string Generated_AC_PWD = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/Generated_Events/20240530/Gold_AC_Generated.root";
    std::string Generated_AC_CP_PWD = "/rat/MyCode/Work/Geo-nu-Data/Accidental/Method_I/Generated_Events/20240530/Gold_AC_Generated_CP.root";
    // Data_Cleaning_Cut(OutPWD, Prompt_Files, Delayed_Files);
    // Randomly_Generate_AC_Pairs(Generated_AC_PWD, Data_Clean_PWD);
    Apply_CP_Cut(Generated_AC_CP_PWD, Generated_AC_PWD);
    // Compute_Rate("", OutPWD);
    return 0;
}