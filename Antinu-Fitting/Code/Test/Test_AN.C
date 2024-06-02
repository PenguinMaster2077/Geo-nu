//CPP
#include <iostream>
//ROOT
#include <TH1D.h>
//RAT
//Self-Defined
#include "../HeadFile/Models/AN.hh"

int Test_AN()
{
    //Test--Get_Global_Point()
    AN* an = AN::Get_Global_Point();
    
    an->Open_Test_Mode();
    // an->Close_Test_Mode();
    //Test--Initialize(), Get_Default_PDFs()
    an->Initialize();

    //Test--Setup_PDFs()
    TH1D *example_hist = new TH1D("123", "", 100, 0.5, 10);
    an->Setup_PDFs(example_hist);
    return 0;
}