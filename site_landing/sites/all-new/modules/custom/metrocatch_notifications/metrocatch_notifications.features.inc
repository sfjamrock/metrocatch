<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function metrocatch_notifications_imagecache_default_presets() {
  $items = array(
    'userpic_in_email' => array(
      'presetname' => 'userpic_in_email',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '200',
            'height' => '200',
          ),
        ),
      ),
    ),
    'subfeature_userpic_in_email' => array(
      'presetname' => 'subfeature_userpic_in_email',
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
  );
  return $items;
}