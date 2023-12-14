#include "stdAfx.h"
#include "embedded_editor_hud.h"
#include "../../xr_3da/device.h"
#include "player_hud.h"
#include "WeaponMagazined.h"
#include "embedded_editor_helper.h"
#include <addons/ImGuizmo/ImGuizmo.h>
#include "actor.h"
#include "../xr_3da/camerabase.h"
#include "../xr_3da/IGame_Persistent.h"

void ShowHudEditor(bool& show)
{
    ImguiWnd wnd("HUD Editor", &show);
    if (wnd.Collapsed)
        return;

    if (!g_player_hud)
        return;

    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::OPERATION mode = io.KeyCtrl ? ImGuizmo::ROTATE : ImGuizmo::TRANSLATE;

    auto item = g_player_hud->attached_item(0);
	if (item)
	{
		ImGui::Text("Item 0");
		ImGui::Separator();
        ImGui::InputFloat3("hands_position 0", (float*)&item->m_measures.m_hands_attach[0]);
        ImGui::InputFloat3("hands_orientation 0", (float*)&item->m_measures.m_hands_attach[1]);
        ImGui::Separator();

        auto& pos = item->m_measures.m_item_attach;
        ImGuizmo::Manipulate((float*)&Device.mView, (float*)&Device.mProject, mode, ImGuizmo::WORLD,
                             (float*)&item->m_attach_offset);
        if (ImGuizmo::IsUsing())
        {
            Fvector ypr;
            item->m_attach_offset.getHPB(ypr.x, ypr.y, ypr.z);
            ypr.mul(180.f / PI);
            pos[1] = ypr;
            pos[0] = item->m_attach_offset.c;
        }

        ImGui::InputFloat3("item_position 0", (float*)&item->m_measures.m_item_attach[0]);
        ImGui::InputFloat3("item_orientation 0", (float*)&item->m_measures.m_item_attach[1]);

        ImGui::Separator();
        ImGui::InputFloat3("aim_hud_offset_pos 0", (float*)&item->m_measures.m_hands_offset[0][1]);
        ImGui::InputFloat3("aim_hud_offset_rot 0", (float*)&item->m_measures.m_hands_offset[1][1]);
        ImGui::Separator();
        ImGui::InputFloat3("gl_hud_offset_pos 0", (float*)&item->m_measures.m_hands_offset[0][2]);
        ImGui::InputFloat3("gl_hud_offset_rot 0", (float*)&item->m_measures.m_hands_offset[1][2]);
        ImGui::Separator();
        ImGui::InputFloat3("fire_point 0", (float*)&item->m_measures.m_fire_point_offset);
        ImGui::InputFloat3("fire_point2 0", (float*)&item->m_measures.m_fire_point2_offset);
        ImGui::InputFloat3("shell_point 0", (float*)&item->m_measures.m_shell_point_offset);
		if (const auto Wpn = smart_cast<CWeapon*>(item->m_parent_hud_item))
		{
			ImGui::Separator();
			ImGui::InputFloat3("laserdot_attach_offset 0", (float*)&Wpn->laserdot_attach_offset);
			ImGui::InputFloat3("torch_attach_offset 0", (float*)&Wpn->flashlight_attach_offset);
		}
	}

    item = g_player_hud->attached_item(1);
	if (item)
	{
		ImGui::Text("\nItem 1");
		ImGui::Separator();
        ImGui::InputFloat3("hands_position 1", (float*)&item->m_measures.m_hands_attach[0][0]);
        ImGui::InputFloat3("hands_orientation 1", (float*)&item->m_measures.m_hands_attach[1][0]);
        ImGui::Separator();
        ImGui::InputFloat3("item_position 1", (float*)&item->m_measures.m_item_attach[0]);
        ImGui::InputFloat3("item_orientation 1", (float*)&item->m_measures.m_item_attach[1]);
	}
	if (ImGui::Button("Save"))
	{
        g_player_hud->SaveCfg(0);
        g_player_hud->SaveCfg(1);
	}
}