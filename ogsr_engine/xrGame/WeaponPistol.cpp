#include "stdafx.h"
#include "weaponpistol.h"
#include "ParticlesObject.h"
#include "actor.h"

CWeaponPistol::CWeaponPistol(LPCSTR name) : CWeaponCustomPistol(name)
{
    m_eSoundClose = ESoundTypes(SOUND_TYPE_WEAPON_RECHARGING /*| eSoundType*/);
    m_opened = false;
    SetPending(FALSE);
}

CWeaponPistol::~CWeaponPistol(void) {}

void CWeaponPistol::net_Destroy()
{
    inherited::net_Destroy();

    // sounds
    HUD_SOUND::DestroySound(sndClose);
}

void CWeaponPistol::net_Relcase(CObject* object) { inherited::net_Relcase(object); }

void CWeaponPistol::OnDrawUI() { inherited::OnDrawUI(); }

void CWeaponPistol::Load(LPCSTR section)
{
    inherited::Load(section);

    HUD_SOUND::LoadSound(section, "snd_close", sndClose, m_eSoundClose);
}

void CWeaponPistol::OnH_B_Chield()
{
    inherited::OnH_B_Chield();
    m_opened = false;
}

void CWeaponPistol::PlayAnimShow()
{
    VERIFY(GetState() == eShowing);
    if (iAmmoElapsed >= 1)
        m_opened = false;
    else
        m_opened = true;

    inherited::PlayAnimShow();
}

/*void CWeaponPistol::PlayAnimBore()
{
    if (m_opened)
        PlayHUDMotion({ "anim_empty", "anm_bore_empty" }, true, GetState());
    else
        inherited::PlayAnimBore();
}*/

void CWeaponPistol::PlayAnimIdleSprint()
{
    inherited::PlayAnimIdleSprint();
}

void CWeaponPistol::PlayAnimIdleMoving()
{
    inherited::PlayAnimIdleMoving();
}

void CWeaponPistol::PlayAnimIdleMovingSlow()
{
    inherited::PlayAnimIdleMovingSlow();
}

void CWeaponPistol::PlayAnimIdleMovingCrouch()
{
    inherited::PlayAnimIdleMovingCrouch();
}

void CWeaponPistol::PlayAnimIdleMovingCrouchSlow()
{
    inherited::PlayAnimIdleMovingCrouchSlow();
}

void CWeaponPistol::PlayAnimIdle()
{
    VERIFY(GetState() == eIdle);

    if (TryPlayAnimIdle())
        return;

    if (IsZoomed())
        PlayAnimAim();
    else
        inherited::PlayAnimIdle();
}

void CWeaponPistol::PlayAnimAim()
{
    inherited::PlayAnimAim();
}

void CWeaponPistol::PlayAnimReload()
{
    VERIFY(GetState() == eReload);
    inherited::PlayAnimReload();

    m_opened = false;
}

void CWeaponPistol::PlayAnimHide()
{
    VERIFY(GetState() == eHiding);
    inherited::PlayAnimHide();
}

void CWeaponPistol::PlayAnimShoot()
{
    string128 guns_shoot_anm;
    xr_strconcat(guns_shoot_anm, "anm_shoot", (this->IsZoomed() && !this->IsRotatingToZoom()) ? "_aim" : "", GetFireModeMask(), iAmmoElapsed == 1 ? "_last" : "",
                 this->IsSilencerAttached() ? "_sil" : "");
    if (AnimationExist(guns_shoot_anm))
    {
        PlayHUDMotion(guns_shoot_anm, used_cop_fire_point(), GetState());
        m_opened = iAmmoElapsed <= 1;
        return;
    }

    if (iAmmoElapsed > 1)
    {
        PlayHUDMotion({"anim_shoot", "anm_shots"}, used_cop_fire_point(), GetState());
        m_opened = false;
    }
    else
    {
        PlayHUDMotion({"anim_shot_last", "anm_shot_l"}, used_cop_fire_point(), GetState());
        m_opened = true;
    }
}

void CWeaponPistol::switch2_Reload()
{
    //.	if(GetState()==eReload) return;
    inherited::switch2_Reload();
}

void CWeaponPistol::OnAnimationEnd(u32 state)
{
    if (state == eHiding && m_opened)
    {
        m_opened = false;
        //		switch2_Hiding();
    }
    inherited::OnAnimationEnd(state);
}

/*
void CWeaponPistol::OnShot		()
{
    // Sound
    PlaySound		(*m_pSndShotCurrent,get_LastFP());

    AddShotEffector	();

    PlayAnimShoot	();

    // Shell Drop
    Fvector vel;
    PHGetLinearVell(vel);
    OnShellDrop					(get_LastSP(),  vel);

    // Огонь из ствола

    StartFlameParticles	();
    R_ASSERT2(!m_pFlameParticles || !m_pFlameParticles->IsLooped(),
              "can't set looped particles system for shoting with pistol");

    //дым из ствола
    StartSmokeParticles	(get_LastFP(), vel);
}
*/

void CWeaponPistol::UpdateSounds()
{
    inherited::UpdateSounds();

    if (sndClose.playing())
        sndClose.set_position(get_LastFP());
}
