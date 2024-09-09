//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "./HeadFiles/Grid_Scan.hh"
#include "./HeadFiles/Constants_Setting.hh"

int Grid_Scan()
{
    // Scan_U_Th_Ratio(1, 10000, 10);
    Scan_U_Th_Ratio_Detail(10000, 20000, 10, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD, true);
    // Scan_Reactor_Number(20, 40, 20, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD);
    // Scan_Geonu_Number(0, 30, 20, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD);
//2D Scan
    // std::vector<Double_t> Geo_Range, Reactor_Range;
    // Geo_Range.push_back(10);
    // Geo_Range.push_back(20);
    // Reactor_Range.push_back(10);
    // Reactor_Range.push_back(60);
    // std::vector<Int_t> Steps;
    // Steps.push_back(10);
    // Steps.push_back(10);
    // Scan_Geonu_Reactor(Geo_Range, Reactor_Range, Steps, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD, true);
    return 0;
}