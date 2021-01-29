<?php
/**
 * @file
 * Contains views hook implementations and helpers.
 */

/**
 * Implements hook_views_plugin().
 */
function metrocatch_gallery_views_plugins() {
  return array(
    'style' => array(
      'metrocatch_gallery_jcarousel' => array(
        'title' => t('Metrocatch Gallery jCarousel'),
        'help' => t('Display rows in a carousel via jCarousel.'),
        'parent' => 'jcarousel',
        'handler' => 'metrocatch_gallery_plugin_style_jcarousel',
        'theme' => 'metrocatch_gallery_jcarousel',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}

/**
 * Preprocess variables for the Gallery jCarousel template.
 */
function template_preprocess_metrocatch_gallery_jcarousel(&$vars) {
  static $dom_counter = 0;
  $view = $vars['view'];
  drupal_add_js(drupal_get_path('module', 'metrocatch_gallery') .'/metrocatch_gallery.js');
  $options = $view->style_plugin->options;
  $settings = array(
    'metrocatch_gallery' => array(
      'play_timeout' => $options['play_timeout'],
    ),
  );
  drupal_add_js($settings, 'setting');
  $vars += array(
    'display_controls' => $options['display_controls'],
    'controls' => ((!empty($options['display_controls']) && $view->total_rows > 1) ? theme('metrocatch_gallery_contols', $view, ++$dom_counter) : ''),
  );
}
