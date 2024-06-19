//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined
#include "../HeadFile/Fit_Parameters.hh"

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
    FitParameters *FitPar = FitParameters::Get_Global_Point();
    FitPar->Show_All_Parameters();
};

int Test_FitParameters()
{
    FitParameters *FitPar = FitParameters::Get_Global_Point();

    // //Test--GetTotalNumber()
    // std::cout << FitPar->GetTotalNumber() << std::endl;
//Add Parameters
    std::string Name[4] = {"AN", "Reactor", "U", "Th"};
    Double_t Value[4] = {111, 222, 333, 444};
    Double_t Error[4] = {11, 22, 33, 44};
    Int_t Error_Factor[4] = {1, 2, 3, 4};
    Double_t Value_Min[4] = {0.1, 0.2, 0.3, 0.4};
    Double_t Value_Max[4] = {1111, 2222, 3333, 4444};
//Test--Add_Parameter()
    // for(int ii1 = 0; ii1 < 4; ii1++)
    // {
    //     // FitPar->AddParameter(Name[ii1], Value[ii1], Error[ii1], Error_Factor[ii1]);
    //     FitPar->Add_Parameter(Name[ii1], Value[ii1], Error[ii1], 0, Value_Min[ii1], Value_Max[ii1]);
    // };

//Test--Compute_Events()
    FitPar->Compute_Events(88.23, 300733, 306498);

//Test--Get_Total_Number()
    // std::cout << FitPar->Get_Total_Number() << std::endl;

//Test--Get_Index()
    // std::cout << FitPar->Get_Index(Name[1]) << std::endl;
    // std::cout << FitPar->Get_Index("askjdlkasjdkjl") << std::endl;

//Test--Get_Name()
    // std::cout << FitPar->Get_Name(-1) << std::endl;
    // std::cout << FitPar->Get_Name(0) << std::endl;
    // std::cout << FitPar->Get_Name(1) << std::endl;
    // std::cout << FitPar->Get_Name(3) << std::endl;
    // std::cout << FitPar->Get_Name(4) << std::endl;
    // std::cout << FitPar->Get_Name(10) << std::endl;

//Test--Get_Value()
    // std::cout << FitPar->Get_Value(-1) << std::endl;
    // std::cout << FitPar->Get_Value(0) << std::endl;
    // std::cout << FitPar->Get_Value(1) << std::endl;
    // std::cout << FitPar->Get_Value(4) << std::endl;
    // std::cout << FitPar->Get_Value(10) << std::endl;
    // std::cout << FitPar->Get_Value(Name[0]) << std::endl;
    // std::cout << FitPar->Get_Value("sadkjlkasjd") << std::endl;

//Test--Set_Value()
    // FitPar->Set_Value(-1, 999);
    // ShowAll();
    // FitPar->Set_Value(0, 999);
    // ShowAll();
    // FitPar->Set_Value(1, 999);
    // ShowAll();
    // FitPar->Set_Value(4, 999);
    // ShowAll();
    // FitPar->Set_Value(Name[0], 999);
    // ShowAll();
    // FitPar->Set_Value("alkjsdlka", 999);
    // ShowAll();

//Test--Get_Prior_Value()
    // FitPar->Set_Value(0, 999);
    // FitPar->Set_Value(1, 888);
    // std::cout << FitPar->Get_Prior_Value(-1) << std::endl;
    // std::cout << FitPar->Get_Prior_Value(0) << std::endl;
    // std::cout << FitPar->Get_Prior_Value(1) << std::endl;
    // std::cout << FitPar->Get_Prior_Value(4) << std::endl;
    // std::cout << FitPar->Get_Prior_Value(Name[0]) << std::endl;
    // std::cout << FitPar->Get_Prior_Value("asdasd") << std::endl;

//Test--Get_Error()
    // std::cout << FitPar->Get_Error(-1) << std::endl;
    // std::cout << FitPar->Get_Error(0) << std::endl;
    // std::cout << FitPar->Get_Error(1) << std::endl;
    // std::cout << FitPar->Get_Error(4) << std::endl;
    // std::cout << FitPar->Get_Error(10) << std::endl;
    // std::cout << FitPar->Get_Error(Name[0]) << std::endl;
    // std::cout << FitPar->Get_Error("sadkjlkasjd") << std::endl;

//Test--Get_Parameter()
    // unsigned temp_index;
    // std::string temp_name;
    // Double_t temp_value, temp_error, temp_value_min, temp_value_max;
    // Int_t temp_error_factor;
    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->Get_Parameter(-1, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->Get_Parameter(0, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->Get_Parameter(1, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->Get_Parameter(10, temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->Get_Parameter(Name[0], temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

    // Clear(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // FitPar->Get_Parameter("SAJDljl", temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);
    // Show(temp_index, temp_name, temp_value, temp_error, temp_error_factor, temp_value_min, temp_value_max);

//Test--Get_All_Parametets()
    // std::vector<std::string> vector_name;
    // std::vector<Double_t> vector_value, vector_error, vector_value_min, vector_value_max;
    // std::vector<Int_t> vector_error_factor;
    // FitPar->Get_All_Parameters(vector_name, vector_value, vector_error, vector_error_factor, vector_value_min, vector_value_max);
    // Show(vector_name, vector_value, vector_error, vector_error_factor, vector_value_min, vector_value_max);

//Test--Show_Parameter()
    // FitPar->Show_Parameter(-1);
    // FitPar->Show_Parameter(0);
    // FitPar->Show_Parameter(1);
    // FitPar->Show_Parameter(10);
    // FitPar->Show_Parameter(Name[0]);
    // FitPar->Show_Parameter("asdasd");

//Test-----Show_All_Parameters()
    // FitPar->Show_All_Parameters();

    ShowAll();

    return 0;
}