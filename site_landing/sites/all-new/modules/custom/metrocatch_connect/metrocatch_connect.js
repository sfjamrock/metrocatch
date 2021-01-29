/**
 * Create isset() function.
 * Check that variable is defined.
 */
if (window.isset == undefined) {
  window.isset = function(val) {
    try {
      eval('window.isset.tmp='+val);
      return (window.isset.tmp != undefined && window.isset.tmp != null);
    } catch(e) {
      return false;
    }
  }
}

/**
 * Handles connection actions buttons. E.g. Break connection, make permanent, etc.
 * Show popup form (accept/deny/break connection) on click.
 */
Drupal.behaviors.metroCatchConnectActions = function(context) {
  $('a.connection-action, .metromail-message-connection-actions a', context).click(function(event) {
    var args = splitUrlIntoArgs($(this).attr('href'));
    var callback  = function(form_state) {
      // Show messages:
      var messages = $('<div></div>').html(form_state.result.messages);
      Drupal.attachBehaviors(messages);
      $('#main-wrapper').prepend(messages);
      // If is on his connections page, replace a themed connection with new one:
      var id = '#connection-' + form_state.values.connection.cid;
      if (!$(id).length) {
        return;
      }
      var new_content = $('<div></div>').html(form_state.result.themed_connection);
      $(id).replaceWith(new_content.html());
      Drupal.attachBehaviors($(id));
    };
    popupFormsFormShow('metrocatch_connect_change_connection_type_form', callback, {
      new_type: args.pop(),
      old_type: args.pop(),
      cid: args.pop()
     },
    {
      fail_callback: function(options){window.location.href = options.backurl;},
      backurl: $(this).attr('href')
    }
    );
    event.stopPropagation();
    return false;
  });
  if (typeof(Drupal.popup_forms_child) != 'undefined') {
    $('#metrocatch-connect-change-connection-type-form .container-inline a', context).click(function() {
      $('.popup-forms-container').dialog('close');
      return false;
    });
  }
  /**
   * Redirects user to profile creation form
   * when he clicks "Create" link in popup.
   */
  $('a.close-popup-link', context).click(function() {
    parent.window.location = $(this).attr('href');
    event.stopPropagation();
    return false;
  });
}

/**
 * Drupal behavior for "connect" link on all pages
 * Makes all "connect" links shown in popups.
 */
Drupal.behaviors.metrocatch_connect_connect = function(context) {
  if (context == document) {
    // When guest clicks on 'connect' button
    // we show him login form.
    // When he logs in we show him connect dialog if he can connect with this user
    // or error message in popup window otherwise.
    if (isset('Drupal.settings.metrocatch_connect.show_form') && !!Drupal.settings.metrocatch_connect.show_form) {
      showConnectDialog(Drupal.settings.metrocatch_connect.uid_to);
    }
    else if (isset('Drupal.settings.metrocatch_connect.show_message') && !!Drupal.settings.metrocatch_connect.show_message) {
      var message = $('<div class="message warning"></div>').html(Drupal.settings.metrocatch_connect.message);
      var modal = $('<div class="messages"></div>').html(message)
      modal.dialog({
        modal: true,
        width: 500,
        buttons: {
          Close: function() {
            $(this).dialog('close');
          }
        }
      });
      $('a.connection-action', message).click(function(event) {
        modal.dialog('close');
      });
    }
  }
  //handle "connect" link
  $('a.metrocatch-connect-link', context).click(function(event) {
    if (!Drupal.settings.metrocatch_connect.uid_from) {
      // User is not logged in, show login dialog:
      showLoginDialog(Drupal.settings.metrocatch_connect.uid_to);
    }
    else {
      var uid_to = Drupal.settings.metrocatch_connect.uid_to;
      showConnectDialog(uid_to);
    }
    event.stopPropagation();
    return false;
  });
  // handle paypal form in popup
  $('form#uc-paypal-wps-form', context).attr('target', '_blank').find('input').click(function(event) {
    if (typeof (Drupal.popup_forms_child) != 'undefined') {
      window.setTimeout(function() {Drupal.popup_forms_child.closeDialog();}, 1000);
    }
  });

  /**
   * Show popup "connect" form
   */
  function showConnectDialog(uid_to) {
    var callback = function(form_state) {
      var new_content = $('<div></div>').html(form_state.result.update);
      // Replace 'Connect' button with new message:
      $('#metrocatch-connect-to-'+ form_state.result.uid_to).replaceWith(new_content);
      // Show user status messages:
      $('#main-wrapper').prepend(form_state.result.messages);
      Drupal.attachBehaviors(new_content);
      Drupal.settings.metrocatch_connect.show_form = false; // Do not show form anymore.
    }
    popupFormsFormShow('metrocatch_connect_connect_form', callback, {
      uid_to : uid_to,
      uid_from : Drupal.settings.metrocatch_connect.uid_from,
      redirect: window.location.pathname.substr(Drupal.settings.basePath.length)
    }, {fail_callback: function(){window.location.href = '/connect/new/' + uid_to + '?destination=' + window.location.pathname.substr(Drupal.settings.basePath.length)}});
  }

  /**
   * Show "login" popup form
   */
  function showLoginDialog(uid_to) {
      var loc = document.location.href;
      loc += (loc.indexOf('?') >= 0) ? '&connect=' : '?connect=';
      loc += uid_to;
    var callback = function(form_state) {
      var location = document.location.href;
      location += (location.indexOf('?') >= 0) ? '&connect=' : '?connect=';
      location += uid_to;
      // When user submits Login form we redirect him on this page
      // with additiona parameter 'connect'
      // which means we shoud show him connect form in popup window.
      window.location = location;
    }
    popupFormsFormShow('user_login', callback, {connect_to : uid_to},
      {
        fail_callback: function(options){window.location.href = options.backurl;},
        backurl: '/connect/new/' + uid_to + '?destination=' + loc
      }
    );
  }
}

/**
 * Behavior for "Become a metrocatch" page. Contains calendar logic.
 */
Drupal.behaviors.metrocatch_connect_purchase_form = function(context) {
  /**
   * "beforeShowDay" event handler for catch datepicker
   */
  function checkForBusyDay(date) {
    featureType = $('#feature-type-radios input:checked').val();
    if (featureType == 'feature') {
      busydays = Drupal.settings.metrocatch.feature.busydays;
    }
    else {
      busydays = Drupal.settings.metrocatch.subfeature.busydays;
    }
    var myDate = date.getDate() +'_'+ (date.getMonth() + 1) +'_'+ date.getFullYear();
    if (typeof(busydays[myDate]) != 'undefined') {
      return [false, "", Drupal.t('Occuped by other metrocatch')];
    }
    if($('#edit-three-months-plan').attr('checked')) {
      if (!checkDateForThreeMonthsPlan(myDate)) {
        return [false, "", ""];
      }
    }
    return [true, "", ""];
  }

  $('#metrocatch-connect-feature-purchase-form input[name="catch_date"]', context).each(function(){
    hidden = $(this);
    hidden.before('<div id="catch-date"></div>');
    myDatepicker = hidden.parents('#metrocatch-connect-feature-purchase-form').find('#catch-date');
    datepickerOptions = {onSelect: catchDateSelected, beforeShowDay: checkForBusyDay};
    // set selected date for datepicker, if set
    selectedDate = $('input[name="catch_date"]').val();
    if (selectedDate != '') {
      parts = selectedDate.split('_');
      var date = new Date(parseInt(parts[2]), parseInt(parts[1]) - 1, parseInt(parts[0]));
      datepickerOptions.defaultDate = date;
    }
    Drupal.settings.metrocatch_datepicker = $(myDatepicker.datepicker(datepickerOptions));
    $('#feature-type-radios input', context).click(function(){
      refreshCatchCalendar();
      refreshThreeMonthsPlan();
    });
    refreshCatchCalendar();
    refreshThreeMonthsPlan();
  });
  $('#metrocatch-connect-feature-purchase-form #edit-three-months-plan', context).click(function(){
    Drupal.settings.metrocatch_datepicker.datepicker("refresh");
  });
}

/**
 * Behavior for "Chanracters left" field on connection form.
 */
Drupal.behaviors.metrocatch_connect_connect_form = function(context) {
  var form = $('#metrocatch-connect-connect-form', context);
  if (form.size() == 0) {
    return;
  }
  form.find('#edit-message-wrapper label').append('<span class="metrocatch-connect-number-container">(<span class="metrocatch-connect-character-left"></span> '+ Drupal.t('characters left') +')</span>');
  var container = form.find('.metrocatch-connect-character-left');
  var updateNumber = function(textarea) {
    var charactersNumber = textarea.val().length;
    var numbersLeft = 500 - charactersNumber;
    var numberStr = '';
    if (numbersLeft < 0) {
      numberStr = '-' + Math.abs(numbersLeft);
      container.addClass("metrocatch-connect-negative-number");
      container.removeClass("metrocatch-connect-positive-number");
    }
    else {
      numberStr = '' + Math.abs(numbersLeft);
      container.addClass("metrocatch-connect-positive-number");
      container.removeClass("metrocatch-connect-negative-number");
    }
    container.text(numberStr)
  };

  var textarea = form.find('textarea#edit-message');
  if (textarea.length) {
    textarea.bind('input propertychange', function(){
      updateNumber($(this));
    });
    updateNumber(textarea);
  }
}

/**
 * Handle 'Facebook share' links.
 */
Drupal.behaviors.metroCatchShareFacebook = function(context) {
  var URL = location.protocol + '//graph.facebook.com';
  $('a.catch-share-facebook', context).each(function() {
    var self = $(this);
    self.attr('href', self.attr('href') +'?_'+ Math.random());
    // Get number of shares:
    $.ajax({
      url: URL + '/' + encodeURI(self.attr('share_url')),
      data: {},
      type: 'GET',
      dataType: 'jsonp',
      success: function(response, status) {
        var count = response.shares || 0;
        self.append(' ( <b>' + count + '</b> )');
      }
    });
    self.bind('click', function(event) {
      // IE does not support title argument.
      var title = ($.browser.msie ? '' : Drupal.t('Share this Catch'));
      window.open($(this).attr('href'), title, 'toolbar=0,status=0,width=548,height=325');
      event.stopPropagation();
      return false;
    });
  });
}

/**
 * Handle 'Tweet' links.
 */
Drupal.behaviors.metroCatchShareTwitter = function(context) {
  var URL = location.protocol + '//urls.api.twitter.com/1/urls/count.json';
  $('a.twitter-share-button', context).each(function() {
    var self = $(this);
    self.attr('href', self.attr('href') +'?_'+ Math.random());
    // Get number of tweets:
    $.ajax({
      url: URL,
      data: {
        'url': self.attr('share_url')
      },
      type: 'GET',
      dataType: 'jsonp',
      success: function(response, status) {
        var count = response.count || 0;
        self.append(' ( <b>' + count + '</b> )');
      }
    });
    self.bind('click', function(event) {
      // IE does not support title argument.
      var title = ($.browser.msie ? '' : Drupal.t('Share this Catch'));
      window.open($(this).attr('href'), title, 'toolbar=0,status=0,width=548,height=325');
      event.stopPropagation();
      return false;
    });
  });
}

/**
 * Handle 'Email share' links.
 */
Drupal.behaviors.metroCatchShareEmail = function(context) {
  $('a.email-share-button', context).click(function(event) {
    var args = splitQueryIntoArgs($(this).attr('href'));
    var callback = function (form_state) {
      // Show messages:
      var messages = $('<div></div>').html(form_state.result.messages);
      Drupal.attachBehaviors(messages);
      $('#main-wrapper').prepend(messages);
    };
    popupFormsFormShow('metrocatch_connect_share_email_form', callback, {
      url: args.url
    });
    event.stopPropagation();
    return false;
  });
}

/**
 * Handles Today's catches tabs.
 *
 * Clicking on a catch tab rotates that one to the featured slot.
 */
Drupal.behaviors.metrocatchTodaysCatchesTabs = function(context) {
  $('.todays-catches.tabs a', context).click(function(event) {
    var args = splitUrlIntoArgs($(this).attr('href'));
    var uid = args.pop();
    var _this = this;
    var callback = function() {
      $('.todays-catches.tabs li', context).each(function() {
        $(this).removeClass('active');
      });
      $(_this).parent('li').addClass('active');
    }
    metrocatchConnectRotateFeatureSlot(uid, 'featured_teaser', callback);
    event.stopPropagation();
    return false;
  });
}

/**
 * On the metrocatch of the Day Sub-Feature Page
 * when user clicks on any Catch in the Others Catch block,
 * this catch rotates to the featured slot.
 */
Drupal.behaviors.metrocatch_connect_rotate_featured_slot = function(context) {
  $('a.other-catch', context).click(function(event) {
    if ($(this).parents('#featured-catches-block').length) {
      // On the Today's catch page do not rotate feature slot.
      return;
    }
    var args = splitUrlIntoArgs($(this).attr('href'));
    var uid = args.pop();
    if (uid == 'archive') {
      uid = args.pop();
    }
    metrocatchConnectRotateFeatureSlot(uid, 'subfeatured');
    event.stopPropagation();
    return false;
  });
}

/**
 * "Select" callback for catch datepicker
 */
function catchDateSelected(dateText, inst) {
  var date = new Date(dateText);
  hidden = $('#metrocatch-connect-feature-purchase-form input[name="catch_date"]');
  hidden.val(date.getDate() +'_'+ (date.getMonth()+1) +'_'+ date.getFullYear()) ;
  refreshThreeMonthsPlan();
}

/**
 * Refresh grayed days in the catch calendar
 */
function refreshCatchCalendar() {
  // 1. get selected feature type
  featureType = $('#feature-type-radios input:checked').val();
  if (featureType == 'feature') {
    Drupal.settings.metrocatch_datepicker.datepicker('option', {
      minDate: Drupal.settings.metrocatch.feature.daterange_min,
      maxDate: Drupal.settings.metrocatch.feature.daterange_max
    });
  }
  else {
    // subfeature
    Drupal.settings.metrocatch_datepicker.datepicker('option', {
      minDate: Drupal.settings.metrocatch.subfeature.daterange_min,
      maxDate: Drupal.settings.metrocatch.subfeature.daterange_max
    });
  }
  // check current selected date
  var date = Drupal.settings.metrocatch_datepicker.datepicker('getDate')
  if (date != null) {
    // refresh value in hidden textfield
    catchDateSelected(date);
  }
}

function splitUrlIntoArgs(url) {
  var args = [];
  if (url.indexOf('?') >= 0) {
    url = url.split('?')[0];
  }
  if (url.indexOf('#') >= 0) {
    args = url.split('#')[0].split('/');
  }
  else {
    args = url.split('/');
  }
  return args;
}

function splitQueryIntoArgs(url) {
  var args = {}, key = 0, val = 0;
  $.each(url.substr(url.indexOf('?') + 1).split('&'), function (n, arg) {
    key = arg.split('=')[0];
    val = arg.substr(arg.indexOf('=') + 1);
    args[key] = val;
  });
  return args;
}

/**
 * Checks that date can be used as a start date for "3 months plan"
 *
 * @param string selectedDate
 *   date in m_d_Y format
 *
 * @return true or false
 */
function checkDateForThreeMonthsPlan(selectedDate) {
  dateParts = selectedDate.split('_');
  var date = new Date();
  date.setFullYear(dateParts[2], parseInt(dateParts[1]) - 1, dateParts[0]); // create date object
  for (i = 1; i < 3; ++i) {
    var dayNumber = date.getDate();
    if (date.getMonth() == 11) {
      date.setMonth(0);
      date.setYear(date.getFullYear() + 1);
    }
    else {
      date.setMonth(date.getMonth() + 1);
    }
    if ((dayNumber != date.getDate()) || (typeof(busydays[date.getDate() +'_'+ (date.getMonth() + 1) +'_'+ date.getFullYear()]) != 'undefined')) {
      return false;
      break;
    }
  }
  return true;
}

/**
 * Refresh "Make it a 3 Month Plan" checkbox
 */
function refreshThreeMonthsPlan() {
  featureType = $('#feature-type-radios input:checked').val();
  if (featureType == 'feature') {
    busydays = Drupal.settings.metrocatch.feature.busydays;
  }
  else {
    busydays = Drupal.settings.metrocatch.subfeature.busydays;
  }


  selectedDate = $('input[name="catch_date"]').val();
  userCanCheck = true;
  if (selectedDate != '') {
    userCanCheck = checkDateForThreeMonthsPlan(selectedDate);
  }
  // find checkbox
  checkbox = $('#edit-three-months-plan');
  // and update its state
  if (userCanCheck) {
    checkbox.attr('disabled', false);
  }
  else {
    checkbox.attr('checked', false);
    checkbox.attr('disabled', true);
  }
}

/**
 * Rotates catch to the featured slot.
 *
 * @param uid
 *  The UID of the user to display in the Feature slot.
 * @param type
 *  Indicating of how to theme feature slot (e.g. 'subfeatured', 'featured').
 * @param callback
 *  (Optional) Callback to call on success.
 */
function metrocatchConnectRotateFeatureSlot(uid, type, callback) {
  var feature_slot = $('.feature-slot:first', document);
  $.ajax({
    url: Drupal.settings.basePath + '?q=ajax-metrocatch-connect-rotate-feature-slot',
    data: {
      uid: uid,
      type: type
    },
    type: 'GET',
    success: function(response, status) {
      window.FB = {};
      if (typeof(response) == 'string') {
        response = Drupal.parseJson(response);
      }
      $.extend(Drupal.settings, response.settings);
      // Move clicked catch into the Feature slot:
      var new_feature_slot = $('<div></div>').html(response.featured_slot);
      Drupal.attachBehaviors(new_feature_slot);
      feature_slot.html(new_feature_slot.children());
      if (!!callback) {
        callback.call();
      }
    }
  });
}
