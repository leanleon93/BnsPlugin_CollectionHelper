#include "DatafileService.h"
#include <algorithm>
#include <Windows.h>
#include <EU/BnsTableNames.h>
#include <KR/BnsTableNames.h>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

using namespace BnsTables;

DatafileService g_DatafileService;

Data::DataManager* DatafileService::GetDataManager() {
	if (this->dataManagerPtr == nullptr || *this->dataManagerPtr == NULL) {
		return nullptr;
	}
	return reinterpret_cast<Data::DataManager*>(*this->dataManagerPtr);
}

void DatafileService::SetDataManagerPtr(__int64 const* ptr) {
	this->dataManagerPtr = ptr;
}

__int16 DatafileService::GetZoneenv2TableId() const
{
	return zoneenv2TableId;
}

bool DatafileService::IsSetupComplete() const {
	return SetupComplete;
}

bool DatafileService::IsCriticalFail() const {
	return CriticalFail;
}

bool DatafileService::AllVersionsSuccess() const {
	auto allSuccess = std::ranges::all_of(versionCheckSuccess, [](auto const& kvp) { return kvp.second; });
	return allSuccess;
}

bool DatafileService::CompatabilityCheck() {
	if (this->dataManagerPtr == nullptr || *this->dataManagerPtr == NULL) {
		return false;
	}
	const auto manager = reinterpret_cast<Data::DataManager*>(*this->dataManagerPtr);
	for (auto const& tableName : usedTables) {
		if (auto table = DataHelper::GetTable(manager, tableName.c_str()); table == nullptr) {
			return false;
		}
#ifdef _DEBUG
		std::wcout << "Table " << tableName << " found." << std::endl;
#endif // _DEBUG
		auto tableDef = DataHelper::GetTableDef(manager, tableName.c_str());
		if (tableDef == nullptr) {
			continue;
		}
#ifdef _DEBUG
		std::wcout << "\tID: " << tableDef->type << std::endl;
		std::wcout << "\tVersion union: " << tableDef->version.ver << std::endl;
		std::wcout << "\tMajor Version: " << tableDef->version.major_ver << std::endl;
		std::wcout << "\tMinor Version: " << tableDef->version.minor_ver << std::endl;
		printf("\tAddress of %s is %p\n", "type", &tableDef->type);
		std::cout << std::endl;
#endif // _DEBUG
#if _BNSEU
		auto confirmedVersion = EU::TableNames::GetTableVersion(EU::TableNames::GetTableId(tableName));
#elif _BNSKR
		auto confirmedVersion = KR::TableNames::GetTableVersion(KR::TableNames::GetTableId(tableName));
#endif

#ifdef _DEBUG
		std::wcout << "Confirmed version for " << tableName << " is " << confirmedVersion.Version.VersionKey << std::endl;
		std::wcout << "Confirmed major version for " << tableName << " is " << confirmedVersion.Version.MajorVersion << std::endl;
		std::wcout << "Confirmed minor version for " << tableName << " is " << confirmedVersion.Version.MinorVersion << std::endl;
		std::cout << std::endl;
#endif // _DEBUG
		if (tableDef->version.ver == confirmedVersion.Version.VersionKey) {
			versionCheckSuccess[tableName] = true;
		}
	}
	return true;
}

bool DatafileService::SetupTableIds()
{
	if (this->dataManagerPtr == nullptr || *this->dataManagerPtr == NULL) {
		return false;
	}
	bool success = false;
	const auto manager = reinterpret_cast<Data::DataManager*>(*this->dataManagerPtr);
	if (const auto tableId = DataHelper::GetTableId(manager, L"zoneenv2"); tableId != 0) {
		success = true;
		zoneenv2TableId = tableId;
	}
	else {
		success = false;
	}
	return success;
}

bool DatafileService::Setup() {
	try {
		if (auto successVersionCheck = CompatabilityCheck(); !successVersionCheck) return false;
		if (!AllVersionsSuccess()) {
			MessageBox(nullptr, L"Plugin version is not 100% compatible with the game version.\nSome Features might not work but your game will not break.\nPlease update the plugin if available.", L"Collection Helper Version Mismatch", MB_OK | MB_ICONWARNING);
		}
		auto success = SetupTableIds();
		if (success) {
			SetupComplete = true;
		}
		return success;
	}
	catch (const std::exception& ex) {
		// Log the exception message if needed
#ifdef _DEBUG
		std::cerr << "Exception caught in Setup: " << ex.what() << std::endl;
#endif
		return false;
	}
	catch (...) {
		// Catch any other types of exceptions
#ifdef _DEBUG
		std::cerr << "Unknown exception caught in Setup." << std::endl;
#endif
		return false;
	}
}