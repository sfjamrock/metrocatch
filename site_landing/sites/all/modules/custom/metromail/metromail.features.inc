<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function metromail_flag_default_flags() {
  $flags = array();
  // Exported flag: "Email user when he gets new MetroMail".
  $flags['metromail_notify_flag'] = array(
    'content_type' => 'user',
    'title' => 'Email user when he gets new MetroMail',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'Email me when i get new MetroMail',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Do not email me when i get new MetroMail',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 2,
      ),
      'unflag' => array(
        '0' => 2,
      ),
    ),
    'show_on_profile' => 0,
    'access_uid' => '',
    'api_version' => 2,
    'module' => 'metromail_feature',
    'locked' => array(
      '0' => 'name',
    ),
  );
  return $flags;
}
