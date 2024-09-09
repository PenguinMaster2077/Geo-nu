//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "../HeadFile/Grid_Scan.hh"
#include "../HeadFile/Constant_Setting.hh"

int Grid_Scan_002()
{
	std::vector<Double_t> Geo_Range, Reactor_Range;
	Geo_Range.push_back(2);
	Geo_Range.push_back(2+1);
	Reactor_Range.push_back(10);
	Reactor_Range.push_back(60);
	std::vector<Int_t> Steps;
	Steps.push_back(2);
	Steps.push_back(100);
  Scan_Geonu_Reactor(Geo_Range, Reactor_Range, Steps, FITTER_EXTENDED_MAXIMUM_LIKELIHOOD, false);
 	return 0;
}
