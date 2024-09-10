#ifndef GRID_SCAN_PLOT_HH
#define GRID_SCAN_PLOT_HH
//CPP
#include <iostream>
#include <vector>
#include <iomanip>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TGraph2D.h>
//RAT
//Self-Defined
#include "../Constants_Setting.hh"

TH2D* Grid_Scan_Plot_1D_Geo_Ratio(std::string InFile, TH2D *Example_Hist, Bool_t Delta = false)
//Delta = true: Plot Delta Fitting Value
//Delta = false: Plot Fitting Value
{
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
//Set Branch Address
    Double_t number_reactor, number_geonu, number_alphan_ground, number_alphan_exicted;
    intree->SetBranchAddress("NumberReactor", &number_reactor);
    intree->SetBranchAddress("NumberGeonu", &number_geonu);
    intree->SetBranchAddress("NumberAlphaNGround", &number_alphan_ground);
    intree->SetBranchAddress("NumberAlphaNExicted", &number_alphan_exicted);
    Double_t ratio_u_th;
    intree->SetBranchAddress("RatioUTh", &ratio_u_th);
    Double_t fitting_value;
    intree->SetBranchAddress("FittingValue", &fitting_value);
    Int_t method;
    intree->SetBranchAddress("Method", &method);
//Load Data
    std::vector<Double_t> fitting_values;
    std::vector<Double_t> x_values;
    Double_t min_fitting_value, max_fitting_value;
    Int_t best_index;
    for(int index = 0; index < intree->GetEntries(); index++)
    {
        intree->GetEntry(index);
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
};

void Grid_Scan_Plot_1D_Geo_Ratio(std::string InFile)
{
//Set Output Message
    std::cout << std::fixed << std::setprecision(10);
//Read File
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
//Set Branch Address
    Double_t number_reactor, number_geonu, number_alphan_ground, number_alphan_exicted;
    intree->SetBranchAddress("NumberReactor", &number_reactor);
    intree->SetBranchAddress("NumberGeonu", &number_geonu);
    intree->SetBranchAddress("NumberAlphaNGround", &number_alphan_ground);
    intree->SetBranchAddress("NumberAlphaNExicted", &number_alphan_exicted);
    Double_t ratio_u_th;
    intree->SetBranchAddress("RatioUTh", &ratio_u_th);
    Double_t error_reactor, error_geonu, error_alphan_ground, error_alphan_exicted, error_ratio;
    intree->SetBranchAddress("ErrorReactor", &error_reactor);
    intree->SetBranchAddress("ErrorGeonu", &error_geonu);
    intree->SetBranchAddress("ErrorAlphaNGround", &error_alphan_ground);
    intree->SetBranchAddress("ErrorAlphaNExicted", &error_alphan_exicted);
    intree->SetBranchAddress("ErrorRatioUTh", &error_ratio);
    Double_t fitting_value;
    intree->SetBranchAddress("FittingValue", &fitting_value);
    Int_t method;
    intree->SetBranchAddress("Method", &method);
//Load Data
    std::vector<Double_t> fitting_values;
    std::vector<Double_t> numbers_reactor, numbers_geonu, numbers_alphan_ground, numbers_alphan_exicted, ratios_u_th;
    std::vector<Double_t> errors_reactor, errors_geonu, errors_alphan_ground, errors_alphan_exicted, errors_ratio;
    std::vector<Double_t> x_values;
    Double_t min_fitting_value, max_fitting_value;
    Int_t best_index;
    for(int index = 0; index < intree->GetEntries(); index++)
    {
        intree->GetEntry(index);
        fitting_values.push_back(fitting_value);
        x_values.push_back(ratio_u_th);
        //Load
        numbers_reactor.push_back(number_reactor);
        numbers_geonu.push_back(number_geonu);
        numbers_alphan_ground.push_back(number_alphan_ground);
        numbers_alphan_exicted.push_back(number_alphan_exicted);
        ratios_u_th.push_back(ratio_u_th);
        errors_reactor.push_back(error_reactor);
        errors_geonu.push_back(error_geonu);
        errors_alphan_ground.push_back(error_alphan_ground);
        errors_alphan_exicted.push_back(error_alphan_exicted);
        errors_ratio.push_back(error_ratio);
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
        if(ratio_u_th > 200) {break;};
        //Test Part
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
            delta_fitting_values.push_back( 2.0 * temp_res );
        }
        else if(method == 2)
        {
            delta_fitting_values.push_back( temp_res );
        };
    };
//Fill into Histogram
    TCanvas *can = new TCanvas("can", "", 800, 600);
    TGraph *Hist  = new TGraph(x_values.size(), x_values.data(), delta_fitting_values.data());
    Hist->GetXaxis()->SetTitle("U/Th");
    if(method == 1)//Extended Maximum Likelihood
    {
        Hist->GetYaxis()->SetTitle("#Delta#chi^{2}=-2#DeltaLogL");
    }
    else if(method == 2)//Chi Square
    {
        Hist->GetYaxis()->SetTitle("#Delta#chi^{2}");
        can->SetLogy();
    }
    Hist->Draw("AL");

//Mark Best Point
    // TLine *horizon_line = new TLine(x_values.at(0), delta_fitting_values.at(best_index), x_values.at(x_values.size() - 1), delta_fitting_values.at(best_index));
    // horizon_line->SetLineStyle(2);//Dashed
    // horizon_line->Draw();
    TLine *vertical_line = new TLine(x_values.at(best_index), 0, x_values.at(best_index), 0.6);
    vertical_line->SetLineStyle(2);//Dashed
    vertical_line->SetLineColor(kRed);
    vertical_line->Draw();
    can->Update();
//Save File
    std::string OutFile;
    if(method == 1)//Extended Maximum Likelihood
    {
        OutFile = "./Pic/Grid_Scan_Geo_Ratio_Extended_Maximum_Likelihood.pdf";
    }
    else if(method == 2)
    {
        OutFile = "./Pic/Grid_Scan_Geo_Ratio_Chi_Square.pdf";
    };
    can->SaveAs(OutFile.c_str());
    delete Hist; Hist = nullptr;
    delete vertical_line; vertical_line = nullptr;
    delete can; can = nullptr;
//Details of Best Fit
    if(method == 1)
    {
        std::cout << "Method: Extended Maximum Likelihood" << std::endl;
    }
    else if(method == 2)
    {
        std::cout << "Method: Chi-Square" << std::endl;
    };
    std::cout << "max:" << max_fitting_value << ", min:" << min_fitting_value << std::endl; 
    std::cout << "Details of the Best Point:" << std::endl;
    std::cout << "Index:" << best_index << std::endl;
    std::cout << "Reactor:" << numbers_reactor.at(best_index) << "+-" << errors_reactor.at(best_index) << std::endl;
    std::cout << "Geonu:" << numbers_geonu.at(best_index) << "+-" << errors_geonu.at(best_index) << std::endl;
    std::cout << "Geo Ratio:" << ratios_u_th.at(best_index) << "+-" << errors_ratio.at(best_index) << std::endl;
    std::cout << "AlphaN Ground:" << numbers_alphan_ground.at(best_index) << "+-" << errors_alphan_ground.at(best_index) << std::endl;
    std::cout << "AlphaN Exicted:" << numbers_alphan_exicted.at(best_index) << "+-" << errors_alphan_exicted.at(best_index) << std::endl;
}

void Grid_Scan_Plot_2D_Geonu_Reactor(std::string InFile, std::string OutFile = "./Pic/Grid_Scan_Geonu_Reactor_Binned_Extended_Maximum_Likelihood.pdf")
//还没完善，需要补充Chi-Square画图部分
{
    TFile *infile = new TFile(InFile.c_str());
    TTree *intree = (TTree*) infile->Get("output");
//Set Branch Address
    Double_t number_reactor, number_geonu, number_alphan_ground, number_alphan_exicted;
    intree->SetBranchAddress("NumberReactor", &number_reactor);
    intree->SetBranchAddress("NumberGeonu", &number_geonu);
    intree->SetBranchAddress("NumberAlphaNGround", &number_alphan_ground);
    intree->SetBranchAddress("NumberAlphaNExicted", &number_alphan_exicted);
    Double_t ratio_u_th;
    intree->SetBranchAddress("RatioUTh", &ratio_u_th);
    Double_t error_reactor, error_geonu, error_alphan_ground, error_alphan_exicted, error_ratio;
    intree->SetBranchAddress("ErrorReactor", &error_reactor);
    intree->SetBranchAddress("ErrorGeonu", &error_geonu);
    intree->SetBranchAddress("ErrorAlphaNGround", &error_alphan_ground);
    intree->SetBranchAddress("ErrorAlphaNExicted", &error_alphan_exicted);
    intree->SetBranchAddress("ErrorRatioUTh", &error_ratio);
    Double_t fitting_value;
    intree->SetBranchAddress("FittingValue", &fitting_value);
    Int_t method;
    intree->SetBranchAddress("Method", &method);
//Load Data
    std::vector<Double_t> fitting_values;
    std::vector<Double_t> numbers_reactor, numbers_geonu, numbers_alphan_ground, numbers_alphan_exicted, ratios_u_th;
    std::vector<Double_t> errors_reactor, errors_geonu, errors_alphan_ground, errors_alphan_exicted, errors_ratio;
    std::vector<Double_t> x_values, y_values;
    Double_t min_fitting_value, max_fitting_value;
    Int_t best_index;
    for(int index = 0; index < intree->GetEntries(); index++)
    {
        intree->GetEntry(index);
        fitting_values.push_back(fitting_value);
        x_values.push_back(number_geonu);
        y_values.push_back(number_reactor);
        //Load
        numbers_reactor.push_back(number_reactor);
        numbers_geonu.push_back(number_geonu);
        numbers_alphan_ground.push_back(number_alphan_ground);
        numbers_alphan_exicted.push_back(number_alphan_exicted);
        ratios_u_th.push_back(ratio_u_th);
        errors_reactor.push_back(error_reactor);
        errors_geonu.push_back(error_geonu);
        errors_alphan_ground.push_back(error_alphan_ground);
        errors_alphan_exicted.push_back(error_alphan_exicted);
        errors_ratio.push_back(error_ratio);
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
            delta_fitting_values.push_back(2.0 * temp_res );
        }
        else if(method == 2)
        {
            delta_fitting_values.push_back( temp_res );
        };
    };
//Fill into Histogram
    TCanvas *can = new TCanvas("can", "", 800, 600);
    TH2D *Hist = new TH2D("Example", "", 60, 0, 30, 100, 10, 60);
    for(int index = 0; index < delta_fitting_values.size(); index++)
    {
        Hist->Fill(x_values.at(index), y_values.at(index), delta_fitting_values.at(index));
    };
    Double_t countour[3] = {GRID_SCAN_2D_1SIGMA, GRID_SCAN_2D_2SIGMA, GRID_SCAN_2D_3SIGMA};
    Hist->SetContour(3, countour);
    Hist->Draw("CONT1");
    Hist->GetXaxis()->SetTitle("N_{Geo-nu}");
    Hist->GetYaxis()->SetTitle("N_{Reactor}");
    Hist->SetTitle("#Delta#chi^{2} = -2#DeltaLogL");
    gStyle->SetOptStat(false);
    gStyle->SetOptFit(0);
//Mark Best Point
    TMarker *marker = new TMarker(x_values.at(best_index), y_values.at(best_index), 29);
    marker->SetMarkerSize(2);
    marker->Draw();
    //Mark Horizon and Vertical Line
    TLine *horizon_line = new TLine(0, y_values.at(best_index), 30, y_values.at(best_index));
    horizon_line->SetLineStyle(2);//Dashed
    horizon_line->Draw();
    TLine *vertical_line = new TLine(x_values.at(best_index), 10, x_values.at(best_index), 60);
    vertical_line->SetLineStyle(2);//Dashed
    vertical_line->Draw();
    can->Update();
//Save
    can->SaveAs(OutFile.c_str());
//Details of Best Fit
    std::cout << "max:" << max_fitting_value << ", min:" << min_fitting_value << std::endl; 
    std::cout << "Details of the Best Point:" << std::endl;
    std::cout << "Reactor:" << numbers_reactor.at(best_index) << "+-" << errors_reactor.at(best_index) << std::endl;
    std::cout << "Geonu:" << numbers_geonu.at(best_index) << "+-" << errors_geonu.at(best_index) << std::endl;
    std::cout << "Geo Ratio:" << ratios_u_th.at(best_index) << "+-" << errors_ratio.at(best_index) << std::endl;
    std::cout << "AlphaN Ground:" << numbers_alphan_ground.at(best_index) << "+-" << errors_alphan_ground.at(best_index) << std::endl;
    std::cout << "AlphaN Exicted:" << numbers_alphan_exicted.at(best_index) << "+-" << errors_alphan_exicted.at(best_index) << std::endl;
//Test Part

}

#endif