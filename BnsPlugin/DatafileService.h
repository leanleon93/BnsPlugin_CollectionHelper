#pragma once
#include "Data.h"
#include <unordered_set>
#include <unordered_map>
#include <string>

class DatafileService {
public:
	bool Setup();
	bool IsSetupComplete() const;
	bool IsCriticalFail() const;
	Data::DataManager* GetDataManager();
	void SetDataManagerPtr(__int64 const* ptr);
private:
	__int64 const* dataManagerPtr;
	bool SetupComplete;
	bool CriticalFail;
	bool CompatabilityCheck();
	const std::unordered_set<std::wstring> usedTables = {
		//L"skillshow3",
	};

	std::unordered_map<std::wstring, bool> versionCheckSuccess = {
		//{ L"skillshow3", false },
	};
	bool AllVersionsSuccess() const;
};

extern DatafileService g_DatafileService;