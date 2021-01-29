<?php
/** @file
 *  metrocatch_email.admin.inc
 *
 * Administrative page callbacks and related functions.
 */

/**
 * Form callback for module settings form.
 */
function metrocatch_email_settings_form() {
  $send_time = array();
  foreach (range(0, 23) as $hour) {
    $send_time[$hour] = $hour .':00';
  }
  $available_lists = _metrocatch_email_get_available_lists();
  $available_lists_options = array(0 => t('- None -'));
  foreach ($available_lists as $list) {
    $available_lists_options[$list->id] = check_plain($list->name);
  }
  $form = array(
    'vertical_response' => array(
      'lists' => array(
        '#type' => 'fieldset',
        '#title' => t('Email lists'),
        'metrocatch_email_men_interested_in_women_list_id' => array(
          '#type' => 'select',
          '#title' => t('Men Interested in Women List'),
          '#default_value' => metrocatch_email_get_user_list('man', 'women'),
          '#options' => $available_lists_options,
        ),
        'metrocatch_email_men_interested_in_men_list_id' => array(
          '#type' => 'select',
          '#title' => t('Men Interested in Men List'),
          '#default_value' => metrocatch_email_get_user_list('man', 'men'),
          '#options' => $available_lists_options,
        ),
        'metrocatch_email_women_interested_in_men_list_id' => array(
          '#type' => 'select',
          '#title' => t('Women Interested in Men List'),
          '#default_value' => metrocatch_email_get_user_list('woman', 'men'),
          '#options' => $available_lists_options,
        ),
        'metrocatch_email_women_interested_in_women_list_id' => array(
          '#type' => 'select',
          '#title' => t('Women Interested in Women List'),
          '#default_value' => metrocatch_email_get_user_list('woman', 'women'),
          '#options' => $available_lists_options,
        ),
      ),
    ),
  );
  return system_settings_form($form);
}


/**
 * Helper function: get an array of available lists.
 */
function _metrocatch_email_get_available_lists() {
  $params = array(
    'type' => 'email',
    'is_master' => FALSE, // ommit master list.
  );
  return vr_api('enumerateLists', $params);
}
