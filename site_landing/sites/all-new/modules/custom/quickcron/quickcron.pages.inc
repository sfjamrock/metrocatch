<?php

/**
 * @file
 * Quickcron admin pages and launch functions.
 */

/**
 * Quickcron management form.
 */
function quickcron_admin_page_form(&$form_state) {
  $form = array();
  $quickcron_status = !lock_may_be_available('quickcron');
  if ($quickcron_status) {
    $form['status'] = array(
      '#type' => 'item',
      '#title' => t('Status'),
      '#value' => t('Quickcron is running.'),
    );
    $form['stop'] = array(
      '#type' => 'submit',
      '#value' => t('Stop'),
    );
    $form['restart'] = array(
      '#type' => 'submit',
      '#value' => t('Restart'),
    );
    if ($form_state['storage']['clear_locks']) {
      $form['clear'] = array(
        '#type' => 'submit',
        '#value' => t('Clear locks'),
      );
    }
  }
  else {
    $form['status'] = array(
      '#type' => 'item',
      '#title' => t('Status'),
      '#value' => t('Quickcron is NOT running.'),
    );
    $form['start'] = array(
      '#type' => 'submit',
      '#value' => t('Start'),
    );
  }
  $form['#submit'] = array('quickcron_admin_page_form_submit');
  return $form;
}

/**
 * Quickcron management form submit handler.
 */
function quickcron_admin_page_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == $form['start']['#id']) {
    quickcron_start();
  }
  if ($form_state['clicked_button']['#id'] == $form['stop']['#id']) {
    quickcron_stop($form_state);
  }
  if ($form_state['clicked_button']['#id'] == $form['restart']['#id']) {
    quickcron_stop($form_state);
    quickcron_start();
  }
  if ($form_state['clicked_button']['#id'] == $form['clear']['#id'] && isset($form_state['storage']['clear_locks'])) {
    unset($form_state['storage']['clear_locks']);
    // Clearing locks.
    quickcron_reset();
    drupal_set_message(t('Quickcron locks cleared.'));
  }
}

/**
 * Quickcron config page form.
 */
function quickcron_config_page_form() {
  $form = array(
    'quickcron_run_time' => array(
      '#type' => 'textfield',
      '#title' => t('Maximum execution time'),
      '#description' => t('Quickcron max execution time, seconds'),
      '#default_value' => variable_get('quickcron_run_time', '650'),
    ),
    'quickcron_period' => array(
      '#type' => 'textfield',
      '#title' => t('Minimum execution period'),
      '#description' => t('A period hook_quickcron must be called, seconds'),
      '#default_value' => variable_get('quickcron_period', '1'),
    ),
    'quickcron_log' => array(
      '#type' => 'checkbox',
      '#title' => t('Log quickcron startup and shutdown to watchdog'),
      '#default_value' => variable_get('quickcron_log', 1),
    ),
  );
  return system_settings_form($form);
}

/**
 * Start quickcron handler.
 */
function quickcron_start() {
  if (!lock_may_be_available('quickcron')) {
    drupal_set_message(t('Quickcron is already running.'));
    return;
  }

  global $base_url;
  $script_url = $base_url .'/'. drupal_get_path('module', 'quickcron') .'/quickcron.php';

  $str = exec('wget '. $script_url .' > /tmp/quickcron-buf &');

  // Sleep to make sure quickcron.php started and lock acquired.
  sleep(1);

  drupal_set_message(t('Quickcron started.'));
}

/**
 * Stop quickcron handler.
 */
function quickcron_stop(&$form_state) {
  // Set lock.
  if (!lock_acquire('new_quickcron', 5)) {
    drupal_set_message(t('Can\'t acquire lock "new_quickcron", try again later.'), 'warning');
    return;
  }
  else {
    // Wait for quickcron shutdown.
    $max_wait_time = 10;
    $time = 0;
    while (!lock_may_be_available('quickcron')) {
      sleep(1);
      $time++;
      if ($time >= $max_wait_time) {
        drupal_set_message(t('Quickcron refuses to shut down. If you are certain that quickcron is not running you may use "clear locks" button.'), 'error');
        $form_state['storage']['clear_locks'] = TRUE;
        lock_release('new_quickcron');
        return;
      }
    }
    drupal_set_message(t('Quickcron was stopped.'));
    lock_release('new_quickcron');
    return;
  }
}
