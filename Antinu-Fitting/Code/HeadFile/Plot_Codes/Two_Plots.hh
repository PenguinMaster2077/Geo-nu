#ifndef TWO_PLOTS_HH
#define TWO_PLOTS_HH
//CPP
#include <iostream>
//ROOT
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TCanvas.h>
//RAT
//Self-Defined

//TH1D


//TGraph
void SaveTwoPlots(std::string Out, TGraph *graph1, std::string LegTitle1, TGraph *graph2, std::string LegTitle2, std::string Title, std::string XTitle, std::string YTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TCanvas *canvas = new TCanvas("canvas", Title.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(graph1, LegTitle1.c_str(), "lp");
    leg->AddEntry(graph2, LegTitle2.c_str(), "lp");

    graph1->Draw("AP");
    graph1->SetMarkerColor(4);
    graph1->SetMarkerSize(1);
    graph1->SetMarkerStyle(21);
    graph1->SetTitle(Title.c_str());
    graph1->GetXaxis()->SetTitle(XTitle.c_str());
    graph1->GetYaxis()->SetTitle(YTitle.c_str());

    graph2->Draw("P SAME");
    graph2->SetMarkerColor(2);
    graph2->SetMarkerSize(1);
    graph2->SetMarkerStyle(21);
    graph2->SetTitle(Title.c_str());
    graph2->GetXaxis()->SetTitle(XTitle.c_str());
    graph2->GetYaxis()->SetTitle(YTitle.c_str());

    TGraph *contour = new TGraph;
    Double_t Z, Rho_2;
    for(int ii1 = 0; ii1 <= 1200; ii1++)
    {
        Z = -6000 + ii1 * 10;
        Rho_2 = 6000 * 6000 - Z * Z;
        contour->SetPoint(ii1, Rho_2, Z);
    };   
    contour->Draw("SAME");

    TGraph *contour2 = new TGraph;
    for(int ii1 = 0; ii1 <= 570 * 2; ii1++)
    {
        Z = -5700 + ii1 * 10;
        Rho_2 = 5700 * 5700 - Z * Z;
        contour2->SetPoint(ii1, Rho_2, Z);
    };   
    contour2->Draw("SAME");

    int numPoints = TMath::Min(graph1->GetN(), graph2->GetN());
    for (int i = 0; i < numPoints; ++i) {
        Double_t x1, y1, x2, y2;
        graph1->GetPoint(i, x1, y1);
        graph2->GetPoint(i, x2, y2);
        TLine *line = new TLine(x1, y1, x2, y2);
        line->SetLineStyle(2); // 设置虚线样式
        line->Draw("SAME");
    };
    

    leg->Draw("SAME");
    canvas->SaveAs(Out.c_str());
    delete canvas; canvas = NULL;
};

//TGraph


#endif