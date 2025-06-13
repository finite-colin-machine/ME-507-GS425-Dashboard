#include <gui/containers/tempGaugeContainer.hpp>
#include <touchgfx/Unicode.hpp>

tempGaugeContainer::tempGaugeContainer()
{

}

void tempGaugeContainer::initialize()
{
    tempGaugeContainerBase::initialize();
}

/** @brief   Update temperature display function
 *  @details This is a function used to refresh measured temperature values
 *  		 and display them on the screen. Updates both the progress bar and the
 *  		 numerical reading
 *  @param   temp integer value of measure engine temperature
 */
void tempGaugeContainer::updateTemp(uint16_t temp)
{
	// check if the temperature has changed since last reading.
	// only update if there has been a change in temperature
	if (temp != lastTemp)
	{
    	// °F = °C × 9/5 + 32 with rounding
    	uint16_t tempF = static_cast<uint16_t>((temp * 9 + 2) / 5 + 32);


		//Update temperature progress bar
		tempProgress.setValue(tempF);		//Update the temperature progress bar
		tempProgress.invalidate();		//Redraw only parts of the screen that need to be updated

		//Update temperature text
		touchgfx::Unicode::snprintf(tempTextBuffer,TEMPTEXT_SIZE,"%u", tempF);
		tempText.setWildcard(tempTextBuffer); //Update the temperature text
		tempText.resizeToCurrentText();		  //Resize drawn area to fit all characters
		tempText.invalidate();				  //Redraw only parts of the screen that need to be updated

		lastTemp = temp; //reset last temp
	}
}
