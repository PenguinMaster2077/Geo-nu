#ifndef GRID_SCAN_PLOT_HH
#define GRID_SCAN_PLOT_HH
//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
//RAT
//Self-Defined
#include "../Constant_Setting.hh"

TH2D* Grid_Scan_Plot_1D_Geo_Ratio(std::string InFile, TH2D *Example_Hist, Bool_t Delta = false)
//Delta = true: Plot Delta Fitting Value
//Delta = false: Plot Fitting Value
{
    TFile *infile = new TFile(InFile.c_str());
    TTree *tree = (TTree*) infile->Get("output");
//Set Branch Address
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
//Load Data
    std::vector<Double_t> fitting_values;
    std::vector<Double_t> x_values;
    Double_t min_fitting_value, max_fitting_value;
    Int_t best_index;
    for(int index = 0; index < tree->GetEntries(); index++)
    {
        tree->GetEntry(index);
        fitting_values.push_back(fitting_value);
        x_values.push_back(ratio_u_th);
        //Intialization
        if(index == 0)
        {
            min_fitting_value = max_fitting_value = fitting_value;
            best_index = 0;
        };
        //Update Maximum and Minimum
        if(max_fitting_value < fitting_value)
        {
            max_fitting_value = fitting_value;
        };
        if(min_fitting_value > fitting_value)
        {
            min_fitting_value = fitting_value;
            best_index = index;
        };
        //Test Part
        // std::cout << index << "," << fitting_value << std::endl;
    };
//Close Files
    infile->Close();
    delete infile; infile = nullptr;
//Compute -2\Delta LogL or \Delta Chi-Square
    std::vector<Double_t> delta_fitting_values;
    Double_t temp_res;
    for(int index = 0; index < fitting_values.size(); index++)
    {
        temp_res = fitting_values.at(index) - min_fitting_value;
        if(method == 1)//Extended Maximum Likelihood
        {
            delta_fitting_values.push_back(- 2.0 * temp_res);
        }
        else if(method == 2)
        {
            delta_fitting_values.push_back(temp_res);
        };
        std::cout << index << "," << temp_res << std::endl;
    }
//Fill into Histogram
    TH2D *temp_hist;
    if(Delta == true)
    {
        temp_hist = (TH2D*) Example_Hist->Clone("temp_hist_delta_fitting_values");
    }
    else
    {
        temp_hist = (TH2D*) Example_Hist->Clone("temp_hist_fitting_values");
    };
    temp_hist->Reset("ICES");//Make it to be empty;
    for(int index = 0; index < fitting_values.size(); index++)
    {
        if(Delta == true)
        {
            temp_hist->Fill(x_values.at(index), delta_fitting_values.at(index));
        }
        else
        {
            temp_hist->Fill(x_values.at(index), fitting_values.at(index));
        }
    };
//Return
    return temp_hist;
}

#endif