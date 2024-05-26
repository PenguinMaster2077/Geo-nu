#ifndef GENERATINGPDFS_HH
#define GENERATINGPDFS_HH
//CPP
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1D.h>
#include <TVector3.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"
#include "./Result.hh"
#include "./PlotCodes/SinglePlot.hh"

bool Prompt_Energy_Cut(Double_t energy)
{
    if(energy >= PROMPT_ENERGY_MIN && energy <= PROMPT_ENERGY_MAX){return true;}
    else {return false;}
};

bool Delayed_Energy_Cut(Double_t energy)
{
    if(energy >= DELAYED_ENERGY_MIN && energy <= DELAYED_ENERGY_MAX){return true;}
    else {return false;}
};

bool DeltaT_Cut(Double_t DeltaT)
{
    if(DeltaT >= DELTA_T_MIN && DeltaT <= DELTA_T_MAX){return true;}
    else {return false;};
};

void Initialize(int Len, std::vector<Double_t> &vector)
{
    for(int ii1 = 0; ii1 < Len; ii1++)
    {
        vector.push_back(0);
    };
};

void Initialize(int Len, std::vector<ULong64_t> &vector)
{
    for(int ii1 = 0; ii1 < Len; ii1++)
    {
        vector.push_back(0);
    };
}

void ReInitialize(std::vector<Double_t> &vector)
{
    for(int ii1 = 0; ii1 < vector.size(); ii1++)
    {
        vector.at(ii1) = 0;
    };
};

void ReInitialize(std::vector<ULong64_t> &vector)
{
    for(int ii1 = 0; ii1 < vector.size(); ii1++)
    {
        vector.at(ii1) = 0;
    };
};

Double_t ComputeDelta_T(ULong64_t &Last_50MHz, ULong64_t &Present_50MHz, std::ofstream &logfile)
{
    
    if(Last_50MHz > Present_50MHz && Last_50MHz > MIN_CARE_NUMBER && Present_50MHz < MIN_CARE_NUMBER)
    {
        ULong64_t Part1 = MAX_NUMBER - Last_50MHz + 1;
        ULong64_t ClockGap = (Part1 + Present_50MHz);
        logfile << "[ComputeDelta_T] 注意Clock反转！！！！！！！Max Clock:" << MAX_NUMBER << ",Last Clock:" << Last_50MHz << ", Present Clock:" << Present_50MHz << ", Delta T(s):" << 20.0 * ClockGap / 1e9 << ",返回 false"<< std::endl;
        return 20.0 * ClockGap;
    }
    else
    {
        ULong64_t ClockGap = (Present_50MHz - Last_50MHz);
        return 20.0 *ClockGap;
    };
};

Double_t ComputeDelta_T(ULong64_t &Last_50MHz, ULong64_t &Present_50MHz)
{
    
    if(Last_50MHz > Present_50MHz && Last_50MHz > MIN_CARE_NUMBER && Present_50MHz < MIN_CARE_NUMBER)
    {
        ULong64_t Part1 = MAX_NUMBER - Last_50MHz + 1;
        ULong64_t ClockGap = (Part1 + Present_50MHz);
        std::cout << "[ComputeDelta_T] 注意Clock反转！！！！！！！Max Clock:" << MAX_NUMBER << ",Last Clock:" << Last_50MHz << ", Present Clock:" << Present_50MHz << ", Delta T(s):" << 20.0 * ClockGap / 1e9 << ",返回 false"<< std::endl;
        return 20.0 * ClockGap;
    }
    else
    {
        ULong64_t ClockGap = (Present_50MHz - Last_50MHz);
        return 20.0 *ClockGap;
    };
};

void Find_Coincidence_Pairs(std::string InFilePWD, std::string OutFile)
//Select Coincidence Pairs within cuts
//Cut:
//Prompt: 0.9 <= energy <= 8.0; R<= 5700;
//Delayed: 1.85 <= energy <= 2.5; R<= 5700;
//DeltaT: 400 ns-2 ms; DeltaR <= 2500
{
//Log File
    std::ofstream logfile;
    logfile.open((OutFile + ".log.txt").c_str(), std::ios::out);
//Add Files
    TChain * chain = new TChain("output");
    chain->Add((InFilePWD + "/*").c_str());
//Set Branch Address
    Int_t runID, mcIndex, evIndex, eventID;
    chain->SetBranchAddress("runID", &runID);
    chain->SetBranchAddress("mcIndex", &mcIndex);
    chain->SetBranchAddress("evIndex", &evIndex);
    chain->SetBranchAddress("eventID", &eventID);
    Int_t nhits, nhitsCleaned, owlnhits;
    chain->SetBranchAddress("nhits", &nhits);
    chain->SetBranchAddress("nhitsCleaned", &nhitsCleaned);
    chain->SetBranchAddress("owlnhits", &owlnhits);
    ULong64_t dcApplied, dcFlagged, clockCount50;
    chain->SetBranchAddress("dcApplied", &dcApplied);
    chain->SetBranchAddress("dcFlagged", &dcFlagged);
    chain->SetBranchAddress("clockCount50", &clockCount50);
    Bool_t fitValid;
    chain->SetBranchAddress("fitValid", &fitValid);
    Double_t posx, posy, posz, energy;
    chain->SetBranchAddress("posx", &posx);
    chain->SetBranchAddress("posy", &posy);
    chain->SetBranchAddress("posz", &posz);
    chain->SetBranchAddress("energy", &energy);
//Create Output File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "");
//Create Wanted Data
    myresult res;
    outtree->Branch("RunID", &res.C_RunID);
    outtree->Branch("SubRunID", &res.C_SubRunID);
    outtree->Branch("Date", &res.C_Date);
    outtree->Branch("Time", &res.C_Time);
    outtree->Branch("RunType", &res.C_RunType);
    outtree->Branch("ZOffSet", &res.C_ZOffSet);
    //Prompt
    outtree->Branch("PromptEntry", &res.C_Prompt.C_Entry);
    outtree->Branch("PromptEV", &res.C_Prompt.C_EV);
    outtree->Branch("PromptGTID", &res.C_Prompt.C_GTID);
    outtree->Branch("PromptOWLHits", &res.C_Prompt.C_OWLHits);
    outtree->Branch("PromptNHits", &res.C_Prompt.C_Nhits);
    outtree->Branch("PromptNHitsCleaned", &res.C_Prompt.C_Nhits_Cleaned);
    outtree->Branch("PromptNHitsCorrected", &res.C_Prompt.C_Nhits_Corrected);
    outtree->Branch("Prompt50MHz", &res.C_Prompt.C_50MHZ);
    outtree->Branch("PromptApplied", &res.C_Prompt.C_Data_Applied);
    outtree->Branch("PromptFlags", &res.C_Prompt.C_Data_Flags);
    outtree->Branch("PromptEnergy", &res.C_Prompt.C_Energy);
    outtree->Branch("PromptPosX", &res.C_Prompt.C_Pos_X);
    outtree->Branch("PromptPosY", &res.C_Prompt.C_Pos_Y);
    outtree->Branch("PromptPosZ", &res.C_Prompt.C_Pos_Z);
    outtree->Branch("PromptOriginalEnergy", &res.C_Prompt.C_Original_Energy);
    outtree->Branch("PromptTonyEnergy", &res.C_Prompt.C_Tony_Energy);
    //Delayed
    outtree->Branch("DelayedEntry", &res.C_Delayed.C_Entry);
    outtree->Branch("DelayedEV", &res.C_Delayed.C_EV);
    outtree->Branch("DelayedGTID", &res.C_Delayed.C_GTID);
    outtree->Branch("DelayedOWLHits", &res.C_Delayed.C_OWLHits);
    outtree->Branch("DelayedNHits", &res.C_Delayed.C_Nhits);
    outtree->Branch("DelayedNHitsCleaned", &res.C_Delayed.C_Nhits_Cleaned);
    outtree->Branch("DelayedNHitsCorrected", &res.C_Delayed.C_Nhits_Corrected);
    outtree->Branch("Delayed50MHz", &res.C_Delayed.C_50MHZ);
    outtree->Branch("DelayedApplied", &res.C_Delayed.C_Data_Applied);
    outtree->Branch("DelayedFlags", &res.C_Delayed.C_Data_Flags);
    outtree->Branch("DelayedEnergy", &res.C_Delayed.C_Energy);
    outtree->Branch("DelayedPosX", &res.C_Delayed.C_Pos_X);
    outtree->Branch("DelayedPosY", &res.C_Delayed.C_Pos_Y);
    outtree->Branch("DelayedPosZ", &res.C_Delayed.C_Pos_Z);
    outtree->Branch("DelayedOriginalEnergy", &res.C_Delayed.C_Original_Energy);
    outtree->Branch("DelayedTonyEnergy", &res.C_Delayed.C_Tony_Energy);
//Loop the Data
    Int_t Number_Total_Pass = 0, Number_Pass_Energy[2] = {0, 0}, Number_Pass_Energy_Radius[2] = {0,0};
    Int_t Number_Pass_Energy_Radius_DeltaT = 0, Number_Pass_Energy_Radius_DeltaT_DeltaR = 0;
    Int_t Event_mcIndex[2] = {0, 0};
    std::vector<ULong64_t> Event_ClockCount50;
    Initialize(2, Event_ClockCount50);
    TVector3 Event_Pos[2], temp_TVector3;
    Double_t DeltaT, DeltaR;
    Int_t Len_Entry = 0, Estimated_Total_Pairs_III = 0;
    for(int iPrompt = 0; iPrompt < chain->GetEntries(); iPrompt ++)
    {
        if(iPrompt % 10000 == 0){logfile << "已处理" << iPrompt << ",还剩" << chain->GetEntries() - iPrompt << std::endl; };
        chain->GetEntry(iPrompt);
        if(energy < 0.2 || energy > 10.0) {continue;};
        if(fitValid == false){continue;};
        Number_Total_Pass++;
        if(evIndex == 0){Len_Entry = 1;};
    //Find Prompt Event
        if(Prompt_Energy_Cut(energy) == false){continue;};
        Number_Pass_Energy[0] ++;
        Event_Pos[0].SetXYZ(posx, posy, posz - Z_OFFSET);
        if(Event_Pos[0].Mag() > RADIUS_MAX){continue;};
        Number_Pass_Energy_Radius[0] ++;
        //Record Prompt Info
        Event_ClockCount50.at(0) = clockCount50;
        Event_mcIndex[0] = mcIndex;
        //Write in Results
        res.C_RunID = runID;
        res.C_SubRunID = 0;
        res.C_Date = 0;
        res.C_Time = 0;
        res.C_RunType = 0;
        res.C_ZOffSet = Z_OFFSET;

        res.C_Prompt.C_Entry = mcIndex;
        res.C_Prompt.C_EV = evIndex;
        res.C_Prompt.C_GTID = eventID;
        res.C_Prompt.C_OWLHits = owlnhits;
        res.C_Prompt.C_Nhits = nhits;
        res.C_Prompt.C_Nhits_Cleaned = nhitsCleaned;
        res.C_Prompt.C_Nhits_Corrected = nhits;
        res.C_Prompt.C_Data_Applied = dcApplied;
        res.C_Prompt.C_Data_Flags = dcFlagged;
        res.C_Prompt.C_50MHZ = clockCount50;
        res.C_Prompt.C_Energy = energy;
        res.C_Prompt.C_Pos_X = posx;
        res.C_Prompt.C_Pos_Y = posy;
        res.C_Prompt.C_Pos_Z = posz - Z_OFFSET;
        res.C_Prompt.C_Original_Energy = energy;
        res.C_Prompt.C_Tony_Energy = energy;
    //Find Delayed Event
        for(int iDelayed = iPrompt + 1; iDelayed < chain->GetEntries(); iDelayed++)
        {
            chain->GetEntry(iDelayed);
            if(energy < 0.2 || energy > 10.0) {continue;};
            if(fitValid == false){continue;};
            if( Len_Entry == 1 && evIndex == 2 && mcIndex == Event_mcIndex[0]){Estimated_Total_Pairs_III++;};
            if(Delayed_Energy_Cut(energy) == false){continue;};
            Number_Pass_Energy[1]++;
            Event_Pos[1].SetXYZ(posx, posy, posz - Z_OFFSET);
            if(Event_Pos[1].Mag() > RADIUS_MAX){continue;};
            Number_Pass_Energy_Radius[1]++;
            DeltaT = ComputeDelta_T(Event_ClockCount50.at(0), clockCount50, logfile);
            if(DeltaT_Cut(DeltaT) == false && DeltaT > DELTA_T_MAX) {break;};
            Number_Pass_Energy_Radius_DeltaT++;
            temp_TVector3 = Event_Pos[1] - Event_Pos[0];
            if(temp_TVector3.Mag() > DELTA_R_MAX){continue;};
            Number_Pass_Energy_Radius_DeltaT_DeltaR++;
            //Record Dealyed Info
            //Write in Results
            res.C_Delayed.C_Entry = mcIndex;
            res.C_Delayed.C_EV = evIndex;
            res.C_Delayed.C_GTID = eventID;
            res.C_Delayed.C_OWLHits = owlnhits;
            res.C_Delayed.C_Nhits = nhits;
            res.C_Delayed.C_Nhits_Cleaned = nhitsCleaned;
            res.C_Delayed.C_Nhits_Corrected = nhits;
            res.C_Delayed.C_Data_Applied = dcApplied;
            res.C_Delayed.C_Data_Flags = dcFlagged;
            res.C_Delayed.C_50MHZ = clockCount50;
            res.C_Delayed.C_Energy = energy;
            res.C_Delayed.C_Pos_X = posx;
            res.C_Delayed.C_Pos_Y = posy;
            res.C_Delayed.C_Pos_Z = posz - Z_OFFSET;
            res.C_Delayed.C_Original_Energy = energy;
            res.C_Delayed.C_Tony_Energy = energy;
            outtree->Fill();
        };
        //if(Len_Entry == 2){Estimated_Total_Pairs_III++;};
    };
    outfile->Write();
    outfile->Close();
    Double_t Estimated_Pairs_II = chain->GetEntries("energy >= 0.2 && energy <= 10.0 && evIndex == 0");
    Double_t Len = chain->GetEntries()/2;
    Double_t Correct_Pro = Estimated_Pairs_II/Len, Correct_Error = std::sqrt(Len * Correct_Pro * (1 - Correct_Pro));
    Double_t Selection_Eff_II = Number_Pass_Energy_Radius_DeltaT_DeltaR / Estimated_Pairs_II;
    Double_t Selection_Eff_II_Error = Number_Pass_Energy_Radius_DeltaT_DeltaR * Correct_Error/pow(Len, 2);
    logfile << "Estimated Total Pairs I:" << Number_Total_Pass/2 << ", Estimated Total Pairs II:" << Estimated_Pairs_II << ", Error:" << std::sqrt(0.5 * Len * Correct_Pro * (1 - Correct_Pro)) << ", Estimated Total Pairs III:" << Estimated_Total_Pairs_III << std::endl;
    logfile << "Prompt After Energy Cut:" << Number_Pass_Energy[0] << ", Prompt After Radius Cut:" << Number_Pass_Energy_Radius[0] << std::endl;
    logfile << "Delayed After Energy Cut:" << Number_Pass_Energy[1] << ", Delayed After Radius Cut:" << Number_Pass_Energy_Radius[1] << std::endl;
    logfile << "After Delta T Cut:" << Number_Pass_Energy_Radius_DeltaT << ", After Delta R Cut:" << Number_Pass_Energy_Radius_DeltaT_DeltaR << std::endl;
    logfile << "Selection Eff I :" << Number_Pass_Energy_Radius_DeltaT_DeltaR/(0.5 * Number_Total_Pass) << std::endl;
    logfile << "Selection Eff II:" << Selection_Eff_II << ", Error:" << Selection_Eff_II_Error << ", Correct Pro:" << Correct_Pro << std::endl;
    logfile << "Selection Eff III (Don't use this result, because Prompt Cuts reduce the total number):" << 1.0 * Number_Pass_Energy_Radius_DeltaT_DeltaR/Estimated_Total_Pairs_III << std::endl;
};

void Find_Full_Coincidence_Pairs(std::string InFilePWD, std::string OutFile)
//Select all Coincidence Pairs and give an estimated number of Coincidence Pairs
//Cut:
//Prompt: evIndex == 0; R<= 6000; 0.2 <= energy <= 10.0
//Delayed: evIndex == 2; R<= 6000; 0.2 <= energy <= 10.0
{
//Log File
    std::ofstream logfile;
    logfile.open((OutFile + ".log.txt").c_str(), std::ios::out);
//Add Files
    TChain * chain = new TChain("output");
    chain->Add((InFilePWD + "/*").c_str());
//Set Branch Address
    Int_t runID, mcIndex, evIndex, eventID;
    chain->SetBranchAddress("runID", &runID);
    chain->SetBranchAddress("mcIndex", &mcIndex);
    chain->SetBranchAddress("evIndex", &evIndex);
    chain->SetBranchAddress("eventID", &eventID);
    Int_t nhits, nhitsCleaned, owlnhits;
    chain->SetBranchAddress("nhits", &nhits);
    chain->SetBranchAddress("nhitsCleaned", &nhitsCleaned);
    chain->SetBranchAddress("owlnhits", &owlnhits);
    ULong64_t dcApplied, dcFlagged, clockCount50;
    chain->SetBranchAddress("dcApplied", &dcApplied);
    chain->SetBranchAddress("dcFlagged", &dcFlagged);
    chain->SetBranchAddress("clockCount50", &clockCount50);
    Bool_t fitValid;
    chain->SetBranchAddress("fitValid", &fitValid);
    Double_t posx, posy, posz, energy;
    chain->SetBranchAddress("posx", &posx);
    chain->SetBranchAddress("posy", &posy);
    chain->SetBranchAddress("posz", &posz);
    chain->SetBranchAddress("energy", &energy);
//Create Output File
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "");
//Create Wanted Data
    myresult res;
    outtree->Branch("RunID", &res.C_RunID);
    outtree->Branch("SubRunID", &res.C_SubRunID);
    outtree->Branch("Date", &res.C_Date);
    outtree->Branch("Time", &res.C_Time);
    outtree->Branch("RunType", &res.C_RunType);
    outtree->Branch("ZOffSet", &res.C_ZOffSet);
    //Prompt
    outtree->Branch("PromptEntry", &res.C_Prompt.C_Entry);
    outtree->Branch("PromptEV", &res.C_Prompt.C_EV);
    outtree->Branch("PromptGTID", &res.C_Prompt.C_GTID);
    outtree->Branch("PromptOWLHits", &res.C_Prompt.C_OWLHits);
    outtree->Branch("PromptNHits", &res.C_Prompt.C_Nhits);
    outtree->Branch("PromptNHitsCleaned", &res.C_Prompt.C_Nhits_Cleaned);
    outtree->Branch("PromptNHitsCorrected", &res.C_Prompt.C_Nhits_Corrected);
    outtree->Branch("Prompt50MHz", &res.C_Prompt.C_50MHZ);
    outtree->Branch("PromptApplied", &res.C_Prompt.C_Data_Applied);
    outtree->Branch("PromptFlags", &res.C_Prompt.C_Data_Flags);
    outtree->Branch("PromptEnergy", &res.C_Prompt.C_Energy);
    outtree->Branch("PromptPosX", &res.C_Prompt.C_Pos_X);
    outtree->Branch("PromptPosY", &res.C_Prompt.C_Pos_Y);
    outtree->Branch("PromptPosZ", &res.C_Prompt.C_Pos_Z);
    outtree->Branch("PromptOriginalEnergy", &res.C_Prompt.C_Original_Energy);
    outtree->Branch("PromptTonyEnergy", &res.C_Prompt.C_Tony_Energy);
    //Delayed
    outtree->Branch("DelayedEntry", &res.C_Delayed.C_Entry);
    outtree->Branch("DelayedEV", &res.C_Delayed.C_EV);
    outtree->Branch("DelayedGTID", &res.C_Delayed.C_GTID);
    outtree->Branch("DelayedOWLHits", &res.C_Delayed.C_OWLHits);
    outtree->Branch("DelayedNHits", &res.C_Delayed.C_Nhits);
    outtree->Branch("DelayedNHitsCleaned", &res.C_Delayed.C_Nhits_Cleaned);
    outtree->Branch("DelayedNHitsCorrected", &res.C_Delayed.C_Nhits_Corrected);
    outtree->Branch("Delayed50MHz", &res.C_Delayed.C_50MHZ);
    outtree->Branch("DelayedApplied", &res.C_Delayed.C_Data_Applied);
    outtree->Branch("DelayedFlags", &res.C_Delayed.C_Data_Flags);
    outtree->Branch("DelayedEnergy", &res.C_Delayed.C_Energy);
    outtree->Branch("DelayedPosX", &res.C_Delayed.C_Pos_X);
    outtree->Branch("DelayedPosY", &res.C_Delayed.C_Pos_Y);
    outtree->Branch("DelayedPosZ", &res.C_Delayed.C_Pos_Z);
    outtree->Branch("DelayedOriginalEnergy", &res.C_Delayed.C_Original_Energy);
    outtree->Branch("DelayedTonyEnergy", &res.C_Delayed.C_Tony_Energy);
//Loop the Data
    Double_t Number_Total_Pass = 0, Number_Pass_Energy[2] = {0, 0}, Number_Pass_Energy_Radius[2] = {0,0};
    Double_t Number_Pass_Energy_Radius_DeltaT = 0, Number_Pass_Energy_Radius_DeltaT_DeltaR = 0;
    Int_t Event_mcIndex[2] = {0, 0}, Event_evIndex[2] = {0, 0};//Prompt, Last Prompt;
    std::vector<ULong64_t> Event_ClockCount50;
    Initialize(2, Event_ClockCount50);
    TVector3 Event_Pos[2], temp_TVector3;
    Double_t DeltaT, DeltaR;
    Int_t Len_Entry = 0, Estimated_Total_Pairs_III = 0;
    logfile << "Begin to Loop" << std::endl;
    for(int iPrompt = 0; iPrompt < chain->GetEntries(); iPrompt ++)
    {
        if(iPrompt % 10000 == 0){logfile << "[Loop] 已处理" << iPrompt << ",还剩" << chain->GetEntries() - iPrompt << std::endl; };
        chain->GetEntry(iPrompt);
        if(energy < 0.2 || energy > 10.0) {continue;};
        if(fitValid == false){continue;};
        Number_Total_Pass++;
        if(evIndex != 0){continue;};
        Event_Pos[0].SetXYZ(posx, posy, posz - Z_OFFSET);
        if(Event_Pos[0].Mag()>6000){continue;};
        Len_Entry = 1;
    //Find Prompt Event
        Number_Pass_Energy[0]++;
        Number_Pass_Energy_Radius[0]++;
        //Record Prompt Info
        Event_ClockCount50.at(0) = clockCount50;
        Event_mcIndex[0] = Event_mcIndex[1] = mcIndex;
        Event_evIndex[0] = Event_evIndex[1] = evIndex;
        //Write in Results
        res.C_RunID = runID;
        res.C_SubRunID = 0;
        res.C_Date = 0;
        res.C_Time = 0;
        res.C_RunType = 0;
        res.C_ZOffSet = Z_OFFSET;

        res.C_Prompt.C_Entry = mcIndex;
        res.C_Prompt.C_EV = evIndex;
        res.C_Prompt.C_GTID = eventID;
        res.C_Prompt.C_OWLHits = owlnhits;
        res.C_Prompt.C_Nhits = nhits;
        res.C_Prompt.C_Nhits_Cleaned = nhitsCleaned;
        res.C_Prompt.C_Nhits_Corrected = nhits;
        res.C_Prompt.C_Data_Applied = dcApplied;
        res.C_Prompt.C_Data_Flags = dcFlagged;
        res.C_Prompt.C_50MHZ = clockCount50;
        res.C_Prompt.C_Energy = energy;
        res.C_Prompt.C_Pos_X = posx;
        res.C_Prompt.C_Pos_Y = posy;
        res.C_Delayed.C_Pos_Z = posz - Z_OFFSET;
        res.C_Delayed.C_Original_Energy = energy;
        res.C_Delayed.C_Tony_Energy = energy;
    //Find Delayed Event
        for(int iDelayed = iPrompt + 1; iDelayed < iPrompt + 4; iDelayed++)
        {
            chain->GetEntry(iDelayed);
            if(energy < 0.2 || energy > 10.0) {continue;};
            if(fitValid == false){continue;};
            if(Event_mcIndex[0] != mcIndex){break;};
            if(evIndex != 2){continue;};
            Event_Pos[1].SetXYZ(posx, posy, posz - Z_OFFSET);
            if(Event_Pos[1].Mag() > 6000){continue;};
            Len_Entry++;
            Number_Pass_Energy[1]++;
            Number_Pass_Energy_Radius[1]++;
            Number_Pass_Energy_Radius_DeltaT++;
            Number_Pass_Energy_Radius_DeltaT_DeltaR++;
            //Write in Results
            res.C_Delayed.C_Entry = mcIndex;
            res.C_Delayed.C_EV = evIndex;
            res.C_Delayed.C_GTID = eventID;
            res.C_Delayed.C_OWLHits = owlnhits;
            res.C_Delayed.C_Nhits = nhits;
            res.C_Delayed.C_Nhits_Cleaned = nhitsCleaned;
            res.C_Delayed.C_Nhits_Corrected = nhits;
            res.C_Delayed.C_Data_Applied = dcApplied;
            res.C_Delayed.C_Data_Flags = dcFlagged;
            res.C_Delayed.C_50MHZ = clockCount50;
            res.C_Delayed.C_Energy = energy;
            res.C_Delayed.C_Pos_X = posx;
            res.C_Delayed.C_Pos_Y = posy;
            res.C_Delayed.C_Pos_Z = posz - Z_OFFSET;
            res.C_Delayed.C_Original_Energy = energy;
            res.C_Delayed.C_Tony_Energy = energy;
            outtree->Fill();
        };
        if(Len_Entry == 2){Estimated_Total_Pairs_III++;};
    };
    outfile->Write();
    outfile->Close();
    Double_t Estimated_Pairs_II = chain->GetEntries("energy >= 0.2 && energy <= 10.0 && evIndex == 0");
    Double_t Len = chain->GetEntries()/2;
    Double_t Correct_Pro = Estimated_Pairs_II/Len, Correct_Error = std::sqrt(Len * Correct_Pro * (1 - Correct_Pro));
    Double_t Selection_Eff_II = Number_Pass_Energy_Radius_DeltaT_DeltaR / Estimated_Pairs_II;
    Double_t Selection_Eff_II_Error = Number_Pass_Energy_Radius_DeltaT_DeltaR * Correct_Error/pow(Len, 2);
    logfile << "Estimated Total Pairs I:" << Number_Total_Pass/2 << ", Estimated Total Pairs II:" << Estimated_Pairs_II << ", Error:" << std::sqrt(0.5 * Len * Correct_Pro * (1 - Correct_Pro)) << ", Estimated Total Pairs III:" << Estimated_Total_Pairs_III << std::endl;
    logfile << "Prompt After Energy Cut:" << Number_Pass_Energy[0] << ", Prompt After Radius Cut:" << Number_Pass_Energy_Radius[0] << std::endl;
    logfile << "Delayed After Energy Cut:" << Number_Pass_Energy[1] << ", Delayed After Radius Cut:" << Number_Pass_Energy_Radius[1] << std::endl;
    logfile << "After Delta T Cut:" << Number_Pass_Energy_Radius_DeltaT << ", After Delta R Cut:" << Number_Pass_Energy_Radius_DeltaT_DeltaR << std::endl;
    logfile << "Selection Eff I :" << Number_Pass_Energy_Radius_DeltaT_DeltaR/(0.5 * Number_Total_Pass) << std::endl;
    logfile << "Selection Eff II:" << Selection_Eff_II << ", Error:" << Selection_Eff_II_Error << ", Correct Pro:" << Correct_Pro << std::endl;
    logfile << "Selection Eff III:" << 1.0 * Number_Pass_Energy_Radius_DeltaT_DeltaR/Estimated_Total_Pairs_III << std::endl;
};

void Create_PDFs(std::string InFile, std::string Name, std::string OutFile, std::vector<TH1D*> Example_Hists, bool Full_PDF)
{
//Read File
    TFile *infile = new TFile(InFile.c_str());
    TTree *tree = (TTree*) infile->Get("output");
//Set Branch Address
    //Prompt
    Double_t Prompt_Energy, Prompt_PosX, Prompt_PosY, Prompt_PosZ;
    tree->SetBranchAddress("PromptEnergy", &Prompt_Energy);
    tree->SetBranchAddress("PromptPosX", &Prompt_PosX);
    tree->SetBranchAddress("PromptPosY", &Prompt_PosY);
    tree->SetBranchAddress("PromptPosZ", &Prompt_PosZ);
    ULong64_t Prompt_50MHz;
    tree->SetBranchAddress("Prompt50MHz", &Prompt_50MHz);
    //Delayed
    Double_t Delayed_Energy, Delayed_PosX, Delayed_PosY, Delayed_PosZ;
    tree->SetBranchAddress("DelayedEnergy", &Delayed_Energy);
    tree->SetBranchAddress("DelayedPosX", &Delayed_PosX);
    tree->SetBranchAddress("DelayedPosY", &Delayed_PosY);
    tree->SetBranchAddress("DelayedPosZ", &Delayed_PosZ);
    ULong64_t Delayed_50MHz;
    tree->SetBranchAddress("Delayed50MHz", &Delayed_50MHz);
//Create File
    TFile *outfile = new TFile(OutFile.c_str(),"recreate");
//Initialize Histograms
    std::vector<TH1D*> Out_Hists;
    Out_Hists.push_back((TH1D*)Example_Hists.at(0)->Clone((Name + "_Delta_T").c_str()));
    Out_Hists.push_back((TH1D*)Example_Hists.at(1)->Clone((Name + "_Delta_R").c_str()));
    Out_Hists.push_back((TH1D*)Example_Hists.at(2)->Clone((Name + "_Prompt_Energy").c_str()));
    Out_Hists.push_back((TH1D*)Example_Hists.at(3)->Clone((Name + "_Delayed_Energy").c_str()));

    TH1D* temp_th1d = (TH1D*)Example_Hists.at(0)->Clone("test");
    for(int ii1 = 0; ii1 < Out_Hists.size(); ii1++)
    {
        Out_Hists.at(ii1)->Reset("ICES");
    };
//Loop the Data
    Double_t Delta_T;
    TVector3 Pos[2], Delta_R;
    for(int ii1 = 0; ii1 < tree->GetEntries(); ii1++)
    {
        tree->GetEntry(ii1);
        Pos[0].SetXYZ(Prompt_PosX, Prompt_PosY, Prompt_PosZ);
        Pos[1].SetXYZ(Delayed_PosX, Delayed_PosY, Delayed_PosZ);
        Delta_R = Pos[0] - Pos[1];
        Delta_T = ComputeDelta_T(Prompt_50MHz, Delayed_50MHz);
        Out_Hists.at(0)->Fill(Delta_T/1e3);
        Out_Hists.at(1)->Fill(Delta_R.Mag());
        Out_Hists.at(2)->Fill(Prompt_Energy);
        Out_Hists.at(3)->Fill(Delayed_Energy);
    };
//Save into Pictures
    std::vector<std::string> Pic_Name;
    if(Full_PDF == true)
    {
        Pic_Name.push_back("./Pic/" + Name + "_Delta_T_Full.jpg");
        Pic_Name.push_back("./Pic/" + Name + "_Delta_R_Full.jpg");
        Pic_Name.push_back("./Pic/" + Name + "_Prompt_Energy_Full.jpg");
        Pic_Name.push_back("./Pic/" + Name + "_Delayed_Energy_Full.jpg");
    }
    else
    {
        Pic_Name.push_back("./Pic/" + Name + "_Delta_T.jpg");
        Pic_Name.push_back("./Pic/" + Name + "_Delta_R.jpg");
        Pic_Name.push_back("./Pic/" + Name + "_Prompt_Energy.jpg");
        Pic_Name.push_back("./Pic/" + Name + "_Delayed_Energy.jpg");
    }

    FitSavePlot_E(Pic_Name.at(0), Out_Hists.at(0), "#Delta T", "#Delta T/#mu s", "p.d.f.", true, 0, 0);
    SavePlot(Pic_Name.at(1), Out_Hists.at(1), "#Delta R", "#Delta R/mm","p.d.f.", true, 0, 0);
    SavePlot(Pic_Name.at(2), Out_Hists.at(2), "Prompt Energy", "E_{Prompt}/MeV","p.d.f.", true, 0, 0);
    FitSavePlot_G(Pic_Name.at(3), Out_Hists.at(3), "Delayed Energy", "E_{Delayed}/MeV", "p.d.f.", true, 0, 0);
//Write in Files
    for(int ii1 = 0; ii1 < Out_Hists.size(); ii1++)
    {
        Out_Hists.at(ii1)->Write();
    };
    outfile->Close();

};

/*Back Code For Testing
    std::cout << "Prompt Info" << std::endl;
    std::cout << "Energy:" << Prompt_Energy << ",X:" << Prompt_PosX << ",Y:" << Prompt_PosY << ",Z:" << Prompt_PosZ << std::endl;
    std::cout << "50MHz:" << Prompt_50MHz << std::endl;
    std::cout << "Delayed Info" << std::endl;
    std::cout << "Energy:" << Delayed_Energy << ",X:" << Delayed_PosX << ",Y:" << Delayed_PosY << ",Z:" << Delayed_PosZ << std::endl;
    std::cout << "50MHz:" << Delayed_50MHz << std::endl;
*/

#endif