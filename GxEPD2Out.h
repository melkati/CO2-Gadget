/********************
May. 2024 Sergio Coscolín https://github.com/Coscolin
with special help from ChatGPT 4o

Based on U8GLibOut.h from Rui Azevedo - ruihfazevedo(@rrob@)gmail.com
and TFT_eSPIOut.H from  https://github.com/fa1ke5 (Jan. 2019)
Original from: https://github.com/christophepersoz

Adaptaded for GxEPD2 displays use
***/
#ifndef RSITE_ARDUINOP_MENU_GXEPD2
#define RSITE_ARDUINOP_MENU_GXEPD2
#include <GxEPD2.h>

#include "menuDefs.h"

namespace Menu {

template <typename GxEPD2_Type>
class GxEPD2Out : public gfxOut {
   public:
    int8_t offsetX = 0;
    int8_t offsetY = 0;
    GxEPD2_Type& gfx;
    const colorDef<uint8_t> (&colors)[nColors];

    GxEPD2Out(
        GxEPD2_Type& gfx,
        const colorDef<uint8_t> (&c)[nColors],
        idx_t* t,
        panelsList& p,
        idx_t resX = 6,
        idx_t resY = 9,
        idx_t offsetX = 0,
        idx_t offsetY = 0,
        int fontMarginY = 1,
        int fontMarginX = 1) : gfxOut(resX, resY, t, p, (styles)(menuOut::redraw | menuOut::rasterDraw)), gfx(gfx), colors(c) {
        gfx.setTextWrap(false);
        this->offsetX = offsetX;
        this->offsetY = offsetY;
        this->fontMarginY = fontMarginY;
        this->fontMarginX = fontMarginX;
    }

    size_t write(uint8_t ch) override { return gfx.write(ch); }

    inline uint8_t getColor(colorDefs color = bgColor, bool selected = false, status stat = enabledStatus, bool edit = false) const {
        return memByte(&(stat == enabledStatus ? colors[color].enabled[selected + edit] : colors[color].disabled[selected]));
    }

    void setColor(colorDefs c, bool selected = false, status s = enabledStatus, bool edit = false) override {
        if (c == titleColor) {
            gfx.setTextColor(getColor(titleColor, selected, s, edit), getColor(titleColor, false, s, edit));
        } else {
            gfx.setTextColor(getColor(c, selected, s, edit), getColor(bgColor, selected, s, edit));
        }
    }

    void clearLine(idx_t ln, idx_t panelNr = 0, colorDefs color = bgColor, bool selected = false, status stat = enabledStatus, bool edit = false) override {
        const panel p = panels[panelNr];
        setColor(color, selected, stat, edit);
        gfx.fillRect(p.x * resX + offsetX, (p.y + ln) * resY + offsetY, maxX() * resX, resY, getColor(color, selected, stat, edit));
    }

    void clear() override {
        setCursor(0, 0);
        display.fillScreen(getColor(bgColor, false, enabledStatus, false));
        setColor(fgColor);
        panels.reset();
    }

    void clear(idx_t panelNr) override {
        const panel p = panels[panelNr];
        gfx.fillRect(p.x * resX + offsetX, p.y * resY + offsetY, p.w * resX, p.h * resY, getColor(bgColor, false, enabledStatus, false));
        panels.nodes[panelNr] = NULL;
    }

    void box(idx_t panelNr, idx_t x, idx_t y, idx_t w = 1, idx_t h = 1, colorDefs c = bgColor, bool selected = false, status stat = enabledStatus, bool edit = false) override {
        const panel p = panels[panelNr];
        gfx.drawRect((p.x + x) * resX, (p.y + y) * resY, w * resX, h * resY, getColor(c, selected, stat, edit));
    }

    void rect(idx_t panelNr, idx_t x, idx_t y, idx_t w = 1, idx_t h = 1, colorDefs c = bgColor, bool selected = false, status stat = enabledStatus, bool edit = false) override {
        const panel p = panels[panelNr];
        gfx.fillRect((p.x + x) * resX, (p.y + y) * resY, w * resX, h * resY, getColor(c, selected, stat, edit));
    }

    void setCursor(idx_t x, idx_t y, idx_t panelNr = 0) override {
        const panel p = panels[panelNr];
        gfx.setCursor((p.x + x) * resX + fontMarginX + offsetX, (p.y + y + 1) * resY - fontMarginY + offsetY);
    }

    idx_t startCursor(navRoot& root, idx_t x, idx_t y, bool charEdit, idx_t panelNr) override {
        if (charEdit) {
            const panel p = panels[panelNr];
            gfx.fillRect((p.x + x) * resX + offsetX + fontMarginX, (p.y + y) * resY + offsetY, resX, resY, getColor(fgColor, false, enabledStatus, false));
        }
        return 0;
    }

    idx_t editCursor(navRoot& root, idx_t x, idx_t y, bool editing, bool charEdit, idx_t panelNr = 0) override {
        if (editing) {
            const panel p = panels[panelNr];
            _trace(Serial << "editCursor" << endl);
            gfx.drawRect((x + p.x - 1) * resX + offsetX + fontMarginX - 1, (p.y + y) * resY + offsetY, resX + 1, resY, cursorColor);
        }
        return 0;
    }

    void drawCursor(idx_t ln, bool selected, status stat, bool edit = false, idx_t panelNr = 0) override {
        const panel p = panels[panelNr];
        gfx.drawRect(p.x * resX, (p.y + ln) * resY, maxX() * resX, resY, getColor(cursorColor, selected, enabledStatus, false));
    }

    idx_t printRaw(const char* at, idx_t len) override {
        trace(Serial << "GxEPD2Out::printRaw" << endl);
        trace(Serial << "[" << at << "]");
        return print((__FlashStringHelper*)at);
    }
};
}  // namespace Menu
#endif  // RSITE_ARDUINOP_MENU_GxEPD2
