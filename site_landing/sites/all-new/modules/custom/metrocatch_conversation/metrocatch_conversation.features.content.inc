<?php

/**
 * Implementation of hook_content_default_fields().
 */
function metrocatch_conversation_content_default_fields() {
  $fields = array();

  // Exported field: field_conversation_entry_conv_id
  $fields['conversation_entry-field_conversation_entry_conv_id'] = array(
    'field_name' => 'field_conversation_entry_conv_id',
    'type_name' => 'conversation_entry',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_plain' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_html' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'userreference',
    'required' => '1',
    'multiple' => '0',
    'module' => 'userreference',
    'active' => '1',
    'referenceable_roles' => array(
      '2' => 2,
    ),
    'referenceable_status' => '',
    'advanced_view' => '',
    'advanced_view_args' => '',
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => '60',
      'reverse_link' => 0,
      'default_value' => array(
        '0' => array(
          'uid' => NULL,
          '_error_element' => 'default_value_widget][field_conversation_entry_conv_id][0][uid][uid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Conversation id',
      'weight' => '31',
      'description' => '',
      'type' => 'userreference_autocomplete',
      'module' => 'userreference',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Conversation id');

  return $fields;
}
