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
#include "../HeadFile/Fit_Parameters.hh"
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
    Fit_Par->Compute_Events(114.09, 300733, 307612);
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
    //Fit_Par->Show_All_Parameters();

//Test--Get_All_Models()
    //fitter->Draw_All();
//Test--Show_Results()
    fitter->Show_Results();


    return 0;
}
