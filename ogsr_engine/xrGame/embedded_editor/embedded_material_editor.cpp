#include "stdAfx.h"
#include "embedded_editor_hud.h"
#include "../../xr_3da/device.h"
#include "embedded_editor_helper.h"

extern ENGINE_API char* d_texture_name;
extern ENGINE_API float d_material_weight;
extern ENGINE_API int d_material;
extern ENGINE_API bool override_material;

void ShowMaterialEditor(bool& show)
{ 
    ImguiWnd wnd("Material Editor", &show);
    if (wnd.Collapsed)
        return;

    ImGui::InputText("Texture path", d_texture_name, 99);

    constexpr const char* items[] = {
        "OrenNayar Blin",
        "Blin Phong",
        "Phong Metal",
        "Metal OrenNayar",
    };
    ImGui::Combo("Material", &d_material, items, IM_ARRAYSIZE(items));
    ImGui::InputFloat("Material weight", &d_material_weight, 0.f, 0.f);
    ImGui::Separator();
    const auto tex = Device.m_pRender->GetResourceManager()->FindTexture(d_texture_name);
    if (!tex.size())
    {
        ImGui::Text("Warning! Texture was not found or was not loaded.");
        override_material = false;
    }
    else
        ImGui::Checkbox("Apply Material", &override_material);
}