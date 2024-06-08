//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "../HeadFile/Constant_Setting.hh"
#include "../HeadFile/Models.hh"
#include "../HeadFile/Fitter.hh"

int Test_Fitter()
{
//Example Histogram
    TH1D *Example_Hist = new TH1D("Example", "", 20, 0.8, 8.0);
//Models
    Geonu *geonu = Geonu::Get_Global_Point();
    Reactor *reactor = Reactor::Get_Global_Point();
    AN *an = AN::Get_Global_Point();

    geonu->Setup_PDFs(Example_Hist);
    reactor->Setup_PDFs(Example_Hist);
    an->Setup_PDFs(Example_Hist);
//Add Fit Parameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Fit_Par->Add_Parameter(NAME_GEO_U, 3.40, 3.40, 1);
    Fit_Par->Add_Parameter(NAME_GEO_TH, 0.95, 0.95, 1);
    Fit_Par->Add_Parameter(NAME_REACTOR, 18.32, 0.51, 1);
    Fit_Par->Add_Parameter(NAME_AN_GROUND, 11.08, 3.32, 1);
    Fit_Par->Add_Parameter(NAME_AN_EXICTED, 1.23, 1.23, 1);
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();
    fitter->Initialize();
    fitter->Open_Test_Mode();

//Test--Initialize_Hist()
    fitter->Initialize_Hist(Example_Hist);

//Test--Load_Data()
    fitter->Load_Data(DATA_IBD_CANDIDATE);

//Test--Fitting()
    fitter->Fitting();
    Fit_Par->Show_All_Parameters();

//Test--Get_All_Models()
    fitter->Draw_All();


    return 0;
}
