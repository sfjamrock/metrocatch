/**
 * Behavior for "view page data" link
 */
Drupal.behaviors.cyd_utils_links = function(context) {
  if (typeof(Drupal.settings.cyd_utils_page_id) != 'undefined') {
    tmp = $('<a class=\"cyd-utils-page-view-link\" href=\"/admin/settings/cyd-utils/pages/'+ Drupal.settings.cyd_utils_page_id +'\">'+ Drupal.t('View page data') +'</a>')
      .css(
        {
          'dsiplay': 'block',
          'position': 'fixed',
          'bottom': '0px',
          'right': '0px',
          'padding': '4px 16px',
          'font-size': '14px',
          'text-decoration': 'underline',
          'color': 'blue'
        }
      )
      .attr('target', '_blank');
    // add "view page data" link to the page
    $('body', context).append(tmp);
  }
};
