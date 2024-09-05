//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "./HeadFile/Grid_Scan.hh"
#include "./HeadFile/Constant_Setting.hh"

int Grid_Scan()
{
    // Scan_U_Th_Ratio(0.1, 50, 100);
    // Scan_Reactor_Number(20, 40, 20, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD);
    // Scan_Geonu_Number(0, 30, 20, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD);
    std::vector<Double_t> Geo_Range, Reactor_Range;
    Geo_Range.push_back(0);
    Geo_Range.push_back(40);
    Reactor_Range.push_back(0);
    Reactor_Range.push_back(60);
    std::vector<Int_t> Steps;
    Steps.push_back(10);
    Steps.push_back(10);
    Scan_Geonu_Reactor(Geo_Range, Reactor_Range, Steps, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD);
    return 0;
}