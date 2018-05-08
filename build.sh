#!/usr/bin/env bash

cd nanogui-GLES-wasm
if [ ! -d "./build" ]; then
  mkdir build
fi

#em++ -DNANOVG_GLES3_IMPLEMENTATION -DGLFW_INCLUDE_ES3 -DGLFW_INCLUDE_GLEXT -DNANOGUI_LINUX -Iinclude/ -Iext/nanovg/ -Iext/eigen/ button.cpp checkbox.cpp colorpicker.cpp colorwheel.cpp combobox.cpp common.cpp glcanvas.cpp glutil.cpp graph.cpp imagepanel.cpp imageview.cpp label.cpp layout.cpp messagedialog.cpp popup.cpp popupbutton.cpp progressbar.cpp screen.cpp serializer.cpp slider.cpp stackedwidget.cpp tabheader.cpp tabwidget.cpp textbox.cpp theme.cpp vscrollpanel.cpp widget.cpp window.cpp nanogui_resources.cpp nanovg.bc --std=c++11 -O3 -lGL -lGLU -lm -lGLEW -s USE_GLFW=3 -s FULL_ES3=1 -s USE_WEBGL2=1 -s WASM=1 -o nanogui.bc

em++ -DNANOVG_GLES3_IMPLEMENTATION -DGLFW_INCLUDE_ES3 -DGLFW_INCLUDE_GLEXT -DNANOGUI_LINUX -Iinclude/ -Iext/nanovg/ -Iext/eigen/ nanogui.bc ../lib/gui.cpp --std=c++11 -O3 -lGL -lGLU -lm -lGLEW -s USE_GLFW=3 -s FULL_ES3=1 -s USE_WEBGL2=1 -s WASM=1 -o build/nanogui.html --preload-file ./icons \
    --bind --js-library ../lib/js/lib.js -s EXPORT_NAME='"Nanogui"'
cp ./build/nanogui.data ./build/nanogui.js ./build/nanogui.wasm ../build/
rm -r ./build/