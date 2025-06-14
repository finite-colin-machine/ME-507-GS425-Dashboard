/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREENVIEWBASE_HPP
#define SCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen_screen/screenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <gui/containers/speedGaugeContainer.hpp>
#include <gui/containers/tempGaugeContainer.hpp>
#include <gui/containers/tacGaugeContainer.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class screenViewBase : public touchgfx::View<screenPresenter>
{
public:
    screenViewBase();
    virtual ~screenViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::TextAreaWithOneWildcard odoText;
    touchgfx::TextAreaWithOneWildcard gearText;
    touchgfx::BoxProgress gasProgress;
    touchgfx::BoxProgress voltProgress;
    touchgfx::TextAreaWithOneWildcard gasText;
    touchgfx::TextAreaWithOneWildcard voltText;
    speedGaugeContainer speedGaugeContainer1;
    tempGaugeContainer tempGaugeContainer1;
    tacGaugeContainer tacGaugeContainer1;
    touchgfx::TextArea autoChoke;

    /*
     * Wildcard Buffers
     */
    static const uint16_t ODOTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar odoTextBuffer[ODOTEXT_SIZE];
    static const uint16_t GEARTEXT_SIZE = 4;
    touchgfx::Unicode::UnicodeChar gearTextBuffer[GEARTEXT_SIZE];
    static const uint16_t GASTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar gasTextBuffer[GASTEXT_SIZE];
    static const uint16_t VOLTTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar voltTextBuffer[VOLTTEXT_SIZE];

private:

    /*
     * Canvas Buffer Size
     */
    static const uint32_t CANVAS_BUFFER_SIZE = 9000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

};

#endif // SCREENVIEWBASE_HPP
