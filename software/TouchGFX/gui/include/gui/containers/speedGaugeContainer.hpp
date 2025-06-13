/** @file    speedGaugeContainer.hpp
 *  @brief   Container wrapper for a multi‑segment speed gauge
 *  @details Declares the ::speedGaugeContainer class – a lightweight
 *           C++ wrapper that adds application-specific logic on top of the
 *           designer‑generated ::speedGaugeContainerBase.  The container
 *           exposes two public methods:
 *             • initialize()   – called once to bind all widgets
 *             • updateSpeed()  – called whenever the measured speed value
 *                                changes; splits the numeric value into
 *                                bar segments and updates the dynamic text.
 *
 *           The actual drawing primitives live inside the generated base
 *           class; this header only provides the API used by the presenter
 *           / view code.
 *
 *  @date    2025 Jun‑10  NN  created file
 */

#ifndef SPEEDGAUGECONTAINER_HPP
#define SPEEDGAUGECONTAINER_HPP

#include <gui_generated/containers/speedGaugeContainerBase.hpp>

/** @brief   Custom TouchGFX container that visualises vehicle speed
 *  @details The class inherits from the generated
 *           ::speedGaugeContainerBase, which contains:
 *             – 7 horizontally aligned ::ProgressBar widgets (bars)
 *             – 1 ::TextAreaWithOneWildcard (numeric read‑out)
 *
 *           This wrapper adds plain C++ logic only – no textures or
 *           designer resources.  The typical call sequence is:
 *           @code
 *               speedGaugeContainer1.initialize();
 *               …
 *               speedGaugeContainer1.updateSpeed(currentSpeed);
 *           @endcode
 */
class speedGaugeContainer : public speedGaugeContainerBase
{
public:
    speedGaugeContainer();
    virtual ~speedGaugeContainer() {}

    /** @brief Bind generated widgets before first use              */
    virtual void initialize() override;

    /** @brief Update bars + numeric text for a new speed value
     *  @param speed  Raw speed measurement (ticks / s, km/h, etc.).
     *                The function internally scales and rounds this value
     *                so that each bar receives a number within its own
     *                @c setRange() interval,
     *                then updates the wildcard text to match.
     */
    void updateSpeed(uint16_t speed);

protected:
    float val;  ///< Temporary scaled value used when computing bar fill
};

#endif // SPEEDGAUGECONTAINER_HPP
