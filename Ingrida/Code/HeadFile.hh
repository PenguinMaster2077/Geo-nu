//CPP
#include <iostream>
//ROOT
//RAT
#include <RAT/DU/DSReader.hh>
#include <RAT/DS/Run.hh>
#include <RAT/DS/Entry.hh>
#include <RAT/DS/EV.hh>
#include <RAT/DB.hh>
#include <RAT/DU/Utility.hh>

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
//Run Information
    UInt_t C_RunID, C_SubRunID, C_Date, C_Time, C_RunType;
};
void RecordRunInfo(myresult res, const RAT::DS::Run run)
{
    res.ClearRunInfo();
    res.C_RunID = run.GetRunID();
    res.C_SubRunID = run.GetSubRunID();
    res.C_Date = run.GetDate();
    res.C_Time = run.GetTime();
    res.C_RunType = run.GetRunType();
};
void FindCoincidencePair(const RAT::DS::EV ev);