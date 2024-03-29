//CPP
#include <iostream>
#include <vector>
//RAT
#include <RAT/DU/DSReader.hh>
// #include <RAT/DS/Run.hh>
// #include <RAT/DS/Entry.hh>
// #include <RAT/DS/EV.hh>
// #include <RAT/DS/FitResult.hh>
// #include <RAT/DS/FitVertex.hh>
// #include <RAT/DB.hh>
// #include <RAT/DU/Utility.hh>
// #include <RAT/DU/ReconCalibrator.hh>
// #include <RAT/DU/DetectorStateCorrection.hh>

class myresult;
void RecordRunInfo(myresult &res, RAT::DU::DSReader &dsreader);
bool GetReconInfo(std::vector<double> &Energy, RAT::DS::EV &ev);

int test()
{
    std::string InFile = "/home/shuaioy/scratch/Geo/Gold/300000-306498/ratds/300050/Analysis20R_r0000300000_s000_p000.root";
    myresult res;
    
    RAT::DU::DSReader dsreader(InFile);
    RAT::DU::ReconCalibrator recon_cali = RAT::DU::Utility::Get()->GetReconCalibrator();
    RAT::DU::Utility::Get()->BeginOfRun();
    RAT::DU::DetectorStateCorrection detector_state_correct = RAT::DU::Utility::Get()->GetDetectorStateCorrection();
    
    RecordRunInfo(res, dsreader);

    RAT::DS::Entry entry;
    RAT::DS::EV ev;
    std::vector<double> test(2, -99.0);
    std::cout << test.at(0) << std::endl;
    return 0;
};

class myresult
{
public:
    void ClearRunInfo()
    {
        C_RunID = 0;
        C_SubRunID = 0;
        C_Date = 0;
        C_Time = 0;
        C_RunType = 0;
    };
    void ShowRunInfo()
    {
        std::cout << "RunID:" << C_RunID << std::endl;
        std::cout << "SubRunID:" << C_SubRunID << std::endl;
        std::cout << "Date:" << C_Date << std::endl;
        std::cout << "Time:" << C_Time << std::endl;
        std::cout << "RunType:" << C_RunType << std::endl;
    };
//Run Information
    UInt_t C_RunID, C_SubRunID, C_Date, C_Time, C_RunType;
};
void RecordRunInfo(myresult &res, RAT::DU::DSReader &dsreader)
{
    RAT::DS::Run run = dsreader.GetRun();
    res.ClearRunInfo();
    res.C_RunID = run.GetRunID();
    res.C_SubRunID = run.GetSubRunID();
    res.C_Date = run.GetDate();
    res.C_Time = run.GetTime();
    res.C_RunType = run.GetRunType();
};