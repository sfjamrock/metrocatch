<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function metrocatch_conversation_flag_default_flags() {
  $flags = array();
  // Exported flag: "Access to conversation page".
  $flags['metrocatch_conversation_private'] = array(
    'content_type' => 'user',
    'title' => 'Access to conversation page',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'Display own posts',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Public',
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
    'module' => 'metrocatch_conversation',
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
function metrocatch_conversation_imagecache_default_presets() {
  $items = array(
    'metrocatch_conversation_entry_author' => array(
      'presetname' => 'metrocatch_conversation_entry_author',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '65',
            'height' => '75',
          ),
        ),
      ),
    ),
    'metrocatch_conversation_comment' => array(
      'presetname' => 'metrocatch_conversation_comment',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '34',
            'height' => '40',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function metrocatch_conversation_node_info() {
  $items = array(
    'conversation_entry' => array(
      'name' => t('Conversation Entry'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Subject'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
