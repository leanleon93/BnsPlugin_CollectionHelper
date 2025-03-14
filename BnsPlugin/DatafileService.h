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
	__int16 GetZoneenv2TableId() const;
private:
	__int16 zoneenv2TableId;
	__int64 const* dataManagerPtr;
	bool SetupComplete;
	bool CriticalFail;
	bool CompatabilityCheck();
	bool SetupTableIds();
	const std::unordered_set<std::wstring> usedTables = {
		L"zoneenv2",
	};

	std::unordered_map<std::wstring, bool> versionCheckSuccess = {
		{ L"zoneenv2", false },
	};
	bool AllVersionsSuccess() const;
};

extern DatafileService g_DatafileService;