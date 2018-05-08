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




        new Label(window, "Popup buttons", "sans-bold");
        PopupButton *popupBtn = new PopupButton(window, "Popup", ENTYPO_ICON_EXPORT);
        Popup *popup = popupBtn->popup();
        popup->setLayout(new GroupLayout());
        new Label(popup, "Arbitrary widgets can be placed here");
        new CheckBox(popup, "A check box");
        // popup right
        popupBtn = new PopupButton(popup, "Recursive popup", ENTYPO_ICON_FLASH);
        Popup *popupRight = popupBtn->popup();
        popupRight->setLayout(new GroupLayout());
        new CheckBox(popupRight, "Another check box");
        // popup left
        popupBtn = new PopupButton(popup, "Recursive popup", ENTYPO_ICON_FLASH);
        popupBtn->setSide(Popup::Side::Left);
        Popup *popupLeft = popupBtn->popup();
        popupLeft->setLayout(new GroupLayout());
        new CheckBox(popupLeft, "Another check box");



        window = new Window(this, "Basic widgets");
        window->setPosition(Vector2i(200, 15));
        window->setLayout(new GroupLayout());

        new Label(window, "Message dialog", "sans-bold");
        tools = new Widget(window);
        tools->setLayout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));
        b = new Button(tools, "Info");

        b->setCallback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Information, "Title", "This is an information message");
            dlg->setCallback([](int result) { cout << "Dialog result: " << result << endl; });
        });
        b = new Button(tools, "Warn");
        b->setCallback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Warning, "Title", "This is a warning message");
            dlg->setCallback([](int result) { cout << "Dialog result: " << result << endl; });
        });
        b = new Button(tools, "Ask");
        b->setCallback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Warning, "Title", "This is a question message", "Yes", "No", true);
            dlg->setCallback([](int result) { cout << "Dialog result: " << result << endl; });
        });




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

  extern void dropEvent(int from, int event, int value1, int value2=0, int value3=0);
}


class JSButton: public nanogui::Button {
public:
    JSButton(Widget *parent, const std::string &caption = "Untitled", int icon = 0)
    : nanogui::Button(parent, caption, icon) {

    }

    int setCallbackJS() {
        //
        setCallback([this](){
            dropEvent(1, 1, (int)this);
        });
        //
        setChangeCallback([this](bool state) {
            dropEvent(1, 2, (int)this, state);
        });
        //
        return (int)this;
    }
};

class JSPopupButton: public PopupButton {
public:
    JSPopupButton(Widget *parent, const std::string &caption = "Untitled", int icon = 0)
    : nanogui::PopupButton(parent, caption, icon) {

    }

    int setCallbackJS() {
        setCallback([this](){
            dropEvent(2, 1, (int)this);
        });
        //
        setChangeCallback([this](bool state) {
            dropEvent(2, 2, (int)this, state);
        });
        //
        return (int)this;
    }

    void setSideJS(int side) {
        if (side == 0) {
           setSide(Popup::Left);
        } else {
           setSide(Popup::Right);
        }
    }

    Popup* getPopup() {return popup();}
};

Orientation intToOrientation(int o) {
    if (o == 1) {
        return Orientation::Vertical;
    } else {
        return Orientation::Horizontal;
    }
}
Alignment intToAlignment(int a) {
    switch (a) {
        //case 0: return Alignment::Minimum;
        case 1: return Alignment::Middle;
        case 2: return Alignment::Maximum;
        case 3: return Alignment::Fill;
        default: return Alignment::Minimum;
    }
}

class JSBoxLayout: public BoxLayout {
public:
    JSBoxLayout(int orientation, int alignment, int margin, int spacing)
    : BoxLayout(intToOrientation(orientation), intToAlignment(alignment), margin, spacing) {
    }
};




class JSCheckBox: public CheckBox {
public:
    JSCheckBox(Widget *parent, const std::string &caption = "Untitled")
    : CheckBox(parent, caption) {

    }

    int setCallbackJS() {
        //
        setCallback([this](bool state) {
            dropEvent(3, 2, (int)this, state);
        });
        //
        return (int)this;
    }
};


class JSComboBox: public ComboBox {
public:
    JSComboBox(Widget *parent, std::vector<std::string> items)
    : ComboBox(parent, items) {

    }

    int setCallbackJS() {
        //
        setCallback([this](int index) {
            dropEvent(5, 2, (int)this, index);
        });
        //
        return (int)this;
    }
};

class JSSlider: public Slider {
public:
    JSSlider(Widget *parent, float min, float max)
    : Slider(parent) {
        std::pair<float, float> range;
        range.first = min;
        range.second = max;
        setRange(range);
    }

    int setCallbackJS() {
        //
        setCallback([this](float value) {
            dropEvent(5, 2, (int)this, value);
        });
        //
        return (int)this;
    }
};



class JSMessageDialog: public MessageDialog {
public:
    JSMessageDialog(Widget *parent, int type, std::string title,
        std::string message,
        std::string buttonText,
        std::string altButtonText,
        bool altButton)
    : MessageDialog(parent, (Type)type, title, message, buttonText, altButtonText, altButton) {

    }

    int setCallbackJS() {
        //
        setCallback([this](int state) {
            dropEvent(4, 1, (int)this, state);
        });
        //
        return (int)this;
    }
};




EMSCRIPTEN_BINDINGS(ExampleApplication) {
    emscripten::class_<ExampleApplication>("ExampleApplication")
        .constructor<std::string>();
}

EMSCRIPTEN_BINDINGS(Vector2i) {
    emscripten::class_<Eigen::Matrix<int,2,1>>("Vector2i")
        .constructor<int, int>();
    emscripten::class_<Color>("Color")
         .constructor<int, int, int, int>();
}

EMSCRIPTEN_BINDINGS(Screen) {
    emscripten::class_<nanogui::Screen, base<Widget>>("Screen")
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
    emscripten::class_<Layout>("Layout");
    emscripten::class_<GroupLayout, base<Layout>>("GroupLayout")
        .constructor<int, int, int, int>();
    emscripten::class_<BoxLayout, base<Layout>>("__BoxLayout")
        .constructor<Orientation, Alignment, int, int>();
    emscripten::class_<JSBoxLayout, base<BoxLayout>>("BoxLayout")
        .constructor<int, int, int, int>();
}

EMSCRIPTEN_BINDINGS(Window) {
    emscripten::class_<Widget>("Widget")
        .constructor<Widget*>()
        .function("setPosition", &Widget::setPosition)
        .function("setTooltip", &Widget::setTooltip)
        .function("setFixedSize", &Widget::setFixedSize)
        .function("setFixedWidth", &Widget::setFixedWidth)
        .function("setFixedHeight", &Widget::setFixedHeight)
        .function("setLayout", &Widget::setLayout, allow_raw_pointers());
    emscripten::class_<Window, emscripten::base<Widget>>("Window")
        .constructor<
            nanogui::Screen*,
            std::string
        >();
}

EMSCRIPTEN_BINDINGS(Button) {
    emscripten::class_<Button, base<Widget>>("__Button")
        .constructor<
            nanogui::Widget*,
            std::string,
            int
        >()
        .function("setFlags", &Button::setFlags)
        .function("setBackgroundColor", &Button::setBackgroundColor);
    emscripten::class_<JSButton, base<Button>>("Button")
        .constructor<
            nanogui::Widget*,
            std::string,
            int
        >()
        .function("setCallbacks", &JSButton::setCallbackJS);
    emscripten::class_<PopupButton, base<Button>>("__PopupButton")
        .constructor<
            nanogui::Widget*,
            std::string,
            int
        >();
    emscripten::class_<Popup, base<Window>>("Popup")
        .constructor<
            nanogui::Widget*,
            nanogui::Window*
        >();
    emscripten::class_<JSPopupButton, base<Button>>("PopupButton")
        .constructor<
            nanogui::Widget*,
            std::string,
            int
        >()
        .function("setSide", &JSPopupButton::setSideJS)
        .function("popup", &JSPopupButton::getPopup, allow_raw_pointers());
}

EMSCRIPTEN_BINDINGS(CheckBox) {
    emscripten::class_<CheckBox, base<Widget>>("__CheckBox")
        .function("setChecked", &CheckBox::setChecked);
    emscripten::class_<JSCheckBox, base<CheckBox>>("CheckBox")
        .constructor<
            nanogui::Widget*,
            std::string
        >()
        .function("setCallbacks", &JSCheckBox::setCallbackJS);
}

EMSCRIPTEN_BINDINGS(ComboBox) {
    emscripten::class_<ComboBox, base<Widget>>("__ComboBox")
        .function("selectedIndex", &ComboBox::selectedIndex)
        .function("setSelectedIndex", &ComboBox::setSelectedIndex);
//        .function("setItems", &ComboBox::setItems);
    emscripten::class_<JSComboBox, base<ComboBox>>("ComboBox")
        .constructor<
            nanogui::Widget*,
            std::vector<std::string>
        >()
        .function("setCallbacks", &JSComboBox::setCallbackJS);
}


EMSCRIPTEN_BINDINGS(MessageDialog) {
    emscripten::class_<MessageDialog, base<Window>>("__MessageDialog");
    emscripten::class_<JSMessageDialog, base<MessageDialog>>("MessageDialog")
        .constructor<
            Widget*,
            int,
            std::string,
            std::string,
            std::string,
            std::string,
            bool
        >()
        .function("setCallbacks", &JSMessageDialog::setCallbackJS);
}


EMSCRIPTEN_BINDINGS(Label) {
    emscripten::class_<nanogui::Label, base<Widget>>("Label")
        .constructor<
            nanogui::Widget*,
            std::string,
            std::string,
            int
        >();
}

EMSCRIPTEN_BINDINGS(ProgressBar) {
    emscripten::class_<ProgressBar, base<Widget>>("ProgressBar")
        .constructor<
            nanogui::Widget*
        >()
        .function("value", &ProgressBar::value)
        .function("setValue", &ProgressBar::setValue);
}

EMSCRIPTEN_BINDINGS(Slider) {
    emscripten::class_<Slider, base<Widget>>("__Slider")
        .constructor<
            nanogui::Widget*
        >()
        .function("value", &Slider::value)
        .function("setValue", &Slider::setValue)
        .function("highlightColor", &Slider::highlightColor)
        .function("setHighlightColor", &Slider::setHighlightColor)
        .function("setHighlightedRange", &Slider::setHighlightedRange);
    emscripten::class_<JSSlider, base<Slider>>("Slider")
        .constructor<
            nanogui::Widget*,
            float,
            float
        >()
        .function("setCallbacks", &JSSlider::setCallbackJS);
}

EMSCRIPTEN_BINDINGS(TextBox) {
    emscripten::class_<TextBox, base<Widget>>("TextBox")
        .constructor<
            nanogui::Widget*,
            std::string
        >()
        .function("value", &TextBox::value)
        .function("setValue", &TextBox::setValue)
        .function("editable", &TextBox::editable)
        .function("setEditable", &TextBox::setEditable)
        .function("spinnable", &TextBox::spinnable)
        .function("setSpinnable", &TextBox::setSpinnable)
        .function("alignment", &TextBox::alignment)
        .function("setAlignment", &TextBox::setAlignment)
        .function("placeholder", &TextBox::placeholder)
        .function("setPlaceholder", &TextBox::setPlaceholder)
        ;
}




EMSCRIPTEN_BINDINGS ( stl_wrappers )  {
    register_vector<std::string> ( "VectorString" );
}