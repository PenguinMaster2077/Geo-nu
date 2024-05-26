#ifndef GEONU_HH
#define GEONU_HH
//CPP
#include <iostream>
#include <vector>
#include <cmath>
//ROOT
#include <TFile.h>
#include <TH1D.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"

class Geonu
{
public:
    //Constructor and Destructor
    //Global Point
    static Geonu *Point_Geonu_;
    //Member Functions
    static Geonu *GetGlobalPoint();
    void Initialize(Double_t Sin_Square_Theta_12, Double_t Sin_Square_Theta_13);
    void Compute_Survival_Probability();

private:
    Bool_t Is_Initialized;
    Double_t C_Sin_Square_Theta_12, C_Cos_Square_Theta_12;
    Double_t C_Sin_Square_Theta_13, C_Cos_Square_Theta_13;
    Double_t C_Survival_Probability;
    TH1D *Hist_U, *Hist_Th;
};

Geonu *Geonu::Point_Geonu_ = new Geonu();

Geonu *Geonu::GetGlobalPoint()
{
    return Point_Geonu_;
};

void Geonu::Compute_Survival_Probability()
{
    Double_t Part1 = pow(C_Cos_Square_Theta_13, 2);
    Double_t Part2 = 1 - 2 * pow(C_Sin_Square_Theta_12, 2) * pow(C_Cos_Square_Theta_12, 2);
    Double_t Part3 = pow(C_Sin_Square_Theta_12, 2);
    C_Survival_Probability = Part1 * Part2 + Part3;
}

void Geonu::Initialize(Double_t Sin_Square_Theta_12, Double_t Sin_Square_Theta_13)
{
    Is_Initialized = true;

    C_Sin_Square_Theta_12 = Sin_Square_Theta_12;
    C_Cos_Square_Theta_12 = 1 - C_Sin_Square_Theta_12;
    C_Sin_Square_Theta_13 = Sin_Square_Theta_13;
    C_Cos_Square_Theta_13 = 1 - C_Sin_Square_Theta_13;
    Compute_Survival_Probability();
}

#endif
