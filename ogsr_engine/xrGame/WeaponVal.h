#pragma once

#include "weaponmagazined.h"
#include "script_export_space.h"

class CWeaponVal : public CWeaponMagazined
{
    using inherited = CWeaponMagazined;
	bool IsGauss{}, MUIEnabled{};
protected:
	virtual void switch2_PrevFireMode() override;
	virtual void switch2_NextFireMode() override;
    virtual void OnAnimationEnd(u32 state) override;

public:
    CWeaponVal();
    virtual ~CWeaponVal();
	virtual void Load(LPCSTR section) override;
    virtual bool Action(s32 cmd, u32 flags) override;
	virtual const char* GetFireModeMask() override;

    DECLARE_SCRIPT_REGISTER_FUNCTION
};
add_to_type_list(CWeaponVal)
#undef script_type_list
#define script_type_list save_type_list(CWeaponVal)
