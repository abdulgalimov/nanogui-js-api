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
});