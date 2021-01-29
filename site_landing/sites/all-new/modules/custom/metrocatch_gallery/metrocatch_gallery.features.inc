<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function metrocatch_gallery_flag_default_flags() {
  $flags = array();
  // Exported flag: "Inappropriate pictures".
  $flags['metrocatch_gallery_inappropriate'] = array(
    'content_type' => 'node',
    'title' => 'Inappropriate pictures',
    'global' => '0',
    'types' => array(
      '0' => 'metrocatch_gallery_picture',
    ),
    'flag_short' => 'Flag as inappropriate',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Flagged as inappropriate',
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
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'metrocatch_gallery',
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
function metrocatch_gallery_imagecache_default_presets() {
  $items = array(
    'metrocatch_gallery_manage_picture' => array(
      'presetname' => 'metrocatch_gallery_manage_picture',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '100',
            'height' => '100',
          ),
        ),
      ),
    ),
    'metrocatch_gallery_manage_upload_preview' => array(
      'presetname' => 'metrocatch_gallery_manage_upload_preview',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '150',
            'height' => '150',
          ),
        ),
      ),
    ),
    'metrocatch_gallery_picture' => array(
      'presetname' => 'metrocatch_gallery_picture',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '675',
            'height' => '375',
            'upscale' => 0,
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
function metrocatch_gallery_node_info() {
  $items = array(
    'metrocatch_gallery_picture' => array(
      'name' => t('Metrocatch Gallery Image'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function metrocatch_gallery_views_api() {
  return array(
    'api' => '2',
  );
}