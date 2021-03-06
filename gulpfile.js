/**
 * Created by Zaur abdulgalimov@gmail.com on 04.04.2018.
 */

var gulp = require('gulp');
var compressor = require('node-minify');
var fs = require('fs');
var exec = require('child_process').exec;

var PROJECT_DIR = process.env.PWD+'/';
var WASM_DIR = PROJECT_DIR+'nanogui-GLES-wasm/';
var WASM_OUT_DIR = WASM_DIR+'build/';
var BUILD_DIR = PROJECT_DIR+'build/';
console.log('PROJECT_DIR', PROJECT_DIR);

gulp.task('libs', function(taskDone) {
  console.log('libs');
  buildLibs()
    .then(function() {
      console.log('libs complete');
      taskDone()
    });
});

gulp.task('build', function(taskDone) {
  console.log('build');
  buildWasm()
    .then(function() {
      console.log('build complete');
      return addJsPrototype();
    });
});

function buildLibs() {
  var query = 'em++ -DNANOVG_GLES3_IMPLEMENTATION -DGLFW_INCLUDE_ES3 -DGLFW_INCLUDE_GLEXT -DNANOGUI_LINUX -Iinclude/ -Iext/nanovg/ -Iext/eigen/ button.cpp checkbox.cpp colorpicker.cpp colorwheel.cpp combobox.cpp common.cpp glcanvas.cpp glutil.cpp graph.cpp imagepanel.cpp imageview.cpp label.cpp layout.cpp messagedialog.cpp popup.cpp popupbutton.cpp progressbar.cpp screen.cpp serializer.cpp slider.cpp stackedwidget.cpp tabheader.cpp tabwidget.cpp textbox.cpp theme.cpp vscrollpanel.cpp widget.cpp window.cpp nanogui_resources.cpp nanovg.bc --std=c++11 -O3 -lGL -lGLU -lm -lGLEW -s USE_GLFW=3 -s FULL_ES3=1 -s USE_WEBGL2=1 -s WASM=1 -o nanogui.bc';
  //
  var options = {
    cwd: WASM_DIR
  };
  return new Promise(function(resolve) {
    exec(query, options, function(err, stdout, stderr) {
      console.log(err);
      console.log(stdout);
      console.log(stderr);
      //
      resolve();
    })
  });
}

function buildWasm() {
  if (!fs.existsSync(WASM_OUT_DIR)) {
    fs.mkdirSync(WASM_OUT_DIR);
  }
  //
  var query = '';
  query += ' em++ -DNANOVG_GLES3_IMPLEMENTATION -DGLFW_INCLUDE_ES3 -DGLFW_INCLUDE_GLEXT -DNANOGUI_LINUX -Iinclude/ -Iext/nanovg/ -Iext/eigen/ nanogui.bc ../lib/gui.cpp --std=c++11 -O3 -lGL -lGLU -lm -lGLEW -s USE_GLFW=3 -s FULL_ES3=1 -s USE_WEBGL2=1 -s WASM=1 -o build/nanogui.html --preload-file ./icons ';
  query += ' --bind --js-library ../lib/js/lib.js -s EXPORT_NAME=\'"NanoguiModule"\' -s MODULARIZE=1 ';
  query += ' -s EXTRA_EXPORTED_RUNTIME_METHODS=\'["ccall", "cwrap", "intArrayFromString", "allocate", "ALLOC_NORMAL", "writeArrayToMemory"]\'';
  //
  var options = {
    cwd: WASM_DIR
  };
  return new Promise(function(resolve) {
    exec(query, options, function(err, stdout, stderr) {
      console.log(err);
      console.log(stdout);
      console.log(stderr);
      //
      fs.copyFileSync(WASM_OUT_DIR+'nanogui.data', BUILD_DIR+'nanogui.data');
      fs.copyFileSync(WASM_OUT_DIR+'nanogui.js', BUILD_DIR+'nanogui.js');
      fs.copyFileSync(WASM_OUT_DIR+'nanogui.wasm', BUILD_DIR+'nanogui.wasm');
      //
      resolve();
    })
  });
}


function addJsPrototype() {
  return compressor.minify({
    compressor: 'uglifyjs',
    input: PROJECT_DIR+'lib/js/prototype.js',
    output: BUILD_DIR+'prototype.js',
    callback: function (err, min) {}
  })
    .then(function() {
      return new Promise(function(resolve) {
        var nanoguiCode = fs.readFileSync(BUILD_DIR+'nanogui.js', {encoding:'utf8'});
        var prototypeCode = fs.readFileSync(BUILD_DIR+'prototype.js', {encoding:'utf8'});
        //
        var index = nanoguiCode.indexOf('return NanoguiModule;');
        nanoguiCode = nanoguiCode.substr(0, index);
        nanoguiCode += prototypeCode;
        nanoguiCode += '\n\n  return NanoguiModule;\n};';
        //fs.appendFileSync(BUILD_DIR+'nanogui.js', prototypeCode);
        fs.writeFileSync(BUILD_DIR+'nanogui.js', nanoguiCode);
        //
        resolve();
      });
    });
}