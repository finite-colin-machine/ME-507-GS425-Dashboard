
/** @file    speedGaugeContainer.cpp
 *  @brief   Container for a multi-bar speed gauge with dynamic text
 *  @details Provides a custom container that links seven progress bars
 *           together and updates them in sequence based on a single speed
 *           value. A central text widget is updated via a wildcard to
 *           display the numeric speed. Remaining bars are zeroed when the
 *           speed value is exhausted.
 *  @date    2025-Jun-10
 */

 #include <gui/containers/speedGaugeContainer.hpp>
#include <touchgfx/Unicode.hpp>

/**
 * @brief   Default constructor for the SpeedGauge container
 */
speedGaugeContainer::speedGaugeContainer()
{
}

/**
 * @brief   Initialize the SpeedGauge container
 * @details Calls the generated base-class initialize to bind all widgets
 *          (progress bars, text) before use.
 */
void speedGaugeContainer::initialize()
{
    speedGaugeContainerBase::initialize();
}



/**
 * @brief   Update the speed gauge display
 * @details Formats the provided speed into a wildcard text, updates the
 *          dynamic text widget, then fills each of the seven progress
 *          bars sequentially based on configured capacities. Bars beyond
 *          the fill range are zeroed out.
 * @param   speed  Integer speed value (e.g. 0–120) to display
 */
void speedGaugeContainer::updateSpeed(uint16_t speed)
{
    // used to scale the integer of speed with the progress bars smoothly
    constexpr uint16_t SCALE   = 100;
    constexpr uint16_t DIVISOR = 367;
    constexpr uint16_t HALF    = DIVISOR / 2;

    uint16_t val = (speed * SCALE + HALF) / DIVISOR;   // rounded result
        // Format and update the dynamic text
    touchgfx::Unicode::snprintf(
        speedTextDynBuffer,
        SPEEDTEXTDYN_SIZE,
        "%d",
        val
    );
    speedTextDyn.setWildcard(speedTextDynBuffer);
    speedTextDyn.invalidate();

    // Define segment capacities of each progress bar
    const int capacities[7] = {20, 14, 20, 30, 100, 14, 170};

    int remaining = speed;
    for (int i = 0; i < 7; i++) {
        int segVal = (remaining >= capacities[i]) ? capacities[i] : remaining;
        switch (i) {
            case 0: speedBar1.setValue(segVal); speedBar1.invalidate(); break;
            case 1: speedBar2.setValue(segVal); speedBar2.invalidate(); break;
            case 2: speedBar3.setValue(segVal); speedBar3.invalidate(); break;
            case 3: speedBar4.setValue(segVal); speedBar4.invalidate(); break;
            case 4: speedBar5.setValue(segVal); speedBar5.invalidate(); break;
            case 5: speedBar6.setValue(segVal); speedBar6.invalidate(); break;
            case 6: speedBar7.setValue(segVal); speedBar7.invalidate(); break;
        }
        remaining -= segVal;
        if (remaining <= 0) {
            // Zero out any remaining bars
            for (int j = i + 1; j < 7; j++) {
                switch (j) {
                    case 0: speedBar1.setValue(0); speedBar1.invalidate(); break;
                    case 1: speedBar2.setValue(0); speedBar2.invalidate(); break;
                    case 2: speedBar3.setValue(0); speedBar3.invalidate(); break;
                    case 3: speedBar4.setValue(0); speedBar4.invalidate(); break;
                    case 4: speedBar5.setValue(0); speedBar5.invalidate(); break;
                    case 5: speedBar6.setValue(0); speedBar6.invalidate(); break;
                    case 6: speedBar7.setValue(0); speedBar7.invalidate(); break;
                }
            }
            break;
        }
    }
}
