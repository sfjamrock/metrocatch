<?php
/**
 * @file
 * Contains Views2 hook implementations.
 */

/**
 * Implements hook_views_plugin().
 */
function metrocatch_connect_views_plugins() {
  return array(
    'argument validator' => array(
      'metrocatch_connect_users_are_connected' => array(
        'title' => t('Metrocatch: Users are connected'),
        'handler' => 'views_plugin_argument_validate_metrocatch_connect_users_are_connected',
      ),
    ),
  );
}
