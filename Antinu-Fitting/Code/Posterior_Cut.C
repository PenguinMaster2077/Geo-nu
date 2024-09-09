//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
//RAT
//Self-Defined
#include "./HeadFiles/Posterior_Cut.hh"

int Posterior_Cut()
{
//Example Hists
    TH1D *Example_TH1D_Delayed = new TH1D("temp_th1d_delayed", "", 35, 1.85, 2.5);
    TH1D *Example_TH1D_Posterior = new TH1D("temp_th1d_posterior", "", 20, -6, 6);
    TH2D *Example_TH2D = new TH2D("temp_th2d", "", 50, 0, 2500, 40, 0, 2000);
//Load Geo PDFs
    geo.Setup_Histogram(Example_TH1D_Delayed, Example_TH1D_Posterior, Example_TH2D);
    //geo.Open_Test_Mode();
    Get_Geo_PDF(geo);
//Load AC PDFs
    AC.Setup_Histogram(Example_TH1D_Delayed, Example_TH1D_Posterior, Example_TH2D);
    //AC.Open_Test_Mode();
    Get_AC_PDF(AC);
//Posterior
    Test_Posterior();
//Test
    //std::cout << geo.DeltaR_DeltaT->GetMinimum(1e-20) << "," << AC.DeltaR_DeltaT->GetMinimum(1e-20) << std::endl;
    return 0;
}