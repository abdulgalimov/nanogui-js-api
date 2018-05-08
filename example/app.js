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
  //
  //
  new Nanogui.Label(win, "Push buttons", "sans-bold", 16);
  //
  var plainButton = new Nanogui.Button(win, "Plain button", 0);
  plainButton.addClick(function(b) {
    console.log('plain button clicked');
  });
  plainButton.setTooltip("short tooltip");
  //
  var styledButton = new Nanogui.Button(win, "Styled", 0x0000F235);
  styledButton.addClick(function(b) {
    console.log('styled button clicked');
  });
  styledButton.setTooltip("This button has a fairly long tooltip. It is so long, in fact, that the shown text will span several lines.");
  styledButton.setBackgroundColor(new Nanogui.Color(0, 0, 255, 25));
  //
  //
  //
  new Nanogui.Label(win, "Toggle buttons", "sans-bold", 16);
  //
  var toggleButton = new Nanogui.Button(win, 'Toggle me', 0);
  toggleButton.setFlags(Nanogui.Button.Flags.ToggleButton);
  toggleButton.addChange(function(button, state) {
    console.log('toggle button:', state);
  });
  //
  //
  //
  new Nanogui.Label(win, "Radio buttons", "sans-bold", 16);
  function onRadioButtonChange(button, state) {
    console.log('radio button change:', state, button);
  }
  var radioButton1 = new Nanogui.Button(win, 'Radio button 1', 0);
  radioButton1.setFlags(Nanogui.Button.Flags.RadioButton);
  radioButton1.addChange(onRadioButtonChange);
  var radioButton2 = new Nanogui.Button(win, 'Radio button 2', 0);
  radioButton2.setFlags(Nanogui.Button.Flags.RadioButton);
  radioButton2.addChange(onRadioButtonChange);
  //
  //
  //
  new Nanogui.Label(win, "A tool palette", "sans-bold", 16);
  var tools = new Nanogui.Widget(win);
  layout = new Nanogui.BoxLayout(Nanogui.Orientation.Horizontal, Nanogui.Alignment.Middle, 0, 2);
  tools.setLayout(layout);
  Nanogui.createToolButton(tools, Nanogui.Icons.FACEBOOK, 'Facebook');
  Nanogui.createToolButton(tools, Nanogui.Icons.TWITTER, 'Twitter');
  Nanogui.createToolButton(tools, Nanogui.Icons.BAIDU, 'Baidu');
  Nanogui.createToolButton(tools, Nanogui.Icons.VK, 'Vkontakte');
  //
  screen.performLayout();
}

NanoguiModule(Nanogui);