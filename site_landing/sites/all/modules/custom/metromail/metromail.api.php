<?php

/**
 * @file
 * Hooks provided by the metromail.module.
 */

/**
 * Allows other modules to define their own MetroMail message types.
 *
 * @return array
 *   An associative array of provided message types, keys are machine-readable names of types.
 *   Values of returned array must be arrays with following keys:
 *   -title: A string to be used as title in admin settings form.
 *   -description: (optional) A string to be used as description of this message types.
 *    It is also displayed only in admin settings form.
 *   -enabled: (optional) Boolean indicating whether this message type is enabled.
 *    Default TRUE but admin can always change this setting (disable sending messages of this type)
 *    in admin settings form. In this case this setting will be stored in Drupal vatiable which name
 *    is metromail_MESSAGETYPE_enabled, where MESSAGETYPE is machine-readable name of this type.
 *   -default_template: A string containing default message template.
 *    This template can be changed by admin in module settings form.
 *    In this case new template will be stored in Drupal vatiable which name
 *    is metromail_MESSAGETYPE_template, where MESSAGETYPE is machine-readable name of this type.
 *  - placeholders: An array of available placeholders which can be used in message template.
 *    They will be listed in admin settings form below the template textarea.
 */
function hook_metromail_info() {
  return array(
    'request_pending' => array(
      'title' => t('Sends a verification request for an initial connection.'),
      'description' => t('Optional description'),
      'enabled' => FALSE, // Means this message type is disabled by default.
      'default_template' => '!from have suggested you to initialize connection. !accept or !deny?',
      'placeholders' => array('!from', '!accept', '!deny'),
    ),
  );
}
