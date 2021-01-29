(function ($) {
  Drupal.popup_forms = Drupal.popup_forms || {dialogs: Array()};
  Drupal.popup_forms.loadCallbacks = {};
  /**
   * Open a jquery ui dialog
   * @param options
   *   Properties of the modal frame to open:
   *   - url : the URL of the page to open. If not provided, you have to handle the loading yourself
   *   - method(get) : What method should use to load the url (get/post)
   *   - data : Post DATA you can pass, if you use method=post
   *   - for all the other options please refer the jquery ui dialog documentation http://jqueryui.com/demos/dialog/#options
   */
  Drupal.popup_forms.open = function (c_options) {
    var self = this;

    // initialize new dialog
    var newDialog = {
      options: {},
      childDocumentSize: {},
      container: undefined
    };

    newDialog.options = {
      // custom
      url: '',
      method: 'get',
      data: {},
      // jquery dialog
      modal: true,
      autoOpen: true,
      closeOnEscape: true,
      resizable: false,
      draggable: false,
      autoresize: true,
      namespace: 'popup_forms_default_ns',
      dialogClass: 'popup_forms-dialog',
      title: Drupal.t('Loading...'),
      drag: self.drag
    };
    $.extend(newDialog.options, c_options);
    newDialog.options.autoOpen = newDialog.options.autoresize = true;
    // push dialog data into array
    newIndex = Drupal.popup_forms.dialogs.push(newDialog) - 1;

    // Create the dialog and related DOM elements.
    // Setting dialog
    self.create(newIndex);
    if (self.dialogs[newIndex].options.url != '') {
      self.dialogs[newIndex].options.url = self.dialogs[newIndex].options.url + '&popup_forms_num=' + newIndex;
      self.loadIframe(self.dialogs[newIndex].iframe.get(0), self.dialogs[newIndex].options);
    }

    return newIndex;
  };

  /**
   * Inititalize the dialog.
   */
  Drupal.popup_forms.create = function (index) {
    var self = this;
    // Note: We use scrolling="yes" for IE as a workaround to yet another IE bug
    // where the horizontal scrollbar is always rendered, no matter how wide the
    // iframe element is defined.
    var iframe = $('<iframe frameborder="0" allowtransparency class="popup-forms-iframe" id="popup-forms-iframe-' + index + '" name="popup-forms-iframe' + index + '"' + ($.browser.msie ? ' scrolling="yes"' : '') + '/>');
    $('body').append(iframe);
    iframeHeight =  $(document).height();
    $('#popup-forms-iframe-' + index).css('height', iframeHeight +'px');
    self.dialogs[index].iframe = $('#popup-forms-iframe-' + index);
  };

  /**
   * Load the given URL into the dialog iframe.
   */
  Drupal.popup_forms.loadIframe = function (iframe, options) {
    var self = this;
    // Get the document object of the iframe window.
    // @see http://xkr.us/articles/dom/iframe-document/
    var doc = (iframe.contentWindow || iframe.contentDocument);
    if ('document' in doc) {
      doc = doc.document;
    }

    if (options.method == 'post') {
      $(doc).post(
      options.url, options.data, function (data) {
        $(doc).html(data);
      });
      return;
    }
    //else
    doc.location.replace(options.url);
  };

  /**
   * JQuery UI dialog "close" callback
   */
  Drupal.popup_forms.closeDialog = function(index) {
    // remove iframe
    $('#popup-forms-iframe-' + index).remove();
  }

  /**
   * Returns parameters of window (called from child iframe)
   */
  Drupal.popup_forms.getWindowParameters = function() {
    return {
      scrollTop: $(document).scrollTop(),
      wndHeight: $(window).height(),
      wndWidth: $(window).width()
    };
  }

  /**
   * Called from child js, when dialod is loaded
   */
  Drupal.popup_forms.finishLoading = function(formId) {
    if (typeof(Drupal.popup_forms.loadCallbacks[formId]) != 'undefined') {
      Drupal.popup_forms.loadCallbacks[formId](formId);
    }
  }
})(jQuery);


/**
 * Show  form in ajax popup
 * @param string form_id drupal form id
 * @param string callback called when dialog finished
 * @param form_data various data, that transfered to drupal form callback
 * @param dialog_options jQuery dialog options (size, parameters, etc)
 * @return int popup form id
 */
function popupFormsFormShow(form_id, callback, form_data, dialog_options) {
  var loc = window.location;
  var query_url = "" + loc.protocol + "//" + loc.host + Drupal.settings.basePath + '?q=ajax-get-popup-form/'+ form_id;
  for (param in form_data) {
    query_url += '&' + param +'='+ encodeURIComponent(form_data[param]);
  }
  options = {
      method: 'get',
      url: query_url,
      data: {},
      popupFormsCallback: callback,
      // jquery dialog
      modal: false,
      autoOpen: true,
      closeOnEscape: false,
      resizable: false,
      draggable: true,
      autoresize: true
    };
    $.extend(options, dialog_options);
    if (options.dialogClass !== undefined) {
      options.dialogClass += ' form-id-' + form_id;
    }
    else {
      options.dialogClass += ' popup_forms-dialog popup_forms_default_ns-dialog form-id-' + form_id;
    }

    return Drupal.popup_forms.open(options);
}

/**
 * This callback executed from dialog child JS, when it ready to close
 * @return void
 */
function popupFormsFinishDialog(returnedData) {
  // remove iframe
  $('#popup-forms-iframe-' + returnedData.dialog_index).remove();
  // execute callback
  Drupal.popup_forms.dialogs[returnedData.dialog_index].options.popupFormsCallback(returnedData);
}

function popupFormsMovePopup(popupformId, newLeft, newTop) {
  contents = $('#popup-forms-iframe-'+ popupformId).contents();
  contents.find('.ui-dialog.ui-draggable').css('left', newLeft +'px').css('top', newTop +'px');
}

function popupFormsGetFormElement(popupformId) {
  contents = $('#popup-forms-iframe-'+ popupformId).contents();
  return contents.find('.ui-dialog.ui-draggable');
}

function popupFormsSetDialogLoadingCallback(func, formId) {
  Drupal.popup_forms.loadCallbacks[formId] = func;
}
