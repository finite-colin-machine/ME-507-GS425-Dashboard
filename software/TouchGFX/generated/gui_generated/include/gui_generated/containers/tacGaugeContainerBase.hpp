/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef TACGAUGECONTAINERBASE_HPP
#define TACGAUGECONTAINERBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <touchgfx/containers/progress_indicators/CircleProgress.hpp>
#include <touchgfx/widgets/canvas/PainterRGBA2222.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/TextArea.hpp>

class tacGaugeContainerBase : public touchgfx::Container
{
public:
    tacGaugeContainerBase();
    virtual ~tacGaugeContainerBase();
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::BoxProgress tacBar7;
    touchgfx::CircleProgress tacBar6;
    touchgfx::PainterRGBA2222 tacBar6Painter;
    touchgfx::BoxProgress tacBar5;
    touchgfx::CircleProgress tacBar4;
    touchgfx::PainterRGBA2222 tacBar4Painter;
    touchgfx::BoxProgress tacBar3;
    touchgfx::CircleProgress tacBar2;
    touchgfx::PainterRGBA2222 tacBar2Painter;
    touchgfx::BoxProgress tacBar1;
    touchgfx::TextAreaWithOneWildcard tacTextDyn;
    touchgfx::TextArea RPM;
    touchgfx::TextArea RPM_1;
    touchgfx::TextArea _20;
    touchgfx::TextArea _40;
    touchgfx::TextArea _60;
    touchgfx::TextArea _80;
    touchgfx::TextArea _100;
    touchgfx::TextArea _100_1;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TACTEXTDYN_SIZE = 10;
    touchgfx::Unicode::UnicodeChar tacTextDynBuffer[TACTEXTDYN_SIZE];

private:

};

#endif // TACGAUGECONTAINERBASE_HPP
