mergeInto(LibraryManager.library, {
  dropEvent: function(event, v1) {
    if (window['nanoguiEvent']) {
      window['nanoguiEvent'].apply(null, arguments);
    }
  },
});