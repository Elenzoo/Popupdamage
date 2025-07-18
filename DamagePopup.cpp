#include "DamagePopup.h"

namespace GOTHIC_ENGINE {
    zCArray<DamagePopup*> g_Popups;

    DamagePopup::DamagePopup(zCVob* target, int dmg, bool isCrit, oEDamageIndex dmgIndex) {
        this->target = target;
        this->lifetime = 400;
        this->offset = zVEC3(0, 0, 0);
        this->text = zSTRING(dmg);

        if (isCrit) {
            color = zCOLOR(231, 76, 60, 255);  // Czerwony dla krytyków
            scale = 1.2f;
        }
        else {
            switch (dmgIndex) {
            case oEDamageIndex::oEDamageIndex_Fire:
                color = zCOLOR(230, 126, 34, 255); break;
            case oEDamageIndex::oEDamageIndex_Magic:
                color = zCOLOR(155, 89, 182, 255); break;
            default:
                color = zCOLOR(255, 180, 50, 255); break;  // Żółty domyślny
            }
            scale = 1.0f;
        }

        view = new zCView(0, 0, 8192, 8192);
        view->SetFont("FONT_OLD_20_WHITE.TGA");
        view->SetFontColor(color);
        view->Open();

        g_Popups.InsertEnd(this);
    }

    DamagePopup::~DamagePopup() {
        if (view) {
            view->Close();
            delete view;
            view = nullptr;
        }
    }

    void DamagePopup::Update() {
        lifetime--;
        if (lifetime <= 0 || !target || !target->globalVobTreeNode) {
            g_Popups.Remove(this);
            delete this;
            return;
        }

        // Nad głową gracza, ale bardziej na środku
        zVEC3 worldPos = player->GetPositionWorld();
        worldPos[VY] += (player->bbox3D.maxs[VY] - player->bbox3D.mins[VY]) * 1.1f;

        zVEC2 viewPos;
        ogame->GetCamera()->Project(&worldPos, viewPos[VX], viewPos[VY]);

        // Ograniczenie do 8192x8192 (zgodnie z Union SDK)
        if (viewPos[VX] < 0) viewPos[VX] = 0;
        if (viewPos[VX] > 8191) viewPos[VX] = 8191;
        if (viewPos[VY] < 0) viewPos[VY] = 0;
        if (viewPos[VY] > 8191) viewPos[VY] = 8191;

        float progress = float(lifetime) / 400.0f;
        color.alpha = (int)(progress * 255.0f);

        view->SetFontColor(color);
        view->PrintCX((int)viewPos[VY], text);  // Centrum w poziomie, wysokość dynamiczna
    }

}
