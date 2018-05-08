mergeInto(LibraryManager.library, {
  dropEvent: function(event, v1) {
    try {
      Nanogui.eventCallback.apply(null, arguments);
    } catch(e) {
    }
  }
});