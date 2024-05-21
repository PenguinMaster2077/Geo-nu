//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined
#include "../HeadFile/FitParameters.hh"

void Clear(unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max)
{
    Index = 999;
    Name = "";
    Value = Error = Value_Min = Value_Max = 0;
    Error_Factor = 0;
};

void Show(unsigned int Index, std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor, Double_t Value_Min, Double_t Value_Max)
{
    std::cout << "Index:" << Index << ",Name:" << Name << ",Value:" << Value << ",Error:" << Error << ",Error Factor:" << Error_Factor << ",Min Value:" << Value_Min << ",Max Value:" << Value_Max << std::endl;
};

void Show(std::vector<std::string> Name, std::vector<Double_t> Value, std::vector<Double_t> Error, std::vector<Int_t> Error_Factor, std::vector<Double_t> Value_Min, std::vector<Double_t> Value_Max)
{
    for(int Index = 0; Index < Name.size(); Index++)
    {
        Show(Index, Name.at(Index), Value.at(Index), Error.at(Index), Error_Factor.at(Index), Value_Min.at(Index), Value_Min.at(Index));
    };
};

void ShowAll()
{
    FitParameters *FitPar = FitParameters::GetGlobalPoint();
    FitPar->ShowAllParameters();
};

int Test_FitParameters()
{
    FitParameters *FitPar = FitParameters::GetGlobalPoint();

    // //Test--GetTotalNumber()
    // std::cout << FitPar->GetTotalNumber() << std::endl;
//Add Parameters
    std::string Name[4] = {"AN", "Reactor", "U", "Th"};
    Double_t Value[4] = {100, 200, 300, 400};
    Double_t Error[4] = {11, 22, 33, 44};
    Int_t Error_Factor[4] = {1, 2, 3, 4};
    Double_t Value_Min[4] = {0.1, 0.2, 0.3, 0.4};
    Double_t Value_Max[4] = {1000, 2000, 3000, 4000};
    for(int ii1 = 0; ii1 < 4; ii1++)
    {
        // FitPar->AddParameter(Name[ii1], Value[ii1], Error[ii1], Error_Factor[ii1]);
        FitPar->AddParameter(Name[ii1], Value[ii1], Error[ii1], 0, Value_Min[ii1], Value_Max[ii1]);
    };
    // //Test--GetTotalNumber()
    // std::cout << FitPar->GetTotalNumber() << std::endl;

    //Test--GetIndex()
    // std::cout << FitPar->GetIndex(Name[1]) << std::endl;
    // std::cout << FitPar->GetIndex("askjdlkasjdkjl") << std::endl;

    //Test--GetName()
    // std::cout << FitPar->GetName(-1) << std::endl;
    // std::cout << FitPar->GetName(0) << std::endl;
    // std::cout << FitPar->GetName(1) << std::endl;
    // std::cout << FitPar->GetName(3) << std::endl;
    // std::cout << FitPar->GetName(4) << std::endl;
    // std::cout << FitPar->GetName(10) << std::endl;

    //Test--GetValue()
    // std::cout << FitPar->GetValue(-1) << std::endl;
    // std::cout << FitPar->GetValue(0) << std::endl;
    // std::cout << FitPar->GetValue(1) << std::endl;
    // std::cout << FitPar->GetValue(4) << std::endl;
    // std::cout << FitPar->GetValue(10) << std::endl;
    // std::cout << FitPar->GetValue(Name[0]) << std::endl;
    // std::cout << FitPar->GetValue("sadkjlkasjd") << std::endl;

    //Test--GetError()
    // std::cout << FitPar->GetError(-1) << std::endl;
    // std::cout << FitPar->GetError(0) << std::endl;
    // std::cout << FitPar->GetError(1) << std::endl;
    // std::cout << FitPar->GetError(4) << std::endl;
    // std::cout << FitPar->GetError(10) << std::endl;
    // std::cout << FitPar->GetError(Name[0]) << std::endl;
    // std::cout << FitPar->GetError("sadkjlkasjd") << std::endl;

    //Test--GetParameter()
    // unsigned temp_index;
    // std::string temp_name;
    // Double_t temp_value, temp_error, temp_value_min, temp_value_max;
    // Int_t temp_error_factor;
    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->GetParameter(-1, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->GetParameter(0, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->GetParameter(1, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->GetParameter(10, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->GetParameter(Name[0], temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->GetParameter("SAJDljl", temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    //Test--GetAllParametets()
    // std::vector<std::string> vector_name;
    // std::vector<Double_t> vector_value, vector_error, vector_value_min, vector_value_max;
    // std::vector<Int_t> vector_error_factor;
    // FitPar->GetAllParameters(vector_name, vector_value, vector_error, vector_error_factor, vector_value_min, vector_value_max);
    // Show(vector_name, vector_value, vector_error, vector_error_factor, vector_value_min, vector_value_max);

    //Test--ShowParameter()
    // FitPar->ShowParameter(-1);
    // FitPar->ShowParameter(0);
    // FitPar->ShowParameter(1);
    // FitPar->ShowParameter(10);

    //Test-----ShowAllParameters()
    // FitPar->ShowAllParameters();

    ShowAll();

    return 0;
}