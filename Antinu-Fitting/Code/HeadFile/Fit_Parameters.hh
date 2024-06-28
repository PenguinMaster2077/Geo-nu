#ifndef FITPARAMETERS_HH
#define FITPARAMETERS_HH
//Great thanks for James Page's code
//CPP
#include <iostream>
#include <vector>
//ROOT
#include <TFile.h>
#include <TTree.h>
//RAT
//Self-Defined
#include "./Constant_Setting.hh"

class Fitter;

class FitParameters
{
public:
    //Constructor and destructor
    FitParameters() {C_Total_Number = 0;};
    ~FitParameters();
    //Global Point
    static FitParameters *Point_FitParameters_;
    //Member Functions
    static FitParameters *Get_Global_Point();
    unsigned int Get_Total_Number() {return C_Total_Number;};
    unsigned int Get_Index(std::string Name);
    void Compute_Events(Double_t Duration, Int_t Start_Run, Int_t End_Run);
    std::string Get_Name(unsigned int Index);
    Double_t Get_Value(unsigned int Index);
    Double_t Get_Value(std::string Name);
    void Set_Value(unsigned int Index, Double_t Value);
    void Set_Value(std::string Name, Double_t Value);
    Double_t Get_Prior_Value(unsigned int Index);
    Double_t Get_Prior_Value(std::string Name);
    Double_t Get_Error(unsigned int Index);
    Double_t Get_Error(std::string Name);
    Double_t Get_Prior_Error(unsigned int Index);
    Double_t Get_Prior_Error(std::string Name);
    void Set_Error(unsigned int Index, Double_t Value);
    void Set_Error(std::string Name, Double_t Value);
    void Set_Value_Min(unsigned int Index, Double_t Value);
    void Set_Value_Min(std::string Name, Double_t Value);
    void Set_Value_Max(unsigned int Index, Double_t Value);
    void Set_Value_Max(std::string Name, Double_t Value);
    void Add_Parameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor, Double_t Value_Min, Double_t Value_Max);
    void Add_Parameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor);
    void Get_Parameter(unsigned int InPut_Index, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max);
    //Get the information of parametes by Index
    void Get_Parameter(std::string InPut_Name, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max);
    //Get the information of parametes by Name
    void Get_All_Parameters(std::vector<std::string> &Name, std::vector<Double_t> &Value, std::vector<Double_t> &Error, std::vector<Int_t> &Error_Factor, std::vector<Double_t> &Value_Min, std::vector<Double_t> &Value_Max); 
    void Show_Parameter(unsigned int Index);
    void Show_Parameter(std::string Name);
    void Show_All_Parameters();
private:
    unsigned int C_Total_Number;// Total Number of Parameters
    std::vector<std::string> C_Name;
    std::vector<Double_t> C_Value;//Fitted Value
    std::vector<Double_t> C_Prior_Value;
    std::vector<Double_t> C_Error;
    std::vector<Double_t> C_Prior_Error;
    std::vector<Int_t> C_Error_Factor; //Compute the minimum and maximum of parameter
    std::vector<Double_t> C_Value_Min;
    std::vector<Double_t> C_Value_Max;  
};

FitParameters *FitParameters::Point_FitParameters_ = new FitParameters();

FitParameters *FitParameters::Get_Global_Point()
{
    return Point_FitParameters_;
};

void FitParameters::Compute_Events(Double_t Duration, Int_t Start_Run, Int_t End_Run)
{
//Read Muon File
    TFile *infile_muon = new TFile(MUON_FILE.c_str());
    TTree *intree_muon = (TTree*) infile_muon->Get("output");
    //Set Branch Address
    UInt_t Muon_RunID;
    intree_muon->SetBranchAddress("RunID", &Muon_RunID);
    Int_t Number_Muon = 0;
    for(int ii1 = 0; ii1 < intree_muon->GetEntries(); ii1++)
    {
        intree_muon->GetEntry(ii1);
        if(Muon_RunID < Start_Run){continue;};
        if(Muon_RunID >= Start_Run && Muon_RunID <= End_Run){Number_Muon++;};
        if(Muon_RunID > End_Run){break;};
    };
//Effective Time
    Duration = Duration - Number_Muon * 20.0/86400;//days
    std::cout << "[FitParameters::Compute_Events] Duration(days):" << Duration + Number_Muon * 20.0/86400 << ", Muon:" << Number_Muon << ", Muon Vote(days):" << Number_Muon * 20.0/86400 << ", Effective Duraton(days):" << Duration << std::endl;
//Compute Events
    //Reactor
    Double_t Number_Reactor = Duration * REACTOR_EVENTS_RATE * REACTOR_SELECTION_EFFIENCY/ 365.0;
    Double_t Error_Reactor = REACTOR_EVENTS_RELATIVE_ERROR * Number_Reactor;
    //Geo U
    Double_t Number_U = Duration * U_EVENTS_RATE * U_SELECTION_EFFICIENCY / 365.0;
    Double_t Error_U = U_EVENTS_RELATIVE_ERROR * Number_U;
    //Geo Th
    Double_t Number_Th = Duration * TH_EVENTS_RATE * TH_SELECTION_EFFICIENCY / 365.0;
    Double_t Error_Th = TH_EVENTS_RELATIVE_ERROR * Number_Th;
    //AN
    Double_t Number_AN = Duration * AN_EVENTS_RATE * AN_SELECTION_EFFICIENCY / 365;
    Double_t Error_AN = AN_EVENTS_RELATIVE_ERROR * Number_AN;

    Double_t Number_AN_PR = Number_AN * PROTON_RECOIL_FRACTION;
    Double_t Error_AN_PR = Number_AN_PR * PROTON_RECOIL_RELATIVE_ERROR;
    Double_t Number_AN_C12 = Number_AN * C12_FRACTION;
    Double_t Error_AN_C12 = Number_AN_C12 * C12_RELATIVE_ERROR;
    Double_t Number_AN_O16 = Number_AN * O16_FRACTION;
    Double_t Error_AN_O16 = Number_AN_O16 * O16_RELATIVE_ERROR;

    Double_t Number_AN_Ground = Number_AN_PR + Number_AN_C12;
    Double_t Error_AN_Ground = sqrt(pow(Error_AN_PR, 2) + pow(Error_AN_C12, 2));
    Double_t Number_AN_Exicted = Number_AN_O16;
    Double_t Error_AN_Exicted = Error_AN_O16;
    //Total
    Double_t Number_Total = Number_Reactor + Number_U + Number_Th + Number_AN;
    Double_t Error_Total = sqrt(pow(Error_Reactor, 2) + pow(Error_U, 2) + pow(Error_Th, 2) + pow(Error_AN, 2));
    std::cout << "[FitParameters::Compute_Events] Reactor:" << Number_Reactor << "+-" << Error_Reactor << std::endl;
    std::cout << "[FitParameters::Compute_Events] U:" << Number_U << "+-" << Error_U << std::endl;
    std::cout << "[FitParameters::Compute_Events] Th:" << Number_Th << "+-" << Error_Th << std::endl;
    std::cout << "[FitParameters::Compute_Events] AN:" << Number_AN << "+-" << Error_AN << std::endl;
    std::cout << "[FitParameters::Compute_Events] Total:" << Number_Total << "+-" << Error_Total << std::endl;
//Add in Parameters
    FitParameters *Fit_Par = FitParameters::Get_Global_Point();
    Fit_Par->Add_Parameter(NAME_REACTOR, Number_Reactor, Error_Reactor, 1);
    Fit_Par->Add_Parameter(NAME_GEO_U, Number_U, Error_U, 1);
    Fit_Par->Add_Parameter(NAME_GEO_TH, Number_Th, Error_Th, 1);
    Fit_Par->Add_Parameter(NAME_AN_GROUND, Number_AN_Ground, Error_AN_Ground, 1);
    Fit_Par->Add_Parameter(NAME_AN_EXICTED, Number_AN_Exicted, Error_AN_Exicted, 1);
};

unsigned int FitParameters::Get_Index(std::string Name)
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
        std::cout << "[FitParameters::Get_Index] Couldn't find " + Name + ". Quit."  << std::endl;
        exit(1);
    }
    else
    {
        return Index;
    };
};

std::string FitParameters::Get_Name(unsigned int Index)
{   
    std::string Name = "Nothing";
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Get_Name] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        Name = C_Name.at(Index);
        return Name;
    };
};

Double_t FitParameters::Get_Value(unsigned int Index)
{
    Double_t Value = 999;
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Get_Value] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        Value = C_Value.at(Index);
        return Value;
    }
};

Double_t FitParameters::Get_Value(std::string Name)
{   
    unsigned int Index = Get_Index(Name);
    Double_t Value = Get_Value(Index);
    return Value;
};

void FitParameters::Set_Value(unsigned int Index, Double_t Value)
{
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Set_Value] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        C_Value.at(Index) = Value;
    };
};

void FitParameters::Set_Value(std::string Name, Double_t Value)
{
    Int_t Index = Get_Index(Name);
    Set_Value(Index, Value);
};

Double_t FitParameters::Get_Prior_Value(unsigned int Index)
{
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Get_Prior_Value] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        return C_Prior_Value.at(Index);
    };
};

Double_t FitParameters::Get_Prior_Value(std::string Name)
{
    Int_t Index = Get_Index(Name);
    return C_Prior_Value.at(Index);
};

Double_t FitParameters::Get_Error(unsigned int Index)
{
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Get_Error] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        return C_Error.at(Index);
    };
};

Double_t FitParameters::Get_Error(std::string Name)
{
    unsigned int Index = Get_Index(Name);
    Double_t Error = Get_Error(Index);
    return Error;
};

Double_t FitParameters::Get_Prior_Error(unsigned int Index)
{
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Get_Prior_Error] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        return C_Prior_Error.at(Index);
    };
};

Double_t FitParameters::Get_Prior_Error(std::string Name)
{
    Int_t Index = Get_Value(Name);
    return Get_Prior_Error(Index);
};

void FitParameters::Set_Error(unsigned int Index, Double_t Value)
{
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Set_Error] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        C_Error.at(Index) = Value;
    };
};

void FitParameters::Set_Error(std::string Name, Double_t Value)
{
    Int_t Index = Get_Index(Name);
    Set_Error(Index, Value);
};

void FitParameters::Set_Value_Min(unsigned int Index, Double_t Value)
{
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Set_Value_Min] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        C_Value_Min.at(Index) = Value;
    };
};

void FitParameters::Set_Value_Min(std::string Name, Double_t Value)
{
    Int_t Index = Get_Index(Name);
    Set_Value_Min(Index, Value);
};

void FitParameters::Set_Value_Max(unsigned int Index, Double_t Value)
{
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Set_Value_Max] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        C_Value_Max.at(Index) = Value;
    };
};

void FitParameters::Set_Value_Max(std::string Name, Double_t Value)
{
    Int_t Index = Get_Index(Name);
    Set_Value_Max(Index, Value);
};

void FitParameters::Add_Parameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor, Double_t Value_Min, Double_t Value_Max)
{
    int Index = C_Total_Number;
    C_Name.push_back(Name);
    C_Value.push_back(Value);
    C_Prior_Value.push_back(Value);
    C_Error.push_back(Error);
    C_Prior_Error.push_back(Error);
    C_Error_Factor.push_back(Error_Factor);
    if(Error_Factor == 0.0)
    {
        C_Value_Min.push_back(Value_Min);
        C_Value_Max.push_back(Value_Max);
    }
    else
    {
        if(Value_Min <= 0) 
        {
            C_Value_Min.push_back(0);
        }
        else
        {
            C_Value_Min.push_back(Value_Min);
        }
        C_Value_Max.push_back(Value_Max);
    };

    C_Total_Number = C_Name.size();
};

void FitParameters::Add_Parameter(std::string Name, Double_t Value, Double_t Error, Int_t Error_Factor)
{
    Double_t temp_value_min = Value - Error_Factor * Error;
    Double_t temp_value_max = Value + Error_Factor * Error;
    if(temp_value_min <= 0) {temp_value_min = 0;};
    Add_Parameter(Name, Value, Error, Error_Factor, temp_value_min, temp_value_max);
};

void FitParameters::Get_Parameter(unsigned int InPut_Index, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max)
{
    if(InPut_Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Get_Parameter] Index " << InPut_Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
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

void FitParameters::Get_Parameter(std::string InPut_Name, unsigned int &Index, std::string &Name, Double_t &Value, Double_t &Error, Int_t &Error_Factor, Double_t &Value_Min, Double_t &Value_Max)
{
    Index = Get_Index(InPut_Name);
    Name = C_Name.at(Index);
    Value = C_Value.at(Index);
    Error = C_Error.at(Index);
    Error_Factor = C_Error_Factor.at(Index);
    Value_Min = C_Value_Min.at(Index);
    Value_Max = C_Value_Max.at(Index);

};

void FitParameters::Get_All_Parameters(std::vector<std::string> &Name, std::vector<Double_t> &Value, std::vector<Double_t> &Error, std::vector<Int_t> &Error_Factor, std::vector<Double_t> &Value_Min, std::vector<Double_t> &Value_Max)
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

void FitParameters::Show_Parameter(unsigned int Index)
{   
    std::string Name;
    Double_t Value, Prior_Value, Error, Prior_Error, Value_Min, Value_Max;
    Int_t Error_Factor;
    if(Index >= C_Total_Number)
    {
        std::cout << "[FitParameters::Show_Parameter] Index " << Index << " is larger than total number of parameters. Quit." << std::endl;
        exit(1);
    }
    else
    {
        Name = C_Name.at(Index);
        Value = C_Value.at(Index);
        Prior_Value = C_Prior_Value.at(Index);
        Error = C_Error.at(Index);
        Prior_Error = C_Prior_Error.at(Index);
        Error_Factor =  C_Error_Factor.at(Index);
        Value_Min = C_Value_Min.at(Index);
        Value_Max = C_Value_Max.at(Index);
        std::cout << "[FitParameters::Show_Parameter] Name:" << Name << ", Value:" << Value << ", Error:" << Error << ", Relative Error:" << Error/Value << ", Prior Value:" << Prior_Value << ", Prior Error:" << Prior_Error << ", Prior Relative Error:" << Prior_Error/Prior_Value << ", Error Factor:" << Error_Factor << ", Min Value:" << Value_Min << ", Max Value:" << Value_Max << std::endl;
    };
};

void FitParameters::Show_Parameter(std::string Name)
{
    unsigned int Index = Get_Index(Name);
    Show_Parameter(Index);
};

void FitParameters::Show_All_Parameters()
{
    std::string Name;
    Double_t Value, Prior_Value, Error, Prior_Error, Value_Min, Value_Max;
    Int_t Error_Factor;
    for(int Index = 0; Index < C_Total_Number; Index++)
    {
        Name = C_Name.at(Index);
        Value = C_Value.at(Index);
        Prior_Value = C_Prior_Value.at(Index);
        Error = C_Error.at(Index);
        Prior_Error = C_Prior_Error.at(Index);
        Error_Factor =  C_Error_Factor.at(Index);
        Value_Min = C_Value_Min.at(Index);
        Value_Max = C_Value_Max.at(Index);
        std::cout << "[FitParameters::Show_All_Parameter] Name:" << Name << ", Value:" << Value << ", Error:" << Error << ", Relative Error:" << Error/Value << ", Prior Value:" << Prior_Value << ", Prior Error:" << Prior_Error << ", Prior Relative Error:" << Prior_Error/Prior_Value << ", Error Factor:" << Error_Factor << ", Min Value:" << Value_Min << ", Max Value:" << Value_Max << std::endl;
    };
};

#endif
