/**
 * Created by Zaur abdulgalimov@gmail.com on 08.05.2018.
 */


NanoguiModule.postRun.push(function() {
  Nanogui.Button.prototype.addClick = function(func) {
    if (!this.pointerId) {
      this._clickListeners = [];
      var id = this.pointerId = this.setCallback();
      //
      if (!Nanogui.Button.items) {
        Nanogui.Button.items = {};
      }
      Nanogui.Button.items[id] = this;
    }
    this._clickListeners.push({
      func: func
    });
  };
  //
  Nanogui.eventCallback = function nanoguiEvent(event) {
    switch (event) {
      case 1:
        var id = arguments[1];
        if (Nanogui.Button.items[id]) {
          var button = Nanogui.Button.items[id];
          var listeners = button._clickListeners;
          for (var i=0; i<listeners.length; i++) {
            listeners[i].func(button);
          }
        }
        break;
    }
  }
});