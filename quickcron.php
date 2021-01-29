<?php

/**
 * @file
 * quickcron handler file
 * quickcron runs every 10 minutes and calls hook_quickcron every second.
 */

// Current float timestamp
$start = microtime(TRUE);

require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$max_script_time = 650; // Maximum script execution time, seconds.
$quickcron_period = 1.0; // A period hook_quickcron must be called.
$microseconds_in_second = 1000000;
$lock_new_quickcron_time = 650;
$lock_quickcron_time = 650;

if (!lock_acquire('new_quickcron', $lock_new_quickcron_time)) {
  watchdog('quickcron', 'quickcron can\'t start (another awaiting quickcron detected)');
  return;
}

watchdog('quickcron', 'waiting for old quickcron shutdown');
// Wait for old quickcron shutdown.
while (!lock_acquire('quickcron', $lock_quickcron_time)) {
  usleep(1000000); // Time in microseconds.
  if(microtime(TRUE) - $start > $max_script_time) {
        watchdog('quickcron', 'quickcron exited due timeout (can\'t lock "quickcron" mutex)');
  	lock_release('new_quickcron');
  	return;
  }
}
lock_release('new_quickcron');

watchdog('quickcron', 'quickcron started');
// Call hook_quicktime until new instance is started or $max_script_time is expired
do {
  $time_of_hook_start = microtime(TRUE);
  module_invoke_all('quickcron');
  $time_of_hook_end = microtime(TRUE);
  // Sleep for remaining time.
  $sleep = ($quickcron_period - $time_of_hook_end + $time_of_hook_start) * $microseconds_in_second;
  if ($sleep > 0) {
    usleep($sleep);
  }
} while (lock_may_be_available('new_quickcron') && microtime(TRUE) - $start < $max_script_time);

lock_release('quickcron');

watchdog('quickcron', 'shutdown');
