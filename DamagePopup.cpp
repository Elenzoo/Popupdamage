#include "DamagePopup.h"

namespace GOTHIC_ENGINE
{
    zCArray<DamagePopup*> g_Popups;

    DamagePopup::DamagePopup(zCVob* target, int dmg, bool isCrit, oEDamageIndex dmgIndex)
    {
        this->target = target;
        this->lifetime = 400; // ok. 4 sekundy przy 100 FPS

        // Każdy kolejny popup jest wyżej, żeby się nie nakładały
        this->offset = zVEC3(
            0,
            target->bbox3D.maxs[VY] + 50.0f + g_Popups.GetNumInList() * 30.0f,
            0
        );

        // Usunięty minus — dmg pokazujemy jako liczbę dodatnią
        text = zSTRING(dmg);

        if (isCrit)
        {
            color = zCOLOR(231, 76, 60, 255);
            scale = 1.2f;
        }
        else
        {
            switch (dmgIndex)
            {
            case oEDamageIndex::oEDamageIndex_Fire:
                color = zCOLOR(230, 126, 34, 255);
                break;
            case oEDamageIndex::oEDamageIndex_Magic:
                color = zCOLOR(155, 89, 182, 255);
                break;
            default:
                color = zCOLOR(255, 180, 50, 255);
                break;
            }
            scale = 0.8f;
        }

        view = new zCView(0, 0, 8192, 8192);
        view->SetFont("FONT_OLD_20_WHITE.TGA");
        view->SetFontColor(color);
        view->Open();

        g_Popups.InsertEnd(this);
    }

    DamagePopup::~DamagePopup()
    {
        if (view)
        {
            view->Close();
            view = nullptr;
        }
    }

    void DamagePopup::Update()
    {
        lifetime--;

        if (lifetime <= 0 || !target || !target->globalVobTreeNode)
        {
            g_Popups.Remove(this);
            delete this;
            return;
        }

        offset[VY] += 0.5f;

        zVEC3 worldPos = target->GetPositionWorld() + offset;

        int x, y;
        ogame->GetCamera()->Project(&worldPos, x, y);

        // Ograniczamy współrzędne do ekranu
        x = max(0, min(x, 1920));
        y = max(0, min(y, 1080));

        float progress = float(lifetime) / 400.0f;
        float alpha = progress * 255.0f;
        color.alpha = (int)alpha;

        view->SetFontColor(color);
        view->Print(x, y, text);
    }

}
