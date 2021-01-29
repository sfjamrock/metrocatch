<?php
/**
 * @file
 * Contains Metrocatch Gallery plugin.
 */

class metrocatch_gallery_plugin_style_jcarousel extends jcarousel_style_plugin {

  function option_definition() {
    $options = parent::option_definition();
    $options['display_controls'] = array(
      'default' => 'below',
    );
    $options['play_timeout'] = array(
      'default' => '5',
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['auto']);
    $form['display_controls'] = array(
      '#type' => 'select',
      '#title' => t('Display controls'),
      '#options' => array(
        '' => t('Do not display'),
        'above' => t('Above'),
        'below' => t('Below'),
      ),
      '#default_value' => $this->options['display_controls'],
      '#description' => t('Display control buttons'),
    );
    $form['play_timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Play timeout'),
      '#field_suffix' => t('seconds'),
      '#size' => 4,
      '#default_value' => $this->options['play_timeout'],
    );
  }
}
