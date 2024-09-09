#ifndef MULTI_PLOTS_HH
#define MULTI_PLOTS_HH
//CPP
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TCanvas.h>
//RAT
//Self-Defined
std::vector<int> colors = {kBlack, kRed, kGreen, kBlue, kYellow, kMagenta, kCyan};

void Plot(std::string Out, std::vector<TH1D*> &Histograms, std::vector<std::string> &Legs, std::string XTitle, std::string CanTitle, bool Normalized, double down, double up,  double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    std::vector<TH1D*> copy_his;
    TH1D* temp_hist;
    for(int ii1 = 0; ii1 < Histograms.size(); ii1++)
    {
        std::string name = "temp_" + Legs.at(ii1);
        copy_his.push_back((TH1D*) Histograms.at(ii1)->Clone(name.c_str() ));
    }
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    for(int ii1 = 0; ii1 < Histograms.size(); ii1++)
    {
        leg->AddEntry(copy_his.at(ii1), Legs.at(ii1).c_str(), "l");
    }
//Normalize
    if(Normalized == true)
    {
        for(int ii1 = 0; ii1 < copy_his.size(); ii1++)
        {
                
            copy_his.at(ii1)->Scale( 1.0 / copy_his.at(ii1)->Integral());
        };
    };
//Up limit of histogram
    if(up == 0)
    {
        int temp_index = copy_his.at(0)->GetMaximumBin();
        up = 1.5 * copy_his.at(0)->GetBinContent(temp_index);
    };
//Setup Range
    for(int ii1 = 0; ii1 < copy_his.size(); ii1++)
    {
        copy_his.at(ii1)->GetYaxis()->SetRangeUser(down, up);
    };
//Setup Draw
    for(int ii1 = 0; ii1 < copy_his.size(); ii1++)
    {
        if(ii1 == 0)
        {
            copy_his.at(ii1)->Draw("HIST");
        }
        else
        {
            copy_his.at(ii1)->Draw("HISTSAME");
        };
    };
//Setup Line Color
    for(int ii1 = 0; ii1 < copy_his.size(); ii1++)
    {
        copy_his.at(ii1)->SetLineColor(colors.at(ii1));
    };
//Setup XTitle
    for(int ii1 = 0; ii1 < copy_his.size(); ii1++)
    {
        copy_his.at(ii1)->SetXTitle(XTitle.c_str());
    }
//Add Bin Width
    Double_t Bin_Width = copy_his.at(0)->GetBinWidth(1);
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3) << Bin_Width;
    std::string Bin_Width_Annotation = "X Width:" + ss.str();
    for(int ii1 = 0; ii1 < copy_his.size(); ii1++)
    {
        copy_his.at(ii1)->GetYaxis()->SetTitle(Bin_Width_Annotation.c_str());
    };
//Legends and Pad
    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    //delete f1; f1 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
}
#endif