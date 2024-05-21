//Great thanks for James Page's code
//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
//RAT
//Self-Defined

class FitParameters
{
public:
    //Constructor and destructor
    FitParameters() {C_Total_Number = 0;};
    ~FitParameters();
    //Global Point
    static FitParameters *Point_FitParameters_;
    //Member Functions
    static FitParameters *GetGlobalPoint();
    unsigned int GetTotalNumber() {return C_Total_Number;};
    unsigned int GetIndex(std::string Name);
    std::string GetName(unsigned int Index);
    Double_t GetValue(unsigned int Index);
    Double_t GetValue(std::string Name);
    Double_t GetError(unsigned int Index);
    Double_t GetError(std::string Name);
    void AddParameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor, Double_t Value_Min, Double_t Value_Max);
    void AddParameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor);
    void GetParameter(unsigned int InPut_Index, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max);
    //Get the information of parametes by Index
    void GetParameter(std::string InPut_Name, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max);
    //Get the information of parametes by Name
    void GetAllParameters(std::vector<std::string> &Name, std::vector<Double_t> &Value, std::vector<Double_t> &Error, std::vector<Int_t> &Error_Factor, std::vector<Double_t> &Value_Min, std::vector<Double_t> &Value_Max); 
    void ShowParameter(unsigned int Index);
    void ShowParameter(std::string Name);
    void ShowAllParameters();
private:
    unsigned int C_Total_Number;// Total Number of Parameters
    std::vector<std::string> C_Name;
    std::vector<Double_t> C_Value;
    std::vector<Double_t> C_Error;
    std::vector<Int_t> C_Error_Factor; //Compute the minimum and maximum of parameter
    std::vector<Double_t> C_Value_Min;
    std::vector<Double_t> C_Value_Max;  
};

FitParameters *FitParameters::Point_FitParameters_ = new FitParameters();

FitParameters *FitParameters::GetGlobalPoint()
{
    return Point_FitParameters_;
};

unsigned int FitParameters::GetIndex(std::string Name)
{
    unsigned int Index = 999;
    for(int ii1 = 0; ii1 < C_Total_Number; ii1++)
    {
        if(Name == C_Name.at(ii1))
        {
            Index = ii1;
        };
    };
    if(Index == 999)
    {
        std::cout << "[FitParameters::GetIndex] Couldn't find " + Name + ", return 999"  << std::endl;
        return Index;
    }
    else
    {
        return Index;
    };
};

std::string FitParameters::GetName(unsigned int Index)
{   
    std::string Name = "Nothing";
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::GetName] Index " << Index << " is larger than total number of parameters, return Nothing" << std::endl;
        return Name;
    }
    else
    {
        Name = C_Name.at(Index);
        return Name;
    };
};

Double_t FitParameters::GetValue(unsigned int Index)
{
    Double_t Value = 999;
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::GetValue] Index " << Index << " is larger than total number of parameters, return 999" << std::endl;
        return Value; 
    }
    else
    {
        Value = C_Value.at(Index);
        return Value;
    }
};

Double_t FitParameters::GetValue(std::string Name)
{   
    unsigned int Index = GetIndex(Name);
    Double_t Value = GetValue(Index);
    return Value;
};

Double_t FitParameters::GetError(unsigned int Index)
{
    Double_t Error = 999;
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::GetError] Index " << Index << " is larger than total number of parameters, return 999" << std::endl;
        return Error;
    }
    else
    {
        Error = C_Error.at(Index);
        return Error;
    };
};

Double_t FitParameters::GetError(std::string Name)
{
    unsigned int Index = GetIndex(Name);
    Double_t Error = GetError(Index);
    return Error;
};

void FitParameters::AddParameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor, Double_t Value_Min, Double_t Value_Max)
{
    int Index = C_Total_Number;
    C_Name.push_back(Name);
    C_Value.push_back(Value);
    C_Error.push_back(Error);
    C_Error_Factor.push_back(Error_Factor);
    if(Error_Factor == 0.0)
    {
        C_Value_Min.push_back(Value_Min);
        C_Value_Max.push_back(Value_Max);
    }
    else
    {
        Double_t temp_value_min = Value - Error_Factor * Error;
        Double_t temp_value_max = Value + Error_Factor * Error;
        if(temp_value_min <= 0) {temp_value_min = 0;};
        C_Value_Min.push_back(temp_value_min);
        C_Value_Max.push_back(temp_value_max);
    };

    C_Total_Number = C_Name.size();
};

void FitParameters::AddParameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor)
{
    Double_t temp_value_min = Value - Error_Factor * Error;
    Double_t temp_value_max = Value + Error_Factor * Error;
    if(temp_value_min <= 0) {temp_value_min = 0;};
    AddParameter(Name, Value, Error, Error_Factor, temp_value_min, temp_value_max);
};

void FitParameters::GetParameter(unsigned int InPut_Index, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max)
{
    if(InPut_Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::GetParameter] Index " << InPut_Index << " is larger than total number of parameters, quit!!" << std::endl;
    }
    else
    {   
        Index = InPut_Index;
        Name = C_Name.at(Index);
        Value = C_Value.at(Index);
        Error = C_Error.at(Index);
        Error_Factor = C_Error_Factor.at(Index);
        Value_Min = C_Value_Min.at(Index);
        Value_Max = C_Value_Max.at(Index);
    };
};

void FitParameters::GetParameter(std::string InPut_Name, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max)
{
    Index = GetIndex(InPut_Name);
    if(Index != 999)
    {
        Name = C_Name.at(Index);
        Value = C_Value.at(Index);
        Error = C_Error.at(Index);
        Error_Factor = C_Error_Factor.at(Index);
        Value_Min = C_Value_Min.at(Index);
        Value_Max = C_Value_Max.at(Index);
    }
    else
    {
        Name = "Nothing";
        Value = 0;
        Error = 0;
        Error_Factor = 0;
        Value_Min = 0;
        Value_Max = 0;
    }
};

void FitParameters::GetAllParameters(std::vector<std::string> &Name, std::vector<Double_t> &Value, std::vector<Double_t> &Error, std::vector<Int_t> &Error_Factor, std::vector<Double_t> &Value_Min, std::vector<Double_t> &Value_Max)
{
    for(int Index = 0; Index < C_Total_Number; Index++)
    {
        Name.push_back(C_Name.at(Index));
        Value.push_back(C_Value.at(Index));
        Error.push_back(C_Error.at(Index));
        Error_Factor.push_back(C_Error_Factor.at(Index));
        Value_Min.push_back(C_Value_Min.at(Index));
        Value_Max.push_back(C_Value_Max.at(Index));
    };
};

void FitParameters::ShowParameter(unsigned int Index)
{   
    std::string Name;
    Double_t Value, Error, Value_Min, Value_Max;
    Int_t Error_Factor;
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::ShowParameter] Index " << Index << " is larger than total number of parameters, quit!!" << std::endl;
    }
    else
    {
        Name = C_Name.at(Index);
        Value = C_Value.at(Index);
        Error = C_Error.at(Index);
        Error_Factor =  C_Error_Factor.at(Index);
        Value_Min = C_Value_Min.at(Index);
        Value_Max = C_Value_Max.at(Index);
        std::cout << "Name:" << Name << ",Value:" << Value << ",Error:" << Error << ",Error Factor:" << Error_Factor << ",Min Value:" << Value_Min << ",Max Value:" << Value_Max << std::endl;
    };
};

void FitParameters::ShowParameter(std::string Name)
{
    unsigned int Index = GetIndex(Name);
    ShowParameter(Index);
};

void FitParameters::ShowAllParameters()
{
    std::string Name;
    Double_t Value, Error, Value_Min, Value_Max;
    Int_t Error_Factor;
    for(int Index = 0; Index < C_Total_Number; Index++)
    {
        Name = C_Name.at(Index);
        Value = C_Value.at(Index);
        Error = C_Error.at(Index);
        Error_Factor =  C_Error_Factor.at(Index);
        Value_Min = C_Value_Min.at(Index);
        Value_Max = C_Value_Max.at(Index);
        std::cout << "Name:" << Name << ",Value:" << Value << ",Error:" << Error << ",Error Factor:" << Error_Factor << ",Min Value:" << Value_Min << ",Max Value:" << Value_Max << std::endl;
    };
};