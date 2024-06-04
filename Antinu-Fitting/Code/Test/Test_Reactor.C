//CPP
#include <iostream>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined
#include "../HeadFile/Models/Reactor.hh"
#include "../HeadFile/Constant_Setting.hh"

int Test_Reactor()
{
//Test--Get_Global_Point()
    Reactor *reactor = Reactor::Get_Global_Point();
    reactor->Open_Test_Mode();
    // reactor->Close_Test_Mode();

//Test--Setup_PDFs()
    TH1D *example_hist = new TH1D("123", "", 10, 1, 5);
    reactor->Setup_PDFs(example_hist);
//Test--Compute_
    FitParameters *fit_par = FitParameters::Get_Global_Point();
    fit_par->Add_Parameter(NAME_REACTOR, 10, 1, 1);
    reactor->Compute_Predictions();
    return 0;
}