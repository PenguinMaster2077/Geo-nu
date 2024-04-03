//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
//Self-Defined
//#include "/home/penguin/Rat/rat-7.0.14-ROOT5/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

#include "/rat/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

int Plot()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Reactor_20240329/Reactor_CoincidencePair.root";
    std::string OutPWD = "/rat/MyCode/Work/Geo-nu/Ingrida/Pic/";

    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");

    //intree->Print();

    TH1D *th1d_1 = new TH1D("h1", "", 88, 0.8, 8.0);
    TH1D *th1d_2 = new TH1D("h2", "", 100, 1.5, 2.5);
    intree->Project("h1", "PromptEnergy");
    intree->Project("h2", "DelayedEnergy");

    SavePlot(OutPWD + "PromptEnergy.png", th1d_1, "Prompt", "E_{Prompt}/MeV", "Counts", 0.7, 0.72, 0.8, 0.82);
    SavePlot(OutPWD + "DelayedEnergy.png", th1d_2, "Delayed", "E_{Delayed}/MeV", "Counts");
    return 0;
}
