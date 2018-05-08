/*
    src/example1.cpp -- C++ version of an example application that shows
    how to use the various widget classes. For a Python implementation, see
    '../python/example1.py'.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <iostream>
#include <string>
#include <emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;
using namespace nanogui;

// Includes for the GLTexture class.
#include <cstdint>
#include <memory>
#include <utility>

#if defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#if defined(_WIN32)
#  pragma warning(push)
#  pragma warning(disable: 4457 4456 4005 4312)
#endif

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#if defined(_WIN32)
#  pragma warning(pop)
#endif
#if defined(_WIN32)
#  if defined(APIENTRY)
#    undef APIENTRY
#  endif
#  include <windows.h>
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::to_string;




class ExampleApplication : public nanogui::Screen {
public:
    ExampleApplication(const std::string &title) : nanogui::Screen(Eigen::Vector2i(1024, 768), "NanoGUI Test", /*resizable*/true, /*fullscreen*/false, /*colorBits*/8,
                                /*alphaBits*/8, /*depthBits*/24, /*stencilBits*/8,
                                /*nSamples*/0, /*glMajor*/3, /*glMinor*/0) {
        using namespace nanogui;
        Window *window = new Window(this, title);
        window->setPosition(Vector2i(15, 15));
        window->setLayout(new GroupLayout());
        /* No need to store a pointer, the data structure will be automatically
           freed when the parent window is deleted */
        new Label(window, "Push buttons", "sans-bold");

        Button *b = new Button(window, "Plain button");
        b->setCallback([] { cout << "pushed!" << endl; });
        b->setTooltip("short tooltip");

        /* Alternative construction notation using variadic template */
        b = window->add<Button>("Styled", ENTYPO_ICON_ROCKET);
        b->setBackgroundColor(Color(0, 0, 255, 25));
        b->setCallback([] { cout << "pushed!" << endl; });
        b->setTooltip("This button has a fairly long tooltip. It is so long, in "
                "fact, that the shown text will span several lines.");

        new Label(window, "Toggle buttons", "sans-bold");
        b = new Button(window, "Toggle me");
        b->setFlags(Button::ToggleButton);
        b->setChangeCallback([](bool state) { cout << "Toggle button state: " << state << endl; });

        new Label(window, "Radio buttons", "sans-bold");
        b = new Button(window, "Radio button 1");
        b->setFlags(Button::RadioButton);
        b = new Button(window, "Radio button 2");
        b->setFlags(Button::RadioButton);

        new Label(window, "A tool palette", "sans-bold");
        Widget *tools = new Widget(window);
        tools->setLayout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));

        b = new ToolButton(tools, ENTYPO_ICON_CLOUD);
        b = new ToolButton(tools, ENTYPO_ICON_CONTROLLER_FAST_FORWARD);
        b = new ToolButton(tools, ENTYPO_ICON_COMPASS);
        b = new ToolButton(tools, ENTYPO_ICON_INSTALL);

        

        performLayout();
    }

    ~ExampleApplication() {
    }
};

void mainloop(){
	//nanogui::mainloop();
}

nanogui::ref<ExampleApplication> app;

int main(int /* argc */, char ** /* argv */) {
    try {
        nanogui::init();
        //emscripten_set_main_loop(mainloop, 0,1);
        //nanogui::shutdown();
    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        #if defined(_WIN32)
            MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
        #else
            std::cerr << error_msg << endl;
        #endif
        return -1;
    }
    return 0;
}



extern "C" {
  EMSCRIPTEN_KEEPALIVE
  void jsLoop() {
    //std::cout << "jsLoop" << std::endl;
    nanogui::mainloop();
  }

  EMSCRIPTEN_KEEPALIVE
  void createAppp(int p) {
    char* title = (char*)p;
    app = new ExampleApplication(title);
    app->drawAll();
    app->setVisible(true);
  }

  EMSCRIPTEN_KEEPALIVE
  void jsClear() {
    //std::cout << "glClear" << std::endl;
    if (app) {
        app->clear();
    }
  }  
}

EMSCRIPTEN_BINDINGS(ExampleApplication) {
    emscripten::class_<ExampleApplication>("ExampleApplication")
        .constructor<std::string>();
}

EMSCRIPTEN_BINDINGS(Vector2i) {
    emscripten::class_<Eigen::Matrix<int,2,1>>("Vector2i")
        .constructor<int, int>();
}

EMSCRIPTEN_BINDINGS(Screen) {
    emscripten::class_<nanogui::Screen>("Screen")
        .constructor<
            Eigen::Matrix<int,2,1>,
            std::string,
            bool,
            bool,
            int,
            int,
            int,
            int,
            int,
            unsigned int,
            unsigned int,
            double
        >()
        .function("drawAll", &nanogui::Screen::drawAll)
        .function("performLayout", &nanogui::Screen::performLayout)
        .function("setVisible", &ExampleApplication::setVisible);
}

EMSCRIPTEN_BINDINGS(GroupLayout) {
    emscripten::class_<nanogui::Layout>("Layout");
    emscripten::class_<nanogui::GroupLayout, emscripten::base<nanogui::Layout>>("GroupLayout")
        .constructor<int, int, int, int>();
}

EMSCRIPTEN_BINDINGS(Window) {
    emscripten::class_<Widget>("Widget")
        .function("setPosition", &Widget::setPosition)
        .function("setTooltip", &Widget::setTooltip)
        .function("setLayout", &Widget::setLayout, allow_raw_pointers());
    emscripten::class_<Window, emscripten::base<Widget>>("Window")
        .constructor<
            nanogui::Screen*,
            std::string
        >();
}

extern "C" {
    extern void dropEvent(int event, int value1);
}

class JSButton: public nanogui::Button {
public:
    JSButton(Widget *parent, const std::string &caption = "Untitled", int icon = 0)
    : nanogui::Button(parent, caption, icon) {

    }
    void doCallback() {
        dropEvent(1, (int)this);
    }
    int _setCallback() {
        this->mCallback = std::bind(&JSButton::doCallback, this);
        return (int)this;
    }
};

EMSCRIPTEN_BINDINGS(Label) {
    emscripten::class_<Button, emscripten::base<Widget>>("__Button")
        .constructor<
            nanogui::Window*,
            std::string,
            int
        >()
        .function("setBackgroundColor", &Button::setBackgroundColor);
    emscripten::class_<JSButton, emscripten::base<Button>>("Button")
        .constructor<
            nanogui::Window*,
            std::string,
            int
        >()
        .function("setCallback", &JSButton::_setCallback);
    emscripten::class_<nanogui::Label>("Label")
        .constructor<
            nanogui::Window*,
            std::string,
            std::string,
            int
        >();
}
