#include "Hooks.h"
#include <unordered_map>
#include <EU/zoneenv2/AAA_zoneenv2_RecordBase.h>
#include <KR/zoneenv2/AAA_zoneenv2_RecordBase.h>
#include <EU/zoneenv2/zoneenv2_chest_Record.h>
#include <KR/zoneenv2/zoneenv2_chest_Record.h>
#include <unordered_set>
#include "DatafileService.h"

extern uintptr_t* BNSClientInstancePtr;
extern _AddInstantNotification oAddInstantNotification;

template <typename Callable>
void handleKeyEvent(EInputKeyEvent const* InputKeyEvent, int vKeyTarget, const Callable& onPress) {
	static std::unordered_map<int, bool> toggleKeys;
	if (vKeyTarget == 0)  return;
	if (InputKeyEvent->_vKey == vKeyTarget) {
		bool& toggleKey = toggleKeys[vKeyTarget];
		if (!toggleKey && InputKeyEvent->KeyState == EngineKeyStateType::EKS_PRESSED) {
			toggleKey = true;
			onPress();
		}
		else if (toggleKey && InputKeyEvent->KeyState == EngineKeyStateType::EKS_RELEASED) {
			toggleKey = false;
		}
	}
}

template <typename Callable>
void handleKeyEventWithModifiers(
	EInputKeyEvent const* InputKeyEvent,
	int vKeyTarget,
	bool alt,
	bool shift,
	bool ctrl,
	const Callable& onPress
) {
	static std::unordered_map<int, bool> toggleKeys;
	if (vKeyTarget == 0)  return;
	if (InputKeyEvent->_vKey == vKeyTarget) {
		bool& toggleKey = toggleKeys[vKeyTarget];
		if (!toggleKey && InputKeyEvent->KeyState == EngineKeyStateType::EKS_PRESSED) {
			// Check for Alt, Shift, and Ctrl modifiers
			if ((alt == InputKeyEvent->bAltPressed) &&
				(shift == InputKeyEvent->bShiftPressed) &&
				(ctrl == InputKeyEvent->bCtrlPressed)) {
				toggleKey = true;
				onPress();
			}
		}
		else if (toggleKey && InputKeyEvent->KeyState == EngineKeyStateType::EKS_RELEASED) {
			toggleKey = false;
		}
	}
}

World* (__fastcall* BNSClient_GetWorld)();

const wchar_t* jungIndicator = L"00008603.Envguide_5510_2";
const wchar_t* jungIcon = L"00009499.WeaponHolder";
const wchar_t* jungIconOver = L"00009499.WeaponHolder_over";

const wchar_t* wonIndicator = L"00008603.Envguide_2000";
const wchar_t* wonIcon = L"00009499.StoneLantern";
const wchar_t* wonIconOver = L"00009499.StoneLantern_over";

const wchar_t* gakIndicator = L"00008603.Envguide_4453";
const wchar_t* gakIcon = L"00009499.Dungeon_FrozenArk_Lever";
const wchar_t* gakIconOver = L"00009499.Dungeon_FrozenArk_Lever_over";

const wchar_t* juIndicator = L"00008603.Envguide_3010";
const wchar_t* juIcon = L"00009499.Dungeon_NaryuPole";
const wchar_t* juIconOver = L"00009499.Dungeon_NaryuPole_over";

const std::unordered_set<std::wstring> jungEntries = {
	L"e_chest_jeryoungrim_collectD",
	L"e_chest_Daesamak_collectD",
	L"e_chest_Suwal_collectD"
};

const std::unordered_set<std::wstring> wonEntries = {
	L"e_chest_jeryoungrim_collectB",
	L"e_chest_Daesamak_collectB",
	L"e_chest_Suwal_collectB"
};

const std::unordered_set<std::wstring> gakEntries = {
	L"e_chest_jeryoungrim_collectC",
	L"e_chest_Daesamak_collectC",
	L"e_chest_Suwal_collectC"
};

const std::unordered_set<std::wstring> juEntries = {
	L"e_chest_jeryoungrim_collectA",
	L"e_chest_Daesamak_collectA",
	L"e_chest_Suwal_collectA"
};

void setChestRecordProperties(
#ifdef _BNSKR
	BnsTables::KR::zoneenv2_chest_Record* chestRecord,
#elif _BNSEU
	BnsTables::EU::zoneenv2_chest_Record* chestRecord,
#endif
	const wchar_t* indicator,
	const wchar_t* icon,
	const wchar_t* iconOver
) {
	chestRecord->default_indicator_image = const_cast<wchar_t*>(indicator);
	chestRecord->mapunit_image_enable_close_true_imageset = const_cast<wchar_t*>(icon);
	chestRecord->mapunit_image_enable_close_true_over_imageset = const_cast<wchar_t*>(iconOver);
	chestRecord->mapunit_image_enable_close_false_imageset = const_cast<wchar_t*>(icon);
	chestRecord->mapunit_image_enable_close_false_over_imageset = const_cast<wchar_t*>(iconOver);
	chestRecord->mapunit_image_enable_open_imageset = const_cast<wchar_t*>(icon);
	chestRecord->mapunit_image_enable_open_over_imageset = const_cast<wchar_t*>(iconOver);
}

/// <summary>
/// Hook into Datamanager resolving a table reference
/// </summary>
DrEl* (__fastcall* oFind_b8AutoId)(DrMultiKeyTable* thisptr, unsigned __int64 key);
DrEl* __fastcall hkFind_b8AutoId(DrMultiKeyTable* thisptr, unsigned __int64 key) {
	auto recordBase = oFind_b8AutoId(thisptr, key);
	if (recordBase == nullptr) return recordBase;
	if (thisptr->_tabledef->type != g_DatafileService.GetZoneenv2TableId()) return recordBase;
#ifdef _BNSKR
	auto record = (BnsTables::KR::zoneenv2_Record*)recordBase;
	if (record->subtype != BnsTables::KR::zoneenv2_chest_Record::SubType()) return recordBase;
	auto chestRecord = (BnsTables::KR::zoneenv2_chest_Record*)record;
#elif _BNSEU
	auto record = (BnsTables::EU::zoneenv2_Record*)recordBase;
	if (record->subtype != BnsTables::EU::zoneenv2_chest_Record::SubType()) return recordBase;
	auto chestRecord = (BnsTables::EU::zoneenv2_chest_Record*)record;
#endif
	if (chestRecord->expedition_type != 1) return recordBase;
	chestRecord->show_quest_indicator = true;
	chestRecord->init_enable = true;

	if (gakEntries.contains(chestRecord->alias)) {
		setChestRecordProperties(chestRecord, gakIndicator, gakIcon, gakIconOver);
	}
	else if (jungEntries.contains(chestRecord->alias)) {
		setChestRecordProperties(chestRecord, jungIndicator, jungIcon, jungIconOver);
	}
	else if (juEntries.contains(chestRecord->alias)) {
		setChestRecordProperties(chestRecord, juIndicator, juIcon, juIconOver);
	}
	else if (wonEntries.contains(chestRecord->alias)) {
		setChestRecordProperties(chestRecord, wonIndicator, wonIcon, wonIconOver);
	}
	chestRecord->mapunit_image_enable_close_true_size_x = 23;
	chestRecord->mapunit_image_enable_close_true_size_y = 23;
	chestRecord->mapunit_image_enable_close_false_size_x = 23;
	chestRecord->mapunit_image_enable_close_false_size_y = 23;
	chestRecord->mapunit_image_enable_open_size_x = 23;
	chestRecord->mapunit_image_enable_open_size_y = 23;
	chestRecord->mapunit_image_disable_size_x = 23;
	chestRecord->mapunit_image_disable_size_y = 23;
	chestRecord->mapunit_image_unconfirmed_size_x = 18;
	chestRecord->mapunit_image_unconfirmed_size_y = 18;

	return recordBase;
}