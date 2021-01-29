<?php
/**
 * @file
 * Module page callbacks and related functions.
 */

/**
 * Page callback: Retrieve a JSON object containing autocomplete suggestions for user's connections.
 *
 * Path: ajax-metrocatch-connect-autocomplete
 */
function metrocatch_connect_autocomplete($string = '') {
  global $user;
  $matches = array();
  if ($string) {
    $active_types = metrocatch_connect_get_active_connection_types();
    $args = $active_types;
    $args[] = $string;
    $args[] = $user->uid;
    $args[] = $user->uid;
    $result = db_query_range(
      "SELECT u.uid, cck.field_profile_first_name_value AS name FROM {users} u
      INNER JOIN {node} n ON n.uid = u.uid AND n.type = 'profile'
      INNER JOIN {content_type_profile} cck ON cck.vid = n.vid
      INNER JOIN {metrocatch_connect_connection} mc ON mc.uid_to = u.uid OR mc.uid_from = u.uid
      WHERE mc.type IN (". db_placeholders($active_types, 'varchar') .") AND LOWER(cck.field_profile_first_name_value) LIKE LOWER('%s%%') AND ((mc.uid_to = u.uid AND mc.uid_from = %d) OR (mc.uid_from = u.uid AND mc.uid_to = %d))", $args, 0, 10);
    while ($account = db_fetch_object($result)) {
      $matches[$account->name ." [ID: $account->uid]"] = theme('metrocatch_connect_autocomplete', $account);
    }
  }
  return drupal_json($matches);
}

/**
 * Page callback: displays "Become a metrocatch" page.
 *
 * Path: purchase/feature
 */
function metrocatch_connect_become_metrocatch() {
  $out = theme('metrocatch_connect_purchase_top_block', variable_get('metrocatch_connect_purchase_feature_text', ''));
  if (user_is_logged_in()) {
    $out .= drupal_get_form('metrocatch_connect_feature_purchase_form');
  }
  return $out;
}

/**
 * Page callback: displays a 'Thank you' page.
 *
 * Path: purchase/feature/complete
 *       purchase/connections/complete
 *
 * User is redirected on this page after purchasing a feature or connections.
 */
function metrocatch_connect_become_metrocatch_complete() {
  $output = '<p>'. t('Thank you for your purchase.') .'</p>';
  $output .= '<p>'. l(t('Return to the metrocatch homepage.'), '<front>') .'</p>';
  return $output;
}

/**
 * Page callback: displays change connection type form or error message if user has already changed this connection type.
 *
 * Path: connections/change_state/%metrocatch_connect_connection/%
 *
 * @param $cid
 *   Primary connection id.
 * @param $old_type
 *   New type of connection
 * @param $new_type
 *   New type of connection
 */
function metrocatch_connect_change_connection_type_page($cid, $old_type, $new_type) {
  return drupal_get_form('metrocatch_connect_change_connection_type_form', $cid, $old_type, $new_type);
}

/**
 * Form constructor for the metrocatch_connect_change_connection_type_form form.
 *
 * Returns a confirmation form.
 *
 * @param $form_state
 * @param $cid
 *   Primary connection id.
 * @param $old_type
 *   Old type of connection. Usually it is equal to $connection['type'], otherwise
 *   There will be validation error.
 * @param $new_type
 *   New type of connection
 *
 * @see metrocatch_connect_menu()
 */
function metrocatch_connect_change_connection_type_form(&$form_state, $cid, $old_type, $new_type) {
  global $user;
  $form = array();
  $connection = metrocatch_connect_connection_load($cid);
  if (isset($_GET['popup_forms_num'])) {
    // form is being displayed in popup window. Handle Cancel link:
    drupal_add_js(drupal_get_path('module', 'metrocatch_connect') .'/metrocatch_connect.js');
  }
  if ($old_type != $connection['type']) {
    drupal_set_message(t('Sorry, this link has expired.'), 'error');
    return array(
      'back' => array(
        '#value' => '<div class="container-inline">'. l(t('Go to metromail'), 'user/'. $user->uid .'/metromail') .'</div>',
      ),
    );
  }
  $state_transition = metrocatch_connect_get_connection_transition($old_type, $new_type);
  $form = array(
    'connection' => array(
      '#type' => 'value',
      '#value' => $connection,
    ),
    'new_type' => array(
      '#type' => 'value',
      '#value' => $new_type,
    ),
    'transition' => array(
      '#type' => 'value',
      '#value' => $state_transition,
    ),
    '#redirect' => (isset($_GET['destination']) ? $_GET['destination'] : 'user/' . $user->uid . '/metromail'),
  );
  return confirm_form($form, t('Please confirm your action'), $form['#redirect'], $state_transition['question'], $state_transition['yes'], $state_transition['no'], $old_type .'_'. $new_type);
}

/**
 * Form validation handler for the metrocatch_connect_change_connection_type_form form.
 *
 * @see metrocatch_connect_change_connection_type_form()
 */
function metrocatch_connect_change_connection_type_form_validate($form, &$form_state) {
  $connection = $form_state['values']['connection'];
  $new_type = $form_state['values']['new_type'];
  // Check if user has permissions to make this action:
  if (!metrocatch_connect_connection_transition_access($connection, $new_type)) {
    $transition = $form_state['values']['transition'];
    form_set_error('connection', t('Sorry, you have no permissions to make this action. Please contact site administrator.'));
  }
}

/**
 * Form submission handler for the metrocatch_connect_change_connection_type_form form.
 *
 * @see metrocatch_connect_change_connection_type_form_validate()
 */
function metrocatch_connect_change_connection_type_form_submit($form, &$form_state) {
  global $user;
  $connection = $form_state['values']['connection'];
  $connection['requester'] = $user->uid;
  $connection['type'] = $form_state['values']['new_type'];
  $connection = metrocatch_connect_save_connection($connection);
  $transition = $form_state['values']['transition'];
  drupal_set_message($transition['message']);
  if (isset($_GET['popup_forms_num'])) {
    // If form is being displayed in popup window, we need to store updated themed
    // connection in $form_state in order to update it in JS callback.
    $requestee = user_load($connection['data']['requestee']);
    $form_state['result'] = array(
      'messages' => theme('status_messages'),
    );
    if ($connection['type'] == 'cancelled' || $connection['type'] == 'denied') {
      $form_state['result']['themed_connection'] = '';
    }
    else {
      $form_state['result']['themed_connection'] = theme('metrocatch_connect_connection', $requestee, $connection);
    }
  }
}

/**
 * Page callback: displays user connections page.
 *
 * Path: user/%user/connections
 *
 * Here user can overview his connections, change their types, approve or deny connection requests.
 *
 * @param $account
 *   StdClass object representing user whose connections are displayed.
 */
function metrocatch_connect_connections_page($account) {
  $output = metrocatch_connect_view_profile('teaser', $account, array('show_more_link' => TRUE));
  $connections = metrocatch_connect_load_user_connections($account->uid);
  $requests = metrocatch_connect_load_user_connections($account->uid, array('pending'));
  foreach ($requests as $id => $request) {
    if ($request['uid_to'] != $account->uid) {
      unset($requests[$id]);
    }
  }
  $connections = array_merge($connections, $requests);
  $output .= theme('metrocatch_connect_connections_view', $connections);
  return $output;
}

/**
 * Page callback: displays connect page.
 *
 * Path: connect/new/%user
 *
 * If user is not logged in, displaylogin form.
 * If user tries to connect with user he already has pending connection to,
 * display an appropriate message.
 * If user can't connect with this user, return 403 error.
 * Otherwise display new conenction form.
 *
 * Please notice that this page will only be displayed for non-JS users and email referals.
 * Normally login/connect forms are displayed via popup_forms module, and message about
 * existing pending connection is displayed in jQuery UI Dialog.
 *
 * @param $to
 *   StdClass object representing user with whom current user initiates the connection.
 */
function metrocatch_connect_connect_page($to) {
  global $user;
  if (!user_is_logged_in()) {
    // If user is not logged in we can't figure out if he can connection to this user.
    $ret = drupal_get_form('user_login', $to->uid);
  }
  elseif ($pending_connection = metrocatch_connect_load_users_connection($user->uid, $to->uid, 'pending')) {
    $ret = theme('metrocatch_connect_pending_connection_exists', $user, $to, $pending_connection);
  }
  elseif (!metrocatch_connect_connect_access($user, $to)) {
    $ret = MENU_ACCESS_DENIED;
  }
  else {
    $ret = drupal_get_form('metrocatch_connect_connect_form', $user->uid, $to->uid);
  }
  return $ret;
}

/**
 * Page callback: displays send message form
 *
 * Path: user/sendmessage
 *
 * @param stdClass  $recipient
 *   stdClass object representing message recipient.
 *
 * @see metrocatch_connect_send_message_form()
 */
function metrocatch_connect_sendmessage_page($recipient) {
  drupal_set_title(t('Send message to @username', array('@username' => $recipient->name)));
  drupal_add_css(drupal_get_path('module', 'metrocatch_connect') .'/metrocatch_connect.css');
  return drupal_get_form('metrocatch_connect_send_message_form', $recipient);
}

/**
 * Form constructor for sending messages.
 *
 * @param stdClass  $recipient
 *   stdClass object representing message recipient.

 * @see metrocatch_connect_send_message_form_validate()
 * @see metrocatch_connect_send_message_form_submit()
 */
function metrocatch_connect_send_message_form(&$form_state, $recipient) {
  global $user;
  return array(
    'recipient' => array(
      '#type' => 'value',
      '#value' => $recipient,
    ),
    'body' => array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#required' => TRUE,
    ),
    'type' => array(
      '#type' => 'value',
      '#value' => METROMAIL_MESSAGE_DEFAULT,
    ),
    'from' => array(
      '#type' => 'value',
      '#value' => $user,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Send message'),
    ),
  );
}

/**
 * Form submission handler for metrocatch_connect_send_message_form form.
 *
 * @see metrocatch_connect_send_message_form()
 */
function metrocatch_connect_send_message_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $recipient = $values['recipient'];
  $message = array(
    'type' => $values['type'],
    'body' => $values['body'],
    'uid_from' => $values['from']->uid,
    'uid_to' => $recipient->uid,
  );
  metromail_send_message($message);
  drupal_set_message(t('Your message to !username has been sent successfully.', array('!username' => l($recipient->name, 'user/' . $recipient->uid))));
  $form_state['redirect'] = isset($_GET['destination']) ? $_GET['destination'] : 'metromail';
}

/**
 * Form "become a metrocatch" callback.
 */
function metrocatch_connect_feature_purchase_form(&$form_state) {
  if (!isset($form_state['storage']['step'])) {
    $form_state['storage']['step'] = 'fill_form';
  }
  $form_state['redirect'] = 'purchase/feature/complete';
  switch ($form_state['storage']['step']) {
    case 'fill_form';
    if (isset($form_state['storage']['old_values'])) {
      $form_state['values'] = $form_state['storage']['old_values'];
      unset($form_state['storage']['old_values']);
    }
    $feature_price = variable_get('metrocatch_connect_feature_price', 9.99);
    $subfeature_price = variable_get('metrocatch_connect_subfeature_price', 5.99);
    $form = array(
      'feature_type' => array(
        '#prefix' => '<div id="feature-type-radios">',
        '#suffix' => '</div>',
        '#type' => 'radios',
        '#title' => '',
        '#options' => array(METROCATCH_CATCH_TYPE_FEATURE => t('metrocatch of the Day Feature: !price', array('!price' => metrocatch_uc_format_price($feature_price))), METROCATCH_CATCH_TYPE_SUBFEATURE => t('metrocatch of the Day Subfeature: !price', array('!price' => metrocatch_uc_format_price($subfeature_price)))),
        '#default_value' => isset($form_state['values']['feature_type']) ? $form_state['values']['feature_type'] : METROCATCH_CATCH_TYPE_FEATURE,
      ),
      'catch_date' => array(
        '#type' => 'hidden',
        '#default_value' => isset($form_state['values']['catch_date']) ? $form_state['values']['catch_date'] : '',
      ),
      'three_months_plan' => array(
        '#type' => 'checkbox',
        '#title' => t('Make it a 3 Month Plan'),
        '#default_value' => isset($form_state['values']['three_months_plan']) ? $form_state['values']['three_months_plan'] : 0,
      ),
    );
    break;
    case 'review':
      $form = array(
        '#redirect' => 'purchase/feature/complete',
      );
      break;
    default:
      $form = array();
  }
  // add "purchase form"
  $form =  array_merge(
    $form,
    metrocatch_uc_purchase_form(
      $form_state,
      array(
        'product_callback' => 'metrocatch_connect_features_product_callback',
        'promo_codes' => TRUE,
        'file' => drupal_get_path('module', 'metrocatch_connect') .'/metrocatch_connect.pages.inc',
      )
    )
  );
  $form['#attributes']['class'] = 'metrocatch-purchase-form';
  // set weights
  $weight = 1.0;
  foreach (element_children($form) as $key) {
    $form[$key]['#weight'] = $weight++;
  }
  if (!is_array($form['#after_build'])) {
    $form['#after_build'] = array();
  }
  if ($form_state['storage']['step'] == 'fill_form') {
    // add another validate handler
    array_unshift($form['submit']['#validate'], 'metrocatch_connect_feature_purchase_form_validate');
    array_unshift($form['submit']['#submit'], 'metrocatch_connect_feature_purchase_form_submit');
    if (isset($form_state['values']['feature_question'])) {
      $question_default_value = $form_state['values']['feature_question'];
    }
    else {
      global $user;
      $profile = metrocatch_profile_get_profile($user->uid);
      $question_default_value = $profile->field_profile_feature_question[0]['value'];
    }
    $form['feature_question'] = array(
      '#type' => 'textarea',
      '#title' => t('What should your potential dates know about you?'),
      '#default_value' => $question_default_value,
      '#weight' => $form['promo_code']['#weight'] + 0.1,
    );
  }
  $form['#after_build'][] = 'metrocatch_connect_feature_purchase_form_after_build';
  return $form;
}

/**
 * Additional validate handler for the "purchase" form.
 *
 * Validates feature type, day and other conditions.
 */
function metrocatch_connect_feature_purchase_form_validate(&$form, &$form_state) {
  if ($form_state['storage']['step'] != 'fill_form') {
    return;
  }
  global $user;
  // validate selected date values
  $feature_type = $form_state['values']['feature_type'];
  // 1. check date format
  $date_parts = $form_state['values']['catch_date'];
  if (!$date_parts) {
    form_error($form['catch_date'], t('You must selected a metrocatch day.'));
    return;
  }
  $date_parts = explode('_', $date_parts);
  if (!checkdate($date_parts[1], $date_parts[0], $date_parts[2])) {
    form_error($form['catch_date'], t('Invalid catch date selected.'));
    return;
  }
  // 2. check date range
  $today = strtotime(date('Y-m-d'));
  if ($feature_type == METROCATCH_CATCH_TYPE_FEATURE) {
    $min_date = $today + 86400 * 7; // 7 days
  }
  else {
    // $feature_type == 'subfeature'
    $min_date = $today + 86400 * 1; // 1 day
  }
  $max_date = strtotime(date("Y-m-d", $today) ." +3 months");
  $checked_date = strtotime($date_parts[2] .'-'. $date_parts[1] .'-'. $date_parts[0]);
  if ($checked_date < $min_date || $checked_date > $max_date) {
    form_error($form['catch_date'], t('Invalid catch date selected.'));
    return;
  }
  // check one day, and maybe two "plan" days
  $dates_to_check = array($form_state['values']['catch_date']);
  if ($form_state['values']['three_months_plan']) {
    $dates_to_check[] = date('d_m_Y', strtotime(date("Y-m-d", $checked_date) ." +1 month"));
    $dates_to_check[] = date('d_m_Y', strtotime(date("Y-m-d", $checked_date) ." +2 months"));
  }
  // compare the new day values with the original, if they not match, that next month has no original day
  $original_day = $date_parts[0];
  $busy_days = metrocatch_connect_get_busy_days($user->uid, $feature_type);
  foreach ($dates_to_check as $date) {
    if (isset($busy_days[$date]) || (int)$date != $original_day) {
      // force conversion to int required for selecting day from d_m_Y string.
      // we can also use explode('_', $date), but conversion is shorter
      form_error($form['catch_date'], t("You can't select this day."));
      return;
    }
  }
}

/**
 * Additional submit handler for the "purchase" form.
 *
 * Save "Feature Question" to user's profile
 */
function metrocatch_connect_feature_purchase_form_submit($form, $form_state) {
  // save "Feature Question" field
  global $user;
  $profile = metrocatch_profile_get_profile($user->uid);
  if (!$profile) {
    // :-(
    return;
  }
  $profile->field_profile_feature_question[0]['value'] = $form_state['values']['feature_question'];
  node_save($profile);
}

/**
 * Form construction handler for the metrocatch_connect_connect_form form.
 *
 * Using this form user can connect to another one and puchase connections if
 * his free connections have run out.
 *
 * @param int $from
 *   uid from
 * @param int $to
 *   uid to
 */
function metrocatch_connect_connect_form(&$form_state, $uid_from, $uid_to) {
  drupal_set_title(t('Connect'));
  $form = array(
    'uid_from' => array(
      '#type' => 'value',
      '#value' => $uid_from,
    ),
    'uid_to' => array(
      '#type' => 'value',
      '#value' => $uid_to,
    ),
  );
  $settings = array(
    'metrocatch_connect' => array(
      'uid_from' => $uid_from,
      'uid_to' => $uid_to,
    ),
  );
  $form['metrocatch_connect_connection'] = array('#type' => 'value', '#value' => TRUE);
  drupal_add_js($settings, 'setting');
  drupal_add_js(drupal_get_path('module', 'metrocatch_connect') .'/metrocatch_connect.js');
  // add some display-only form elements on all steps, except 'review'
  if ($form_state['storage']['step'] != 'review') {
    $profile = metrocatch_profile_get_profile($uid_from);
    $form += array(
      'connections_left' => array(
        '#prefix' => '<div class="metrocatch-connections-left">',
        '#suffix' => '</div>',
        '#title' => t('Connections left'). ':',
        '#type' => 'item',
        '#value' => metrocatch_connect_get_connections_number_text($uid_from),
        '#access' => FALSE, // Temporary hide the 'Connections left' text.
      ),
      'description' => array(
        '#type' => 'item',
        '#value' =>
          format_plural(variable_get('metrocatch_connect_days_to_approve_connection', 3),
            'Are you sure you’d like to initiate a connection? If this person does not verify your request, it will automatically expire in @count day.',
            'Are you sure you’d like to initiate a connection? If this person does not verify your request, it will automatically expire in @count days.'
          ),
        '#weight' => -10,
      ),
      'message' => array(),
      'logged_info' => array(
        '#prefix' => '<div class="metrocatch-logged-in">',
        '#suffix' => '</div>',
        '#type' => 'item',
        '#value' => t(
          "You are logged in as @firstname @lastname.\n Not @firstname? !signout_link.",
          array(
            '@firstname' => $profile->field_profile_first_name[0]['value'],
            '@lastname' => $profile->field_profile_last_name[0]['value'],
            '!signout_link' => l(t('sign out'), 'logout', array('attributes' => array('target' => '_parent'))),
          )
        ),
      )
    );
    if (!isset($form_state['storage']['step'])) {
      $form['message'] = array(
        '#type' => 'textarea',
        '#title' => t('Your message'),
        '#default_value' => isset($form_state['values']['message']) ? $form_state['values']['message'] : '',
        '#required' => TRUE,
        '#rows' => 4,
        '#resizable' => FALSE,
      );
    }
  }
  // add 'connect' button on the first form step
  if (!isset($form_state['storage']['step']) && !isset($form_state['ahah_submission'])) {
    $form += array(
      'connect' => array(
        '#type' => 'submit',
        '#value' => t('Connect'),
        '#name' => 'metrocatch-connect-connect-button',
      ),
    );
  }
  else { // add puchase form
    if (isset($_GET['redirect'])) {
      $form_state['redirect'] = $_GET['redirect'];
    }
    $form += metrocatch_connect_purchase_connections_form($form_state);
    $form['#attributes']['class'] = 'metrocatch-purchase-form';
    if (isset($_GET['popup_forms_num']) && $form_state['storage']['step'] == 'review') {
      // If user submits Review form in popup window
      // we should save result message in $form_state
      // so that we can show this message in JS callback.
      $form['submit']['#submit'][] = 'metrocatch_connect_connect_form_after_submit';
    }
  }
  if (isset($form['connections_number'])) {
    $form['connections_number']['#weight'] = -1;
  }
  return $form;
}

/**
 * Validate handler for "connect" form - go to the next form step
 */
function metrocatch_connect_connect_form_validate(&$form, &$form_state) {
  if (!isset($form_state['storage']['step']) || $form_state['storage']['step'] != 'fill_form') {
    $from = user_load($form_state['values']['uid_from']);
    $to = user_load($form_state['values']['uid_to']);
    if (!metrocatch_connect_connect_access($from, $to)) {
      form_set_error('error', t('Unknown error occurred. Please contact site administrator.'));
    }
    if ($pending_connection = metrocatch_connect_load_users_connection($from->uid, $to->uid, 'pending')) {
      form_set_error('error', t('You already have connection with this user.'));
    }
    if (strlen($form_state['values']['message']) > 500) {
      form_error($form['message'], t('Message should not be longer than 500 characters.'));
    }
    else {
      $form_state['storage']['message'] = $form_state['values']['message'];
    }
  }
}

/**
 * Submit handler for "connect" form - go to the next form step
 */
function metrocatch_connect_connect_form_submit(&$form, &$form_state) {
  global $user;
  $connections_number = metrocatch_connect_get_connections_number($form_state['values']['uid_from']);
  // Temporary allow everyone to connect (even if one has no connections left).
  if (TRUE || $connections_number > 0 || (metrocatch_connect_user_is_feature_affected($form_state['values']['uid_from']))) {
    // 1. create connect
    $connection = array(
      'uid_from' => $form_state['values']['uid_from'],
      'uid_to' => $form_state['values']['uid_to'],
      'data' => array('message' => $form_state['storage']['message']),
    );
    $connection = metrocatch_connect_save_connection($connection);
    // "from"-user is featured and do not required to charge any money
    // 2. interrupt purchase process
    unset($form_state['storage']);
    $account_to = user_load($connection['uid_to']);
    drupal_set_message(t('You have sent a connection request to @username. Please wait for approval.', array('@username' => $account_to->name)));
    // Form is being displayed in popup window, set arguments for JS callback:
    $form_state['result'] = array(
      'messages' => theme('status_messages'),
      'update' => metrocatch_connect_get_new_connection_link($user, $account_to),
      'uid_to' => $connection['uid_to'],
    );
    $form_state['redirect'] = 'user/'. $account_to->uid;
  }
  else {
    $form_state['storage']['step'] = 'fill_form';
  }
}

/**
 * Additional submit handler for the metrocatch_connect_connect_form form.
 *
 * Triggers only when user clicks 'Review' button in popup window after he has puchased a connection.
 * Here we save result message in $form_state so that we can show it in JS callback.
 */
function metrocatch_connect_connect_form_after_submit($form, &$form_state) {
  global $user;
  $account_to = user_load($form_state['values']['uid_to']);
  // Form is being displayed in popup window, set arguments for JS callback:
  $form_state['result'] = array(
    'messages' => theme('status_messages'),
    'update' => metrocatch_connect_get_new_connection_link($user, $account_to),
    'uid_to' => $account_to->uid,
  );
}

/**
 * Page callback: displays the Today's Catch page.
 *
 * Path: catches
 *
 * Get and display random Catch of the Day, Become metrocatch adv and
 * block with Other Catches.
 */
function metrocatch_connect_random_catch_page() {
  global $user;
  drupal_set_title(t('Today\'s Catch'));
  $today_catches = metrocatch_connect_load_features()->types('feature')
    ->todays()
    ->fetchAll();
  if (!empty($user->uid)) {
    // Display catch based on user sex and preference.
    $profile = metrocatch_profile_get_profile($user->uid);
    $sex = ($profile->field_profile_sex_preference['0']['value'] == 'men' ? 'man' : 'woman');
    $preference = ($profile->field_profile_sex[0]['value'] == 'man' ? 'men' : 'women');
    $catch = metrocatch_connect_load_features()->types('feature')
      ->todays()
      ->segment($sex, $preference)
      ->limit(1)
      ->fetchSingle();
  }
  // For anonymous users or if there is no today's catch in an appropriate category
  // display random catch.
  if (empty($user->uid) || empty($catch)) {
    // Display random catch for anonymous user.
    $rnd = array_rand($today_catches);
    $catch = $today_catches[$rnd];
  }
  $other_catches = metrocatch_connect_load_features()->types('subfeature')
    ->active()
    ->limit(12)
    ->shuffle()
    ->fetchAll();
  return theme('metrocatch_connect_random_catch_page', $catch, $today_catches, $other_catches);
}

/**
 * Page callback: displays the metrocatch of the Day Feature Landing Page.
 *
 * Path: catches/catch/%user
 *
 * @param $account
 *   StdClass object of a featured user.
 */
function metrocatch_connect_catch_page($account) {
  $catch = metrocatch_connect_load_features()->types('feature', 'subfeature')
    ->active()
    ->uid($account->uid)
    ->limit(1)
    ->fetchSingle();
  if (!$catch) {
    drupal_goto('<front>');
  }
  return metrocatch_connect_view_profile('featured', $catch->account, array('catch' => $catch));
}

/**
 * Page callback: Displays catches in a given segment.
 *
 * Path: catches/subfeature/%
 *
 * @param $sex
 *   A string representing sex of a given segment.
 *   E.g. 'man' or 'woman'.
 * @param $preference
 *   A string representing sex preference of a given segment.
 *   E.g. 'men' or 'women'.
 * @param $uid
 *   (optional) The UID of the featured user.
 */
function metrocatch_connect_catch_subfeature_page($sex, $preference, $uid = NULL, $archive = FALSE) {
  $archive = (!empty($archive)); // boolean indicator.
  $limit = $archive ? -1 : 12;
  if (isset($_GET['connect']) && is_numeric($_GET['connect'])) {
    $uid = $_GET['connect'];
  }
  // Show the Catch with a given UID.
  if (!empty($uid)) {
    $catch = metrocatch_connect_load_features()->types('feature', 'subfeature')
      ->active()
      ->uid($uid)
      ->segment($sex, $preference)
      ->fetchSingle();
    // If could not load Feature for the specified user.
    if (!$catch) {
      if (!$account = user_load($uid)) {
        // User does not exist.
        return MENU_NOT_FOUND;
      }
      else {
        // @TODO: User is not featured anymore - show message?
      }
    }
  }
  // If no UID is given or user with a given UID is not featured anymore
  // then get the latest Feature.
  if (empty($uid) || !$catch) {
    $catch = metrocatch_connect_load_features()->types('feature')
      ->active()
      ->segment($sex, $preference)
      ->limit(1)
      ->fetchSingle();
    if (!$catch) {
      // If there is no Featured Catch in this category, get the latest SubFeatured catch.
      $catch = metrocatch_connect_load_features()->types('subfeature')
        ->active()
        ->segment($sex, $preference)
        ->limit(1)
        ->fetchSingle();
    }
  }
  $catches = metrocatch_connect_load_features()->types('feature', 'subfeature')
    ->active()
    ->segment($sex, $preference)
    ->limit($limit)
    ->shuffle()
    ->fetchAll();
  $close_archive_link = $archive;
  if (!$archive) {
    $count_displayed = count($catches);
    $count_all = metrocatch_connect_load_features()->types('feature', 'subfeature')
      ->active()
      ->segment($sex, $preference)
      ->fetchCount();
    $archive = ($count_displayed >= $count_all);
  }
  return theme('metrocatch_connect_catch_subfeature_page', $catch, $catches, $sex, $preference, $archive, $close_archive_link);
}

/**
 * Page callback: Retrieve a JSON object containing themed Feature slot and Other catches block.
 *
 * Path: ajax-metrocatch-connect-rotate-feature-slot
 */
function metrocatch_connect_rotate_featured_slot_page() {
  if ($catch = metrocatch_connect_get_user_catch($_GET['uid'])) {
    $type = $_GET['type'];
    if (!in_array($type, array('featured_teaser', 'subfeatured'))) {
      return MENU_NOT_FOUND;
    }
    $featured_slot = '<div id="feature-slot-'. $catch->uid .'" class="feature-slot">'. metrocatch_connect_view_profile($type, $catch->account, array('catch' => $catch)) .'</div>';
    // When user clicks on some catch in the Other Catches block,
    // we move an old Feature slot in the clicked feature place.
    $javascript = drupal_add_js(NULL, 'header');
    $output = array(
      'status' => TRUE,
      'featured_slot' => $featured_slot,
      'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
    );
    return drupal_json($output);
  }
  return MENU_NOT_FOUND;
}


/**
 * Page callback: displays "Purchase Connects" page.
 *
 * Path: purchase/conects
 */
function metrocatch_connect_purchase_connections() {
  $out = theme('metrocatch_connect_purchase_top_block', variable_get('metrocatch_connect_purchase_connect_text', ''));
  if (user_is_logged_in()) {
    $out .= drupal_get_form('metrocatch_connect_purchase_connections_form');
  }
  return $out;
}

/**
 * "Purchase Connections" form callback, displayed on 'purchase/connections' path.
 * Also used as popup form.
 */
function metrocatch_connect_purchase_connections_form(&$form_state) {
  $form = array(
    '#attributes' => array('class' => 'metrocatch-purchase-form'),
    '#redirect' => 'purchase/connections/complete',
  );
  if (!isset($form_state['storage']['step']) || $form_state['storage']['step'] == 'fill_form') {
    // build form top
    $form += array(
      'connections_number' => array(
        '#type' => 'radios',
        '#required' => TRUE,
        '#options' => array(
          1 => t('1 Connection: @price', array('@price' => '$0.99')),
          5 => t('@num Connections: @price', array('@num' => 5, '@price' => '$4.00')),
          10 => t('@num Connections: @price', array('@num' => 10, '@price' => '$7.00')),
        ),
        '#default_value' => isset($form_state['values']['connections_number']) ? $form_state['values']['connections_number'] : 1,
        '#prefix' => '<div class="purchase-connections-number">',
        '#suffix' => '</div>',
      ),
    );
  }
  //  add purchase form
  $form += metrocatch_uc_purchase_form(
    $form_state,
    array(
      'promo_codes' => TRUE,
      'file' => drupal_get_path('module', 'metrocatch_connect') .'/metrocatch_connect.pages.inc',
      'product_callback' => 'metrocatch_connect_connect_product_callback',
    )
  );
  return $form;
}

/**
 * Product callback for "purchase connect" form
 *
 * @see metrocatch_connect_features_product_callback()
 */
function metrocatch_connect_connect_product_callback($op, $arg1 = NULL, $arg2 = NULL) {
  switch ($op) {
    case 'purchased':
      // products purchased, add connections to user
      $product = $arg1->products[0];
      // add connections to user
      metrocatch_connect_change_connections_number($arg1->uid, $product->qty);
      // create new connection, if it is required
      if (isset($product->data['create_connection'])) {
        $connection = array('uid_from' => $product->data['create_connection']['from'], 'uid_to' => $product->data['create_connection']['to']);
        metrocatch_connect_save_connection($connection);
      }
      break;
    case 'products':
      $values = $arg2['values'];
      $connect = new stdClass();
      $connect->nid = variable_get('metrocatch_connect_connect_product_nid', 0);
      $node = node_load($connect->nid);
      switch ($values['connections_number']) {
        case 10:
          $connect->price = 7/10;
          $connect->qty = 10;
          break;
        case 5:
          $connect->price = 4/5;
          $connect->qty = 5;
          break;
        default:
          $connect->price = 0.99;
          $connect->qty = 1;
          break;
      }
      if (isset($arg2['values']['metrocatch_connect_connection'])) {
        $connect->data = array(
          'create_connection' => array(
            'from' => $arg2['values']['uid_from'],
            'to' => $arg2['values']['uid_to'],
          ),
        );
      }
      return array($connect);
      break;
  }
}

/**
 * Product callback for "purchase" for of the metrocatch_uc module
 *
 * @param string $op
 *   operation - 'products', 'validate' or 'purchased'.
 * @param array $arg1
 *   entire purchase form or order (depends on $op).
 * @param array $arg2
 *   form state or NULL.
 * @return
 *   depends on $op:
 *     - 'validate': TRUE or FALSE, function can set errors by form_error() function.
 *     - 'purchased': none.
 *     - 'products': array of products to purchasem array with the follwing keys:
 *       - nid: nid of the product node
 *       - qty: - quantity
 *       - price: price
 *       - data: oprional array with custom data
 */
function metrocatch_connect_features_product_callback($op, $arg1 = NULL, $arg2 = NULL) {
  switch ($op) {
    case 'purchased':
      // products purchased, create catch records
      $product = $arg1->products[0];
      if (!$product) {
        return;
      }
      $profile = metrocatch_profile_get_profile($arg1->uid);
      $data = array(
        'sex' => $profile->field_profile_sex[0]['value'],
        'sex_preference' => $profile->field_profile_sex_preference[0]['value'],
        'type' => $product->data['feature_type'],
        'uid' => $arg1->uid,
      );
      $catch_date = str_replace('_', '-', $product->data['catch_date']);
      $dates_to_create = array($catch_date);
      if ($product->data['three_monts_plan']) {
        $dates_to_create[] = date('d-m-Y', strtotime($catch_date .' +1 month'));
        $dates_to_create[] = date('d-m-Y', strtotime($catch_date .' +2 months'));
      }
      foreach ($dates_to_create as $date) {
        $data['date'] = $date;
        if ($error = metrocatch_connect_create_catch_feature($data)) {
          watchdog(
            'metrocatch_connect',
            'Error during creating catch of type @type for user @uid on date @date (error code: @code))',
            array('@type' => $product->data['feature_type'], '@uid' => $arg1->uid, '@date' => $date, '@code' => $error)
          );
        }
      }
      break;
    case 'products':
      $form_state = $arg2;
      $feature = new stdClass();
      $feature_type = $form_state['values']['feature_type'];
      if ($feature_type == METROCATCH_CATCH_TYPE_FEATURE) {
        $feature->nid = variable_get('metrocatch_connect_feature_product_nid', 0);
      }
      else {
        // subfeature
        $feature->nid = variable_get('metrocatch_connect_subfeature_product_nid', 0);
      }
      $node = node_load($feature->nid);
      $feature->price = $node->sell_price;
      if ($form_state['values']['three_months_plan']) {
        $feature->qty = 3;
        if ($form_state['values']['feature_type'] == METROCATCH_CATCH_TYPE_FEATURE) {
          $feature->price *= 0.75;
        }
        else {
          $feature->price *= 0.85;
        }
      }
      else {
        $feature->qty = 1;
      }
      $feature->data = array(
        'catch_date' => $form_state['values']['catch_date'],
        'three_monts_plan' => $form_state['values']['three_months_plan'],
        'feature_type' => $feature_type,
      );
      return array($feature);
      break;
  }
}

/**
 * Pre-render function for purchase form
 * Adds some js for datepicker (files and settings)
 */
function metrocatch_connect_feature_purchase_form_after_build($form, $form_state) {
  static $added = FALSE;
  if ($added) {
    return $form;
  }
  $added = TRUE;
  jquery_ui_add('ui.datepicker', 'lightness');
  drupal_add_js(drupal_get_path('module', 'metrocatch_connect') .'/metrocatch_connect.js');
  global $user;
  // add required js settings
  drupal_add_js(
    array(
      'metrocatch' => array(
        'feature' => array(
          'daterange_min' => '+7D',
          'daterange_max' => '+3M',
          'busydays' => metrocatch_connect_get_busy_days($user->uid, METROCATCH_CATCH_TYPE_FEATURE),
        ),
        'subfeature' => array(
          'daterange_min' => '+1D',
          'daterange_max' => '+3M',
          'busydays' => metrocatch_connect_get_busy_days($user->uid, METROCATCH_CATCH_TYPE_SUBFEATURE)
        ),
      ),
    ),
    'setting'
  );
  return $form;
}

/**
 * Get days, that are filled up by catches
 *
 * @param int $uid
 *   user id
 * @param string $type
 *   feature or subfeature
 * @return array
 *   array of days (string representation, where key id date and value = TRUE)
 */
function metrocatch_connect_get_busy_days($uid, $type, $format = 'j_n_Y') {
  $node = metrocatch_profile_get_profile($uid);
  if ($type == METROCATCH_CATCH_TYPE_FEATURE) {
    // feature
    $max_records = 1;
  }
  else {
    // subfeature
    $max_records = 3;
  }
  $busy_dates = array();
  // select busy dates
  $now = time();
  $rows = db_query(
    "SELECT tmp.date FROM (
      SELECT COUNT(fid) AS feature_count, date AS date
      FROM {metrocatch_connect_catches}
      WHERE type='%s'
        AND sex='%s'
        AND sex_preference='%s'
        AND date BETWEEN '%s' AND '%s'
      GROUP BY date
    ) tmp
    WHERE tmp.feature_count >= %d",
    $type,
    $node->field_profile_sex[0]['value'],
    $node->field_profile_sex_preference[0]['value'],
    date('Y-m-d', $now + 86400*1), // 1 day
    date('Y-m-d', $now + 86400*90), // 90 days
    $max_records
  );
  while ($date = db_result($rows)) {
    $busy_dates[$date] = TRUE;
  }
  // select dates, already planned by this user
  $rows = db_query(
    "SELECT date AS date
    FROM {metrocatch_connect_catches}
    WHERE uid=%d
      AND date BETWEEN '%s' AND '%s'",
    $uid,
    date('Y-m-d', $now + 86400 * 1), // 1 day
    date('Y-m-d', $now + 86400 * 186) // 6 months
  );
  while ($date = db_result($rows)) {
    $busy_dates[$date] = TRUE;
  }
  $dates = array();
  foreach ($busy_dates as $date => $true) {
    $dates[date($format, strtotime($date))] = TRUE;
  }
  return $dates;
}

/**
 * Form construction handler for the 'Email share' form.
 *
 * Path: metrocatch-connect/share/email
 *
 * Please notice that this page will only be displayed for non-JS users and email referals.
 * In another case share this form will be displayed via popup_forms module.
 */
function metrocatch_connect_share_email_form(&$form_state, $url = NULL) {
  global $user;
  if (empty($url) && !empty($_GET['url'])) {
    $url = $_GET['url'];
  }
  $url = urldecode($url);
  drupal_set_title(t('Send email'));
  return array(
    'url' => array(
      '#type' => 'value',
      '#value' => $url,
    ),
    'from' => array(
      '#type' => 'textfield',
      '#title' => t('From'),
      '#required' => TRUE,
      '#description' => t('Your email address'),
      '#default_value' => (!empty($user->uid) ? $user->mail : ''),
    ),
    'recipients' => array(
      '#type' => 'textarea',
      '#title' => t('To'),
      '#rows' => 3,
      '#required' => TRUE,
      '#description' => t('Comma separated list of emails'),
    ),
    'message' => array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#rows' => 6,
      '#required' => TRUE,
    ),
    'actions' => array(
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Share'),
      ),
    ),
  );
}

/**
 * Form validation handler for the 'Email share' form.
 *
 * Validates recipients list.
 */
function metrocatch_connect_share_email_form_validate($form, &$form_state) {
  $form_state['#recipients'] = array();
  foreach (explode(',', $form_state['values']['recipients']) as $recipient) {
    $recipient = trim($recipient);
    if (!valid_email_address($recipient)) {
      form_set_error('recipients', t('@email is not valid email.', array('@email' => $recipient)));
    }
    else {
      $form_state['#recipients'][] = $recipient;
    }
  }
  if (!valid_email_address($form_state['values']['from'])) {
    form_set_error('from', t('@email is not valid email.', array('@email' => $form_state['values']['from'])));
  }
}

/**
 * Form submission handler for the 'Email share' form.
 *
 * Sends emails to a given recipients lists.
 */
function metrocatch_connect_share_email_form_submit($form, &$form_state) {
  $metrocatch_share_creator = mailcreators_get_creator('metrocatch_connect_share_mail');
  $params = array(
    'recipients' => $form_state['#recipients'],
    'from' => $form_state['values']['from'],
    'message' => $form_state['values']['message'],
    'url' => $form_state['values']['url'],
  );
  if (!$metrocatch_share_creator->send($params)) {
    drupal_set_message(t('Unknown error occurred while sending email, please try again later or contact site administrator.'), 'error');
    watchdog('metrocatch_connect', 'Error occurred while sending share email: <pre>@params</pre>', array('@params' => print_r($params, TRUE)), WATCHDOG_WARNING);
  }
  else {
    drupal_set_message(t('Your email to @recipients has been successfully sent.', array('@recipients' => implode(', ', $form_state['#recipients']))));
  }
  // @TODO: redirect after submission.
  $form_state['result'] = array(
    'messages' => theme('status_messages'),
  );
}

/**
 * Page callback: triggers share event and redirects user to the Facebook share dialog.
 *
 * Path: metrocatch-connect/share/facebook/%user/%
 *
 * @param $account
 *   StdClass object of a user who is being shared.
 */
function metrocatch_connect_share_facebook_page($account) {
  global $user;
  metrocatch_connect_add_share('facebook', $user, $account);
  $share_url = url('catches/catch/'. $account->uid, array('absolute' => TRUE));
  $featured = metrocatch_connect_load_features()
    ->type('feature')
    ->active()
    ->uid($account->uid)
    ->limit(1)
    ->fetchSingle();
  // For the featured users diaply marketing text,
  // for the subfeatueed display site mission.
  $summary = ($featured ? metrocatch_connect_get_marketing_text($account) : variable_get('metrocatch_connect_facebook_share_text', ''));
  $query = array(
    's' => 100,
    'p' => array(
      'title' => check_plain($account->name) .'|'. variable_get('site_name', 'metrocatch'),
      'summary' => filter_xss($summary, array()),
      'url' => url($share_url, array('absolute' => TRUE)),
      'images' => array(
        imagecache_create_url('facebook_share', $account->picture),
      ),
    ),
  );
  if ($logo = theme_get_settings('logo_path')) {
    $query['p']['images'][] = imagecache_create_url('logo_path', $logo);
  }
  $protocol = (!empty($_SERVER['HTTPS']) ? 'https' : 'http');
  drupal_goto($protocol .'://www.facebook.com/sharer.php', $query);
}

/**
 * Page callback: triggers share event and redirects user to the Twitter share dialog.
 *
 * Path: metrocatch-connect/share/twitter/%user/%
 *
 * @param $account
 *   StdClass object of a user who is being shared.
 */
function metrocatch_connect_share_twitter_page($account) {
  global $user;
  metrocatch_connect_add_share('twitter', $user, $account);
  $share_url = url('catches/catch/'. $account->uid, array('absolute' => TRUE));
  $query = array(
    'url' => $share_url,
    'count' => 'horizontal',
    'via' => t(variable_get('metrocatch_connect_tweet_via', variable_get('site_name', 'metrocatch'))),
    'text' => t(variable_get('metrocatch_connect_tweet_text', '')),
    'counturl' => $share_url,
  );
  $protocol = (!empty($_SERVER['HTTPS']) ? 'https' : 'http');
  drupal_goto($protocol .'://twitter.com/share', $query);
}
