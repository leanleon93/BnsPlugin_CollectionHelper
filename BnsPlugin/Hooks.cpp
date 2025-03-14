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

const wchar_t* newDefaultIndicatorImage2 = L"00008603.Envguide_5510_2";
const wchar_t* newWeaponHolder2 = L"00009499.WeaponHolder";
const wchar_t* newWeaponHolderOver2 = L"00009499.WeaponHolder_over";

const wchar_t* gakIndicator = L"00008603.Envguide_4453";
const wchar_t* gakIcon = L"00009499.Dungeon_FrozenArk_Lever";
const wchar_t* gakIconOver = L"00009499.Dungeon_FrozenArk_Lever_over";

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
		chestRecord->default_indicator_image = const_cast<wchar_t*>(gakIndicator);
		chestRecord->mapunit_image_enable_close_true_imageset = const_cast<wchar_t*>(gakIcon);
		chestRecord->mapunit_image_enable_close_true_over_imageset = const_cast<wchar_t*>(gakIconOver);
		chestRecord->mapunit_image_enable_close_false_imageset = const_cast<wchar_t*>(gakIcon);
		chestRecord->mapunit_image_enable_close_false_over_imageset = const_cast<wchar_t*>(gakIconOver);
		chestRecord->mapunit_image_enable_open_imageset = const_cast<wchar_t*>(gakIcon);
		chestRecord->mapunit_image_enable_open_over_imageset = const_cast<wchar_t*>(gakIconOver);
	}
	else {
		chestRecord->default_indicator_image = const_cast<wchar_t*>(newDefaultIndicatorImage2);
		chestRecord->mapunit_image_enable_close_true_imageset = const_cast<wchar_t*>(newWeaponHolder2);
		chestRecord->mapunit_image_enable_close_true_over_imageset = const_cast<wchar_t*>(newWeaponHolderOver2);
		chestRecord->mapunit_image_enable_close_false_imageset = const_cast<wchar_t*>(newWeaponHolder2);
		chestRecord->mapunit_image_enable_close_false_over_imageset = const_cast<wchar_t*>(newWeaponHolderOver2);
		chestRecord->mapunit_image_enable_open_imageset = const_cast<wchar_t*>(newWeaponHolder2);
		chestRecord->mapunit_image_enable_open_over_imageset = const_cast<wchar_t*>(newWeaponHolderOver2);
	}
	chestRecord->mapunit_image_enable_close_true_size_x = 23;
	chestRecord->mapunit_image_enable_close_true_size_y = 23;
	chestRecord->mapunit_image_enable_close_false_size_x = 23;
	chestRecord->mapunit_image_enable_close_false_size_y = 23;
	chestRecord->mapunit_image_enable_open_size_x = 23;
	chestRecord->mapunit_image_enable_open_size_y = 23;
	chestRecord->mapunit_image_disable_size_x = 23;
	chestRecord->mapunit_image_disable_size_y = 23;
	chestRecord->mapunit_image_unconfirmed_size_x = 23;
	chestRecord->mapunit_image_unconfirmed_size_y = 23;

	return recordBase;
}