

/**
 * Handles Manage Gallery form.
 */
Drupal.behaviors.metrocatchGalleryManageForm = function(context) {
  var form = $('#metrocatch-gallery-manage-form', context);
  var items = $('.gallery-items', form);
  $('.gallery-item select', items).hide();
  $('input[name="okay"]', form).hide();
  if (items.length) {
    items.sortable({
      items: '.gallery-item',
      start: function(event, ui) {
        $('a', this).css('visibility', 'hidden');
      },
      stop: function(event, ui) {
        $('a', this).css('visibility', 'visible');
        var weight = 1;
        var nid = 0;
        var data = {};
        var element = {};
        $.each(items.sortable('toArray'), function(k, item) {
          element = $('#'+ item);
          $('select', element).val(weight);
          nid = element.attr('id').split('gallery-item-')[1];
          data['nodes['+ nid + ']'] = weight;
          weight++;
        });
        data['token'] = $('input[name="form_token"]', form).val();
        $.post(Drupal.settings.basePath +'?q=ajax-metrocatch-gallery-change-weights', data);
      }
    }).disableSelection();
  }
  $('a.edit-picture', items).click(function(event) {
    var url = $(this).attr('href');
    var form = $(this).parents('form');
    var wrapper = form.parent('div');
    url += (url.indexOf('?') >= 0 ? '&ajax=1' : '?ajax=1');
    url += '&action='+ encodeURIComponent(form.attr('action'));
    $.ajax({
      url: url,
      dataType: 'json',
      type: 'GET',
      success: function(response, status) {
        if (typeof(response) == 'string') {
          response = Drupal.parseJson(response);
        }
        Drupal.freezeHeight();
        wrapper.empty().append(response.data);
        Drupal.attachBehaviors(wrapper);
        Drupal.unfreezeHeight();
      }
    });
    event.stopPropagation();
    return false;
  });
}

/**
 * Open Manage gallery form in popup.
 */
Drupal.behaviors.metrocatchGalleryOpenManageForm = function(context) {
  $('a.metrocatch-gallery-manage', context).click(function(event) {
    var uid = $(this).attr('id').split('metrocatch-gallery-manage-')[1];
    popupFormsFormShow('metrocatch_gallery_manage_form', function() {}, {
      uid: uid
      },
      {
        fail_callback: function(options){window.location.href = options.backurl;},
        backurl: $(this).attr('href') + '?destination=' + window.location.href
      }
    );
    event.stopPropagation();
    return false;
  });
}

/**
 * Handle Gallery carousel.
 */
Drupal.behaviors.metrocatchGallery = function(context) {
  $(window).bind('load.jcarousel', function() {
    var carousel = $('.jcarousel-dom-1', context);
    if (!carousel.length) {
      return;
    }
    var instance = jQuery('.jcarousel-dom-1').data('jcarousel');
    var options = instance.options;
    instance.control = $('.jcarousel-control').get(0);
    instance.buttonNext = $('.jcarousel-next');
    instance.buttonPrev = $('.jcarousel-prev');
    instance.buttons(true, false);
    options.buttonNextCallback = function(self, button, state) {
      if (state == false && !self.paused) {
        self.pause();
      }
    }
    options.playCallback = function(self) {
      $(self.control).removeClass('play').addClass('pause');
    }
    options.pauseCallback = function(self) {
      $(self.control).removeClass('pause').addClass('play');
    }
    options.itemVisibleInCallback = {
        onBeforeAnimation: function(jc, item, pos, state) {
          $('.views-field-ops, .views-field-field-picture-file-data').hide();
        },
        onAfterAnimation: function(jc, item, pos, state) {
          $('.views-field-ops, .views-field-field-picture-file-data').show();
        }
      }
    instance.pause = function() {
      carousel.removeClass('played').addClass('paused');
      instance.paused = true;
      instance.stopAuto();
      instance.callback('pauseCallback', 'onAfterAnimation');
    };
    instance.play = function(timeout) {
      timeout = timeout || Drupal.settings.metrocatch_gallery.play_timeout;
      carousel.removeClass('paused').addClass('played');
      instance.paused = false;
      instance.startAuto(timeout);
      instance.callback('playCallback', 'onAfterAnimation');
    };
    $('.jcarousel-dom-1').data('jcarousel', instance);
    $(instance.control).click(function(event) {
      if ($(this).hasClass('play')) {
        instance.play();
      }
      else if ($(this).hasClass('pause')) {
        instance.pause();
      }
    });
  });
}

/**
 * Opens a user gallery in UI dialog.
 */
Drupal.behaviors.metrocatchGalleryBrowse = function(context) {
  $('a.metrocatch-gallery-browse', context).click(function(event) {
    var settings = Drupal.settings.metrocatch_gallery || {};
    var uid = $(this).attr('id').split('metrocatch-gallery-browse-')[1];
    if (!!settings.login) {
      // Display login dialog.
      var callback = function(form_state) {
        // Reload page.
        window.location = document.location.href;
      }
      var args = {connect_to: uid};
      if (!!settings.messages && settings.messages.length) {
        args.messages = JSON.stringify(settings.messages);
      }
      popupFormsFormShow('user_login', callback, args,
      {
        fail_callback: function(options){window.location.href = options.backurl;},
        backurl: $(this).attr('href') + '?destination=' + window.location.href
      }
      );
    }
    else if (!!settings.connect) {
      // Display connect dialog.
      var callback = function(form_state) {
        settings.messages.push($('<div></div>').html(form_state.result.update));
        Drupal.attachBehaviors(form_state.result.messages);
        $('#main-wrapper').prepend(form_state.result.messages);
        settings.connect = false; // Do not show form anymore.
      }
      var args = {
        uid_to : uid,
        uid_from : settings.uid_from
      };
      if (!!settings.messages && settings.messages.length) {
        args.messages = JSON.stringify(settings.messages);
      }
      popupFormsFormShow('metrocatch_connect_connect_form', callback, args,
        {
          fail_callback: function(options){window.location.href = options.backurl;},
          backurl: $(this).attr('href') + '?destination=' + window.location.href
        }
      );
    }
    else if (!!settings.messages && settings.messages.length) {
      // Display message.
      var modal = $('<div class="messages"></div>');
      $.each(settings.messages, function(k, v) {
        var message = $('<div class="message warning"></div>').html(v);
        modal.append(message);
      });
      modal.dialog({
        modal: true,
        width: 500,
        buttons: {
          Close: function() {
            $(this).dialog('close');
          }
        }
      });
      $('a.connection-action', modal).click(function(event) {
        modal.dialog('close');
      });
    }
    else {
      // Open gallery
      var dialog = $('<div style="display:none" class="loading"></div>').appendTo('#content');
      var url = Drupal.settings.basePath +'?q=ajax-metrocatch-gallery-browse/'+ uid;
      dialog.dialog({
          close: function(event, uid) {
            dialog.remove();
          },
          modal: true,
          title: Drupal.settings.metrocatch_gallery.popup_title,
          width: 700,
          height: 520,
          dialogClass: 'metrocatch-gallery-dialog'
        });
      $('.ui-widget-overlay', context).css('background', '#000');
      dialog.append('<iframe frameborder="0" allowtransparency scrolling="no" width="100%" height="100%" class="metrocatch-gallery-iframe"></iframe>')
        .children('iframe.metrocatch-gallery-iframe')
        .attr('src', url)
        .load(function() {
          dialog.removeClass('loading');
        });
    }
    event.stopPropagation();
    return false;
  });
}
