/** @file    tacGaugeContainer.hpp
 *  @brief   Header for tachometer gauge container
 *  @details Provides functions to initialize and update the tachometer gauge,
 *           which includes progress bars and a numeric RPM display.
 *  @date    2025-Jun-11 NN created file
 */

#ifndef TACGAUGECONTAINER_HPP
#define TACGAUGECONTAINER_HPP

#include <gui_generated/containers/tacGaugeContainerBase.hpp>

/** @brief   Class for tachometer gauge
 *  @details Wrapper around the designer‑generated base class that adds
 *           runtime logic to initialize the gauge and push new RPM data
 *           to both the bars and the text read‑out.
 */
class tacGaugeContainer : public tacGaugeContainerBase
{
public:
    tacGaugeContainer();
    virtual ~tacGaugeContainer() {}

    virtual void initialize();  ///< One‑time widget binding
    void updateTac(int rpm);    ///< Refresh gauge with latest RPM value

protected:
    // No extra state needed
};

#endif // TACGAUGECONTAINER_HPP
