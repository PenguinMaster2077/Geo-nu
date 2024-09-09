//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./HeadFile/Constant_Setting.hh"

int Grid_Scan_Plot()
{
    TFile *infile = new TFile(GRID_SCAN_GEONU_REACTOR_EXTENDED_MAXIMUM_LIKELIHOOD.c_str());
    TTree *tree = (TTree*) infile->Get("output");
    Double_t number_reactor, number_geonu, number_alphan_ground, number_alphan_exicted;
    tree->SetBranchAddress("NumberReactor", &number_reactor);
    tree->SetBranchAddress("NumberGeonu", &number_geonu);
    tree->SetBranchAddress("NumberAlphaNGround", &number_alphan_ground);
    tree->SetBranchAddress("NumberAlphaNExicted", &number_alphan_exicted);
    Double_t ratio_u_th;
    tree->SetBranchAddress("RatioUTh", &ratio_u_th);
    Double_t fitting_value;
    tree->SetBranchAddress("FittingValue", &fitting_value);
    Int_t method;
    tree->SetBranchAddress("Method", &method);
    Int_t len = tree->GetEntries();
//Load Data
    std::vector<Double_t> Fitting_Values;
    for(int index = 0; index < len; index++)
    {
        tree->GetEntry(index);
        Fitting_Values.push_back(fitting_value);
    };
//Find the Maximum and Minuim Value
    Double_t Max_Fitting_Value, Min_Fitting_Value;
    Int_t Best_Index;
    Max_Fitting_Value = Min_Fitting_Value = Fitting_Values.at(0);
    for(int index = 0; index < Fitting_Values.size(); index++)
    {
        if(Max_Fitting_Value < Fitting_Values.at(index))
        {
            Max_Fitting_Value = Fitting_Values.at(index);
        };
        if(Min_Fitting_Value > Fitting_Values.at(index))
        {
            Min_Fitting_Value = Fitting_Values.at(index);
            Best_Index = index;
        };
    };
//Compute Delta chi-square
    std::vector<Double_t> Delta_Fitting_Values;
    Double_t temp_res;
    for(int index = 0; index < Fitting_Values.size(); index++)
    {
        if(method == 1)
        {
            temp_res = 2.0 * ( Fitting_Values.at(index) - Min_Fitting_Value);
        }
        else
        {
            temp_res = Fitting_Values.at(index) - Min_Fitting_Value;
        }
        Delta_Fitting_Values.push_back(temp_res);
    };
    
//Show
    tree->GetEntry(Best_Index);
    std::cout << "Best Fit:" << Fitting_Values.at(Best_Index) << ", Max:" << Max_Fitting_Value << ", Geonu:" << number_geonu << ", Reactor:" << number_reactor << std::endl;
    return 0;
}