// =========================
// DamagePopup.h
// =========================
#ifndef __DAMAGE_POPUP_H__
#define __DAMAGE_POPUP_H__

namespace GOTHIC_ENGINE {
    class DamagePopup {
    public:
        int lifetime;
        zVEC3 offset;
        zSTRING text;
        zCOLOR color;
        float scale;
        zCView* view;

        DamagePopup(zCVob* target, int dmg, bool isCrit, oEDamageIndex dmgIndex);
        ~DamagePopup();

        void Update();

    private:
        zCVob* target;
    };

    extern zCArray<DamagePopup*> g_Popups;
}

#endif // __DAMAGE_POPUP_H__
