<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function metrocatch_profile_imagecache_default_presets() {
  $items = array(
    'facebook_share' => array(
      'presetname' => 'facebook_share',
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
    'userpic_featured' => array(
      'presetname' => 'userpic_featured',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '191',
            'height' => '224',
          ),
        ),
      ),
    ),
    'userpic_huge' => array(
      'presetname' => 'userpic_huge',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '215',
            'height' => '254',
          ),
        ),
      ),
    ),
    'userpic_large' => array(
      'presetname' => 'userpic_large',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '191',
            'height' => '226',
          ),
        ),
      ),
    ),
    'userpic_other_catches' => array(
      'presetname' => 'userpic_other_catches',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '85',
            'height' => '107',
          ),
        ),
      ),
    ),
    'userpic_small' => array(
      'presetname' => 'userpic_small',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '85',
            'height' => '107',
          ),
        ),
      ),
    ),
    'userpic_very_small' => array(
      'presetname' => 'userpic_very_small',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '32',
            'height' => '32',
          ),
        ),
      ),
    ),
  );
  return $items;
}
