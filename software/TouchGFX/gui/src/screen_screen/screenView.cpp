/** @file    screenView.cpp
 *  @brief   Source file for animating a image on a TFT screen
 *  @details This file contains functions to initialize the screen, as well as functions
 *  		 that dictate widgets on the screen. handleTickEvent is called at 60hz.
 *  @date    2025-Jun-06 NN created file
 */

#include <gui/screen_screen/screenView.hpp>
#include "task2.h"
#include "task1.h"

extern TASK2 task2; ///< link to global instance defined in task2.cpp
extern TASK1 task1; ///< link to global instance defined in task2.cpp

/**
 * @brief Init screenView
 */
screenView::screenView()
{
}

/**
 * @brief setupScreen function
 * @details  Runs once upon start up. displays the start up screen
 * 			 and initializes variables
 */
void screenView::setupScreen()
{
    screenViewBase::setupScreen(); //set up initial screen

    tickCounter = 0;	//reset tick counter

    //used to check if screen update is needed
    lastGear = 9;	// Impossible current gear to initialize
    lastRPM = 999;	// last read RPM value
    lastSpeed = 999;// last speed value
    lastGas = 0;	// last fuel level measured
    lastVolt = 0;	// last battery voltage measured
    lastTemp = 0;	// last engine temperature measured
}

/**
 * @brief tearDownScreen function
 */
void screenView::tearDownScreen()
{
    screenViewBase::tearDownScreen();
}

/**
 * @brief 	 Runs at every screen tick.
 * @details  Runs at screen refresh rate, ~60Hz.
 * 			 Used to periodically call functions that update individual widgets
 */
void screenView::handleTickEvent()
{
    tickCounter++;		  //increase tick counter
    if (tickCounter < 37) // go through start up sequence for the first 37 ticks
    {
        startUpSequence(tickCounter*10);
    }
    // After start up, check if each widget need to be refreshed, and redrawn them if needed
    else
    {
    	//update the custom temperature container
        if (task2.temp_meas_share != lastTemp){
            tempGaugeContainer1.updateTemp(task2.temp_meas_share);
            lastTemp = task2.temp_meas_share;
        }
        //update the custom wheel speed container
        if (task2.wheel_speed_share != lastSpeed){
            speedGaugeContainer1.updateSpeed(task2.wheel_speed_share);
            lastSpeed = task2.wheel_speed_share;
        }
        //update the custom engine RPM container
        if (task2.engine_speed_share != lastRPM){
            tacGaugeContainer1.updateTac(task2.engine_speed_share);
            lastRPM = task2.engine_speed_share;
        }
        updateGear(task2.gear_share);		//update gear display
        updateOdometer(tickCounter/60);		//update odometer displays
        updateVolts(13.3);                  //update measured battery voltage
        updateGas(1000 - (tickCounter/60)); //update gas level

        // Change the choke icon only when the flag toggles
        bool lastAutoChoke = task1.autoChokeEnable_share;

        if (autoChoke.isVisible() != lastAutoChoke)   // update only on change
        {
            autoChoke.setVisible(lastAutoChoke);
            autoChoke.invalidate();                   // redraw text
        }

    }

}

/** @brief   Update odometer display function
 *  @details This is a function used to refresh measured odometer reading
 *  		 and display them on the screen.
 *  @param   distance integer value of total mileage of the bike
 */
void screenView::updateOdometer(uint32_t odo)
{
	//check if a redrawn is needed
    if (odo == lastOdo){
        return;
    }

    touchgfx::Unicode::snprintf(odoTextBuffer,ODOTEXT_SIZE,"%u", odo);
    odoText.setWildcard(odoTextBuffer);	//Update the odometer text
    odoText.resizeToCurrentText();		//Resize drawn area to fit all characters
    odoText.invalidate();				//Redraw only parts of the screen that need to be updated

    lastOdo = odo;
}

/** @brief   Update gear selector display function
 *  @details This is a function used to refresh current gear of the bike
 *  		 and display them on the screen.
 *  @param   gear[7] array of gpio values of which gear is active
 */
void screenView::updateGear(GPIO_PinState gear[7])
{
    //determine which gear is active:
    int newGear;
    if      (gear[0] == GPIO_PIN_RESET) newGear = 0;  // N
    else if (gear[1] == GPIO_PIN_RESET) newGear = 1;
    else if (gear[2] == GPIO_PIN_RESET) newGear = 2;
    else if (gear[3] == GPIO_PIN_RESET) newGear = 3;
    else if (gear[4] == GPIO_PIN_RESET) newGear = 4;
    else if (gear[5] == GPIO_PIN_RESET) newGear = 5;
    else if (gear[6] == GPIO_PIN_RESET) newGear = 6;
    else                              newGear = 9;  // none

    //If the gear hasn’t changed, do nothing:
    if (newGear == lastGear) {
        return;
    }
    lastGear = newGear; //refresh the last measured gear

    // Pick the currenct one‐character label
    const char* txt;
    switch (newGear) {
        case  0: txt = "N"; break;
        case  1: txt = "1"; break;
        case  2: txt = "2"; break;
        case  3: txt = "3"; break;
        case  4: txt = "4"; break;
        case  5: txt = "5"; break;
        case  6: txt = "6"; break;
        default: txt = "0"; break;  // show “0” when no switch is set
    }

    // Format into the wildcard buffer and redraw:
    touchgfx::Unicode::snprintf(gearTextBuffer, GEARTEXT_SIZE,"%s",txt);
    gearText.setWildcard(gearTextBuffer);
    gearText.resizeToCurrentText();
    gearText.invalidate();
}

/** @brief   Cycle through each gear for start up
 *  @details This is a function used to cycle through all gears
 *  		 and display them on the screen.
 *  @param   gear integer of which gear it to be displayed
 */
void screenView::cycleGear(uint32_t gear)
{
	// check if a redraw is needed
    if (gear == lastGearCyc){
        return;
    }
    // Format into the wildcard buffer and redraw:
    touchgfx::Unicode::snprintf(gearTextBuffer, GEARTEXT_SIZE,"%u",gear);
    gearText.setWildcard(gearTextBuffer);
    gearText.resizeToCurrentText();
    gearText.invalidate();

    lastGearCyc = gear;
}

/** @brief   Update battery voltage and display function
 *  @details This is a function used to refresh the volatge of the bike
 *  		 and display them on the screen.
 *  @param   volt integer of battery level of the bike
 */
void screenView::updateVolts(uint32_t volt)
{
	// check if a redraw is needed
    if (volt == lastVolt){
        return;
    }
    // Format into the wildcard buffer and redraw:
    touchgfx::Unicode::snprintf(voltTextBuffer, VOLTTEXT_SIZE,"%u",volt);
    voltText.setWildcard(voltTextBuffer);
    voltText.resizeToCurrentText();
    voltText.invalidate();

    voltProgress.setValue(volt);

    lastVolt = volt;
}

/** @brief   Update gas left in the tank and display function
 *  @details This is a function used to refresh the gas level of the bike
 *  		 and display them on the screen.
 *  @param   gas integer of gas level of the bike
 */
void screenView::updateGas(uint32_t gas)
{
	// check if a redraw is needed
    if (gas == lastGas){
        return;
    }
    // Format into the wildcard buffer and redraw:
    touchgfx::Unicode::snprintf(gasTextBuffer, GASTEXT_SIZE,"%u",gas);
    gasText.setWildcard(gasTextBuffer);
    gasText.resizeToCurrentText();
    gasText.invalidate();

    gasProgress.setValue(gas);

    lastGas = gas;
}

/** @brief   function to be called on start up to cycle all gauges
 *  @details This is a function used to test the display ranges of all
 *  		 Widgets. as the tick counter is passed in and increased through
 *  		 the tickEventHandler, all gauges progress.
 *  @param   tickCounter uint32_t that increases each time the tickEventHandler is called
 */
void screenView::startUpSequence(uint32_t tickCounter)
{
    // used to round tickcounter with no float decimal multiplication
    constexpr uint16_t SCALE   = 120;
    constexpr uint16_t DIVISOR = 367;
    constexpr uint16_t HALF    = DIVISOR / 2;

    // scaled tickcounter to get a clean tacGauge startup sequence
    uint16_t val = (tickCounter * SCALE + HALF) / DIVISOR;   // rounded result
	tacGaugeContainer1.updateTac(val);	    //update engine RPM displays

    speedGaugeContainer1.updateSpeed(tickCounter);	//update temperature displays
    tempGaugeContainer1.updateTemp(tickCounter);      //update speed displays
	cycleGear(tickCounter/61);		            //update gear display
	updateOdometer(tickCounter);		    //update odometer displays
    updateVolts(tickCounter/31);               //update battery voltage
    updateGas(tickCounter*3);                 //update gas left in tank
}

