#include "DamagePopup.h"

namespace GOTHIC_ENGINE {

    struct DamageLine {
        zSTRING text;
        int timer;
    };

    zCArray<DamageLine> g_DamageLog;
    zCOLOR g_GuiDamageColor = zCOLOR(255, 180, 50, 255);

    int __fastcall My_OnDamage_Hit(oCNpc* victim, void*, oCNpc::oSDamageDescriptor& desc);
    CInvoke<int(__fastcall*)(oCNpc*, void*, oCNpc::oSDamageDescriptor&)> Ivk_oCNpc_OnDamage_Hit(
        (void*)0x00666610, &My_OnDamage_Hit, IVK_NORMAL);

    int __fastcall My_OnDamage_Hit(oCNpc* victim, void* edx, oCNpc::oSDamageDescriptor& desc) {
        int hpBefore = victim->attribute[NPC_ATR_HITPOINTS];
        int result = Ivk_oCNpc_OnDamage_Hit(victim, edx, desc);
        int dmgDealt = hpBefore - victim->attribute[NPC_ATR_HITPOINTS];

        if (desc.pNpcAttacker == player && dmgDealt > 0) {
            new DamagePopup(reinterpret_cast<zCVob*>(player), dmgDealt, false, static_cast<oEDamageIndex>(desc.enuModeDamage));

            DamageLine line;
            line.text = "OBRAŻENIA : " + zSTRING(dmgDealt);
            line.timer = 2160;
            g_DamageLog.InsertEnd(line);
            if (g_DamageLog.GetNumInList() > 5)
                g_DamageLog.RemoveIndex(0);
        }

        return result;
    }

    void Game_Loop() {
        // Aktualizacja popupów
        for (int i = 0; i < g_Popups.GetNumInList(); ++i)
            if (g_Popups[i])
                g_Popups[i]->Update();

        // GUI tekstowe po lewej
        int fontHeight = screen->FontY();
        int spacing = fontHeight + 10;
        int blockHeight = g_DamageLog.GetNumInList() * spacing;
        int startY = (2 * 8192 / 3) - (blockHeight / 2);
        int x = 50;

        screen->SetFontColor(g_GuiDamageColor);
        for (int i = 0; i < g_DamageLog.GetNumInList();) {
            g_DamageLog[i].timer--;
            if (g_DamageLog[i].timer <= 0)
                g_DamageLog.RemoveIndex(i);
            else {
                int y = startY + i * spacing;
                screen->Print(x, y, g_DamageLog[i].text);
                i++;
            }
        }
    }

    // Reszta funkcji Union SDK
    void Game_Init() {}
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
