//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "./HeadFile/Grid_Scan.hh"

int Grid_Scan()
{
    // Scan_U_Th_Ratio(0.1, 50, 100);
    // Scan_Reactor(20, 50, 100);
    Scan_Geo(0, 30, 100);
    return 0;
}