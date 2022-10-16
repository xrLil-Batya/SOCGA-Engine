#include "stdafx.h"
#include "weaponval.h"

CWeaponVal::CWeaponVal() : CWeaponMagazined("VAL", SOUND_TYPE_WEAPON_SUBMACHINEGUN)
{
    m_weight = 1.5f;
    SetSlot(SECOND_WEAPON_SLOT);
}

CWeaponVal::~CWeaponVal() {}

void CWeaponVal::Load(LPCSTR section)
{
	inherited::Load(section);
	IsGauss = READ_IF_EXISTS(pSettings, r_bool, section, "is_gauss", false);
}

const char* CWeaponVal::GetFireModeMask()
{
	if(IsGauss)
	{
		string_path guns_fire_mode_mask;
		xr_strconcat(guns_fire_mode_mask, "mask_firemode_", MUIEnabled ? "a" : "1");
		return READ_IF_EXISTS(pSettings, r_string, *HudSection(), guns_fire_mode_mask, "");
	}
	else return inherited::GetFireModeMask();
}

void CWeaponVal::switch2_PrevFireMode()
{
	if(IsGauss)
	{
		string_path guns_firemode_anm;
		xr_strconcat(guns_firemode_anm, "anm_changefiremode_from_", MUIEnabled ? "a" : "1", "_to_", MUIEnabled ? "1" : "a", iAmmoElapsed == 0 ? "_empty" : (IsMisfire() ? "_jammed" : ""));
		if(AnimationExist(guns_firemode_anm))
		{
			SwitchState(ePrevFireMode);
			PlayHUDMotion(guns_firemode_anm, true, ePrevFireMode);
		}
		else
			OnPrevFireMode();

		PlaySound(sndFireModes, get_LastFP());
	}
	else inherited::switch2_PrevFireMode();
}

void CWeaponVal::switch2_NextFireMode()
{
	if(IsGauss) switch2_PrevFireMode();
	else inherited::switch2_PrevFireMode();
}

void CWeaponVal::OnAnimationEnd(u32 state)
{
	switch(state)
	{
		case ePrevFireMode:
		{
			if(IsGauss)
			{
				SetPending(false);
				MUIEnabled = !MUIEnabled;
				SwitchState(eIdle);
				break;
			}
		}
		default: inherited::OnAnimationEnd(state);
	}
}

bool CWeaponVal::Action(s32 cmd, u32 flags)
{
	switch(cmd)
	{
		case kFLASHLIGHT:
		{
			if(IsGauss)
			{
				if ((flags & CMD_START) && !IsPending())
				{
					switch2_PrevFireMode();
					return true;
				}
				else return false;
			}
		}
		default: return inherited::Action(cmd, flags);
	}
}

using namespace luabind;

#pragma optimize("s", on)
void CWeaponVal::script_register(lua_State* L) { module(L)[class_<CWeaponVal, CGameObject>("CWeaponVal").def(constructor<>())]; }
