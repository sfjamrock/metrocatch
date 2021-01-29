/**
 * Behavior for "subscribe" link, show subscribe form in popup.
 */
Drupal.behaviors.metrocatch_custom = function(context) {
  $('a[href="/#subscribe"]', context).click(function(){
    popupFormsFormShow(
      'metrocatch_custom_subscribe_form',
      null,
      {},
      {title : 'metrocatch subscription'}
    );
    return false;
  });
}

/**
 * Behavior for "N MetroPoints" link on the profile page - shows "redeem MetroPoints" popup.
 */
Drupal.behaviors.metrocatch_custom_redeem_metropoints = function(context) {
  $('.profile-metropoints a[href="/redeem-metropoints"]', context).click(function(){
    link = $(this);

    popupFormsFormShow(
      'metrocatch_custom_redeem_points_form',
      function(data){
        // Update points value.
        link.text(Drupal.formatPlural(data.points_amount, Drupal.t('1 metropoint'), Drupal.t('@count metropoints'), null));
      },
      {},
      {
        fail_callback: function(options){window.location.href = options.backurl;},
        backurl: $(this).attr('href') + '?destination=' + window.location.href
      }
    );
    return false;
  });
}
