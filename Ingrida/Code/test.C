//CPP
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TString.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooArgList.h>
#include <RooArgSet.h>
#include <RooHistPdf.h>
#include <RooGenericPdf.h>
#include <RooGenericPdf.h>
#include <RooFitResult.h>
//RAT
//Self-Defined
// #include "/home/penguin/Rat/rat-7.0.14-ROOT5/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

#include "/rat/MyCode/Work/Geo-nu/Ingrida/Code/PlotCode.hh"

TH1D *h1, *h2, *h3;

double self_function(double *x, double*par)
{
    double x_component = x[0];//横坐标分量
    //std::cout << x_component << "," << par[0] << "," << par[1] << "," << par[2] << std::endl;
    int bin = h1->GetXaxis()->FindBin(x_component);
    double part1 = par[0] * h1->GetBinContent(bin);
    double part2 = par[1] * h2->GetBinContent(bin);
    double part3 = par[2] * h3->GetBinContent(bin);
    return part1 + part2 + part3;
};

int test()
{
    std::string InPWD = "/rat/MyCode/Work/Geo-nu-Data/CoincidencePair/MC/";
    std::string InFile1 = InPWD + "AN/MC_AN_300000-307612.root";
    std::string InFile2 = InPWD + "Reactor/MC_Reactor_OffLine_300000-307612.root";
    std::string InFile3 = InPWD + "U/MC_U_300000-307612.root";
    std::string InFile4 = InPWD + "Th/MC_Th_300000-307612.root";
    std::string DataFile = "/rat/MyCode/Work/Geo-nu-Data/IBDCandidate/Gold_20R_IBD_Candidate_0000300000-0000307612.root";

    std::string Name1 = "AN";
    std::string Name2 = "Reactor";
    std::string Name3 = "U"; 
    std::string Name4 = "Th";
    //Create TFile
    TFile infile1(InFile1.c_str());
    TTree *intree1 = (TTree*) infile1.Get("output");
    TFile infile2(InFile2.c_str());
    TTree *intree2 = (TTree*) infile2.Get("output");
    TFile infile3(InFile3.c_str());
    TTree *intree3 = (TTree*) infile3.Get("output");
    TFile infile4(InFile4.c_str());
    TTree *intree4 = (TTree*) infile4.Get("output");
    TFile datafile(DataFile.c_str());
    TTree *datatree = (TTree*) datafile.Get("output");
    //Create Histogram
    int BinNumber = 10;
    Double_t down = 0.8, up = 8.0;
    h1 = new TH1D(Name1.c_str(), "", BinNumber, down, up);
    h2 = new TH1D(Name2.c_str(), "", BinNumber, down, up);
    TH1D *h_U = new TH1D(Name3.c_str(), "", BinNumber, down, up);
    TH1D *h_Th = new TH1D(Name4.c_str(), "", BinNumber, down, up);
    h3 = new TH1D("Geo", "", BinNumber, down, up);
    TH1D *data_hist = new TH1D("data_hist", "", BinNumber, down, up);

    TH1D *h1_Plot = new TH1D("PDF1_Plot", "", 100, down, up);
    TH1D *h2_Plot = new TH1D("PDF2_Plot", "", 100, down, up);
    TH1D *h3_Plot = new TH1D("PDF3_Plot", "", 100, down, up);
    TH1D *h_Th_Plot = new TH1D("PDF_Th_Plot", "", 100, down, up);
    //Load Data and PDF
    std::string BranchName = "PromptEnergy";
    intree1->Project(Name1.c_str(), BranchName.c_str());
    intree2->Project(Name2.c_str(), BranchName.c_str());
    intree3->Project(Name3.c_str(), BranchName.c_str());
    intree4->Project(Name4.c_str(), BranchName.c_str());
    datatree->Project("data_hist", BranchName.c_str());
    intree3->Project("Geo", BranchName.c_str());
    h3->Add(h_Th, 1.0/3.7);

    intree1->Project("PDF1_Plot", BranchName.c_str());
    intree2->Project("PDF2_Plot", BranchName.c_str());
    intree3->Project("PDF3_Plot", BranchName.c_str());
    intree4->Project("PDF_Th_Plot", BranchName.c_str());
    h3_Plot->Add(h_Th_Plot, 1.0/3.7);
    //Normalized
    h1->Scale(1.0/h1->Integral());
    h2->Scale(1.0/h2->Integral());
    h3->Scale(1.0/h3->Integral());

    h1_Plot->Scale(1.0/h1_Plot->Integral());
    h2_Plot->Scale(1.0/h2_Plot->Integral());
    h3_Plot->Scale(1.0/h3_Plot->Integral());
    //Theory Predictions
    Int_t Total_Number = datatree->GetEntries();
    Double_t Pre_Number1 = 17.2, Pre_Number1_Min = 17.2 - 5.9, Pre_Number1_Max = 17.2 + 5.9;
    Double_t Pre_Number2 = 24.88;
    Double_t Pre_Number3 = 5.22;
    Double_t Pre_Number4 = 1.39;
    //Fit Function
    // 将 TH1D 转换为 RooDataHist
    RooRealVar x("x", "x", down, up);
    RooArgSet vars(x);

    RooDataHist data_hist_Roo("data_hist_Roo", "data_hist_Roo", RooArgList(x), data_hist);
    RooDataHist h1_hist_Roo("h1_hist_Roo", "h1_hist_Roo", RooArgList(x), h1);
    RooDataHist h2_hist_Roo("h2_hist_Roo", "h2_hist_Roo", RooArgList(x), h2);
    RooDataHist h3_hist_Roo("h3_hist_Roo", "h3_hist_Roo", RooArgList(x), h3);
    // 创建 RooHistPdf 对象
    RooHistPdf hist_pdf1("hist_pdf1", "Histogram PDF 1", vars, h1_hist_Roo);
    RooHistPdf hist_pdf2("hist_pdf2", "Histogram PDF 2", vars, h2_hist_Roo);
    RooHistPdf hist_pdf3("hist_pdf3", "Histogram PDF 3", vars, h3_hist_Roo);

    // 自定义 PDF
    RooRealVar p0("p0", "Parameter 0", Pre_Number1, Pre_Number1_Min, Pre_Number1_Max);
    RooRealVar p1("p1", "Parameter 1", Pre_Number2, 0, 56 - Pre_Number1_Min);
    RooRealVar p2("p2", "Parameter 2", Pre_Number3 + Pre_Number4, 0, 56 - Pre_Number1_Min);

    RooGenericPdf pdf("pdf", "Custom PDF", "p0 * hist_pdf1 + p1 * hist_pdf2 + p2 * hist_pdf3", RooArgList(p0, p1, p2));

    // 进行拟合
    RooFitResult *result = pdf.fitTo(data_hist_Roo, RooFit::Save());

    // 打印拟合结果
    result->Print();
    return 0;
    //Get Results
    Double_t Parameter_Best_1 = ftotal->GetParameter(0);
    Double_t Parameter_Error_1 = ftotal->GetParError(0);
    Double_t Parameter_Best_2 = ftotal->GetParameter(1);
    Double_t Parameter_Error_2 = ftotal->GetParError(1);
    Double_t Parameter_Best_3 = ftotal->GetParameter(2);
    Double_t Parameter_Error_3 = ftotal->GetParError(2);
    //Compute Events
    Double_t Number_1 = Parameter_Best_1 * h1->Integral();
    Double_t Error_1 = Parameter_Error_1 * h1->Integral();
    Double_t Number_2 = Parameter_Best_2 * h2->Integral();
    Double_t Error_2 = Parameter_Error_2 * h2->Integral();
    Double_t Number_3 = Parameter_Best_3 * h3->Integral();
    Double_t Error_3 = Parameter_Error_3 * h3->Integral();

    Double_t Number_Sum = Number_1 + Number_2 + Number_3;
    Double_t Error_Sum = Error_1 + Error_2 + Error_3;

    Double_t ChiSquare = ftotal->GetChisquare();
    Double_t NDF = ftotal->GetNDF();

    std::cout << "AN:" << Number_1 << ", Error:" << Error_1 << std::endl;
    std::cout << "Reactor:" << Number_2 << ", Error:" << Error_2 << std::endl;
    std::cout << "Geo:" << Number_3 << ", Error:" << Error_3 << std::endl;
    std::cout << "Total:" << Number_Sum << ", Error:" << Error_Sum << std::endl;
    std::cout << "chi square:" << ChiSquare << ", NDF:" << NDF << ", Ratio:" << ChiSquare / NDF << std::endl;

    //Rescaling TH1D for Plot
    h1_Plot->Scale(5 * Number_1);
    h2_Plot->Scale(5 * Number_2);
    h3_Plot->Scale(5 * Number_3);
    //ReSet Total Bin
    delete data_hist;
    data_hist = new TH1D("data_hist", "", BinNumber, down, up);
    datatree->Project("data_hist", BranchName.c_str());
    TH1D *Sum_Hist = (TH1D*)h1_Plot->Clone();
    Sum_Hist->Add(h2_Plot,1.0);
    Sum_Hist->Add(h3_Plot,1.0);
    SaveFivePlots("./test.png", data_hist, "Data", h1_Plot, "AN", h2_Plot, "Reactor", h3_Plot, "Geo", Sum_Hist, "Sum" , "E_{Prompt}/MeV", 0, 0, 15, "Prompt Event Fitting", 0.7, 0.6, 0.9, 0.8 );
    return 0;
};
