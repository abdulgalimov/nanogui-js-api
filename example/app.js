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
    if (window.location.href.indexOf('127.0.0.1') !== -1) {
      return '../build/'+file;
    } else {
      return 'wasm/'+file;
    }
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
function createApp() {
  var v = new Nanogui.Vector2i(window.innerWidth, window.innerHeight);
  screen = new Nanogui.Screen(v, "NanoGUI for JS", true, false, 8, 8, 24, 8, 0, 3, 3, 1);
  screen.setVisible(true);
  //
  createButtonDemo();
  createBasicWidgets();
  //
  screen.performLayout();
}
function createButtonDemo() {
  var win = new Nanogui.Window(screen, "Button demo");
  win.setPosition(new Nanogui.Vector2i(50, 50));
  var layout = new Nanogui.GroupLayout(15, 6, 14, 20);
  console.log('layout', layout);
  win.setLayout(layout);
  //
  //
  //
  var label = new Nanogui.Label(win, "Push buttons");
  console.log('label', label);
  //
  var plainButton = new Nanogui.Button(win, "Plain button", 0);
  plainButton.addClick(function(b) {
    console.log('plain button clicked', b);
  });
  plainButton.setTooltip("short tooltip");
  //
  var styledButton = new Nanogui.Button(win, "Styled", 0x0000F235);
  styledButton.addClick(function(b) {
    console.log('styled button clicked', b);
  });
  styledButton.setTooltip("This button has a fairly long tooltip. It is so long, in fact, that the shown text will span several lines.");
  styledButton.setBackgroundColor(new Nanogui.Color(0, 0, 255, 25));
  //
  //
  //
  new Nanogui.Label(win, "Toggle buttons");
  //
  var toggleButton = new Nanogui.Button(win, 'Toggle me', 0);
  toggleButton.setFlags(Nanogui.Button.Flags.ToggleButton);
  toggleButton.addChange(function(button, state) {
    console.log('toggle button:', state, button);
  });
  //
  //
  //
  new Nanogui.Label(win, "Radio buttons");
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
  new Nanogui.Label(win, "A tool palette");
  var tools = new Nanogui.Widget(win);
  layout = new Nanogui.BoxLayout(Nanogui.Orientation.Horizontal, Nanogui.Alignment.Middle, 0, 2);
  tools.setLayout(layout);
  Nanogui.createToolButton(tools, Nanogui.Icons.FACEBOOK, 'Facebook');
  Nanogui.createToolButton(tools, Nanogui.Icons.TWITTER, 'Twitter');
  Nanogui.createToolButton(tools, Nanogui.Icons.BAIDU, 'Baidu');
  Nanogui.createToolButton(tools, Nanogui.Icons.VK, 'Vkontakte');
  //
  //
  //
  new Nanogui.Label(win, 'Popup buttons');
  var popupButton = new Nanogui.PopupButton(win, 'Popup', Nanogui.Icons.EXPORT);
  var popup = popupButton.popup();
  popup.setLayout(new Nanogui.GroupLayout(15, 6, 14, 20));
  new Nanogui.Label(popup, "Arbitrary widgets can be placed here");
  var checkbox = new Nanogui.CheckBox(popup, "A check box");
  checkbox.setChecked(true);
  checkbox.addChange(function(checkbox, state) {
    console.log('checkbox change state', state, checkbox);
  });
  //
  // popup right
  var popupRight = new Nanogui.PopupButton(popup, "Recursive popup", Nanogui.Icons.FLASH);
  popupRight.popup().setLayout(new Nanogui.GroupLayout(15, 6, 14, 20));
  new Nanogui.CheckBox(popupRight.popup(), "Another check box");
  //
  // popup left
  var popupLeft = new Nanogui.PopupButton(popup, "Recursive popup", Nanogui.Icons.FLASH);
  popupLeft.popup().setLayout(new Nanogui.GroupLayout(15, 6, 14, 20));
  popupLeft.setSide(Nanogui.Popup.Side.Left);
  new Nanogui.CheckBox(popupLeft.popup(), "Another check box");
}

function createBasicWidgets() {
  var win = new Nanogui.Window(screen, "Basic widgets");
  win.setPosition(new Nanogui.Vector2i(250, 50));
  var layout = new Nanogui.GroupLayout(15, 6, 14, 20);
  win.setLayout(layout);
  //
  new Nanogui.Label(win, "Message dialog");
  var tools = new Nanogui.Widget(win);
  tools.setLayout(new Nanogui.BoxLayout(Nanogui.Orientation.Horizontal, Nanogui.Alignment.Middle, 0, 2));
  function onDialogState(dialog, state) {
    console.log('inDialogState', state, dialog);
  }
  var infoButton = new Nanogui.Button(tools, "Info", 0);
  infoButton.addClick(function() {
    var message = new Nanogui.MessageDialog(screen, Nanogui.MessageDialog.Type.Information, "Title", "This is information message", "OK", "Cancel", false);
    message.addClick(onDialogState);
  });
  var warnButton = new Nanogui.Button(tools, "Warn", 0);
  warnButton.addClick(function() {
    var message = new Nanogui.MessageDialog(screen, Nanogui.MessageDialog.Type.Warning, "Warning", "This is warning message", "OK", "Cancel", false);
    message.addClick(onDialogState);
  });
  var askButton = new Nanogui.Button(tools, "Ask", 0);
  askButton.addClick(function() {
    var message = new Nanogui.MessageDialog(screen, Nanogui.MessageDialog.Type.Warning, "Warning", "This is question message", "Yes", "No", true);
    message.addClick(onDialogState);
  });
  //
  //
  //
  new Nanogui.Label(win, "Combo box");
  var comboBox = new Nanogui.ComboBox(win, Nanogui.createVectorString('item 1', 'item 2', 'item 3'));
  comboBox.addChange(function(cb, index) {
    console.log('combo box selected', index, cb);
  });
  //
  new Nanogui.Label(win, "Check box");
  var checkbox = new Nanogui.CheckBox(win, "Flag 1");
  checkbox.setChecked(true);
  checkbox.addChange(function(checkbox, state) {
    console.log('checkbox change state', state, checkbox);
  });
  new Nanogui.CheckBox(win, "Flag 2");
}

NanoguiModule(Nanogui);