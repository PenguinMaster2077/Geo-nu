//CPP
#include <iostream>
#include <fstream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
#include <RAT/DU/DSReader.hh>
// #include <RAT/DS/Run.hh>
// #include <RAT/DS/Entry.hh>
// #include <RAT/DS/EV.hh>
// #include <RAT/DS/DataQCFlags.hh>
// #include <RAT/DS/BitMask.hh>
// #include <RAT/DS/FitResult.hh>
// #include <RAT/DS/FitVertex.hh>
// #include <RAT/DB.hh>
// #include <RAT/DU/Utility.hh>
// #include <RAT/DU/ReconCalibrator.hh>
// #include <RAT/DU/DetectorStateCorrection.hh>

int test()
{
    // std::string InFile = "/home/shuaioy/scratch/Fisher/Reactor/20240331/ROOT/Reactor_Fisher_Classifier.306499.root";
    
    std::string InFile = "/home/shuaioy/scratch/Geo/Muon/ROOT/6.root";

    // TFile *outfile = new TFile("./test.root", "recreate");
    // TTree *outtree = new TTree("output", "");

    ULong64_t daApplied, daFlags;
    // outtree->Branch("daApplied", &daApplied, "daApplied/L");
    // outtree->Branch("daFlags", &daFlags, "daFlags/L");

    RAT::DU::DSReader dsreader(InFile);

    for(unsigned int iEntry = 0; iEntry < dsreader.GetEntryCount(); iEntry++)
    {
        RAT::DS::Entry entry = dsreader.GetEntry(iEntry);
        for(unsigned int iEv = 0; iEv < entry.GetEVCount(); iEv++)
        {
            RAT::DS::EV ev = entry.GetEV(iEv);
            std::cout << "Entry:" << iEntry << ",EV:" << iEv << ",NHits:" << ev.GetNhits() << ",OWLCount:" << ev.GetUncalPMTs().GetOWLCount() << std::endl;
            std::string FitName = ev.GetDefaultFitName();
            if( FitName == "") {continue;};
            RAT::DS::FitResult fit_res = ev.GetFitResult(FitName);
            RAT::DS::FitVertex fit_ver = fit_res.GetVertex(0);
            std::cout << "FitValid:" << fit_ver.GetValid() << ",Pos Valid:" << fit_ver.ValidPosition() << ",Energy Valid:" << fit_ver.ValidEnergy() << ",Time Valid:" << fit_ver.ValidTime() << std::endl;
            // outtree->Fill();
        };
    };
    
    // outfile->Write();
    // outfile->Close();

    return 0;
};
