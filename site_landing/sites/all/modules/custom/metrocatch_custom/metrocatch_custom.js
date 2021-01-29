/**
 * Behavior for "subscribe" link
 */
Drupal.behaviors.metrocatch_custom = function(context) {
  $('a[href="/#subscribe"]', context).click(function(){
    popupFormsFormShow(
      'metrocatch_custom_subscribe_form',
      null,
      {},
      {title : 'MetroCatch subscription'}
    );
    return false;
  });
}