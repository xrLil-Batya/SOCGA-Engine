#pragma once
#include "hud_item_object.h"
#include "HudSound.h"

class CLAItem;

class CFlashlight : public CHudItemObject {
private:
	using inherited = CHudItemObject;
	inline	bool	can_use_dynamic_lights();

	HUD_SOUND sndShow, sndHide, sndTurnOn, sndTurnOff;
protected:
	bool			m_bFastAnimMode;
	bool			m_bNeedActivation;

	float			fBrightness;
	CLAItem* lanim;

	shared_str		light_trace_bone;

	bool			m_switched_on;
	ref_light		light_render;
	ref_light		light_omni;
	ref_glow		glow_render;
	shared_str		m_light_section;

	bool			CheckCompatibilityInt(CHudItem* itm, u16* slot_to_activate);
	void			UpdateVisibility();
public:

	CFlashlight();
	virtual			~CFlashlight();

	virtual BOOL 	net_Spawn(CSE_Abstract* DC) override;
	virtual void 	Load(LPCSTR section) override;

	virtual void 	OnH_A_Chield() override;
	virtual void 	OnH_B_Independent(bool just_before_destroy) override;

	virtual void 	shedule_Update(u32 dt) override;
	virtual void 	UpdateCL() override;

	bool 			IsWorking();

	virtual void 	OnMoveToSlot() override;
	virtual void 	OnMoveToRuck(EItemPlace prevPlace) override;

	virtual void	OnActiveItem() {};
	virtual void	OnHiddenItem() {};
	virtual void	OnStateSwitch(u32 S, u32 oldState) override;
	virtual void	OnAnimationEnd(u32 state) override;
	virtual	void	UpdateXForm() override;

	void			ToggleDevice(bool bFastMode);
	void			HideDevice(bool bFastMode);
	void			ShowDevice(bool bFastMode);
	virtual bool	CheckCompatibility(CHudItem* itm) override;

	virtual void	create_physic_shell() override;
	virtual void	activate_physic_shell() override;
	virtual void	setup_physic_shell() override;

	void	Switch();
	void	Switch(bool light_on);
	bool	torch_active() const;

    virtual size_t GetWeaponTypeForCollision() const override { return Detector; }
    virtual Fvector GetPositionForCollision() override;
    virtual Fvector GetDirectionForCollision() override;
};