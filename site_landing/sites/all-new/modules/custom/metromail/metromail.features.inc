<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function metromail_flag_default_flags() {
  $flags = array();
  // Exported flag: "Email user when he gets new metromail".
  $flags['metromail_notify_flag'] = array(
    'content_type' => 'user',
    'title' => 'Email user when he gets new metromail',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'Email me when I get new metromail',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Email me when I get new metromail',
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
    'module' => 'metromail',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function metromail_imagecache_default_presets() {
  $items = array(
    'metromail_userpic' => array(
      'presetname' => 'metromail_userpic',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '79',
            'height' => '101',
          ),
        ),
      ),
    ),
  );
  return $items;
}