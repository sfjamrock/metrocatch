<?php
// $Id: vr.admin.inc,v 1.5.2.5 2010/06/22 17:10:16 jbomb Exp $

/**
 * Settings form
 */
function vr_settings_form($form_state) {
  $form['vr_credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login Credentials'),
    '#description' => t('The VerticalResponse API requires login credentials.'),
  );
  $form['vr_credentials']['vr_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#description' => t('The email address you use to login to VerticalResponse.'),
    '#default_value' => variable_get('vr_username', NULL),
  );
  $form['vr_credentials']['vr_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('VerticalResponse password.'),
    '#default_value' => variable_get('vr_password', NULL),
  );
  $vr = _vr_init();
  return system_settings_form($form);
}