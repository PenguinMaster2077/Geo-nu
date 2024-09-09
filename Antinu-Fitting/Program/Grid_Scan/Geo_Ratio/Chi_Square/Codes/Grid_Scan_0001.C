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

int Grid_Scan_0001()
{

	Scan_U_Th_Ratio_Detail(1 * 100, (1 + 1) * 100 , 100, FITTER_CHI_SQUARE, false);
 	return 0;
}
