//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TChain.h>
//RAT
//Self-Defined
#include "./HeadFile/GeneratingPDFs.hh"
#include "./HeadFile/Result.hh"

int GeneratingPDFs()
{
    std::string FilePWD, OutFile, OutFile_Full;

    FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/U/300000-307612";
    OutFile = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/U_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/U_RawData_PairCut_Energy_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile);
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full);
    GetPDfs(OutFile_Full, "U");
    // FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/Th/300000-307612";
    // OutFile = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Th_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    // OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Th_RawData_PairCut_Energy_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile);
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full);
    // FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/AN/300000-307612";
    // OutFile = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/AN_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    // OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/AN_RawData_PairCut_Energy_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile);
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full);
    // FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/Reactor/300000-307612";
    // OutFile = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Reactor_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    // OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Reactor_RawData_PairCut_Energy_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile);
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full);
    return 0;
}