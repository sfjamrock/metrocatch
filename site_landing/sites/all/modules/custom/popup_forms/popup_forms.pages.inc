<?php

/**
 * @file
 * module menu callback ad related functions
 */

/**
 * Page callback for ajax-get-popup-form/% path, returns rendered form and form_state in json format
 * @param string $form_id
 * @return string json content
 */
function popup_forms_get_ajax_form($form_id) {
  $forms = module_invoke_all('popup_forms_data');
  module_invoke_all('popup_forms_data_alter', $forms);

  if (!isset($forms[$form_id])) {
    return MENU_NOT_FOUND;
  }

  $form_data = $forms[$form_id];
  if (isset($form_data['file'])) {
    include_once($form_data['file']);
  }

  // filter parameters
  $args = array();
  if (isset($form_data['params'])) {
    foreach ($form_data['params'] as $key => $default_value) {
      if (isset($_GET[$key])) {
        $args[] = $_GET[$key];
      }
      else {
        $args[] = $default_value;
      }
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////
  $form_state = array('storage' => NULL, 'submitted' => FALSE);

  if (isset($_SESSION['batch_form_state'])) {
    $form_state = $_SESSION['batch_form_state'];
    unset($_SESSION['batch_form_state']);
  }
  else {
    if (isset($_POST['form_id']) && $_POST['form_id'] == $form_id && !empty($_POST['form_build_id'])) {
      $form = form_get_cache($_POST['form_build_id'], $form_state);
    }

    if (!isset($form)) {
      $form_state['post'] = $_POST;
      $args_temp[0] = &$form_state;
      $args_temp = array_merge($args_temp, $args);
      array_unshift($args_temp, $form_id);

      $form = call_user_func_array('drupal_retrieve_form', $args_temp);
      $form['#popup_forms_ajax_form_data'] = $form_data;
      $form_build_id = 'form-'. md5(uniqid(mt_rand(), TRUE));
      $form['#build_id'] = $form_build_id;
      drupal_prepare_form($form_id, $form, $form_state);
      $original_form = $form;
      $cacheable = TRUE;
      unset($form_state['post']);
    }


    $form['#post'] = $_POST;
    if (!isset($form['#after_build'])) {
      $form['#after_build'] = array();
    }
    $form['#after_build'][] = 'popup_forms_form_after_build';
    drupal_process_form($form_id, $form, $form_state);

    if ($cacheable && !empty($form['#cache'])) {
      form_set_cache($form_build_id, $original_form, $form_state);
    }
  }

  if ((!empty($form_state['storage']) || !empty($form_state['rebuild'])) && !empty($form_state['process_input']) && !form_get_errors()) {
    $form = drupal_rebuild_form($form_id, $form_state, $args);
  }

  $output = drupal_render_form($form_id, $form);
  ////////////////////////////////////////////////////////////////////////////////////

  popup_forms_child_js();
  return $output;
}

/**
 * Additional #after_build handler for all popup forms
 */
function popup_forms_form_after_build($form, &$form_state) {
  $button_name = isset($form_state['clicked_button']['#name']) ? $form_state['clicked_button']['#name'] : NULL;
  if (!in_array($button_name, $form['#popup_forms_ajax_form_data']['close_buttons'])) {
    return $form;
  }
  if (isset($form_state['submit_handlers'])) {
    $handlers = $form_state['submit_handlers'];
  }
  elseif (isset($form['#submit'])) {
    $handlers = $form['#submit'];
  }
  else {
    $handlers = array();
  }
  $handlers[] = 'popup_forms_form_submit_handler';
  $form_state['submit_handlers'] = $handlers;
  return $form;
}

/**
 * Additonal submit handler for popup forms (close-handler)
 */
function popup_forms_form_submit_handler($form, &$form_state) {
  if ($form['#programmed'] || !empty($form_state['rebuild']) || !empty($form_state['storage'])) {
    return;
  }
  $form_state['dialog_index'] = $_GET['popup_forms_num'];
  print '<script type="text/javascript">';
  print 'parent.popupFormsFinishDialog('. drupal_to_js($form_state) .');';
  print '</script>';
  drupal_page_footer();
  exit();
}