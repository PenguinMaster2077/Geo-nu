#ifndef COMPUTE_SIGMA_HH
#define COMPUTE_SIGMA_HH
//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./Constants_Setting.hh"

Double_t Compute_Mean(std::vector<Double_t> &vector)
{
    Double_t Sum = 0;
    for(int ii1 = 0; ii1 < vector.size(); ii1++)
    {
        Sum = Sum + vector.at(ii1);
    };
    return Sum / vector.size();
};

Double_t Compute_Standard_Error(std::vector<Double_t> &vector_x, std::vector<Double_t> &vector_y)
{
//Compute the Minimum Length
    Int_t Len;
    if(vector_x.size() >= vector_y.size())
    {
        Len = vector_y.size();
    }
    else 
    {
        Len = vector_x.size();
    };
//Compute the Mean
    Double_t Mean_X = Compute_Mean(vector_x);
    Double_t Mean_Y = Compute_Mean(vector_y);
//Compute Standard Error
    Double_t Sum = 0;
    for(int ii1 = 0; ii1 < Len; ii1++)
    {
        Sum = Sum + (vector_x.at(ii1) - Mean_X) * (vector_y.at(ii1) - Mean_Y);
    };
//Return 
    return Sum/(Len - 1);
}

void Compute_AN()
{
    std::string InFile = PDF_DATA_AN;
//Read File
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
//Setup Branch Address
    Double_t Prompt_Energy;
    intree->SetBranchAddress("PromptEnergy", &Prompt_Energy);
//Loop Data
    std::vector<Double_t> Proton_Recoil, C12, O16;
    std::vector<Double_t> Total;
    for(int ii1 = 0; ii1 < intree->GetEntries(); ii1++)
    {
        intree->GetEntry(ii1);
        if(Prompt_Energy >=0 && Prompt_Energy < AN_ENERGY_PROTON_RECOIL)
        {
            Proton_Recoil.push_back(Prompt_Energy);
        }
        else if(Prompt_Energy >= AN_ENERGY_PROTON_RECOIL && Prompt_Energy < AN_ENERGY_C12)
        {
            C12.push_back(Prompt_Energy);
        }
        else if(Prompt_Energy >= AN_ENERGY_C12)
        {
            O16.push_back(Prompt_Energy);
        };
        Total.push_back(Prompt_Energy);
    };
//Compute Standard Error
    Double_t Sigma_PR = Compute_Standard_Error(Proton_Recoil, Proton_Recoil);
    Double_t Sigma_C12 = Compute_Standard_Error(C12, C12);
    Double_t Sigma_O16 = Compute_Standard_Error(O16, O16);
    Double_t Sigma_PR_C12 = Compute_Standard_Error(Proton_Recoil, C12);
    Double_t Sigma_PR_O16 = Compute_Standard_Error(Proton_Recoil, O16);
    Double_t Sigma_C12_O16 = Compute_Standard_Error(C12, O16);
//Show
    std::cout << "Sigma_PR:" << Sigma_PR << ", Sigma_C12:" << Sigma_C12 << ", Sigma_O16:" << Sigma_O16 << std::endl;
    std::cout << "Cov(PR, C12):" << Sigma_PR_C12 << ", Cov(PR, O16):" << Sigma_PR_O16 << ", Cov(C12, O16):" << Sigma_C12_O16 << std::endl;
    std::cout << "Cov(C12, PR):" << Compute_Standard_Error(C12, Proton_Recoil) << ", Cov(O16, PR):" << Compute_Standard_Error(O16, Proton_Recoil) << ", Cov(O16, C12):" << Compute_Standard_Error(O16, C12) << std::endl;
    std::cout << "Sctrictly Computation:" << Sigma_PR + Sigma_C12 + Sigma_O16 + 2 * Sigma_PR_C12 + 2 * Sigma_PR_O16 + 2 * Sigma_C12_O16 << ", Roughly Computation:" << Sigma_PR + Sigma_C12 + Sigma_O16 << std::endl;
};

void Compute_Geo()
{
    std::string InFile_U = PDF_DATA_GEO_U;
    std::string InFile_Th = PDF_DATA_GEO_TH;
//Read File
    TFile *infile_u = new TFile(InFile_U.c_str());
    TTree *intree_u = (TTree*) infile_u->Get("output");
    TFile *infile_th = new TFile(InFile_Th.c_str());
    TTree *intree_th = (TTree*) infile_th->Get("output");
//Setup Branch Address
    Double_t Prompt_Energy;
    intree_u->SetBranchAddress("PromptEnergy", &Prompt_Energy);
    intree_th->SetBranchAddress("PromptEnergy", &Prompt_Energy);
//Loop Data
    std::vector<Double_t> Prompt_U, Prompt_Th;
    for(int ii1 = 0; ii1 < intree_u->GetEntries(); ii1++)
    {
        intree_u->GetEntry(ii1);
        Prompt_U.push_back(Prompt_Energy);
    };
    for(int ii1 = 0; ii1 < intree_th->GetEntries(); ii1++)
    {
        intree_th->GetEntry(ii1);
        Prompt_Th.push_back(Prompt_Energy);
    };
//Close Files
    infile_u->Close();
    infile_th->Close();
//Compute Cov
    Double_t Sigma_U = Compute_Standard_Error(Prompt_U, Prompt_U);
    Double_t Sigma_Th = Compute_Standard_Error(Prompt_Th, Prompt_Th);
    Double_t Sigma_U_Th = Compute_Standard_Error(Prompt_U, Prompt_Th);
//Show
    std::cout << "Sigma U:" << Sigma_U << ", Sigma Th:" << Sigma_Th << std::endl;
    std::cout << "Sigma U Th:" << Sigma_U_Th << std::endl;

}

#endif