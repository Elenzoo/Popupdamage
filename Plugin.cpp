#include "DamagePopup.h"
namespace GOTHIC_ENGINE
{
    int g_ShowDamageValue = 0;
    int g_ShowDamageTimer = 0;

    zCOLOR g_GuiDamageColor = zCOLOR(255, 180, 50, 255);
    zSTRING g_GuiDamageText;

    // -------------------------------------------------
    // OnDamage_Hit Hook
    // -------------------------------------------------
    int __fastcall My_OnDamage_Hit(
        oCNpc* victim,
        void* edx,
        oCNpc::oSDamageDescriptor& desc
    );

    CInvoke<int(__fastcall*)(
        oCNpc*,
        void*,
        oCNpc::oSDamageDescriptor&
        )> Ivk_oCNpc_OnDamage_Hit(
            (void*)0x00666610,
            &My_OnDamage_Hit,
            IVK_NORMAL
        );

    int __fastcall My_OnDamage_Hit(
        oCNpc* victim,
        void* edx,
        oCNpc::oSDamageDescriptor& desc
    )
    {
        int initialHp = victim->attribute[NPC_ATR_HITPOINTS];
        int result = Ivk_oCNpc_OnDamage_Hit(victim, edx, desc);
        int hpDiff = initialHp - victim->attribute[NPC_ATR_HITPOINTS];

        if (desc.pNpcAttacker == player && hpDiff > 0)
        {
            // Popup
            new DamagePopup(
                reinterpret_cast<zCVob*>(victim),
                hpDiff,
                false,
                static_cast<oEDamageIndex>(desc.enuModeDamage)
            );





            // GUI text
            g_ShowDamageValue = hpDiff;
            g_ShowDamageTimer = 60;

            g_GuiDamageColor = zCOLOR(255, 180, 50, 255);
            g_GuiDamageText = "Zadane obrażenia: " + zSTRING(hpDiff);
        }

        return result;
    }

    // -------------------------------------------------
    // OnDamage Hook
    // -------------------------------------------------
    int __fastcall My_OnDamage(
        oCNpc* victim,
        void* edx,
        zCVob* attacker,
        zCVob* weapon,
        float fDamage,
        int iDamageType,
        const zVEC3& dir
    );

    CInvoke<int(__fastcall*)(
        oCNpc*,
        void*,
        zCVob*,
        zCVob*,
        float,
        int,
        const zVEC3&
        )> Ivk_oCNpc_OnDamage(
            (void*)0x0067B860,
            &My_OnDamage,
            IVK_NORMAL
        );

    int __fastcall My_OnDamage(
        oCNpc* victim,
        void* edx,
        zCVob* attacker,
        zCVob* weapon,
        float fDamage,
        int iDamageType,
        const zVEC3& dir
    )
    {
        int initialHp = victim->attribute[NPC_ATR_HITPOINTS];
        int result = Ivk_oCNpc_OnDamage(victim, edx, attacker, weapon, fDamage, iDamageType, dir);
        int hpDiff = initialHp - victim->attribute[NPC_ATR_HITPOINTS];

        if (attacker == player && hpDiff > 0)
        {
            new DamagePopup(victim, hpDiff, false, (oEDamageIndex)iDamageType);

            g_ShowDamageValue = hpDiff;
            g_ShowDamageTimer = 60;
            g_GuiDamageColor = zCOLOR(255, 180, 50, 255);
            g_GuiDamageText = "Zadane obrażenia: " + zSTRING(hpDiff);
        }

        return result;
    }

    void Game_Init()
    {
        MessageBoxA(0, "PLUGIN LOADED!", "Union Test", MB_OK);
    }

    void Game_Loop()
    {
        // Popup updates
        for (int i = 0; i < g_Popups.GetNumInList(); ++i)
        {
            if (g_Popups[i])
                g_Popups[i]->Update();
        }

        // GUI damage text
        if (g_ShowDamageTimer > 0)
        {
            g_ShowDamageTimer--;

            int screenWidth = screen->anx(0);
            int screenHeight = screen->any(0);

            int x = 50;
            int y = screenHeight / 2;

            screen->SetFontColor(g_GuiDamageColor);
            screen->Print(x, y, g_GuiDamageText);
        }
    }

    void Game_Entry() {}
    void Game_Exit() {}
    void Game_PreLoop() {}
    void Game_PostLoop() {}
    void Game_MenuLoop() {}
    void Game_SaveBegin() {}
    void Game_SaveEnd() {}
    void LoadBegin() {}
    void LoadEnd() {}
    void Game_LoadBegin_NewGame() { LoadBegin(); }
    void Game_LoadEnd_NewGame() { LoadEnd(); }
    void Game_LoadBegin_SaveGame() { LoadBegin(); }
    void Game_LoadEnd_SaveGame() { LoadEnd(); }
    void Game_LoadBegin_ChangeLevel() { LoadBegin(); }
    void Game_LoadEnd_ChangeLevel() { LoadEnd(); }
    void Game_LoadBegin_Trigger() {}
    void Game_LoadEnd_Trigger() {}
    void Game_Pause() {}
    void Game_Unpause() {}
    void Game_DefineExternals() {}
    void Game_ApplyOptions() {}

#define AppDefault True
    CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null :
        CApplication::CreateRefApplication(
            Enabled(AppDefault) Game_Entry,
            Enabled(AppDefault) Game_Init,
            Enabled(AppDefault) Game_Exit,
            Enabled(AppDefault) Game_PreLoop,
            Enabled(AppDefault) Game_Loop,
            Enabled(AppDefault) Game_PostLoop,
            Enabled(AppDefault) Game_MenuLoop,
            Enabled(AppDefault) Game_SaveBegin,
            Enabled(AppDefault) Game_SaveEnd,
            Enabled(AppDefault) Game_LoadBegin_NewGame,
            Enabled(AppDefault) Game_LoadEnd_NewGame,
            Enabled(AppDefault) Game_LoadBegin_SaveGame,
            Enabled(AppDefault) Game_LoadEnd_SaveGame,
            Enabled(AppDefault) Game_LoadBegin_ChangeLevel,
            Enabled(AppDefault) Game_LoadEnd_ChangeLevel,
            Enabled(AppDefault) Game_LoadBegin_Trigger,
            Enabled(AppDefault) Game_LoadEnd_Trigger,
            Enabled(AppDefault) Game_Pause,
            Enabled(AppDefault) Game_Unpause,
            Enabled(AppDefault) Game_DefineExternals,
            Enabled(AppDefault) Game_ApplyOptions
        );
}
