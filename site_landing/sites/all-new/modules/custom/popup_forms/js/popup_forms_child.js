(function ($) {
  Drupal.popup_forms_child = Drupal.popup_forms_child || {processed: false};

  /**
   * Child dialog behavior.
   */
  Drupal.popup_forms_child.attach = function (context) {
    var self = Drupal.popup_forms_child;
    var settings = Drupal.settings.popup_forms_child;

    Drupal.settings.popup_options = JSON.parse(decodeURIComponent(window.location.href.substr(window.location.href.indexOf('&options=') + 9)));
    Drupal.settings.popup_options.backurl = unescape(Drupal.settings.popup_options.backurl);
//    Drupal.settings.popup_options.modal = false;
//    Drupal.settings.popup_options.resizable = false;
//    Drupal.settings.popup_options.drag = undefined;

    // calculate dialog position (centred)
    $('.dialog-page-wrapper').css('position', 'absolute');
    wrapperHeight = $('.dialog-page-wrapper').height();
    wrapperWidth = $('.dialog-page-wrapper').width();
    $('.dialog-page-wrapper').css('position', 'static');
    // open dialog
    Drupal.settings.popup_options.close = Drupal.popup_forms_child.closeDialog;
    Drupal.settings.popup_options.position = ['center', self.computePosition(wrapperHeight, wrapperWidth)[1]];
    Drupal.settings.popup_options.title = $('title').html();
    $('.popup-forms-container').dialog(Drupal.settings.popup_options).dialog('open');
    $.postMessage({type: 'finish_loading', index: settings.index}, Drupal.settings.popup_options.backurl, parent);
    $('.popup-forms-dialog-wrapper').addClass('visible');
  };

  Drupal.popup_forms_child.closeDialog = function(event, ui) {
    // Remove iframe from parent document
    $.postMessage({type: 'close_dialog', index: Drupal.settings.popup_forms_child.index}, Drupal.settings.popup_options.backurl, parent);
  }

  /**
   * Returns computed position for dialog
   */
  Drupal.popup_forms_child.computePosition = function(wrapperHeight, wrapperWidth) {
    opts = Drupal.settings.popup_options;
    return [(parseInt(opts.wndWidth) - wrapperWidth) / 2, (parseInt(opts.wndHeight) - wrapperHeight) / 2 + parseInt(opts.wndScroll)];
  }


  /**
   * Check if the given variable is an object.
   */
  Drupal.popup_forms_child.isObject = function (something) {
    return (something != null && typeof something === 'object');
  };

  Drupal.behaviors.popup_forms_child = Drupal.popup_forms_child.attach;

})(jQuery);
