<?php
/**
 * @file
 * Module menu callback and helper functions
 */

/**
 * Page callback for 'ajax-refresh-payment-method' path - ahah callback for "select payment" radios
 */
function metrocatch_uc_ajax_refresh_payment_method() {
  if (!isset($_POST['form_build_id'])) {
    return MENU_NOT_FOUND;
  }
  $form = metrocath_uc_ahah_helper();
  // render element and return it
  unset($form['payment']['#suffix']);
  unset($form['payment']['#prefix']);
  return drupal_json(array('status' => TRUE, 'data' => drupal_render($form['payment'])));
}

/**
 * Ahah helper, get rebuilt form ($form_id taken from $_POST)
 *
 * @return array
 *   rebuilt form
 */
function metrocath_uc_ahah_helper() {
  // standart operations during AHAH
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = check_plain($_POST['form_build_id']);
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form_state['ahah_submission'] = TRUE;
  _metrocatch_uc_disable_form_validation($form);
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  return drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
}

/**
 * Recursively disable validation on form, used in ahah callbacks
 *
 * @param array $element
 *   element to disable validation on
 */
function _metrocatch_uc_disable_form_validation(&$element) {
  // set to all elements attribute #validated = TRUE
  $element['#validated'] = TRUE;
  // if any element has attribute #required we set it to 0
  if (isset($element['#required'])) {
    $element['#required'] = 0;
  }
  foreach (element_children($element) as $key) {
    if (isset($element[$key]) && $element[$key]) {
      _metrocatch_uc_disable_form_validation($element[$key]);
    }
  }
}

/**
 * Page callback for 'user/%user/purchase-history' and 'purchase-history' paths - user purchases
 *
 * @param int $uid
 *   user id, which purchase history we need to display
 */
function metrocatch_uc_purchase_history($uid = NULL) {
  drupal_add_css(drupal_get_path('module', 'metrocatch_uc') .'/metrocatch_uc.css');
  drupal_set_title(t('Purchase history'));
  if (!$uid) {
    global $user;
    $uid = $user->uid;
  }
  // fetch user orders
  $ids = pager_query("SELECT order_id FROM {uc_orders} WHERE uid=%d AND order_status='completed'", variable_get('metrocatch_uc_records_per_page', 16), 0, NULL, $uid);
  $orders = array();
  while ($order_id = db_result($ids)) {
    $orders[$order_id] = uc_order_load($order_id);
  }
  return theme('metrocatch_uc_purchase_history', $orders, user_load($uid)) . theme('pager');
}

/**
 * Settings form callback (page callback for 'admin/metrocatch/metrocatch_uc')
 */
function metrocatch_uc_settings_form(&$form_state) {
  $form = array(
    'metrocatch_uc_records_per_page' => array(
      '#type' => 'textfield',
      '#title' => t('Number of records on "Purchase History" page'),
      '#default_value' => variable_get('metrocatch_uc_records_per_page', 16),
    ),
  );
  return system_settings_form($form);
}
