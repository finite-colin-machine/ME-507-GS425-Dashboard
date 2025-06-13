/** @file    tempGaugeContainer.hpp
 *  @brief   Header for temperature gauge container
 *  @details Declares the ::tempGaugeContainer class, a runtime wrapper that
 *           initialises the temperature widgets and updates them with new
 *           engine temperature readings.
 *  @date    2025-Jun-11 NN created file
 */

#ifndef TEMPGAUGECONTAINER_HPP
#define TEMPGAUGECONTAINER_HPP

#include <gui_generated/containers/tempGaugeContainerBase.hpp>

/** @brief   Class for engine‑temperature gauge
 *  @details Inherits from the auto‑generated base and adds simple logic to
 *           convert raw °C readings to °F and push the result into the bar
 *           and text widgets.
 */
class tempGaugeContainer : public tempGaugeContainerBase
{
public:
    tempGaugeContainer();
    virtual ~tempGaugeContainer() {}

    virtual void initialize();     ///< One‑time binding of widgets
    void updateTemp(uint16_t c);   ///< Update gauge with new °C reading

protected:
    int   lastTemp; ///< Last °C value drawn – prevents redundant redraws
    float tempF;    ///< Cached °F value for progress‑bar + text display
};

#endif // TEMPGAUGECONTAINER_HPP
