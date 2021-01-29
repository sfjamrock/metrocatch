<?php
// $Id: vr_list.segmentation.inc,v 1.1.2.3 2010/07/09 18:57:21 jbomb Exp $

/**
 * Menu callback for AHAH bindings.
 * 
 * This function selects fields for creating a segmentation query.
 */
function vr_list_segments() {
  // The form is generated in an include file which we need to include manually.
  $path = drupal_get_path('module', 'vr_list');
  include_once $path . '/vr_list.segmentation.inc';
  // We're starting in step #3, preparing for #4.
  $form_state = array('storage' => NULL, 'rebuild' => TRUE);
  $form_build_id = $_POST['form_build_id'];
  // Step #4.
  $form = form_get_cache($form_build_id, $form_state);

  // Preparing for #5.
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;

  // Step #5.
  drupal_process_form($form_id, $form, $form_state);

  // Step #6 and #7 and #8.
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  foreach ($form['operators'] as $id => $field) {
    if (strpos($id, '#') !== 0) {
      $title = $form['operators'][$id]['#title'];
      unset($form['operators'][$id]['#title']);
      $rows[] = array(
        $title,
        drupal_render($form['operators'][$id]),
        drupal_render($form['filters'][$id]),
        drupal_render($form['match_missing'][$id]),
        );
    }
  }
  $headers = array(
    t('Field Name'),
    t('Operator'),
    t('Filter Value'),
    t('Match if Missing'),
    );
  $output = theme('table', $headers, $rows);
  print drupal_json(array('status' => TRUE, 'data' => theme('status_messages') . $output));
}

/**
 * Create a Segmentation Query.
 */
function vr_list_segmentation($form_state) {
  $form['list_properties'] = array(
    '#title' => t('List Properties'),
    '#type' => 'fieldset',
    );
  $form['list_properties']['list_name'] = array(
    '#title' => t('List Name'),
    '#type' => 'textfield',
    '#description' => t('Enter a name for the list that will be generated from this segmentation
      query.'),
    '#default_value' => $form_state['values']['list_name'],
    );
  $form['mailing_lists'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mailing Lists'),
    '#tree' => TRUE,
    );
  // Lists is a key value pairing of list_id and list name
  $lists = vr_get_lists();
  foreach ($lists as $id => $title) {
    $form['mailing_lists'][$id] = array(
      '#title' => t($title),
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => $form_state['values']['mailing_lists'][$id],
      '#ahah' => array(
        'path' => 'vr/list-segments',
        'wrapper' => 'vr-list-segments',
        ),
      );
  }
  
  // Check for mailing lists, and build a form for segmentation query.
  $lists = ($form_state['values']['mailing_lists']) ? $form_state['values']['mailing_lists'] : array();
  $options = array();
  // Refresh list cache as objects and find all fields for all lists.
  vr_get_lists(array('object' => TRUE, 'reset' => TRUE));
  foreach ($lists as $list_id => $bool) {
    $list = vr_get_list_by_id($list_id);
    if (empty($bool) OR !is_array($list->fields)) {
      continue;
    }
    $fields = array_combine($list->fields, $list->fields);
    $options = array_merge($fields, $options);
  }
  
  // Build the segmentation generation form.  There should be three fields
  // for each list member property: operator, filter value and match on 
  // missing record.
  $form['operators']['#tree'] = TRUE;
  $form['filters']['#tree'] = TRUE;
  $form['match_missing']['#tree'] = TRUE;
  foreach ($options as $field => $name) {
    $form['operators'][$name] = array(
      '#title' => ucwords(str_replace('_', ' ', $name)),
      '#type' => 'select',
      '#options' => array(
        '=' => 'equals',
        'LIKE' => 'contains',
        ),
      '#default_value' => $form_state['values']['operators'][$name],
      );

    $form['filters'][$name] = array(
      '#type' => 'textfield',
      '#default_value' => $form_state['values']['filters'][$name],
      );
      
    $form['match_missing'][$name] = array(
      '#type' => 'radios',
      '#options' => array(
        1 => t('Yes'),
        0 => t('No'),
        ),
      '#default_value' => $form_state['values']['match_missing'][$name],
      );
    
  }
  
  $form['vr_wrapper'] = array(
    '#type' => 'item',
    '#prefix' => "<div id='vr-list-segments'>",
    '#suffix' => '</div>',
    );
  
  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
    );

  return $form;
}

/**
 * Create mailing list from segmentation.
 */
function vr_list_segmentation_submit($form, &$form_state) {
  // Retrieve lists that should be segmented
  foreach ($form_state['values']['mailing_lists'] as $list_id => $bool) {
    if ($bool == TRUE) {
      $lists[] = $list_id;
    }
  }
  // Create segmentation query.
  foreach ($form_state['values']['filters'] as $field => $value) {
    if (!empty($value)) {
      // Dress filters up if they're set to "contains"
      $filter = $form_state['values']['filters'][$field];
      if ($form_state['values']['operators'][$field] == 'LIKE') {
        $filter = '%' . $filter . '%';
      }
      $constraints[] = array(
        'constrained_entity' => 'list_member',
        'constrained_field' => $field,
        'constraint_operator' => $form_state['values']['operators'][$field],
        'constraint_values' => array($filter),
        'match_if_field_missing' => $form_state['values']['match_missing'][$field],
        );
    }
  }
  $inputs[] = array(
    'constraints' => $constraints,
    'input_type' => 'list',
    'ids' => $lists,
    'input_constraint_conjunction' => 'and', 
    );
  $query = array(
    'name' => time(),
    'operation_type' => "intersect", // values can be 'union', 'intersect'
    'description' => "testing",
    'inputs' => $inputs,
    //'operation_type' => "AND" 
    // values can be 'union' or 'intersect' if using multiple 'seg_input'
    );

  $seg_query = array('segmentation_query' => $query);
  $sid = vr_api('createSegmentationQuery', $seg_query);
  $param['segmentation_query_id'] = $sid;
  $list_size = vr_api('runSegmentationQuery', $param);
  // Segmentation query must return at least one result.
  if ($list_size > 0) {
    $param = array(
      'segmentation_query_id' => $sid,
      'output_lists' => array(
        array(
          'name' => t($form_state['values']['list_name']),
          'list_type' => 'email',
          'num_records' => $list_size,
          )
        ),
      );
    $lists = vr_api('compileSegmentationQuery', $param);
    $list = current($lists);
    drupal_set_message(t('@list has been created.', array('@list' => $list->name)));
    $form_state['redirect'] = 'admin/build/vr/list/' . $list->id . '/edit';
  }
  else {
    drupal_set_message('No matches found for this segmentation query. No list was created.');
    $form_state['redirect'] = 'admin/build/vr/list/segmentation';
  }
  vr_api('deleteSegmentationQuery', array('segmentation_query_id' => $sid));
}