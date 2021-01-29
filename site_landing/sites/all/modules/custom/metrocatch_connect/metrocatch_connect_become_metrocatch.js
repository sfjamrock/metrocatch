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
      return [false, "", Drupal.t('Occuped by other MetroCatch')];
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

/**
 * Checks that date can be used as a start date for "3 months plan"
 * @param string selectedDate date in m_d_Y format
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
