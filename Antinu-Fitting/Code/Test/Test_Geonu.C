//CPP
#include <iostream>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined
#include "../HeadFile/Models/Geonu.hh"

int Test_Geonu()
{
    //Test--Get_Global_Point()
    Geonu *geo = Geonu::Get_Global_Point();
    geo->Open_Test_Mode();
    // geo->Close_Test_Mode();

    //Test--Initialize(), Compute_Survival_Probability(), Show_Oscillation_Parameters()
    geo->Initialize(0.5, 0.3);

    //Test--Get_Default_PDFs()
    geo->Get_Default_PDFs();

    //Test--SetUp_PDFs()
    TH1D *example_hist = new TH1D("123", "", 10, 1, 2);
    geo->Setup_PDFs(example_hist);

    
    return 0;
}