/** @file    screenView.hpp
 *  @brief   Header for animating a image on a TFT screen
 *  @details This file contains functions to initialize the screen, as well as functions
 *  		 that dictate widgets on the screen. handleTickEvent is called at 60hz.
 *  @date    2025-Jun-06 NN created file
 */

#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP

#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>
#include <touchgfx/Unicode.hpp>

#include "task2.h"
#include "task1.h"

/** @brief   Class for TFT screen
 *  @details This is a class for the TFT screen. Functions are created to
 *  		 initialize the screen, update periodically, and update widgets.
 */
class screenView : public screenViewBase
{
public:
    screenView();
    virtual ~screenView() {}
    virtual void setupScreen(); 	///< Init and displays screen
    virtual void tearDownScreen();
    virtual void handleTickEvent(); ///< called at 60Hz,

    //custom functions
    void updateOdometer(uint32_t distance);	   ///< updates odometer reading
    void startUpSequence(uint32_t tickCounter);///< cycle all displays for testing
    void updateGear(GPIO_PinState gear[7]);    ///< update gear to be displayed
    void cycleGear(uint32_t tickCounter);	   ///< cycle through all gears
    void updateVolts(uint32_t tickCounter);	   ///< updated voltage display
    void updateGas(uint32_t tickCounter);	   ///< update gastank level

protected:
    uint32_t tickCounter; ///< value to keep track on number of ticks that have passed each tick is 1ms

    uint32_t currentGear; ///< current gear that is selected

    int lastGear;		  ///< last gear read, used to see if a redraw is needed
    int lastRPM;		  ///< last RPM read, used to see if a redraw is needed
    int lastSpeed;		  ///< last speed read, used to see if a redraw is needed
    uint32_t lastVolt;	  ///< last voltage read, used to see if a redraw is needed
    uint32_t lastGas;	  ///< last gas level read, used to see if a redraw is needed
    uint32_t lastOdo;	  ///< last speed read, used to see if a redraw is needed
    uint32_t lastGearCyc; ///< last gear in the cycle function, used to see if a redraw is needed
    int lastTemp;		  ///< last temp read, used to see if a redraw is needed
    bool lastAutoChoke;   ///< last value of the bool if if the autochoke is on, used to see if a redraw is needed
};

#endif // SCREENVIEW_HPP
