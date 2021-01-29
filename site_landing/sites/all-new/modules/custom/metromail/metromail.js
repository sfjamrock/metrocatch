/**
 * Handles Metromail page.
 */
Drupal.metroMail = function (settings) {
  this.method = 'replaceWith';
  this.progress = $('<div class="ahah-progress ahah-progress-throbber"><div class="throbber">&nbsp;</div></div>');
  $.extend(this, settings);
};

/**
 * Executes AJAX request.
 */
Drupal.metroMail.prototype.executeAJAX = function() {
  var ajax = this;
  $.ajax({
    url: ajax.callbackURL,
      beforeSend: function (xmlhttprequest, options) {
        ajax.wrapper.append(ajax.progress);
      },
      success: function (response, status) {
        if (typeof response == 'string') {
          response = Drupal.parseJson(response);
        }
        var newContent = $(response.data);
        ajax.wrapper.empty();
        ajax.wrapper[ajax.method](newContent);
        Drupal.attachBehaviors(newContent);
      },
      complete: function (response, status) {
        ajax.wrapper.show();
        $('.ahah-progress', ajax.wrapper).remove();
        if (status == 'error' || status == 'parsererror') {
          ajax.wrapper.empty();
          var newContent = $('<div></div>').html(Drupal.t('Unknown error'));
          ajax.wrapper[ajax.method](newContent);
          Drupal.attachBehaviors(newContent);
        }
      },
      dataType: 'json',
      data: {}, 
      type: 'post'
  }); 
};

/**
 * Handles the "view more" button on metromail page.
 * 
 * @param context
 */
Drupal.behaviors.metroMailMore = function(context) {
  var settings = {
    element: '.metromail-more',
    method: 'replaceWith'
  };
  $(settings.element, context).click(function() {
    settings.element = $(this);
    settings.wrapper = settings.element.parent('.metromail-more-wrapper');
    settings.callbackURL = '?q=' + $(this).attr('href').split('#')[1];
    var metroMail = new Drupal.metroMail(settings);
    metroMail.executeAJAX();
    return false;
  });
};

/**
 * Handles the "Expand" and "Collapse" buttons on metromail page.
 * 
 * @param context
 */
Drupal.behaviors.metroMailThread = function(context) {
  var settings = {
    wrapperSelector: '.children',
    element: '.metromail-thread',
    collapseText: Drupal.t('Collapse'),
    method: 'append'
  };
  $(settings.element, context).mousedown(function(event) {
    if (event.which == 3) {
      return true; // skip right button
    }
    settings.element = $(this);
    var thread_id = settings.element.attr('id');
    settings.expandText = Drupal.t('Expand (!num)', {'!num': Drupal.settings.metromail.children_count[thread_id]});
    settings.wrapper = settings.element.parents('.metromail-message').find(settings.wrapperSelector).eq(0);
    if (settings.element.hasClass('expanded')) {
      settings.element.html(settings.expandText);
      settings.element.removeClass('expanded').addClass('collapsed');
      settings.wrapper.hide();
    }
    else if (settings.element.hasClass('collapsed')) {
      settings.element.html(settings.collapseText);
      settings.element.removeClass('collapsed').addClass('expanded');
      settings.wrapper.show();
    }
    else {
      settings.callbackURL = '?q='+ $(this).attr('href').split('#')[1];
      var metroMail = new Drupal.metroMail(settings);
      metroMail.executeAJAX();
      settings.element.html(settings.collapseText).addClass('expanded');
    }
    event.preventDefault();
    return false;
  });
  if (context == document && !!Drupal.settings.metromail && !!Drupal.settings.metromail.scroll_to) {
    var scroll_to = $('#metromail-message-' + Drupal.settings.metromail.scroll_to, context);
    if (scroll_to.length) {
      var cTop = scroll_to.offset().top;
      $(context).scrollTop(cTop);
    }
  }
}

 /**
  * Toggling "Recipient" textfield in admin "Send message form".
  * 
  * @param context
  */
 Drupal.behaviors.metroMailAdminForm = function(context) {
   if ($('input:checked[name="send_to"]', context).eq(0).val() != 'someone') {
     $('#edit-recipient-wrapper').hide();
   }
   $(context).find('input[name="send_to"]').change(function() {
       $('#edit-recipient-wrapper').toggle();
   });
 };
