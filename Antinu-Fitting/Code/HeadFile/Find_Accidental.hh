#ifndef FIND_ACCIDENTAL_HH
#define FIND_ACCIDENTAL_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TVector3.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"
#include "./Base_Functions.hh"
#include "./Result.hh"

void Data_Cleaning_Cut(std::string OutPWD, std::string Prompt_PWD, std::string Delayed_PWD)
{
//Load Prompt Files
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Load Prompt Files" << std::endl;
    TChain *chain_prompt = new TChain("output");
    chain_prompt->Add((Prompt_PWD + "/*").c_str());
//Set Branch Address;
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Set up Branch Address" << std::endl;
    ULong64_t Prompt_Flags;
    chain_prompt->SetBranchAddress("PromptDataFlags", &Prompt_Flags);
//Set Prompt Output File
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Create Prompt Output File" << std::endl;
    TFile *outfile_prompt = new TFile((OutPWD + "/Prompt/Gold_AC_Loose_CP_Data_Prompt.root").c_str(), "recreate");
    TTree *outtree_prompt = chain_prompt->CloneTree(0);
//Loop Prompt Data
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Loop Prompt Data" << std::endl;
    for(int iPrompt = 0; iPrompt < chain_prompt->GetEntries(); iPrompt++)
    {
        chain_prompt->GetEntry(iPrompt);
        if( (Prompt_Flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK ){continue;};
        outtree_prompt->Fill();
    };
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Total:" << chain_prompt->GetEntries() << ", After Data Cleaning:" << outtree_prompt->GetEntries() << std::endl;
    outfile_prompt->Write();
    outfile_prompt->Close();
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Complete Processing Prompt Files" << std::endl;

//Load Delayed Files
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Load Delayed Files" << std::endl;
    TChain *chain_delayed = new TChain("output");
    chain_delayed->Add((Delayed_PWD + "/*").c_str());
//Set Branch Address
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Set up Branch Address" << std::endl;
    ULong64_t Delayed_Flags;
    chain_delayed->SetBranchAddress("DelayedDataFlags", &Delayed_Flags);
//Set Delyaed Output File
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Create Delayed Output File" << std::endl;
    TFile *outfile_delayed = new TFile( (OutPWD + "/Delayed/Gold_AC_Loose_CP_Data_Delayed.root").c_str(), "recreate");
    TTree *outtree_delayed = chain_delayed->CloneTree(0);
//Loop Delayed Data
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Loop Delayed Data" << std::endl;
    for(int iDelayed = 0; iDelayed < chain_delayed->GetEntries(); iDelayed++)
    {
        chain_delayed->GetEntry(iDelayed);
        if( (Delayed_Flags & DATA_CLEANING_MASK) != DATA_CLEANING_MASK) {continue;};
        outtree_delayed->Fill();
    };
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Total:" << chain_delayed->GetEntries() << ", After Data Cleaning:" << outtree_delayed->GetEntries() << std::endl;
    outfile_delayed->Write();
    outfile_delayed->Close();
    std::cout << "[Find_Accidental::Data_Cleaning_Cut] Complete Processing Delayed Files" << std::endl;
};

void Compute_Rate(std::string Out_File, std::string InPWD)
{
//Load Prompt and Delayed Files
    TFile *infile_prompt = new TFile((InPWD + "/Prompt/Gold_AC_Loose_CP_Data_Prompt.root").c_str());
    TTree *intree_prompt = (TTree*) infile_prompt->Get("output");
    
    TFile *infile_delayed = new TFile((InPWD + "/Delayed/Gold_AC_Loose_CP_Data_Delayed.root").c_str());
    TTree *intree_delayed = (TTree*) infile_delayed->Get("output");
//Set Branch Address
    UInt_t Run_ID;
    intree_prompt->SetBranchAddress("RunID", &Run_ID);
    ULong64_t Begin_Count, End_Count;
    intree_prompt->SetBranchAddress("BeginCount", &Begin_Count);
    intree_prompt->SetBranchAddress("EndCount", &End_Count);
//Loop Prompt Data
    UInt_t Index_Vector, Event_Prompt, Event_Delayed;
    std::vector<UInt_t> Runs, Events_Prompt, Events_Delayed;
    std::vector<ULong64_t> Begin_Counts, End_Counts;
    std::string Selection_Prompt, Selection_Delayed;
    Int_t Len = intree_prompt->GetEntries();
    for(int ii1 = 0; ii1 < intree_prompt->GetEntries(); ii1++)
    {
        if( ii1 % 50000 == 0) { std::cout << "[Find_Accidental::Compute_Rate] 已处理" << 100.0 * ii1/Len << "%，具体:" << ii1 << "，还剩" << Len - ii1 << std::endl;};
        intree_prompt->GetEntry(ii1);
        //Initialization
        if(ii1 == 0)
        {
            
            Index_Vector = 0;
            Runs.push_back(Run_ID);
            Begin_Counts.push_back(Begin_Count);
            End_Counts.push_back(End_Count);

            Selection_Prompt = Form("RunID == %u", Run_ID);
            Selection_Delayed = Form("RunID == %u", Run_ID);
            Event_Prompt = intree_prompt->GetEntries(Selection_Prompt.c_str());
            Event_Delayed = intree_delayed->GetEntries(Selection_Delayed.c_str());
            Events_Prompt.push_back(Event_Prompt);
            Events_Delayed.push_back(Event_Delayed);
            continue;
        };
        //Same Run
        if( Runs.at(Index_Vector) == Run_ID)
        {
            // End_Counts.at(Index_Vector) = End_Count;
            End_Counts.at(Index_Vector) = End_Count;
            continue;
        }
        else if( Runs.at(Index_Vector) != Run_ID)
        {
            Index_Vector++;
            Runs.push_back(Run_ID);
            Begin_Counts.push_back(Begin_Count);
            End_Counts.push_back(End_Count);

            Selection_Prompt = Form("RunID == %u", Run_ID);
            Selection_Delayed = Form("RunID == %u", Run_ID);
            Event_Prompt = intree_prompt->GetEntries(Selection_Prompt.c_str());
            Event_Delayed = intree_delayed->GetEntries(Selection_Delayed.c_str());            
            Events_Prompt.push_back(Event_Prompt);
            Events_Delayed.push_back(Event_Delayed);
        };
    };
//Compute Prompt and Delayed Rate
    Double_t Time = 0;
    std::vector<Double_t> Rates_Prompt, Rates_Delayed, Rates_AC;
    for(int ii1 = 0; ii1 < Runs.size(); ii1++)
    {
        Time = ComputeDelta_T(Begin_Counts.at(ii1), End_Counts.at(ii1))/1e9;
        Rates_Prompt.push_back( 1.0 * Events_Prompt.at(ii1)/ Time);// events/s
        Rates_Delayed.push_back( 1.0 * Events_Delayed.at(ii1)/ Time);// Events/s
        Rates_AC.push_back(Rates_Prompt.at(ii1) * Rates_Delayed.at(ii1) * 2.0e-3);
    };
//Compute Average AC Rate
    Double_t Sum = 0, Rate_Average;
    for(int ii1 = 0; ii1 < Rates_AC.size(); ii1++)
    {
        Sum = Sum + Rates_AC.at(ii1);
    };
    Rate_Average = 1.0 * Sum / Rates_AC.size();
//Show Details
    for(int ii1 = 0; ii1 < Runs.size(); ii1++)
    {
        std::cout << "Runs:" << Runs.at(ii1) << ", Prompt Events:" << Events_Prompt.at(ii1) << ", Delayed Events:" << Events_Delayed.at(ii1) << ", Begin Count:" << Begin_Counts.at(ii1) << ", End Count:" << End_Counts.at(ii1)  << ", Prompt Rate:" << Rates_Prompt.at(ii1) << ", Delayed Rate:" << Rates_Delayed.at(ii1) << ", AC Rate:" << Rates_AC.at(ii1) << std::endl;
    };
    std::cout << Rate_Average << std::endl;
}

#endif