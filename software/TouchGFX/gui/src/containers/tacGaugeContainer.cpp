#include <gui/containers/tacGaugeContainer.hpp>
#include <touchgfx/Unicode.hpp>

tacGaugeContainer::tacGaugeContainer()
{

}

void tacGaugeContainer::initialize()
{
    tacGaugeContainerBase::initialize();
}

/**
 * @brief   Update the tac gauge display
 * @details Formats the provided speed into a wildcard text, updates the
 *          dynamic text widget, then fills each of the seven progress
 *          bars sequentially based on configured capacities. Bars beyond
 *          the fill range are zeroed out.
 * @param   speed  Integer speed value (e.g. 0–100) to display
 */
void tacGaugeContainer::updateTac(int tac)
{
    // divisor = 3.67  →  multiplied by 100 to keep integers
    constexpr uint16_t SCALE   = 367;   // choose any power of 10
    constexpr uint16_t DIVISOR = 120;   // 3.67 * SCALE
    constexpr uint16_t HALF    = DIVISOR / 2;   // 183  ← for rounding

    // --- inside updateSpeed() ---
    uint16_t val = (tac * SCALE + HALF) / DIVISOR;   // rounded result
    // Format and update the dynamic text
    touchgfx::Unicode::snprintf(
        tacTextDynBuffer,
        TACTEXTDYN_SIZE,
        "%d",
        tac);
    tacTextDyn.setWildcard(tacTextDynBuffer);
    tacTextDyn.invalidate();

    tacBar1.setValue(val); tacBar1.invalidate();
    tacBar2.setValue(val); tacBar2.invalidate();
    tacBar3.setValue(val); tacBar3.invalidate();
    tacBar4.setValue(val); tacBar4.invalidate();
    tacBar5.setValue(val); tacBar5.invalidate();
    tacBar6.setValue(val); tacBar6.invalidate();
    tacBar7.setValue(val); tacBar7.invalidate();
}
