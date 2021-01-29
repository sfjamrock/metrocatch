<?php
// $Id: vr_register.admin.inc,v 1.1.2.3 2010/07/13 18:55:31 jbomb Exp $

/**
 * Create settings form
 */
function vr_register_settings($form_state) {
  $lists = vr_get_lists();
  // create checkboxes to enable lists
  foreach ($lists as $list => $name) {
    $form['lists']['vr_register_enabled_' . $list] = array(
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => variable_get('vr_register_enabled_' . $list, NULL),
      );
    $form['states']['vr_register_state_' . $list] = array(
      '#type' => 'radios',
      '#options' => array(
        0 => t('Deselected'),
        1 => t('Selected'),
        2 => t('Hidden (Selected)'),
        ),
      '#default_value' => variable_get('vr_register_state_' . $list, 1),
      );
  }
  return system_settings_form($form);
}

/**
 * Theme the settings form.
 */
function theme_vr_register_settings($form) {
  $header = array(
    t('Enabled'),
    t('List Name'),
    t('Default state'),
    );
  $lists = vr_get_lists();
  $rows = array();
  foreach ($lists as $list => $name) {
    $rows[] = array(
      drupal_render($form['lists']['vr_register_enabled_' . $list]),
      t($name),
      drupal_render($form['states']['vr_register_state_' . $list]),
      );
  }
  $output = theme('table', $header, $rows) . drupal_render($form);
  return $output;
}