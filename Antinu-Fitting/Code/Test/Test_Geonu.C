//CPP
#include <iostream>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined
#include "../HeadFile/Models/Geonu.hh"
#include "../HeadFile/Fit_Parameters.hh"
#include "../HeadFile/Constant_Setting.hh"

int Test_Geonu()
{
//Test--Get_Global_Point(), Load_Default_PDFs()
    Geonu *geo = Geonu::Get_Global_Point();
    geo->Open_Test_Mode();
    geo->Close_Test_Mode();

//Test--SetUp_PDFs()
    TH1D *example_hist = new TH1D("123", "", 10, 1, 5);
    geo->Setup_PDFs(example_hist);

//Test--Compute_Predictions()
    // FitParameters *fit_par = FitParameters::Get_Global_Point();
    // fit_par->Add_Parameter(NAME_GEO_U, 0, 1, 1);
    // fit_par->Add_Parameter(NAME_GEO_TH, 10, 0.2, 1);
    // geo->Compute_Predictions();
    
    return 0;
}