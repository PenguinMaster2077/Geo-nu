#ifndef CONSTANT_SETTING_HH
#define CONSTANT_SETTING_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined

//Reactor
const Double_t REACTOR_EVENTS = 0;//Detected rector events
const Double_t REACTOR_EVENTS_RELATIVE_ERROR = 0; //Unit: 1
const Double_t REACTOR_ERROR = REACTOR_EVENTS * REACTOR_EVENTS_RELATIVE_ERROR;
const Double_t REACTOR_SELECTION_EFFIENCY = 0; //Unit: 1
//Geonu
const Double_t U_EVENTS = 0;
const Double_t U_EVENTS_RELATIVE_ERROR = 0; //Unit: 1
const Double_t U_EVENTS_ERROR = U_EVENTS * U_EVENTS_RELATIVE_ERROR;
const Double_t U_SELECTION_EFFICIENCY = 0;  //Unit: 1

const Double_t TH_EVENTS = 0;
const Double_t TH_EVENTS_RELATIVE_ERROR = 0; //Unit: 1
const Double_t TH_EVENTS_ERROR = TH_EVENTS * TH_EVENTS_RELATIVE_ERROR;
const Double_t TH_SELECTION_EFFICIENCY = 0; //Unit: 1
//AlphaN
const Double_t AN_EVENTS = 0;
const Double_t AN_EVENTS_RELATIVE_ERROR = 0; //Unit: 1
const Double_t AN_EVENTS_ERROR = AN_EVENTS * AN_EVENTS_RELATIVE_ERROR;
//Accidental
//Energy
//Z-Shift
const Double_t Z_OFFSET = 186;//mm; Only use it in the first method of generating PDFs
//Coincidence Pair Cut
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

#endif