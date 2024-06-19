//CPP
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
//ROOT
#include <TFile.h>
#include <TTree.h>
#include <TSystemDirectory.h>
#include <TList.h>
#include <TString.h>
//RAT
#include <RAT/DS/Meta.hh>
#include <RAT/DS/MetaDB.hh>
//Self-Defined

std::vector<std::string> Get_Root_Files(const std::string& dir) {
    std::vector<std::pair<std::string, std::string>> files_with_names;
    TSystemDirectory directory("rootDir", dir.c_str());
    TList* fileList = directory.GetListOfFiles();
    if (fileList) {
        TSystemFile* file;
        TString fileName;
        TIter next(fileList);
        while ((file = (TSystemFile*)next())) {
            fileName = file->GetName();
            if (!file->IsDirectory() && fileName.EndsWith(".root")) {
                files_with_names.emplace_back(fileName.Data(), dir + "/" + fileName.Data());
            }
        }
    }

    // Sort files by their names
    std::sort(files_with_names.begin(), files_with_names.end());

    // Extract the file paths
    std::vector<std::string> files;
    for (const auto& pair : files_with_names) {
        files.push_back(pair.second);
    }

    return files;
};

void Get_Meta_Info(std::vector<std::string> &Files, std::vector<Int_t> &Runs, std::vector<Int_t> &Generated_Events)
{
    RAT::DS::Meta *meta;
    RAT::DS::MetaDB metadb;
    std::vector<Int_t> temp_run, temp_generated;
    std::pair<std::string, std::string> override;
    TString temp_tstring;
    std::string name;
    for(int ii1 = 0; ii1 < Files.size(); ii1++)
    {
        TFile file(Files.at(ii1).c_str());
        meta = (RAT::DS::Meta*) file.Get("meta");
    // Generated Events
        temp_generated = meta->GetEventsGeneratedCounts();
        Generated_Events.push_back(temp_generated.at(0));
    // Runs
        metadb = meta->GetMetaDBs().at(0);
        override = metadb.GetOverrideCommand(6);
        if(override.first == "MC run")
        {
            temp_tstring = override.second;
            Runs.push_back(temp_tstring.Atoi());
        };
        file.Close();
    };
};

int Get_Generated_Events()
{
    std::string InFilePWD = "/rat/MyCode/Work/Geo-nu-Data/RawData/Reactor/Raw/300000-307612";
    std::cout << "Start to Load Files" << std::endl;
    std::vector<std::string> Files = Get_Root_Files(InFilePWD);
    std::vector<Int_t> Runs, Generated;
//Get Meta Info
    Get_Meta_Info(Files, Runs, Generated);
//Create OutPut File
    std::string OutFile = "/rat/MyCode/Work/Geo-nu-Data/RawData/Reactor/Raw/Generated_Events.root";
    TFile *outfile = new TFile(OutFile.c_str(), "recreate");
    TTree *outtree = new TTree("output", "");

    Int_t Run, Events;
    outtree->Branch("Run", &Run);
    outtree->Branch("GeneratedEvents", &Events);
//Write into File
    for(int ii1 = 0; ii1 < Runs.size(); ii1++)
    {
        Run = Runs.at(ii1);
        Events = Generated.at(ii1);
        outtree->Fill();
    };
    Double_t Sum;
    for(int ii1 = 0; ii1 < Runs.size(); ii1++)
    {
        Sum = Sum + Generated.at(ii1);
    };
    std::cout << "Total:" << Sum << std::endl;
    outfile->Write();
    outfile->Close();
//Test
    // for(int ii1 = 0; ii1 < Files.size(); ii1++)
    // {
    //     std::cout << Runs.at(ii1) << "," << Generated.at(ii1)<< std::endl;
    // };
    return 0;
}