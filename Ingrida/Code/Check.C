//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TF1.h>

int Check()
{
    std::string InFile = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Reactor_20240329/Reactor_CoincidencePair.root";

    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");

    //intree->Print();

    Long64_t Prompt50MHz, Delayed50MHz;
    intree->SetBranchAddress("Prompt50MHz", &Prompt50MHz);
    intree->SetBranchAddress("Delayed50MHz", &Delayed50MHz);

    TH1D *Delta_T = new TH1D("Delta T", "", 200, 0, 8e2);

    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        intree->GetEntry(ii1);
        Delta_T->Fill((Delayed50MHz - Prompt50MHz) * 20.0/1e3);
    };

    TF1 *exp_fit = new TF1("exp_fit", "[0]*exp(-x/[1])", 0, 8e2);
    exp_fit -> SetParameters(1,200);//参数初始化
    Delta_T->Fit(exp_fit, "R");

    std::cout << "Lifetime:" << exp_fit->GetParameter(1) << "mu s" << std::endl;

    Delta_T->Draw("SAME");

    return 0;
}
