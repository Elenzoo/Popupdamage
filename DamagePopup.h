#pragma once

#include "Union.h"

namespace GOTHIC_ENGINE
{
    class DamagePopup
    {
    public:
        zCView* view;
        zSTRING text;
        zCOLOR color;
        int lifetime;
        float scale;
        zCVob* target;
        zVEC3 offset;

        DamagePopup(zCVob* target, int dmg, bool isCrit, oEDamageIndex dmgIndex);
        ~DamagePopup();

        void Update();
        void Print();
    };

    extern zCArray<DamagePopup*> g_Popups;
}
