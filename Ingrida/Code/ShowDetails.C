//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TVector3.h>
//RAT
#include <RAT/DU/DSReader.hh>
// #include <RAT/DU/Utility.hh>
// #include <RAT/DU/ReconCalibrator.hh>
// #include <RAT/DU/DetectorStateCorrection.hh>

void ShowLargeNHits(TFile &infile)
{
    TTree *intree = (TTree*) infile.Get("output");

    intree->Scan("mcIndex:eventID:energy:owlnhits:nhits:fitValid:posx:posy:posz", "nhits >= 2500");
};

void ShowItem(TFile &infile, std::string Selecction)
{
    TTree *intree = (TTree*) infile.Get("output");

    intree->Scan("mcIndex:eventID:energy:owlnhits:nhits:fitValid:posx:posy:posz", Selecction.c_str());
};

RAT::DU::ReconCalibrator GiveReconCalibrator(RAT::DU::DSReader &dsreader)
{
    RAT::DU::ReconCalibrator recon_cali = RAT::DU::Utility::Get()->GetReconCalibrator();
    return recon_cali;
};

RAT::DU::DetectorStateCorrection GiveDetectorStateCorrection(RAT::DU::DSReader &dsreader)
{
    RAT::DU::DetectorStateCorrection detector = RAT::DU::Utility::Get()->GetDetectorStateCorrection();
    return detector;
};

Double_t GiveTonyEnergy(Double_t Original_Energy, TVector3 Position, RAT::DU::ReconCalibrator &recon_cali, 
RAT::DU::DetectorStateCorrection &detector_state_correct)
{
    Double_t Tony_Energy = recon_cali.CalibrateEnergyRTF(true, Original_Energy, Position.Perp(), Position.Z());
    return Tony_Energy;
};

Double_t GiveCorrectedEnergy(Double_t Original_Energy, TVector3 Position,
RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct)
{
//Logan's Energy Correction----DocDB: 7730
//Detector Status Correction will be integrated in reconstruction later, thus it should be firstly applied
    RAT::DU::Point3D tempPoint3D(0, Position);
    Double_t Energy = Original_Energy * detector_state_correct.GetCorrection(9394, 0.75058) / detector_state_correct.GetCorrectionPos(tempPoint3D, 0, 0);
//Tony's Energy Correction-----DocDB: 7895
    Energy = recon_cali.CalibrateEnergyRTF(true, Energy, Position.Perp(), Position.Z());
    return Energy;
};

void ShowItemDetails(TFile &infile, int Entry)
{
    TTree *intree = (TTree*) infile.Get("output");
    Int_t ntuple_mcIndex, ntuple_evIndex, ntuple_eventID;
    intree->SetBranchAddress("mcIndex", &ntuple_mcIndex);
    intree->SetBranchAddress("evIndex", &ntuple_evIndex);
    intree->SetBranchAddress("eventID", &ntuple_eventID);
    Int_t ntuple_NHits, ntuple_NHits_Cleaned, ntuple_OWLNHits;
    intree->SetBranchAddress("nhits", &ntuple_NHits);
    intree->SetBranchAddress("nhitsCleaned", &ntuple_NHits_Cleaned);
    intree->SetBranchAddress("owlnhits", &ntuple_OWLNHits);
    ULong64_t ntuple_Applied, ntuple_Flagged;
    intree->SetBranchAddress("dcApplied", &ntuple_Applied);
    intree->SetBranchAddress("dcFlagged", &ntuple_Flagged);
    ULong64_t ntuple_Clock50MHz;
    intree->SetBranchAddress("clockCount50", &ntuple_Clock50MHz);
    Bool_t ntuple_fitValid;
    intree->SetBranchAddress("fitValid", &ntuple_fitValid);
    Double_t ntuple_posx, ntuple_posy, ntuple_posz;
    intree->SetBranchAddress("posx", &ntuple_posx);
    intree->SetBranchAddress("posy", &ntuple_posy);
    intree->SetBranchAddress("posz", &ntuple_posz);
    Double_t ntuple_energy;
    intree->SetBranchAddress("energy", &ntuple_energy);
    intree->GetEntry(Entry);
    TVector3 Pos(ntuple_posx, ntuple_posy, ntuple_posz);
    std::cout << "Show Details of " << Entry << std::endl;
    std::cout << "mcIndex:" << ntuple_mcIndex << ", evIndex:" << ntuple_evIndex << ", eventID:" << ntuple_eventID << std::endl;
    std::cout << "NHits:" << ntuple_NHits << ", NHits Cleaned:" << ntuple_NHits_Cleaned << ", OWL:" << ntuple_OWLNHits << std::endl;
    std::cout << "FitValid:" << ntuple_fitValid << ", Applied:" << ntuple_Applied << ", Flagged:" << ntuple_Flagged << std::endl;
    std::cout << "50MHz Clock:" << ntuple_Clock50MHz << std::endl;
    std::cout << "Not Z Shift and No Energy Correction!!!!!" << std::endl;
    std::cout << "Energy:" << ntuple_energy << ", X:" << ntuple_posx << ", Y:" << ntuple_posy << ", Z:" << ntuple_posz << ", R:" << Pos.Mag() << std::endl;
};

void ShowItemDetails(RAT::DU::DSReader &dsreader, TFile &infile, int Entry, RAT::DU::ReconCalibrator &recon_cali, RAT::DU::DetectorStateCorrection &detector_state_correct, Double_t ZOffSet = 186.0)
{
    TTree *intree = (TTree*) infile.Get("output");
    Int_t ntuple_mcIndex, ntuple_evIndex, ntuple_eventID;
    intree->SetBranchAddress("mcIndex", &ntuple_mcIndex);
    intree->SetBranchAddress("evIndex", &ntuple_evIndex);
    intree->SetBranchAddress("eventID", &ntuple_eventID);
    Int_t ntuple_NHits, ntuple_NHits_Cleaned, ntuple_OWLNHits;
    intree->SetBranchAddress("nhits", &ntuple_NHits);
    intree->SetBranchAddress("nhitsCleaned", &ntuple_NHits_Cleaned);
    intree->SetBranchAddress("owlnhits", &ntuple_OWLNHits);
    ULong64_t ntuple_Applied, ntuple_Flagged;
    intree->SetBranchAddress("dcApplied", &ntuple_Applied);
    intree->SetBranchAddress("dcFlagged", &ntuple_Flagged);
    ULong64_t ntuple_Clock50MHz;
    intree->SetBranchAddress("clockCount50", &ntuple_Clock50MHz);
    Bool_t ntuple_fitValid;
    intree->SetBranchAddress("fitValid", &ntuple_fitValid);
    Double_t ntuple_posx, ntuple_posy, ntuple_posz;
    intree->SetBranchAddress("posx", &ntuple_posx);
    intree->SetBranchAddress("posy", &ntuple_posy);
    intree->SetBranchAddress("posz", &ntuple_posz);
    Double_t ntuple_energy;
    intree->SetBranchAddress("energy", &ntuple_energy);
    intree->GetEntry(Entry);
    TVector3 Pos = TVector3(ntuple_posx, ntuple_posy, ntuple_posz - ZOffSet);
    Double_t Corrected_Energy = GiveCorrectedEnergy(ntuple_energy, Pos, recon_cali, detector_state_correct);
    Double_t Tony_Energy = GiveTonyEnergy(ntuple_energy, Pos, recon_cali, detector_state_correct);
    std::cout << "Show Details of " << Entry << std::endl;
    std::cout << "mcIndex:" << ntuple_mcIndex << ", evIndex:" << ntuple_evIndex << ", eventID:" << ntuple_eventID << std::endl;
    std::cout << "NHits:" << ntuple_NHits << ", NHits Cleaned:" << ntuple_NHits_Cleaned << ", OWL:" << ntuple_OWLNHits << std::endl;
    std::cout << "FitValid:" << ntuple_fitValid << ", Applied:" << ntuple_Applied << ", Flagged:" << ntuple_Flagged << std::endl;
    std::cout << "50MHz Clock:" << ntuple_Clock50MHz << std::endl;
    std::cout << "ZOffSet:" << ZOffSet << std::endl;
    std::cout << "Corrected Energy:" << Corrected_Energy << ", X:" << ntuple_posx << ", Y:" << ntuple_posy << ", Z:" << ntuple_posz << ", R:" << Pos.Mag() << std::endl;
    std::cout << "Original Energy:" << ntuple_energy << ", Tony Energy:" << Tony_Energy << std::endl;
};

void ShowDetails(RAT::DU::DSReader &dsreader, TFile &infile, int Entry,
RAT::DU::ReconCalibrator recon_cali, RAT::DU::DetectorStateCorrection detector_state_correct,
Double_t ZOffset = 186.00)
{
    Double_t original_energy, Tony_energy, Energy;
    //Set Branch Address
    TTree *intree = (TTree*) infile.Get("output");
    intree->SetBranchAddress("energy", &original_energy);
    Double_t posx, posy, posz;
    intree->SetBranchAddress("posx", &posx);
    intree->SetBranchAddress("posy", &posy);
    intree->SetBranchAddress("posz", &posz);
    ULong64_t Clock50, Flags;
    intree->SetBranchAddress("clockCount50", &Clock50);
    intree->SetBranchAddress("dcFlagged", &Flags);
    //Energy Correction
    // RAT::DU::Utility *utility;
    // RAT::DU::ReconCalibrator recon_cali = utility->GetReconCalibrator();
    // RAT::DU::DetectorStateCorrection detector_state_correct = utility->GetDetectorStateCorrection();
    //Begin Energy Correction
    intree->GetEntry(Entry);
    TVector3 Position(posx, posy, posz - ZOffset);

    Tony_energy = GiveTonyEnergy(original_energy, Position, recon_cali, detector_state_correct);

    Energy = GiveCorrectedEnergy(original_energy, Position, recon_cali, detector_state_correct);
    //Show Information
    bool clean = (Flags & 0x2100000042C2) == 0x2100000042C2;
    std::cout << "Position, X:" << Position.X() << ",Y:" << Position.Y() << ",Z:" << Position.Z() << ",R:" << Position.Mag() << std::endl;
    std::cout << "Z Off Set:" << ZOffset << std::endl;
    std::cout << "Energy, Original:" << original_energy << ", Tony Energy:" << Tony_energy << ", Corrected Energy:" << Energy << std::endl;
    std::cout << "Flags:" << Flags << ", Data Clean:" << clean << std::endl;
    std::cout << "Clock:" << Clock50 << std::endl;
};

void ComputeDeltaTR(TFile &infile, int Entry1, int Entry2)
{
    TTree *intree = (TTree*) infile.Get("output");
    Double_t posx, posy, posz;
    intree->SetBranchAddress("posx", &posx);
    intree->SetBranchAddress("posy", &posy);
    intree->SetBranchAddress("posz", &posz);
    ULong64_t Clock50;
    intree->SetBranchAddress("clockCount50", &Clock50);
    //Get Info of Entry 1
    intree->GetEntry(Entry1);
    Long64_t Clock_Entry1 = Clock50;
    TVector3 Position_Entry1(posx, posy, posz);
    //Get Info of Entry 2
    intree->GetEntry(Entry2);
    Long64_t Clock_Entry2 = Clock50;
    Double_t Delta_T = 20.0 * (Clock_Entry2 - Clock_Entry1) / 1e9;//s
    TVector3 Delta_R = TVector3(posx, posy, posz) - Position_Entry1;
    std::cout << "Delta R(mm):" << Delta_R.Mag() << ",Delta T(s):" << Delta_T << std::endl;
};

