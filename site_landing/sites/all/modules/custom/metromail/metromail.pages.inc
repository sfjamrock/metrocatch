<?php
/**
 * @file
 * metromail.pages.inc
 *
 * MetroMail page callbacks.
 */

/**
 * Page callback: displays user messages.
 *
 * Path: user/%user/metromail
 *
 * @param $account
 *   The user object whose messages are displayed.
 *
 * @see metromail_overview_more()
 */
function metromail_overview($account) {
  global $base_url;
  $path_to_module = drupal_get_path('module', 'metromail');
  drupal_add_css($path_to_module . '/metromail.css');
  drupal_add_js($path_to_module . '/metromail.js');
  $settings = array(
    'metromail' => array('base_url' => $base_url . base_path())
  );
  if (isset($_GET['scroll_to']) && is_numeric($_GET['scroll_to'])) {
    $settings['metromail']['scroll_to'] = $_GET['scroll_to'];
  }
  drupal_add_js($settings, 'setting');
  drupal_set_title(t('MetroMail'));

  $flag = flag_create_link(METROMAIL_NOTIFY_FLAG, $account->uid);
  $limit = variable_get('metromail_messages_per_page', 10);
  $load_threads = (isset($_GET['expand'])) ? explode(',', $_GET['expand']) : array();
  $messages = metromail_load_user_messages($account, $limit, 0, $load_threads);
  $offset = count($messages);
  $themed_messages = theme('metromail_messages', $account, $messages);
  // Add "View more" link, if not all messages are already displayed.
  $view_more = '<div class="metromail-more-wrapper">';;

  if ($offset < metromail_user_message_count($account)) {
    $view_more .= theme('metromail_more', $account, $offset);
  }
  $view_more .= '</div>';
  return theme('metromail_page', $themed_messages, $flag, $view_more);
}

/**
 * Page callback: displays user messages.
 *
 * Path: ajax/metromail/%user/%
 *
 * Loads additional messages via AJAX when user clicks "View more" link.
 *
 * @param $account
 *   The user object whose messages are displayed.
 * @param $offset
 *   The first result message to return.
 *
 * @see theme_metromail_messages()
 * @see theme_metromail_more()
 * @see theme_empty_metromail()
 * @see template_preprocess_metromail_message()
 */
function metromail_overview_more($account, $offset) {
  if (!is_numeric($offset)) {
    return MENU_NOT_FOUND;
  }
  $output = '';
  $messages = metromail_load_user_messages($account, variable_get('metromail_more_messages_per_page', 10), $offset);
  $output .= theme('metromail_messages', $account, $messages);
  $offset += count($messages);
  $output .= '<div class="metromail-more-wrapper">';
  // Add "View more" link, if not all messages are already displayed.
  if ($offset < metromail_user_message_count($account)) {
    $output .= theme('metromail_more', $account, $offset);
  }
  $output .= '</div>';
  drupal_json(array('status' => TRUE, 'data' => $output));
}

/**
 * Page callback: displays a fully-loaded thread.
 *
 * Path: ajax-metromail-thread/%metromail_message
 *
 * @param $thread_message
 *   stdClass object of the root message in a thread.
 *
 * @see metromail_load_thread()
 * @see theme_metromail_thread()
 */
function metromail_thread_view($thread_message) {
  $thread = metromail_load_thread($thread_message->mid, TRUE);
  $output = array('status' => TRUE, 'data' => theme('metromail_thread', $thread));
  drupal_json($output);
}

/**
 * Page callback: displays send message form
 *
 * Path: user/%user/sendmessage
 *
 * @param $account
 *   StdClass object of user who is sending a message.
 *
 * @see metromail_send_message_form()
 */
function metromail_send_message_page($account) {
  return drupal_get_form('metromail_send_message_form', $account);
}

/**
 * Page callback: displays a reply message form.
 *
 * Path: ajax-metromail-reply/%user/%metromail_message/%metromail_message
 *
 * @param $account_to
 *   stdClass object Of the recipient account.
 * @param $thread_message
 *   stdClass object Of the root message in a thread.
 * @param $parent_message
 *   stdClass object If the message user replies to.
 *
 * @see metromail_reply_message_access()
 * @see _metromail_reply_title()
 */
function metromail_reply_message_page($account_to, $thread_message, $parent_message) {
  return drupal_get_form('metromail_reply_message_form', $account_to, $thread_message, $parent_message);
}

/**
 * Form constructor for sending messages.
 *
 * @param stdClass  $from_account
 *   The sender user object

 * @see metromail_send_message_form_validate()
 * @see metromail_send_message_form_submit()
 */
function metromail_send_message_form(&$form_state, $to_account) {
  global $user;
  // @TODO: hide recipient field if to_account provided.
  return array(
    'recipient' => array(
      '#type' => 'textfield',
      '#title' => t('User name'),
      '#required' => TRUE,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => isset($form_state['values']['recipient']) ? $form_state['values']['recipient'] : '',
    ),
    'body' => array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#required' => TRUE,
      '#default_value' => isset($form_state['values']['body']) ? $form_state['values']['body'] : '',
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
 * Form validation handler for metromail_send_message_form().
 *
 * @see metromail_send_message_form()
 */
function metromail_send_message_form_validate($form, &$form_state) {
  $form_state['uid_to'] = db_result(db_query("SELECT uid FROM {users} WHERE name = '%s'", $form_state['values']['recipient']));
  if (empty($form_state['uid_to'])) {
    form_error($form['recipient'], t('No users found with such name.'));
  }
}

/**
 * Form submission handler for metromail_send_message_form().
 *
 * @see metromail_send_message_form_validate()
 */
function metromail_send_message_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $message = array(
    'type' => $values['type'],
    'body' => $values['body'],
    'uid_from' => $values['from']->uid,
    'uid_to' => $form_state['uid_to'],
  );
  metromail_send_message($message);
  drupal_set_message(t('Your message to !username has been sent successfully.', array('!username' => l($values['recipient'], 'user/' . $form_state['uid_to']))));
  if (isset($_REQUEST['destination'])) {
    $form_state['redirect'] = isset($_REQUEST['destination']);
  }
}

/**
 * Form constructor for sending reply messages.
 *
 * It doesn't validate incoming arguments.
 *
 * @param $from_account
 *   stdClass object Of the sender user object
 * @param $thread_message
 *   stdClass object Of the root message in a thread.
 * @param $parent_message
 *   stdClass object Of the message user replies to.
 *
 * @see metromail_reply_message_page()
 */
function metromail_reply_message_form(&$form_state, $to_account, $thread_message, $parent_message) {
  global $user;
  if ($user->uid == $parent_message->uid_from) {
    // user replies on his own message
    $parent_message_title = t('You wrote to !username', array('!username' => $to_account->name));
  }
  else {
    $parent_message_title = t('!username wrote', array('!username' => $to_account->name));
  }
  return array(
    'parent_message' => array(
      '#type' => 'item',
      '#title' => $parent_message_title,
      '#value' => check_markup($parent_message->body),
    ),
    'body' => array(
      '#type' => 'textarea',
      '#title' => t('Your answer'),
      '#required' => TRUE,
      '#default_value' => isset($form_state['values']['body']) ? $form_state['values']['body'] : '',
    ),
    'pid' => array(
      '#type' => 'value',
      '#value' => $parent_message->mid,
    ),
    'thread' => array(
      '#type' => 'value',
      '#value' => $thread_message->mid,
    ),
    'type' => array(
      '#type' => 'value',
      '#value' => METROMAIL_MESSAGE_DEFAULT,
    ),
    'to' => array(
      '#type' => 'value',
      '#value' => $to_account,
    ),
    'from' => array(
      '#type' => 'value',
      '#value' => $user,
    ),
    'actions' => array(
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Send message'),
      ),
      'back' => array(
        '#value' => l(t('Back to MetroMail'), $redirect),
      ),
    ),
  );
}

/**
 * Form submission handler for metromail_reply_message_form().
 *
 * @see _metromail_skip_validation()
 */
function metromail_reply_message_form_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $message = array(
    'pid' => $values['pid'],
    'thread_id' => $values['thread'],
    'type' => $values['type'],
    'body' => $values['body'],
    'uid_from' => $values['from']->uid,
    'uid_to' => $values['to']->uid,
  );
  $message = metromail_send_message($message);
  drupal_set_message(t('Your message to !username has been sent successfully.', array('!username' => theme('username', $values['to']))));
  $redirect = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : "user/{$user->uid}/metromail";
  drupal_goto($redirect, array('expand' => $message['thread_id'], 'scroll_to' => $message['mid']));
}

/**
 * Helper function, used in menu callbacks
 *
 * Redirects to the specified page
 *
 * @param $to
 *   A string containing adress to redirect.
 */
function metromail_goto($to) {
  global $user;
  switch ($to) {
    case 'metromail':
      drupal_goto('user/'. $user->uid .'/metromail');
      break;
  }
}
