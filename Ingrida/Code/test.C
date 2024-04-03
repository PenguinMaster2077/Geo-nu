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
    
    std::string InFile = "/home/shuaioy/scratch/Geo/Gold/300000-306498/ratds/300050/Data/Analysis20R_r0000300026_s008_p000.root";

    TFile *outfile = new TFile("./test.root", "recreate");
    TTree *outtree = new TTree("output", "");

    ULong64_t daApplied, daFlags;
    outtree->Branch("daApplied", &daApplied, "daApplied/L");
    outtree->Branch("daFlags", &daFlags, "daFlags/L");

    RAT::DU::DSReader dsreader(InFile);

    for(unsigned int iEntry = 0; iEntry < 10; iEntry++)
    {
        RAT::DS::Entry entry = dsreader.GetEntry(iEntry);
        for(unsigned int iEv = 0; iEv < entry.GetEVCount(); iEv++)
        {
            RAT::DS::EV ev = entry.GetEV(iEv);
            RAT::DS::DataQCFlags data = ev.GetDataCleaningFlags();
            Int_t fpass = data.GetLatestPass();
            RAT::DS::BitMask bit_applied = data.GetApplied(fpass);
            RAT::DS::BitMask bit_flags = data.GetFlags(fpass);
            ULong64_t applied = bit_applied.GetULong64_t(0);
            ULong64_t flags = bit_flags.GetULong64_t(0);
            if((flags & 0x2100000042C2) == 0x2100000042C2)
            {
            std::cout << "Entry:" << iEntry << ",EV:" << iEv  << ",GTID:" << ev.GetGTID() << ",LastPass:" << fpass
            << ",Applied:" << applied << ",Flags:" << flags << ",After:" << (flags & 0x2100000042C2 )
            << "," << ((flags & 0x2100000042C2) == 0x2100000042C2) << std::endl;
            };
            daApplied = applied;
            daFlags = flags;
            outtree->Fill();
        };
    };
    outfile->Write();
    outfile->Close();

    return 0;
};
