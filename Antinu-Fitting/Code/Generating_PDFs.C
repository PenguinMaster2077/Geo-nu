//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TChain.h>
//RAT
//Self-Defined
#include "./HeadFile/Constant_Setting.hh"
#include "./HeadFile/Result.hh"
#include "./HeadFile/Generating_PDFs.hh"

int Generating_PDFs()
{
    std::string FilePWD, OutFile_CP, OutFile_Full, OutFile_PDF, OutFile_FullPDF;

    TH1D* Hist_Delta_T = new TH1D("Example_Delta_T", "", 100, 0, 3e3);// \mu s
    TH1D* Hist_Delta_R = new TH1D("Example_Delta_R", "", 100, 0, 12000);// mm
    TH1D* Hist_Prompt_Energy = new TH1D("Example_Prompt_Energy", "", 200, 0, 10.0);// MeV
    TH1D* Hist_Delayed_Energy = new TH1D("Example_Delayed_Energy", "", 200, 1.2, 3.5);// MeV
    std::vector<TH1D*> Example_Hists;
    Example_Hists.push_back(Hist_Delta_T);
    Example_Hists.push_back(Hist_Delta_R);
    Example_Hists.push_back(Hist_Prompt_Energy);
    Example_Hists.push_back(Hist_Delayed_Energy);

//Geo U
    // FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/U/300000-307612";
    // OutFile_CP = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/U_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    // OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/U_RawData_PairCut_Energy_300000-307612.root";
    // OutFile_PDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/U_RawData_CP_PDF_300000-307612.root";
    // OutFile_FullPDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/U_RawData_Full_PDF_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile_CP);
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full);
    // Create_PDFs(OutFile_CP, "U", OutFile_PDF, Example_Hists, false);
    // Create_PDFs(OutFile_Full, "U", OutFile_FullPDF, Example_Hists, true);
//Geo Th
    // FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/Th/300000-307612";
    // OutFile_CP = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/Th_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    // OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/Th_RawData_PairCut_Energy_300000-307612.root";
    // OutFile_PDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/Th_RawData_CP_PDF_300000-307612.root";
    // OutFile_FullPDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/Th_RawData_Full_PDF_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile_CP);
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full);
    // Create_PDFs(OutFile_CP, "Th", OutFile_PDF, Example_Hists, false);
    // Create_PDFs(OutFile_Full, "Th", OutFile_FullPDF, Example_Hists, true);
//AN
    FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/AN/300000-307612";
    OutFile_CP = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/AN_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/AN_RawData_PairCut_Energy_300000-307612.root";
    OutFile_PDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/AN_RawData_CP_PDF_300000-307612.root";
    OutFile_FullPDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/AN_RawData_Full_PDF_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile_CP, "AN");
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full, "AN");
    Create_PDFs(OutFile_CP, "AN", OutFile_PDF, Example_Hists, false);
    Create_PDFs(OutFile_Full, "AN", OutFile_FullPDF, Example_Hists, true);
//Reactor
    // FilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/Reactor/300000-307612";
    // OutFile_CP = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/Reactor_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
    // OutFile_Full = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/Reactor_RawData_PairCut_Energy_300000-307612.root";
    // OutFile_PDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/Reactor_RawData_CP_PDF_300000-307612.root";
    // OutFile_FullPDF = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/Reactor_RawData_Full_PDF_300000-307612.root";
    // Find_Coincidence_Pairs(FilePWD, OutFile_CP, "Reactor");
    // Find_Full_Coincidence_Pairs(FilePWD, OutFile_Full, "Reactor");
    // Create_PDFs(OutFile_CP, "Reactor", OutFile_PDF, Example_Hists, false);
    // Create_PDFs(OutFile_Full, "Reactor", OutFile_FullPDF, Example_Hists, true);
    return 0;
}