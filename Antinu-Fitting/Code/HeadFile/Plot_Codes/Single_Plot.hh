#ifndef SINGLE_PLOT_HH
#define SINGLE_PLOT_HH
//CPP
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
//ROOT
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TF1.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TCanvas.h>
//RAT
//Self-Defined

//TH1D

void SavePlot(std::string Out,TH1D *th1d, std::string LegTitle, std::string XTitle, std::string CanTitle, bool Normalized, double down, double up,  double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = (TH1D*)th1d->Clone("temp_hist");
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle.c_str(), "l");

    if(Normalized == true){f1->Scale(1.0 /f1->Integral());};
    if(up == 0)
    {
        int temp_index = f1->GetMaximumBin();
        up = 1.5 * f1->GetBinContent(temp_index);
    };
    f1->GetYaxis()->SetRangeUser(down, up);
    f1->Draw("HIST");
    f1->SetLineColor(kBlack);
    f1->SetXTitle(XTitle.c_str());
//Add Bin Width
    Double_t Bin_Width = f1->GetBinWidth(1);
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3) << Bin_Width;
    std::string Bin_Width_Annotation = "X Width:" + ss.str();
    f1->GetYaxis()->SetTitle(Bin_Width_Annotation.c_str());
//Legends and Pad
    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    //delete f1; f1 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};


//TGraph

void SavePlot(std::string Out, TGraph *graph, std::string Title, std::string XTitle, std::string YTitle)
{
    TCanvas *canvas = new TCanvas("canvas", Title.c_str(), 800, 600);
    // graph->Draw("APL");
    graph->Draw("AP");
    graph->SetMarkerColor(4);
    graph->SetMarkerSize(1);
    graph->SetMarkerStyle(21);
    graph->SetTitle(Title.c_str());
    graph->GetXaxis()->SetTitle(XTitle.c_str());
    graph->GetYaxis()->SetTitle(YTitle.c_str());
    canvas->SaveAs(Out.c_str());
    delete canvas; canvas = NULL;
};

//TF1

//Gaussian Fitting
void FitSavePlot_G(std::string Out, TH1D *th1d, std::string LegTitle, std::string XTitle, std::string CanTitle, bool Normalized, double down, double up,  double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = (TH1D*)th1d->Clone("temp_hist");
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->SetBorderSize(0); // 去掉图例的边框
    leg->SetFillStyle(0); // 将图例置于最底层
    //leg->SetEntrySeparation(1); // 设置条目之间的间隔，单位为图例的高度
    leg->SetTextSize(0.03);
    leg->AddEntry(f1, LegTitle.c_str(), "P");

    if(Normalized == true){f1->Scale(1.0/f1->Integral());};
    if(up == 0)
    {
        int temp_index = f1->GetMaximumBin();
        up = 1.5 * f1->GetBinContent(temp_index);
    };
    f1->GetYaxis()->SetRangeUser(down, up);
    f1->Draw("P E1");
    f1->SetLineColor(kBlack);
    f1->SetMarkerStyle(8);
    //f1->SetMarkerSize(2.0);
    f1->SetXTitle(XTitle.c_str());

    //Fit
    TF1 *gaussFunc = new TF1("gaussFunc", "gaus", 0, 800);
    // 设置指数分布函数的初始参数
    gaussFunc->SetParameter(0, 15);  // 初始高斯峰值
    gaussFunc->SetParameter(1, 200); // 初始高斯中心
    gaussFunc->SetParameter(2, 20);  // 初始高斯宽度
    
    f1->Fit("gaussFunc", "R");
    gaussFunc->SetLineColor(kBlue);
    gaussFunc->Draw("SAME");

    double gauss_mean = gaussFunc->GetParameter(1);
    double gauss_sigma = gaussFunc->GetParameter(2);
    double gauss_mean_error = gaussFunc->GetParError(1);
    double gauss_sigma_error = gaussFunc->GetParError(2);
    std::string fit_info = Form("#mu= %.2f #pm %.2f\n #sigma: %.2f #pm %.2f", gauss_mean, gauss_mean_error, gauss_sigma, gauss_sigma_error);
    leg->AddEntry(gaussFunc, fit_info.c_str(), "l");

    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    //delete f1; f1 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
    delete gaussFunc; gaussFunc = NULL;
};

//Expnential Fitting

void FitSavePlot_E(std::string Out,TH1D *th1d, std::string LegTitle, std::string XTitle, std::string CanTitle, bool Normalized, double down, double up, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = (TH1D*)th1d->Clone("temp_hist");
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->SetBorderSize(0); // 去掉图例的边框
    leg->SetFillStyle(0); // 将图例置于最底层
    leg->SetTextSize(0.03);
    leg->AddEntry(f1, LegTitle.c_str(), "P");

    if(Normalized == true) {f1->Scale(1.0 /f1->Integral());};
    if(up == 0)
    {
        int temp_index = f1->GetMaximumBin();
        up = 1.5 * f1->GetBinContent(temp_index);
    };
    f1->GetYaxis()->SetRangeUser(down, up);
    f1->Draw("P E1");
    f1->SetLineColor(kBlack);
    f1->SetMarkerStyle(8);
    //f1->SetMarkerSize(2.0);
    f1->SetXTitle(XTitle.c_str());
//Add Bin Width
    Double_t Bin_Width = f1->GetBinWidth(1);
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3) << Bin_Width;
    std::string Bin_Width_Annotation = "X Width:" + ss.str();
    f1->GetYaxis()->SetTitle(Bin_Width_Annotation.c_str());
//Fit
    TF1 *expFunc = new TF1("expFunc", "[0] * TMath::Exp(- x/[1])", 0, 2000);
    // 设置指数分布函数的初始参数
    expFunc->SetParameter(0, 15); // 初始幅度参数
    expFunc->SetParameter(1, 200); // 初始衰减参数
    
    f1->Fit("expFunc", "R");
    expFunc->Draw("SAME");
    expFunc->SetLineColor(kBlue);

    double mean = expFunc->GetParameter(1);
    double mean_error = expFunc->GetParError(1);
    std::string fit_info = Form("Mean= %.2f #pm %.2f\n#mu s", mean, mean_error);
    leg->AddEntry(expFunc, fit_info.c_str(), "l");

    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    //delete f1; f1 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
    delete expFunc; expFunc = NULL;
};

#endif