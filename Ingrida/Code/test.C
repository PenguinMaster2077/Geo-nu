//CPP
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TString.h>
//RAT
//Self-Defined
// #include "/home/penguin/Rat/rat-7.0.14-ROOT5/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

#include "/rat/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

int test()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/MC/AN/MC_AN_300000-307612.root";

    TFile infile(InFile.c_str());
    TTree* intree = (TTree*) infile.Get("output");
    std::string BranchName = "PromptEnergy";

    TH1D *th1d = new TH1D("test", "test", 1000, 0, 10);//MeV
    intree->Project("test", BranchName.c_str());

    SavePlot("./test.png", th1d, "AN", "E", "test");

    th1d->Scale(1.0/th1d->Integral());
    int startIndex = th1d->FindBin(0.9);
    int endIndex = th1d->FindBin(2.4);
    std::cout << startIndex << "," << endIndex << "," << th1d->Integral(startIndex, endIndex) << std::endl;

    return 0;
};
