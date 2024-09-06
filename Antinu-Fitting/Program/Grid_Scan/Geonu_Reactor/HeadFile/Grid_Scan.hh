#ifndef GRID_SCAN_HH
#define GRID_SCAN_HH
//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph2D.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"
#include "./Fitter.hh"
#include "./Fit_Parameters.hh"

//Example Histogram
TH1D *Example_Hist = new TH1D("Example", "", 20, 0.8, 8.0);
void Initialize()
{
//Models
    Geonu *geonu = Geonu::Get_Global_Point();
    Reactor *reactor = Reactor::Get_Global_Point();
    AN *an = AN::Get_Global_Point();

    geonu->Setup_PDFs(Example_Hist);
    reactor->Setup_PDFs(Example_Hist);
    an->Setup_PDFs(Example_Hist);
//Fit Parameters
    FitParameters *fit_par = FitParameters::Get_Global_Point();
    fit_par->Compute_Events(145.17, 300733, 309277);
};
TH2D* Grid_Scan_1D(Double_t Input_Min, Double_t Input_Max, Int_t Steps, std::string Constant_Name, Double_t &Best_Value)
{
//Fit Parameters
    FitParameters *fit_par = FitParameters::Get_Global_Point();
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();
    std::string Method = fitter->Get_Fit_Method();
//Scan Setting
    Double_t interval = (Input_Max - Input_Min) / Steps;
    Double_t number;
    std::vector<Double_t> fitting_values; // -LogL or chi-square
    std::vector<std::string> Names;
    std::vector<Double_t> Values;
    Names.push_back(Constant_Name);
    Values.push_back(0);
    for(int ii1 = 0; ii1 < Steps; ii1++)
    {
        number = Input_Min + interval * ii1;
        Values.at(0) = number;
        fit_par->Compute_Events(145.17, 300733, 309277, Names, Values);
        fitter->Initialize();
        fitter->Initialize_Hist(Example_Hist);
        fitter->Load_Data(DATA_IBD_CANDIDATE, true);
        fitter->Fitting();
        if( Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
        {
            fitting_values.push_back( - fitter->Get_Fitting_Variable());
        }
        else if( Method == FITTER_CHI_SQUARE)
        {
            fitting_values.push_back( fitter->Get_Fitting_Variable());
        };
    };
//Find Maxnium and Minium Value
    Int_t min_index = 0;
    Double_t min_fitting_value = fitting_values.at(0);
    Double_t max_fitting_value = fitting_values.at(0);
    for(int ii1 = 0; ii1 < fitting_values.size(); ii1++)
    {
        if( min_fitting_value > fitting_values.at(ii1))
        {
            min_fitting_value = fitting_values.at(ii1);
            min_index = ii1;
        };
        if(max_fitting_value < fitting_values.at(ii1))
        {
            max_fitting_value = fitting_values.at(ii1);
        };
    };
//Compute -2\Delta LogL or \Delta chi-square
    TH2D *Hist = new TH2D("temp", "", Steps, Input_Min, Input_Max, 100, 0, max_fitting_value - min_fitting_value + 1);
    Double_t temp_res;
    for(int ii1 = 0; ii1 < fitting_values.size(); ii1++)
    {
        number = Input_Min + interval * ii1;
        if(min_index == ii1) { Best_Value = number; }; // Record the X-value of the best fit point
        temp_res = fitting_values.at(ii1) - min_fitting_value;
        if(Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
        {
            Hist->Fill(number, 2.0 * temp_res);
        }
        else if( Method == FITTER_CHI_SQUARE)
        {
            Hist->Fill(number, temp_res);
        }
    };
//Y-axis Title
    if(Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
    {
        Hist->GetYaxis()->SetTitle("-2#Delta LogL");
    }
    else if(Method == FITTER_CHI_SQUARE)
    {
        Hist->GetYaxis()->SetTitle("#Delta #chi^{2}");
    };
//Return
    return Hist;
}
// Geo-nu Grid Scan
void Scan_U_Th_Ratio(Double_t Input_Min, Double_t Input_Max, Int_t Steps)
{
//Initialization
    Initialize();
//Fit Parameters
    FitParameters *fit_par = FitParameters::Get_Global_Point();
    fit_par->Compute_Events(145.17, 300733, 309277);
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();
//Scan Setting
    Double_t interval = (Input_Max - Input_Min) / Steps;
    Double_t ratio;
    Double_t res;
    std::vector<Double_t> LogL;
    std::vector<std::string> Names;
    std::vector<Double_t> Values, Errors;
    Names.push_back(NAME_GEO_RATIO);
    Values.push_back(0);
    for(int ii1 = 0; ii1 < Steps; ii1++)
    {
        ratio = Input_Min + interval * ii1;
        Values.at(0) = ratio;
        fit_par->Compute_Events(145.17, 300733, 309277, Names, Values);
        fitter->Initialize();
        fitter->Initialize_Hist(Example_Hist);
        fitter->Load_Data(DATA_IBD_CANDIDATE, true);
        fitter->Fitting();
        LogL.push_back(- fitter->Get_Fitting_Variable());
    };
//Find Maxinum and Mininum Value
    Int_t min_index = 0;
    Double_t Best_ratio = 0;
    Double_t Max_LogL = LogL.at(0);
    Double_t Min_LogL = LogL.at(0);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        if( Min_LogL > LogL.at(ii1))
        {
            Min_LogL = LogL.at(ii1);
            min_index = ii1;
        };
        if(Max_LogL < LogL.at(ii1))
        {
            Max_LogL = LogL.at(ii1);
        }
    };
//Compute \Delta LogL
    TH2D *Hist = new TH2D("U/Th Ratio", "", Steps, Input_Min, Input_Max, 100, 0, Max_LogL - Min_LogL);
    for(int ii1 = 0; ii1 < LogL.size(); ii1++)
    {
        ratio = Input_Min + interval * ii1;
        if(min_index == ii1) { Best_ratio = ratio; };
        Hist->Fill(ratio, LogL.at(ii1) - Min_LogL);
    };
    Hist->Draw();
    std::cout << "Complete! Best:" << Best_ratio << std::endl;
};

void Scan_Geonu_Number(Double_t Input_Min, Double_t Input_Max, Int_t Steps, std::string Method = FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
{
    gStyle->SetOptStat(0);
//Initialization
    Initialize();
//Fit Parameters
    FitParameters *fit_par = FitParameters::Get_Global_Point();
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();
    fitter->Set_Fitting_Method(Method);
//Compute -2\Delta LogL or \Delta chi-square
    Double_t Best_Value;
    TH2D *Hist = Grid_Scan_1D(Input_Min, Input_Max, Steps, NAME_GEO, Best_Value);
    Hist->SetTitle("Grid Scan of Number of Geo-nu");
//X-axis Title
    Hist->GetXaxis()->SetTitle(" N_{Geo-nu}");
//Draw
    Hist->Draw();
    std::cout << "Complete! Best Fit:" << Best_Value << std::endl;
};

void Scan_Reactor_Number(Double_t Input_Min, Double_t Input_Max, Int_t Steps, std::string Method = FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
{
    gStyle->SetOptStat(0);
//Initialization
    Initialize();
//Fit Parameters
    FitParameters *fit_par = FitParameters::Get_Global_Point();
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();
    fitter->Set_Fitting_Method(Method);
//Compute -2\Delta LogL or \Delta chi-square
    Double_t Best_Value;
    TH2D *Hist = Grid_Scan_1D(Input_Min, Input_Max, Steps, NAME_REACTOR, Best_Value);
//X-axis Title
    Hist->GetXaxis()->SetTitle(" N_{Reactor}");
//Draw
    Hist->Draw();
    std::cout << "Complete! Best Fit:" << Best_Value << std::endl;
};

void Scan_Geonu_Reactor(std::vector<Double_t> X_Values, std::vector<Double_t> Y_Values, std::vector<Int_t>Steps, std::string Method = FITTER_EXTENDED_MAXIMUM_LIKELIHOOD, Bool_t Create_New_File = false)
{
//Open or Create File
    TFile *file;
    TTree *tree;
    Double_t number_reactor, number_geonu, number_alphan_ground, number_alphan_exicted;
    Double_t ratio_u_th;    
    Double_t error_reactor, error_geonu, error_alphan_ground, error_alphan_exicted, error_u_th;    
    Double_t fitting_value;
    Int_t method; //1->ELL; 2->Chi-square
    if(Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
    {
        method = 1;
    }
    else
    {
        method = 2;
    }
    if(Create_New_File == false)
    {
        std::cout << "False!!!!!!!!!!!!!!!" << std::endl;
        if( Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
        {
            file = new TFile(GRID_SCAN_GEONU_REACTOR_EXTENDED_MAXIMUM_LIKELIHOOD.c_str(), "update");
        }
        else
        {
            file = new TFile(GRID_SCAN_GEONU_REACTOR_CHI_SQUARE.c_str(), "update");
        };
        tree = (TTree*) file->Get("output");
        tree->SetBranchAddress("NumberReactor", &number_reactor);
        tree->SetBranchAddress("NumberGeonu", &number_geonu);
        tree->SetBranchAddress("NumberAlphaNGround", &number_alphan_ground);
        tree->SetBranchAddress("NumberAlphaNExicted", &number_alphan_exicted);
        tree->SetBranchAddress("RatioUTh", &ratio_u_th);
        tree->SetBranchAddress("ErrorReactor", &error_reactor);
        tree->SetBranchAddress("ErrorGeonu", &error_geonu);
        tree->SetBranchAddress("ErrorAlphaNGround", &error_alphan_ground);
        tree->SetBranchAddress("ErrorAlphaNExicted", &error_alphan_exicted);
        tree->SetBranchAddress("ErrorRatioUTh", &error_u_th);
        tree->SetBranchAddress("FittingValue", &fitting_value);
        tree->SetBranchAddress("Method", &method);
    }
    else
    {
        std::cout << "True!!!!!!!!!!!!!!!!!" << std::endl;
        if( Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
        {
            file = new TFile(GRID_SCAN_GEONU_REACTOR_EXTENDED_MAXIMUM_LIKELIHOOD.c_str(), "recreate");
        }
        else
        {
            file = new TFile(GRID_SCAN_GEONU_REACTOR_CHI_SQUARE.c_str(), "recreate");
        };
        tree = new TTree("output", "");
        tree->Branch("NumberReactor", &number_reactor);
        tree->Branch("NumberGeonu", &number_geonu);
        tree->Branch("NumberAlphaNGround", &number_alphan_ground);
        tree->Branch("NumberAlphaNExicted", &number_alphan_exicted);
        tree->Branch("RatioUTh", &ratio_u_th);
        tree->Branch("ErrorReactor", &error_reactor);
        tree->Branch("ErrorGeonu", &error_geonu);
        tree->Branch("ErrorAlphaNGround", &error_alphan_ground);
        tree->Branch("ErrorAlphaNExicted", &error_alphan_exicted);
        tree->Branch("ErrorRatioUTh", &error_u_th);
        tree->Branch("FittingValue", &fitting_value);
        tree->Branch("Method", &method);
    };
//Initialization
    Initialize();
//Fit Parameters
    FitParameters *fit_par = FitParameters::Get_Global_Point();
//Fitter
    Fitter *fitter = Fitter::Get_Global_Point();
    fitter->Set_Fitting_Method(Method);
//Compute -2\Delta LogL or \Delta chi-square
    std::vector<Double_t> Best_Values;
//Scan Setting
    Int_t X_Steps = Steps.at(0), Y_Steps = Steps.at(1);
    Double_t X_number, Y_number;
    Double_t X_min = X_Values.at(0), X_max = X_Values.at(1);
    Double_t Y_min = Y_Values.at(0), Y_max = Y_Values.at(1);
    std::vector<Double_t> intervals;
    Double_t X_interval = (X_max - X_min) / X_Steps;
    Double_t Y_interval = (Y_max - Y_min) / Y_Steps;
    std::vector<std::vector<Double_t>> fitting_values(X_Steps, std::vector<Double_t>(Y_Steps)); // -LogL or chi-square
    std::vector<std::string> Names;
    Names.push_back(NAME_GEO);
    Names.push_back(NAME_REACTOR);
    std::vector<Double_t> Values;
    Values.push_back(0);
    Values.push_back(0);
    for(int index_x = 0; index_x < X_Steps; index_x++)
    {
        X_number = X_min + X_interval * index_x;
        for(int index_y = 0; index_y < Y_Steps; index_y++)
        {
            Y_number = Y_min + Y_interval * index_y;
            Values.at(0) = X_number;
            Values.at(1) = Y_number;
            fit_par->Compute_Events(145.17, 300733, 309277, Names, Values);
            fitter->Initialize();
            fitter->Initialize_Hist(Example_Hist);
            fitter->Load_Data(DATA_IBD_CANDIDATE, true);
            fitter->Fitting();
            if( Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
            {
                fitting_values.at(index_x).at(index_y) = - fitter->Get_Fitting_Variable();
                fitting_value = - fitter->Get_Fitting_Variable();
            }
            else if( Method == FITTER_CHI_SQUARE)
            {
                fitting_values.at(index_x).at(index_y) = fitter->Get_Fitting_Variable();
                fitting_value = fitter->Get_Fitting_Variable();
            };
            //Record
            number_reactor = fit_par->Get_Value(NAME_REACTOR);
            number_geonu = fit_par->Get_Value(NAME_GEO);
            number_alphan_ground = fit_par->Get_Value(NAME_AN_GROUND);
            number_alphan_exicted = fit_par->Get_Value(NAME_AN_EXICTED);
            ratio_u_th = fit_par->Get_Value(NAME_GEO_RATIO);
            error_reactor = fit_par->Get_Error(NAME_REACTOR);
            error_geonu = fit_par->Get_Error(NAME_GEO);
            error_alphan_ground = fit_par->Get_Error(NAME_AN_GROUND);
            error_alphan_exicted = fit_par->Get_Error(NAME_AN_EXICTED);
            error_u_th = fit_par->Get_Error(NAME_GEO_RATIO);
            tree->Fill();
        };
    };
    file->Write();
    file->Close();
//Find Best Value
    // Double_t best_x, best_y, min_fitting_value, max_fitting_value;
    // min_fitting_value  = max_fitting_value = fitting_values.at(0).at(0);
    // Double_t temp_res;
    // for(int index_x = 0; index_x < X_Steps; index_x++)
    // {
    //     for(int index_y = 0; index_y < Y_Steps; index_y++)
    //     {
    //         temp_res = fitting_values.at(index_x).at(index_y);
    //         if(min_fitting_value > temp_res)
    //         {
    //             min_fitting_value = temp_res;
    //             best_x = X_min + index_x * X_interval;
    //             best_y = Y_min + index_y * Y_interval;
    //         };
    //         if(max_fitting_value < temp_res)
    //         {
    //             max_fitting_value = temp_res;
    //         };
    //     };
    // };
    // std::cout << "Best value is Geonu:" << best_x << ", Reactor:" << best_y << ", Maximum Delta fitting value:" << max_fitting_value - min_fitting_value << std::endl;
    // std::cout << "Max:" << max_fitting_value << ", Min:" << min_fitting_value << std::endl;
//Compute -2\Delta LogL or \Delta chi-square
    // std::vector<std::vector<Double_t>> Delta_fitting_Values(X_Steps, std::vector<Double_t>(Y_Steps));
    // for(int index_x = 0; index_x < X_Steps; index_x++)
    // {
    //     for(int index_y = 0; index_y < Y_Steps; index_y++)
    //     {
    //         temp_res = fitting_values.at(index_x).at(index_y) - min_fitting_value;
    //         if(Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
    //         {
    //             Delta_fitting_Values.at(index_x).at(index_y) = - 2.0 * temp_res;
    //         }
    //         else if(Method == FITTER_CHI_SQUARE)
    //         {
    //             Delta_fitting_Values.at(index_x).at(index_y) = temp_res;
    //         }
    //     }
    // };
//Draw
    // TCanvas *can = new TCanvas("c1", "", 800, 600);
    // gStyle->SetOptStat(0);
    // TGraph2D *graph = new TGraph2D();
    // Int_t point = 0;
    // for(int index_x = 0; index_x < X_Steps; index_x++)
    // {
    //     X_number = X_min + index_x * X_interval;
    //     for(int index_y = 0; index_y < Y_Steps; index_y++)
    //     {
    //         Y_number = Y_min + index_y * Y_interval;
    //         graph->SetPoint(point, X_number, Y_number, Delta_fitting_Values.at(index_x).at(index_y));
    //         point++;
    //     };
    // };

    // if(Method == FITTER_EXTENDED_MAXIMUM_LIKELIHOOD)
    // {
    //     graph->SetTitle("N_{Geo-nu}-N_{Reactor} Grid Scan with Extended LL");
    // }
    // else if(Method == FITTER_CHI_SQUARE)
    // {
    //     graph->SetTitle("N_{Geo-nu}-N_{Reactor} Grid Scan with Chi-square");
    // };
    // graph->GetHistogram()->GetXaxis()->SetTitle("N_{Geo-nu}");
    // graph->GetHistogram()->GetYaxis()->SetTitle("N_{Reactor}");

    // graph->Draw("COLZ");
    //Double_t contours[2] = {GRID_SCAN_2D_1SIGMA, GRID_SCAN_2D_2SIGMA};
    //graph->SetContour(2, contours);
//Find 1\sigma and 2\sigma countor
    // std::vector<Double_t> x_1sigma, x_2sigma;
    // std::vector<Double_t> y_1sigma, y_2sigma;
    // TGraph *contour_1sigma = new TGraph();
    // Int_t n1 = 0;
    // for(int index_x = 0; index_x < X_Steps; index_x++)
    // {
    //     X_number = X_min + index_x * X_interval;
    //     for(int index_y = 0; index_y < Y_Steps; index_y++)
    //     {
    //         Y_number = Y_min + index_y * Y_interval;
    //         temp_res = Delta_fitting_Values.at(index_x).at(index_y);
    //         if( temp_res <= GRID_SCAN_2D_1SIGMA + 0.1 && temp_res >= GRID_SCAN_2D_1SIGMA - 0.1)
    //         {
    //             x_1sigma.push_back(X_number);
    //             y_1sigma.push_back(Y_number);
    //             contour_1sigma->SetPoint(n1, X_number, Y_number);
    //             n1++;
    //         };
    //         if( temp_res <= GRID_SCAN_2D_2SIGMA)
    //         {
    //             x_2sigma.push_back(X_number);
    //             y_2sigma.push_back(Y_number);
    //         };
    //     };
    // };
    //contour_1sigma->SetLineColor(kRed);
    //contour_1sigma->SetLineStyle(1);
    //contour_1sigma->SetLineWidth(2);
    //contour_1sigma->Draw("P SAME");
    //can->Draw();
    //can->Update();
    //can->SaveAs("./test.png");
}


#endif