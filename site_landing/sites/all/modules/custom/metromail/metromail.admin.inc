<?php
/**
 * @file
 * metromail.admin.inc
 *
 * Contains administrative forms constructos and handlers.
 */

/**
 * Form constructor for the module settings form.
 *
 * Path: admin/content/metromail
 */
function metromail_settings_form(&$form_state) {
  $form = array(
    'display_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Display settings'),
        'metromail_system_name' => array(
          '#type' => 'textfield',
          '#title' => t('The username displayed in MetroMail for a system messages.'),
          '#default_value' => variable_get('metromail_system_name', t('MetroCatch')),
        ),
        'metromail_messages_per_page' => array(
           '#type' => 'textfield',
           '#title' => t('How many messages are initially displayed'),
           '#default_value' => variable_get('metromail_messages_per_page', 10),
        ),
        'metromail_more_messages_per_page' => array(
          '#type' => 'textfield',
          '#title' => t('How many messages are loaded when user clicks "View More"'),
          '#default_value' => variable_get('metromail_more_messages_per_page', 10),
        ),
    ),
    'email_notifications' => array(
      '#type' => 'fieldset',
      '#title' => t('E-mail notifications'),
      'notification_email_settings' => array(
        '#type' => 'fieldset',
        '#title' => t('Notification about new MetroMail private messages'),
      ),
      'broadcast_notification_email_settings' => array(
        '#type' => 'fieldset',
        '#title' => t('Notification about new MetroMail broadcast messages'),
      ),
    ),
  );
  $metromail_creator = mailcreators_get_creator(METROMAIL_CREATOR_NAME);
  $broadcast_metromail_creator = mailcreators_get_creator(METROMAIL_BROADCAST_CREATOR_NAME);
  $form['email_notifications']['notification_email_settings'][] = $metromail_creator->getSettingsForm($form_state);
  $form['email_notifications']['broadcast_notification_email_settings'][] = $broadcast_metromail_creator->getSettingsForm($form_state);
  $notification_settings = _metromail_notification_admin_settings();
  if (!empty($notification_settings)) {
    $form['metromail_notifications'] = array(
      '#type' => 'fieldset',
      '#title' => t('MetroMail notifications'),
    );
    $form['metromail_notifications'][0] = $notification_settings;
  }
  return system_settings_form($form);
}

/**
 * Form constructor for broadcast messaging.
 *
 * Path: admin/content/metromail/broadcast
 *
 * @see metromail_broadcast_form_validate()
 * @see metromail_broadcast_form_submit()
 */
function metromail_broadcast_form(&$form_state, $pid = 0, $thread = 0) {
  drupal_add_js(drupal_get_path('module', 'metromail') . '/metromail.js');
  return array(
    'send_to' => array(
      '#type' => 'radios',
      '#required' => TRUE,
      '#title' => t('Send to'),
      '#options' => array(
        'someone' => t('One user'),
        'everyone' => t('All users'),
      ),
      '#default_value' => isset($form_state['values']['send_to']) ? $form_state['values']['send_to'] : 'someone',
    ),
    'recipient' => array(
      '#type' => 'textfield',
      '#title' => t('User name'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => isset($form_state['values']['recipient']) ? $form_state['values']['recipient'] : '',
    ),
    'body' => array(
      '#type' => 'textarea',
      '#required' => TRUE,
      '#title' => t('Body'),
      '#default_value' => isset($form_state['values']['body']) ? $form_state['values']['body'] : '',
    ),
    'pid' => array(
      '#type' => 'value',
      '#value' => $pid,
    ),
    'thread' => array(
      '#type' => 'value',
      '#value' => $thread,
    ),
    'type' => array(
      '#type' => 'value',
      '#value' => METROMAIL_MESSAGE_DEFAULT,
    ),
    'submit' => array(
       '#type' => 'submit',
       '#value' => t('Send message'),
    ),
  );
}

/**
 * Form validation handler for metromail_broadcast_form().
 *
 * @see metromail_broadcast_form()
 */
function metromail_broadcast_form_validate($form, &$form_state) {
  if ($form_state['values']['send_to'] == 'someone') {
    $form_state['uid_to'] = db_result(db_query("SELECT uid FROM {users} WHERE name = '%s'", $form_state['values']['recipient']));
    if (empty($form_state['uid_to'])) {
      form_error($form['recipient'], t('You forgot to enter the recipient name'));
    }
  }
}

/**
 * Form submission handler for metromail_broadcast_form().
 *
 * @see metromail_broadcast_form_validate()
 */
function metromail_broadcast_form_submit($form, &$form_state) {
  $message = array(
    'pid' => $form_state['values']['pid'],
    'thread' => $form_state['values']['thread'],
    'type' => $form_state['values']['type'],
    'body' => $form_state['values']['body'],
    'uid_from' => 0,  // it means this is a System message.
  );
  if ($form_state['values']['send_to'] == 'someone') {
    $message['uid_to'] = $form_state['uid_to'];
    metromail_send_message($message);
    $recipient = l($form_state['values']['recipient'], 'user/' . $message['uid_to']);
  }
  elseif ($form_state['values']['send_to'] == 'everyone') {
    metromail_send_broadcast_message($message);
    $recipient = t('All users');
  }
  drupal_set_message(t('You message to !recipient has been sent successfully', array('!recipient' => $recipient)));
}


/**
 *  Helper function which collects settings from all modules which imlements hook_metromail_info().
 *
 *  This function allows admin to override code-based default message settings.
 *  It will prepare a fieldset with Checkbox for For each defined message type
 *
 *  @return
 *    An array of form elements.
 *
 *  @see metromail_settings_form()
 */
function _metromail_notification_admin_settings() {
  $settings = array();
  foreach (module_invoke_all('metromail_info') as $type => $metromail_settings) {
    $settings[$type] = array(
      '#type' => 'fieldset',
      '#title' => $metromail_settings['title'],
    );
    if (isset($metromail_settings['description'])) {
      $settings[$type]['#description'] = $metromail_settings['description'];
    }
    $name_enabled = 'metromail_'. $type .'_enabled';
    // name of variable wich contains template.
    $name_template = 'metromail_'. $type .'_template';
    $default_enabled = isset($metromail_settings['enabled']) ? $metromail_settings['enabled'] : TRUE;
    $settings[$type][$name_enabled] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => variable_get($name_enabled, $default_enabled),
    );
    $settings[$type][$name_template] = array(
      '#type' => 'textarea',
      '#title' => t('Template'),
      '#default_value' => variable_get($name_template, $metromail_settings['default_template']),
    );
    if (!empty($metromail_settings['placeholders'])) {
      $settings[$type][$name_template]['#description'] = t('List of available placeholders') .': '. implode(', ', $metromail_settings['placeholders']);
    }
  }
  return $settings;
}
