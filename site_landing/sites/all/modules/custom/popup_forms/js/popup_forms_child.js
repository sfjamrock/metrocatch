(function ($) {
  Drupal.popup_forms_child = Drupal.popup_forms_child || { processed: false };

  /**
   * Child dialog behavior.
   */
  Drupal.popup_forms_child.attach = function (context) {
    var self = Drupal.popup_forms_child;
    var settings = Drupal.settings.popup_forms_child;

    // If we cannot reach the parent window, then we have nothing else todo here.
    if (!self.isObject(parent.Drupal) || !self.isObject(parent.Drupal.popup_forms.dialogs[settings.index])) {
      return;
    }

    // Shortcuts to parent objects.
    self.$pWindow = parent.jQuery(parent);
    self.pDialog = parent.Drupal.popup_forms.dialogs[settings.index];
    self.handler = parent.Drupal.popup_forms;
    // calculate dialog position (centred)
    $('.dialog-page-wrapper').css('position', 'absolute');
    wrapperHeight = $('.dialog-page-wrapper').height();
    wrapperWidth = $('.dialog-page-wrapper').width();
    $('.dialog-page-wrapper').css('position', 'static');
    self.handler.dialogs[settings.index].options.position = self.computePosition(wrapperHeight, wrapperWidth);
    // open dialog
    self.handler.dialogs[settings.index].options['title'] = $('title').html();
    self.handler.dialogs[settings.index].options.close = Drupal.popup_forms_child.closeDialog;
    $('.popup-forms-container').dialog(self.handler.dialogs[settings.index].options).dialog('open');
    self.handler.finishLoading(settings.index);
    $('.popup-forms-dialog-wrapper').addClass('visible');
  };
  
  Drupal.popup_forms_child.closeDialog = function(event, ui) {
    // remove iframe from parent document
    parent.Drupal.popup_forms.closeDialog(Drupal.settings.popup_forms_child.index);
  }
  
  /**
   * Returns computed position for dialog
   */
  Drupal.popup_forms_child.computePosition = function(wrapperHeight, wrapperWidth) {
    var params = parent.Drupal.popup_forms.getWindowParameters();
    return [(params.wndWidth - wrapperWidth) / 2, (params.wndHeight - wrapperHeight) / 2 + params.scrollTop];
  }
  

  /**
   * Check if the given variable is an object.
   */
  Drupal.popup_forms_child.isObject = function (something) {
    return (something != null && typeof something === 'object');
  };

  Drupal.behaviors.popup_forms_child = Drupal.popup_forms_child.attach;

})(jQuery);
