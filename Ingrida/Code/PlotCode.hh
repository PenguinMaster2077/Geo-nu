//CPP
#include <iostream>
//ROOT
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TPad.h>
#include <TVector3.h>
//TH1D-单个图表
void SavePlot(std::string Out,TH1D *th1d, std::string LegTitle
,std::string XTitle,
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle.c_str(), "l");
    f1->Draw("HIST");
    f1->SetLineColor(kBlack);
    //f1->Scale(1.0 /f1->Integral());
    //f1->GetYaxis()->SetRangeUser(down, up);
    f1->SetXTitle(XTitle.c_str());
    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    //delete f1; f1 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};

void SavePlot(std::string Out,TH1D *th1d, std::string LegTitle
,std::string XTitle, double down, double up, 
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle.c_str(), "l");
    f1->Draw("HIST");
    f1->SetLineColor(kBlack);
    f1->Scale(1.0 /f1->Integral());
    f1->GetYaxis()->SetRangeUser(down, up);
    f1->SetXTitle(XTitle.c_str());
    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    //delete f1; f1 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};
void SavePlot(std::string Out, TGraph *graph, std::string Title, std::string XTitle, std::string YTitle)
{
    TCanvas *canvas = new TCanvas("canvas", Title.c_str(), 800, 600);
    graph->Draw("APL");
    graph->SetTitle(Title.c_str());
    graph->GetXaxis()->SetTitle(XTitle.c_str());
    graph->GetYaxis()->SetTitle(YTitle.c_str());
    canvas->SaveAs(Out.c_str());
    delete canvas; canvas = NULL;
}
//TH1D-两个图标
void SaveTwoPlot(std::string Out,TH1D *th1d, std::string LegTitle, TH1D *th2d, std::string LegTitle2,
std::string XTitle,  
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TH1D *f2 = th2d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle.c_str(), "l");
    leg->AddEntry(f2, LegTitle2.c_str(), "l");

    f1->Draw("HIST");
    f2->Draw("HISTSAME");

    f1->SetLineColor(kBlack);
    f2->SetLineColor(kGreen);

    f1->SetXTitle(XTitle.c_str());
    f2->SetXTitle(XTitle.c_str());

    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    delete f1; f1 = NULL;
    delete f2; f2 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};

void SaveTwoPlot(std::string Out,TH1D *th1d, std::string LegTitle, TH1D *th2d, std::string LegTitle2,
std::string XTitle, double down, double up,   
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TH1D *f2 = th2d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle.c_str(), "l");
    leg->AddEntry(f2, LegTitle2.c_str(), "l");

    f1->Draw("HIST");
    f2->Draw("HISTSAME");

    f1->SetLineColor(kBlack);
    f2->SetLineColor(kGreen);

    f1->Scale(1.0 /f1->Integral());
    f2->Scale(1.0 /f2->Integral());

    f1->GetYaxis()->SetRangeUser(down, up);
    f2->GetYaxis()->SetRangeUser(down, up);

    f1->SetXTitle(XTitle.c_str());
    f2->SetXTitle(XTitle.c_str());

    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    delete f1; f1 = NULL;
    delete f2; f2 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};
void SaveFourPlot(const std::string Out, TH1D *th1d, std::string LegTitle1, TH1D *th2d, std::string LegTitle2,
TH1D *th3d, std::string LegTitle3, TH1D *th4d, std::string LegTitle4, std::string XTitle, double down, double up,
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TH1D *f2 = th2d;
    TH1D *f3 = th3d;
    TH1D *f4 = th4d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle1.c_str(), "l");
    leg->AddEntry(f2, LegTitle2.c_str(), "l");
    leg->AddEntry(f3, LegTitle3.c_str(), "l");
    leg->AddEntry(f4, LegTitle4.c_str(), "l");

    f1->Draw("HIST");
    f2->Draw("HISTSAME");
    f3->Draw("HISTSAME");
    f4->Draw("HISTSAME");

    f1->SetLineColor(kBlack);
    f2->SetLineColor(kGreen);
    f3->SetLineColor(kRed);
    f4->SetLineColor(kBlue);

    f1->Scale(1.0 /f1->Integral());
    f2->Scale(1.0 /f2->Integral());
    f3->Scale(1.0 /f3->Integral());
    f4->Scale(1.0 /f4->Integral());

    f1->GetYaxis()->SetRangeUser(down, up);
    f2->GetYaxis()->SetRangeUser(down, up);
    f3->GetYaxis()->SetRangeUser(down, up);
    f4->GetYaxis()->SetRangeUser(down, up);

    f1->SetXTitle(XTitle.c_str());
    f2->SetXTitle(XTitle.c_str());
    f3->SetXTitle(XTitle.c_str());
    f4->SetXTitle(XTitle.c_str());

    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    delete f1; f1 = NULL;
    delete f2; f2 = NULL;
    delete f3; f3 = NULL;
    delete f4; f4 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};
//TH2D
void SavePlot(std::string Out,TH2D *th2d, std::string LegTitle,
std::string XTitle, std::string YTitle,
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH2D *f1 = th2d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle.c_str(), "l");

    f1->SetStats(0);//Entries等信息栏不显示；
    f1->Draw("colz");

    //f1->Scale(1./f1->Integral());
    
    gPad->SetRightMargin(0.15);

    f1->GetXaxis()->SetTitle(XTitle.c_str());
    f1->GetYaxis()->SetTitle(YTitle.c_str());

    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    delete f1; f1 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};
void ShowVector(const TVector3 &v)
{
    std::cout << "X:" << v.X() << ", Y:" << v.Y() << ", Z:" << v.Z() << ", r:" << v.Mag() << std::endl;
}