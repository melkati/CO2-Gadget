/* -*- C++ -*- */
/**
 * GxEPD2Out.h — ArduinoMenu output driver for GxEPD2 e-paper displays
 *
 * GxEPD2 inherits from Adafruit_GFX so we can reuse adaGfxOut directly.
 * The key difference is that e-paper displays need an explicit display()
 * call to flush the framebuffer to the physical panel.  This driver wraps
 * adaGfxOut and calls display.display(true) (partial update when available)
 * at the end of each flush cycle to keep latency acceptable.
 *
 * Usage:
 *   #include "GxEPD2Out.h"
 *   GxEPD2Out einkOut(display, colors, tops, panelsList, fontW, fontH);
 *
 * Copyright (c) 2024 eMariete — https://emariete.com
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef CO2_GxEPD2_OUT_H
#define CO2_GxEPD2_OUT_H

#include <menuIO/adafruitGfxOut.h>

namespace Menu {

/**
 * GxEPD2Out — thin subclass of adaGfxOut.
 *
 * Overrides flush() to call display.display(true) so the e-paper panel is
 * actually refreshed.  Because adaGfxOut stores a reference to Adafruit_GFX,
 * we accept the concrete GxEPD2 display object as a template parameter so we
 * can call the GxEPD2-specific display() method without a virtual cast.
 *
 * NOTE: full refresh (display.display(false)) is slow (~2 s) on most panels.
 * We use partial update (display.display(true)) which is much faster (~0.3 s)
 * and perfectly adequate for a menu.  Call display.display(false) manually
 * (e.g. from initMenu) to do a clean initial full-refresh.
 */
template<typename GxEPD2_TYPE>
class GxEPD2Out : public adaGfxOut {
public:
    GxEPD2_TYPE &epd;

    GxEPD2Out(GxEPD2_TYPE &d,
              const colorDef<uint16_t> (&c)[nColors],
              idx_t *t,
              panelsList &p,
              idx_t resX = 6,
              idx_t resY = 9)
        : adaGfxOut(d, c, t, p, resX, resY), epd(d) {}

    /**
     * flush() is called by ArduinoMenu after it finishes rendering a frame.
     * We perform a partial update to push the Adafruit_GFX framebuffer
     * out to the physical e-ink panel.
     */
    void flush() override {
        adaGfxOut::flush();
        epd.display(true);  // true = partial update (faster)
    }
};

}  // namespace Menu

#endif  // CO2_GxEPD2_OUT_H
