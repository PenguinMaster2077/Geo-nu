#ifndef CONSTANT_SETTING_HH
#define CONSTANT_SETTING_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined

//Reactor
const Double_t REACTOR_EVENTS_RATE = 98;//Detected rector events in Full AV per year
const Double_t REACTOR_EVENTS_RELATIVE_ERROR = 0.03; //Unit: 1
const Double_t REACTOR_SELECTION_EFFIENCY = 0.78; //Unit: 1

//Geonu
const Double_t GEO_EVENTS_RATE = 26;
const Double_t GEO_EVENTS_RELATIVE_ERROR = 1;
const Double_t GEO_SELECTION_EFFICIENCY = 0.72;

const Double_t GEO_RATIO = 3.7;
const Double_t GEO_RATIO_RELATIVE_ERROR = 0.35;

const Double_t U_EVENTS_RATE = 19.6897;//Detected rector events in Full AV per year
const Double_t U_EVENTS_RELATIVE_ERROR = 1; //Unit: 1
const Double_t U_SELECTION_EFFICIENCY = 0.72;  //Unit: 1

const Double_t TH_EVENTS_RATE = 5.49776;//Detected rector events in Full AV per year
const Double_t TH_EVENTS_RELATIVE_ERROR = 1; //Unit: 1
const Double_t TH_SELECTION_EFFICIENCY = 0.72; //Unit: 1

const Double_t Rate_TNU = 1/(0.54 * GEO_SELECTION_EFFICIENCY); //Unit: TNU/Rate

//AlphaN
const Double_t AN_EVENTS_RATE = 76.65;//Detected rector events in Full AV per year
const Double_t AN_EVENTS_RATE_RELATIVE_ERROR = 0.095; //Unit: 1 It hasn't been considerated in computation
const Double_t AN_SELECTION_EFFICIENCY = 0.67; //Unit: 1
    //Proton Recoil
    const Double_t PROTON_RECOIL_FRACTION = 0.871929;
    const Double_t PROTON_RECOIL_RELATIVE_ERROR = 0.3; //Unit: 1
    //C12
    const Double_t C12_FRACTION = 0.0279653;
    const Double_t C12_RELATIVE_ERROR = 0.3; //Unit: 1
    //O16
    const Double_t O16_FRACTION = 0.100106;
    const Double_t O16_RELATIVE_ERROR = 1.0; //Unit: 1
const Double_t AN_EVENTS_RELATIVE_ERROR = sqrt( pow(PROTON_RECOIL_FRACTION * PROTON_RECOIL_RELATIVE_ERROR, 2) + pow(C12_FRACTION * C12_RELATIVE_ERROR, 2) + pow(O16_FRACTION * O16_RELATIVE_ERROR, 2));

//Accidental
const Int_t TIME_WINDOWS = 2e6;//ns
const Int_t NUMBER_GENERATED = 1e4;
//Energy
//Z-Shift
const Double_t Z_OFFSET = 186;//mm; Only use it in the first method of generating PDFs

//IBD Cut
const Double_t PROMPT_ENERGY_MIN = 0.9, PROMPT_ENERGY_MAX = 8.0;//MeV
const Double_t DELAYED_ENERGY_MIN = 1.85, DELAYED_ENERGY_MAX = 2.5;//MeV
const Double_t RADIUS_MAX = 5700.0;//mm
const Double_t DELTA_T_MIN = 400, DELTA_T_MAX = 2.0e6;//ns
const Double_t DELTA_R_MAX = 2500.0;//mm
const ULong64_t DATA_CLEANING_MASK = 0x2100000042C2;

//50MHz Clock
const Long64_t MAX_NUMBER = 8796093022207;//2^43 - 1; DocDB-481
const Long64_t MIN_CARE_NUMBER = 8596093022207;//Use it in GeneratingPDFs.hh

//Large NHits Cut
const Int_t MIN_NHITS = 3000;

//OWL Hits Cut
const Int_t MIN_OWL = 3;

//BiPo Cut
const Double_t PROMPT_BIPO_ENERGY_MIN = 1.2, PROMPT_BIPO_ENERGY_MAX = 3.5;//MeV
const Double_t DELAYED_BIPO_ENERGY_MIN = 0.6, DELAYED_BIPO_ENERGY_MAX = 1.1;//MeV
const Double_t RADIUS_BIPO_MAX = 6000.0;//mm
const Double_t DELTA_T_BIPO_MIN = 2e3, DELTA_T_BIPO_MAX = 1000e3;//ns
const Double_t DELTA_R_BIPO_MAX = 800;//mm

//PDFs
const std::string PDF_PWD = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Hists/";
const std::string PDF_REACTOR = PDF_PWD + "Reactor_RawData_CP_PDF_300000-307612.root";
const std::string PDF_GEO_U = PDF_PWD + "U_RawData_CP_PDF_300000-307612.root";
const std::string PDF_GEO_TH = PDF_PWD + "Th_RawData_CP_PDF_300000-307612.root";
const std::string PDF_AN = PDF_PWD + "AN_RawData_CP_PDF_300000-307612.root";

//Data for PDFs
const std::string PDF_DATA_PWD = "/rat/MyCode/Work/Geo-nu-Data/PDFs/Method_RawData/Data/";
const std::string PDF_DATA_REACTOR = PDF_DATA_PWD + "Reactor_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
const std::string PDF_DATA_GEO_U = PDF_DATA_PWD + "U_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
const std::string PDF_DATA_GEO_TH = PDF_DATA_PWD + "Th_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";
const std::string PDF_DATA_AN = PDF_DATA_PWD + "AN_RawData_PairCut_Energy_DeltaT_DeltaR_300000-307612.root";

//Name in FitParameters
const std::string NAME_REACTOR = "Reactor";
const std::string NAME_GEO = "Geo";
const std::string NAME_GEO_RATIO = "Geo_Ratio";
const std::string NAME_GEO_U = "Geo_U";
const std::string NAME_GEO_TH = "Geo_Th";
const std::string NAME_AN_GROUND = "AN_Ground";
const std::string NAME_AN_EXICTED = "AN_Exicted";

//Names in Fitter
const std::string NAME_FITTER_REACTOR = "Model_Reactor";
const std::string NAME_FITTER_GEO = "Model_Geo";
const std::string NAME_FITTER_GEO_U = "Model_Geo_U";
const std::string NAME_FITTER_GEO_TH = "Model_Geo_Th";
const std::string NAME_FITTER_AN = "Model_AN";
const std::string NAME_FITTER_AN_GROUND = "Model_AN_Ground";
const std::string NAME_FITTER_AN_EXICTED = "Model_AN_Exicted";
const std::string NAME_FITTER_DATA = "Data";

const std::string FITTER_EXTENDED_MAXIMUM_LIKELIHOOD = "Extended_Maxmium_Likelihood";
const std::string FITTER_CHI_SQUARE = "Chi_Square";
const std::string FITTER_DATA_HIST = "Data_Hist";

//Data File
const std::string DATA_IBD_CANDIDATE = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/Data/20240529/Gold_20R_0000300000-0000309277.root";
const std::string MUON_FILE = "/rat/MyCode/Work/Geo-nu-Data/Muon/Gold_20R_Muon_Abstract_0000300000-0000309277.root";
//AN Setting
//Energy Region Setting
const Double_t AN_ENERGY_PROTON_RECOIL = 3.4;//MeV
const Double_t AN_ENERGY_C12 = 5.1;//MeV

#endif