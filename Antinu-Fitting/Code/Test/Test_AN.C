//CPP
#include <iostream>
//ROOT
#include <TH1D.h>
//RAT
//Self-Defined
#include "../HeadFile/Models/AN.hh"
#include "../HeadFile/Constant_Setting.hh"

int Test_AN()
{
//Test--Get_Global_Point()
    AN* an = AN::Get_Global_Point();
    an->Open_Test_Mode();
    // an->Close_Test_Mode();

//Test--Setup_PDFs()
    TH1D *example_hist = new TH1D("123", "", 100, 0, 10);
    an->Setup_PDFs(example_hist);

//Test--Compute_Predictions()
    FitParameters *fit_par = FitParameters::Get_Global_Point();
    fit_par->Add_Parameter(NAME_AN_GROUND, 10, 1, 1);
    fit_par->Add_Parameter(NAME_AN_EXICTED, 10, 0.2, 1);
    an->Compute_Predictions();
    return 0;
}