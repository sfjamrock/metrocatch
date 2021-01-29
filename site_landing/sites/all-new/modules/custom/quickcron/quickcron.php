<?php

/**
 * @file
 * Quickcron handler file.
 * Quickcron runs every 650 seconds and calls hook_quickcron() every second (by default, can be configured).
 */

$drupal_path = quickcron_drupal_path();
if (empty($drupal_path)) {
  exit;
}

chdir($drupal_path);
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

quickcron_run_quickcron();

/**
 * Detects Drupal path by scanning each level of current path.
 */
function quickcron_drupal_path() {
  $bootstrap = '/includes/bootstrap.inc';
  $path = dirname(str_replace('\\', '/', __FILE__));
  while (($pos = strrpos($path, '/')) !== FALSE) {
    $path = substr($path, 0, $pos);
    if (@file_exists($path . $bootstrap)) {
      return $path;
    }
  }
  return NULL;
}
