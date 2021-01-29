<?php
/**
 * @file
 * page callbacks and helper functions
 */

function cyd_utils_test_page() {
  cyd_utils_print_trace();
  dpdb(array(5, 7), 'In cyd_utils_test_page()');
  dpdb('Hello!');
  return 'test';
}

/**
 * Email test form callback
 * Sends test email from server (shown  on '/cyd-utils-test-email' path)
 */
function cyd_utils_email_test_form($form_state) {
  return array(
    'to' => array(
      '#title' => t('To'),
      '#type' => 'textfield',
      '#default_value' => isset($form_state['to']) ? $form_state['to'] : '',
      '#required' => TRUE,
    ),
    'subject' => array(
      '#title' => t('Subject'),
      '#type' => 'textfield',
      '#default_value' => isset($form_state['subject']) ? $form_state['subject'] : '',
      '#required' => TRUE,
    ),
    'body' => array(
      '#title' => t('Body'),
      '#type' => 'textarea',
      '#default_value' => isset($form_state['body']) ? $form_state['body'] : '',
      '#required' => TRUE,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Send'),
    ),
  );
}

/**
 * Email test form validate callback
 */
function cyd_utils_email_test_form_validate($form, $form_state) {
  $error = user_validate_mail($form_state['values']['to']);
  if ($error) {
    form_error($form['to'], $error);
  }
}

/**
 * Email test form submit callback
 */
function cyd_utils_email_test_form_submit($form, $form_state) {
  $v = $form_state['values'];
  // send e-mail
  $headers = array();
  $headers['From'] = $headers['Sender'] = $headers['Return-Path'] = $headers['Errors-To'] =  variable_get('site_mail', ini_get('sendmail_from'));
  $message = array(
    'id' => 'cyd_utils_test_email',
    'to' => $v['to'],
    'subject' => $v['subject'],
    'body' => $v['body'],
    'headers' => $headers,
    'from' => variable_get('site_mail', ini_get('sendmail_from')),
  );
  $result = drupal_mail_send($message);
  if ($result) {
    drupal_set_message(t('E-mail was sent successfully.'));
  }
  else {
    drupal_set_message(t("Can't send e-mail"), 'error');
  }
}
