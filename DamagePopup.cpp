#include "DamagePopup.h"

namespace GOTHIC_ENGINE
{
    zCArray<DamagePopup*> g_Popups;

    DamagePopup::DamagePopup(zCVob* target, int dmg, bool isCrit, oEDamageIndex dmgIndex)
    {
        this->target = target;
        this->offset = zVEC3(0, 100.0f, 0);
        this->lifetime = 60; // 1s at 60 FPS

        // Prepare text
        text = "-";
        text += zSTRING(dmg);

        // Color logic
        if (isCrit)
        {
            color = zCOLOR(231, 76, 60, 255);
            scale = 2.0f;
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
            scale = 1.0f;
        }

        view = new zCView(0, 0, 8192, 8192);
        view->SetFont("FONT_DEFAULT.TGA");
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
        if (lifetime <= 0)
        {
            delete this;
            return;
        }

        if (!target) return;

        zVEC3 worldPos = target->GetPositionWorld() + offset;
        zVEC3 screenPos = ogame->GetCamera()->GetTransform(zTCamTrafoType::zCAM_TRAFO_VIEW) * worldPos;

        int x, y;
        ogame->GetCamera()->Project(&screenPos, x, y);

        float alpha = min(255, lifetime * 8.5f);
        color.alpha = (int)alpha;

        view->SetFontColor(color);
       
        view->Print(x, y, text);
    }
}
