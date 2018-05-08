var canvas = document.getElementById('canvas');
canvas.addEventListener("webglcontextlost", function(e) {
  alert('WebGL context lost. You will need to reload the page.');
  e.preventDefault();
}, false);

var Nanogui = {
  preRun: [],
  postRun: [],
  print: function(text) {
    console.log(text);
  },
  printErr: function(text) {
    if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
    console.error(text);
  },
  canvas: (function() {
    return canvas;
  })(),
  setStatus: function(text) {
  },
  locateFile : function(file) {
    return '../build/'+file;
  }
};

window.onerror = function(event) {
  console.log('ERROR', event);
};

function start() {
  console.log('start');
  function animate() {
    //Nanogui._jsClear();
    Nanogui._jsLoop();
    requestAnimationFrame( animate );
  }
  requestAnimationFrame( animate );
}
Nanogui.postRun.push(function() {
  start();
  createApp();
});


function stringToPointer(string) {
  return Nanogui.allocate(Nanogui.intArrayFromString(string), 'i8', Nanogui.ALLOC_NORMAL);
}

function createExamle() {
  Nanogui._createAppp(stringToPointer("test"));
}


var screen;
var win;
var button;
function createApp() {
  var v = new Nanogui.Vector2i(1024, 768);
  screen = new Nanogui.Screen(v, "NanoGUI for JS", true, false, 8, 8, 24, 8, 0, 3, 0, 0);
  screen.setVisible(true);
  //
  win = new Nanogui.Window(screen, "test win");
  win.setPosition(new Nanogui.Vector2i(50, 50));
  var layout = new Nanogui.GroupLayout(15, 6, 14, 20);
  win.setLayout(layout);
  //
  var label = new Nanogui.Label(win, "Push buttons", "sans-bold", 16);
  //
  button = new Nanogui.Button(win, "Plain button", 0);
  button.addClick(function(b) {
    console.log('plain button clicked');
  });
  button.setTooltip("short tooltip");
  //
  button = new Nanogui.Button(win, "Styled", 0x0000F235);
  button.addClick(function(b) {
    console.log('styled button clicked');
  });
  button.setTooltip("This button has a fairly long tooltip. It is so long, in fact, that the shown text will span several lines.");
  //button.setBackgroundColor();
  //
  screen.performLayout();
}

NanoguiModule(Nanogui);