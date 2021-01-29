
/**
 * Hides broken user pictures.
 */
Drupal.behaviors.hideBrokenProfileImages = function(context) {
  $('.metrocatch-profile-default-picture img', context).error(function() {
    $(this).remove();
  });
}

/**
 * Handles metromail flags. 
 */
Drupal.behaviors.metroMailFlags = function(context) {
  $('input.metromail-flag-checbox').click(function(event) {
    $('a.flag', $(this).parents('.flag-wrapper')).click();
  });
}
