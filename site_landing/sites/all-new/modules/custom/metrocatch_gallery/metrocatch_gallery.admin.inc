<?php
/**
 * @file
 * Contains adminstration page and form callbacks.
 */

function metrocatch_gallery_settings_form(&$form_state) {
  $form = array(
    'metrocatch_gallery_directory' => array(
      '#type' => 'textfield',
      '#title' => 'Gallery images path',
      '#default_value' => variable_get('metrocatch_gallery_directory', 'galleries'),
      '#maxlength' => 255,
      '#description' => t('Subdirectory in the directory %dir where pictures will be stored.', array('%dir' => file_directory_path() .'/')),
    ),
  );
  // Ensure tha directory exists and is writable.
  $gallery_path = file_create_path(variable_get('metrocatch_gallery_directory', 'galleries'));
  file_check_directory($gallery_path, FILE_CREATE_DIRECTORY, 'metrocatch_gallery_directory');
  return system_settings_form($form);
}
