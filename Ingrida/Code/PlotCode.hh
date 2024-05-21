//CPP
#include <iostream>
//ROOT
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TLine.h>
#include <TPad.h>
#include <TVector3.h>
#include <TAttMarker.h>
#include <TF1.h>
//TH1D-单个图表
void SavePlot(std::string Out,TH1D *th1d, std::string LegTitle
,std::string XTitle,
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle.c_str(), "l");
    f1->Draw("P E");
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

void SavePlot(std::string Out,TH1D *th1d, std::string LegTitle,
std::string XTitle, double down, double up, 
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

void FitSavePlot_G(std::string Out,TH1D *th1d, std::string LegTitle,
std::string XTitle, std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->SetBorderSize(0); // 去掉图例的边框
    leg->SetFillStyle(0); // 将图例置于最底层
    //leg->SetEntrySeparation(1); // 设置条目之间的间隔，单位为图例的高度
    leg->SetTextSize(0.03);
    leg->AddEntry(f1, LegTitle.c_str(), "P");
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

void FitSavePlot_E(std::string Out,TH1D *th1d, std::string LegTitle,
std::string XTitle, std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->SetBorderSize(0); // 去掉图例的边框
    leg->SetFillStyle(0); // 将图例置于最底层
    leg->SetTextSize(0.03);
    leg->AddEntry(f1, LegTitle.c_str(), "P");
    f1->Draw("P E1");
    f1->SetLineColor(kBlack);
    f1->SetMarkerStyle(8);
    //f1->SetMarkerSize(2.0);
    f1->SetXTitle(XTitle.c_str());

    //Fit
    TF1 *expFunc = new TF1("expFunc", "[0] * TMath::Exp(- x/[1])", 0, 800);
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
//TGraph-单个图表
void SavePlot(std::string Out, TGraph *graph, std::string Title, std::string XTitle,
std::string YTitle)
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
//TGraph 2个图表
void SaveTwoPlots(std::string Out, TGraph *graph1, std::string LegTitle1, TGraph *graph2,
std::string LegTitle2, std::string Title, std::string XTitle,
std::string YTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
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

void SaveFourPlots(const std::string Out, TH1D *th1d, std::string LegTitle1, TH1D *th2d,
std::string LegTitle2, TH1D *th3d, std::string LegTitle3, TH1D *th4d, std::string LegTitle4,
std::string XTitle, std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3,
double y2 = 0.87)
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

    f1->Draw("E");
    f2->Draw("L SAME");
    f3->Draw("L SAME");
    f4->Draw("L SAME");

    f2->SetLineStyle(9);
    f3->SetLineStyle(9);
    f4->SetLineStyle(9);

    f1->GetYaxis()->SetRangeUser(0, 20);
    f2->GetYaxis()->SetRangeUser(0, 20);
    f3->GetYaxis()->SetRangeUser(0, 20);
    f4->GetYaxis()->SetRangeUser(0, 20);

    // f1->SetLineColor(kBlack);
    f2->SetLineColor(kGreen);
    f3->SetLineColor(kRed);
    f4->SetLineColor(kBlue);

    // f1->SetXTitle(XTitle.c_str());
    f2->SetXTitle(XTitle.c_str());
    f3->SetXTitle(XTitle.c_str());
    f4->SetXTitle(XTitle.c_str());

    leg->Draw();
    canvas->SetTitle(CanTitle.c_str());
    canvas->Print(Out.c_str());
    // delete f1; f1 = NULL;
    // delete f2; f2 = NULL;
    // delete f3; f3 = NULL;
    // delete f4; f4 = NULL;
    delete canvas; canvas = NULL;
    delete leg; leg = NULL;
};

void SaveFourPlots(const std::string Out, TH1D *th1d, std::string LegTitle1, TH1D *th2d,
std::string LegTitle2,TH1D *th3d, std::string LegTitle3, TH1D *th4d, std::string LegTitle4,
std::string XTitle, int Normalized = 0, double down, double up, std::string CanTitle,
double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
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

    f1->Draw("E");
    f2->Draw("L SAME");
    f3->Draw("L SAME");
    f4->Draw("L SAME");


    f2->SetLineStyle(9);
    f3->SetLineStyle(9);
    f4->SetLineStyle(9);

    f1->SetLineColor(kBlack);
    f2->SetLineColor(kGreen);
    f3->SetLineColor(kRed);
    f4->SetLineColor(kBlue);

    if(Normalized == 1)
    {
        f1->Scale(1.0 /f1->Integral());
        f2->Scale(1.0 /f2->Integral());
        f3->Scale(1.0 /f3->Integral());
        f4->Scale(1.0 /f4->Integral());
    };

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

void SaveFivePlots(const std::string Out, TH1D *th1d, std::string LegTitle1, TH1D *th2d,
std::string LegTitle2,TH1D *th3d, std::string LegTitle3, TH1D *th4d, std::string LegTitle4,
TH1D *th5d, std::string LegTitle5, std::string XTitle, int Normalized = 0, double down, double up,
std::string CanTitle, double x1 = 0.1, double y1 = 0.72, double x2 = 0.3, double y2 = 0.87)
{
    TH1D *f1 = th1d;
    TH1D *f2 = th2d;
    TH1D *f3 = th3d;
    TH1D *f4 = th4d;
    TH1D *f5 = th5d;
    TCanvas *canvas = new TCanvas("canvas", CanTitle.c_str(), 800, 600);
    TLegend *leg = new TLegend(x1, y1, x2, y2);
    leg->AddEntry(f1, LegTitle1.c_str(), "p");
    leg->AddEntry(f2, LegTitle2.c_str(), "l");
    leg->AddEntry(f3, LegTitle3.c_str(), "l");
    leg->AddEntry(f4, LegTitle4.c_str(), "l");
    leg->AddEntry(f5, LegTitle5.c_str(), "l");

    f1->Draw("E1");
    f1->SetMarkerStyle(8);
    f2->Draw("HIST SAME");
    f3->Draw("HIST SAME");
    f4->Draw("HIST SAME");
    f5->Draw("HIST SAME");

    
    // f2->SetLineStyle(9);
    // f3->SetLineStyle(9);
    // f4->SetLineStyle(9);
    // f5->SetLineStyle(9);

    f1->SetLineColor(kBlack);
    f2->SetLineColor(kGreen);
    f3->SetLineColor(kRed);
    f4->SetLineColor(kBlue);
    f5->SetLineColor(6);

    if(Normalized == 1)
    {
        f1->Scale(1.0 /f1->Integral());
        f2->Scale(1.0 /f2->Integral());
        f3->Scale(1.0 /f3->Integral());
        f4->Scale(1.0 /f4->Integral());
    };

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