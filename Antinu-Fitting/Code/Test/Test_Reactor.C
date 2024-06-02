//CPP
#include <iostream>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined
#include "../HeadFile/Models/Reactor.hh"

int Test_Reactor()
{
    //Test--Get_Global_Point()
    Reactor *reactor = Reactor::Get_Global_Point();
    reactor->Open_Test_Mode();
    // reactor->Close_Test_Mode();

    //Test--Initialize(), Get_Default_PDFs()
    reactor->Initialize();

    //Test--Get_Default_PDFs()
    // reactor->Get_Default_PDFs();

    //Test--Setup_PDFs()
    // TH1D *example_hist = new TH1D("123", "", 10, 1, 2);
    // reactor->Setup_PDFs(example_hist);
    return 0;
}