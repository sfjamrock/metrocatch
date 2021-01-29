<?php
// $Id: vr_list.admin.inc,v 1.1.2.19 2010/07/20 14:29:36 jbomb Exp $
vr_list_member_init();

/**
 * vr_list_build
 * 
 * Allow administrators to create a new mailing list from drupal.
 */
function vr_list_build_form($form_state) {
  $form['list_name'] = array(
    '#type' => 'textfield',
    '#title' => t('List Name'),
    );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create List'),
    );
  // Sort and paginate lists
  $lists = vr_list_table_sort(vr_list_build_header(), $_GET['order']);
  $total = count($lists);
  $size = (empty($_GET['size'])) ? 25 : $_GET['size'];
  $offset = $_GET['page'] * $size;
  $pager_end = (($offset + $size) < $total) ? $offset + $size : $total;
  for ($i = $offset; $i < $pager_end; $i++) {
    $list = $lists[$i];
    $row = array(
      l($list->name, 'admin/build/vr/list/' . $list->id . '/edit'),
      $list->size,
      (!empty($list->last_mailed)) ? date('F j, Y g:ia', strtotime($list->last_mailed)) 
                                   : t('never'),
      l('Delete', 'admin/build/vr/list/' . $list->id . '/delete'),
      );
    $rows[] = $row;
  }
  $headers = vr_list_build_header();
  $form['existing_lists'] = array(
    '#type' => 'item',
    '#value' => theme('table', $headers, $rows),
    );

  $control_rows[] = array(
    theme('vr_list_pager', $total, $size),
    array(
      'align' => 'right',
      'data' => theme('vr_list_controls'),
      ),
    );
  $form['list_controls'] = array(
    '#type' => 'item',
    '#value' => theme('table', array(), $control_rows),
    );
  return $form;
}

/**
 * vr_list_build_form_submit
 * 
 * Allow an administraor to create a VerticalResponse mailing list.
 */
function vr_list_build_form_submit($form, &$form_state) {
  $params['name'] = $form_state['values']['list_name'];
  $params['type'] = 'email';
  $params['custom_field_names'] = array();
  $list_id = vr_api('createList', $params, TRUE);
  if (!empty($list_id)) {
    $message = '@list has been created.';
    $tokens = array(
      '@list' => $form_state['values']['list_name'],
      );
    $message = t($message, $tokens);
    drupal_set_message($message);
    variable_set('vr_list_enabled_' . $list_id, TRUE);
  }
}

/**
 * vr_list_delete
 * 
 * Allow administrators to delete a VerticalResponse mailing
 * list using this form.
 */
function vr_list_delete(&$form_state, $list) {
  $form['list'] = array(
    '#type' => value,
    '#value' => $list,
    );
  $question = t(
    'Are you sure you want to delete "@list?" There are @size list members.', 
    array(
      '@list' => $list->name,
      '@size' => $list->size,
      )
    );
  $path = 'admin/build/vr/list/' . $list->id . '/edit';
  return confirm_form($form, $question, $path);
}

/**
 * vr_list_delete_submit
 * 
 * Require administrators to confirm their desire to delete
 * a VerticalResponse mailing list.
 */
function vr_list_delete_submit($form, &$form_state) {
  $list = $form_state['values']['list'];
  $confirm = $form_state['values']['confirm'];
  if (!empty($form_state['values']['confirm'])) {
    $param['list_id'] = $list->id;
    vr_api('deleteList', $param, TRUE);
    drupal_set_message(t('@list has been deleted.', array('@list' => $list->name)));  
  }
  $form_state['redirect'] = 'admin/build/vr/list';
}

/**
 * Show a table of member list information.
 * 
 * TODO: Break this into two seperate functions, one for the primary form
 * and a second for the confirmation form.
 */
function vr_list_view_form($form_state, $list) {
  // Step one:  Identify which action should be applied to each member
  // record
  if (empty($form_state['storage']['values'])) {
    // Include total list size for pager function
    $form['list_size'] = array(
      '#type' => 'value',
      '#value' => $list->size,
      );
    $form['list_id'] = array(
      '#type' => 'value',
      '#value' => $list->id,
      );
    // Add get array values to form. 
    // TODO: This seems less than desirable, but we want to redirect to
    // the page that the form was submitted from, so we need to keep track
    // of sort options, display and pager values.
    $form['_get'] = array(
      '#type' => 'value',
      '#value' => $_GET,
      );
    // Add form fields for each list member record.
    $list_size = (empty($_GET['size'])) ? 25 : $_GET['size'];
    $offset = $_GET['page'] * $list_size;
    // Get sort operation.
    $param = vr_sort_param($_GET['order'], $_GET['sort'], vr_list_view_header(), 'Status');
    $members = vr_list_get_members($list->id, $list_size, $offset, $param);
    
    $form['member']['#tree'] = TRUE;
    // force members to be an array
    $members = (is_array($members)) ? $members : array();
    foreach ($members as $member) {
      $form['member'][$member['hash']]['status']['#value'] = _vr_list_member_status($member['optin_status']);
      // Show email address as a link to member edit form.
      $email = $member['email_address'];
      $edit = l($email, 'admin/build/vr/list/' . $list->id . '/edit/' . $member['hash']);
      $form['member'][$member['hash']]['email_address']['#value'] = $edit;
      $form['member'][$member['hash']]['first_name']['#value'] = $member['first_name'];
      $form['member'][$member['hash']]['last_name']['#value'] = $member['last_name'];
      $form['member'][$member['hash']]['create_date']['#value'] = date('F j, Y', strtotime($member['create_date']));
      // Define options based on current optin_status. 
      // NOTE: once someone has unsubscribed they can only be deleted.
      if ($member['optin_status'] === '0') {
        $options = array(
          '0' => t('Unsubscribe'),
          -1 => t('Delete'),
          );
      }
      else {
        $options = array(
           NULL => t('Subscribe'),
           -3 => t('Supress'),
           0 => t('Unsubscribe'),
           -1 => t('Delete'),
           );
      }
      $form['member'][$member['hash']]['operation'] = array(
       '#type' => 'radios',
       '#attributes' => array('style' => 'display:inline;'),
       '#options' => $options,
       '#default_value' => $member['optin_status'],
       );
      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save Changes'),
        );
    }
    return $form;
  } 
  // Step TWO: confirm that the administrator truly intends on performing
  // the actions specified in the previous step.
  else {
    $form_values = $form_state['storage']['values'];
    // Begin by sorting all of the actions.
    foreach ($form_values['member'] as $member_hash => $value) {
      $operation = $value['operation'];
      // Only track actions that do not match this record's current state.
      $list_member = new vr_list_member($list->id, $member_hash);
      $member = $list_member->member_array;
      if ($member['optin_status'] != $operation) {
        // Add to storage
        $form_state['storage']['values']['action'][$operation][] = $member_hash;
        $rows[] = array(
          $member['email_address'],
          _vr_list_member_status($member['optin_status']),
          _vr_list_member_status($operation),
          );
      }
    } 
    $header = array(
      t('E-mail Address'),
      t('Old Status'),
      t('New Status'),
      );
    $question = t('Please confirm the following record status changes:');
    $url = $_GET;
    unset($url['q']);
    $cancel = array(
      'path' => $_GET['q'],
      'query' => $url,
      );
    $description = t('<p>Unsubscribe and Delete actions cannot be undone.</p>') . theme('table', $header, $rows);
    return confirm_form(array(), $question, $cancel, $description);
  }  
}

/**
 * Update list member status.
 */
function vr_list_view_form_submit($form, &$form_state) {
  if (empty($form_state['storage']['values']) AND $form_state['values']['submit'] == 'Save Changes') {
    // if there is no previous values redraw for second step
    $form_state['storage']['values'] = $form_state['values'];
    $form_state['rebuild'] = true;
  }
  else {
    // Execute actions
    $actions = $form_state['storage']['values']['action'];
    $list_id = $form_state['storage']['values']['list_id'];
    foreach ($actions as $status => $members) {
      // It is VERY important that the type is cast properly! 0 will perminantly
      // unsubscribe an individual where as null is the status code for 
      // a subscribed user.
      if (empty($status)) {
        if ($status !== 0) {
          $status = NULL;
        }
      }
      foreach ($members as $hash) {
        $param['list_member'] = array(
          'list_id' => $list_id,
          'list_type' => 'email',
          'member_data' => array(
            array(
              'name' => 'optin_status',
              'value' => $status,
              ),
            array(
              'name' => 'hash',
              'value' => $hash,
              ),
            ),
          );
        // Delete this user if status is -1
        if ($status == -1) {
          vr_api('deleteListMember', $param);
        } 
        else {
          vr_api('editListMember', $param);
        }
      }
    }
    $query = $form_state['storage']['values']['_get'];
    $redirect = $query['q'];
    unset($query['q']);
    // Set redirect link
    $form_state['redirect'] = array($redirect, $query);
    unset($form_state['storage']);
    drupal_set_message(t('This mailing list has been updated.'));
  }
}

/**
 * Provide a form for editing individual member information.
 * 
 * @param array
 *  form_state array from Drupal.
 * @param object
 *  VerticalResponse mailing list member.
 * @return array
 *  Form.
 */
function vr_list_member_edit($form_state, $member) {
  $map = $member->map_fields();
  $form = $member->form();
  $form['_fields'] = array(
    '#type' => 'value',
    '#value' => $member->member_array,
    );
  // Only include submit button if there is no field map
  if ($map == FALSE) {
    drupal_set_message('No field map was located for this user.');
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      );
  }
  return $form;
}

/**
 * Submit handler for vr_list_member_edit
 */
function vr_list_member_edit_submit($form, &$form_state) {
  $old_member = $form_state['values']['_fields'];
  unset($old_member['optin_status']);
  $member = array_intersect_key($form_state['values'], $old_member);
  $path = drupal_get_path('module', 'vr_list');
  include_once($path . '/vr_list_member.inc');
  $member = new vr_list_member($old_member['list_id'], $member);
  $member->save();
  $message = t('List member @email has been updated.', array('@email' => $member->member_array['email_address']));
  drupal_set_message($message);
  $form_state['redirect'] = url('admin/build/vr/list/' . $form_state['values']['list_id'] . '/edit');
}

/**
 * Generate form to add a user to a list.  This could be easily replaced by creating a
 * user account in Drupal.
 */
function vr_list_member_add($form_state, $list) {
  $member = new vr_list_member($list->id);
  $form = $member->form();
  $form['list_id'] = array(
    '#type' => 'value',
    '#value' => $list->id,
    );
  $form['_fields'] = array(
    '#type' => 'value',
    '#value' => $form,
    );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    );
  return $form;
}

/**
 * Add a new member to a list.
 */
function vr_list_member_add_submit($form, &$form_state) {
  $values = $form_state['values'];
  $member = array_intersect_key($form_state['values'], $values['_fields']);
  $list = $form['#parameters'][2];
  $member = new vr_list_member($list->id, $member);
  $member->save();
  $tokens = array(
    '@email' => $member->member_array['email_address'],
    '@list' => $list->name,
    '@list_link' => url('admin/build/vr/list/' . $list->id . '/edit'),
    );
  $message = 'Added @email to @list.  Add another list member or return to <a href="@list_link">@list</a>.';
  drupal_set_message(t($message, $tokens));
}

/**
 * Provide headers for the mailing list table
 */
function vr_list_build_header() {
  $headers = array(
    array(
      'field' => 'name',
      'data' => t('Mailing list name'),
      ),
    array(
      'field' => 'size',
      'data' => t('List size'),
      ),
    array(
      'data' => t('Last mailed'),
      ),
    t('Options'),
    );
  return $headers;
}

/**
 * Allow administrators to hide mailing list subscription options from users.
 * This could be useful for hiding internal lists.
 */
function vr_list_visibility_settings($form_state) {
  $lists = vr_get_lists();
  // Create checkboxes to enable lists.  Lists that are generated through
  // the website will be enabled by default, however lists generated through
  // VerticalResponse will be disabled by default.
  $form['intro'] = array(
    '#type' => 'item',
    '#value' => t('Add a check to the box to "publish" a mailing list. Users will not 
      have the ability to manage their subscription status for "unpublished" lists.'),
    );
  foreach ($lists as $list => $name) {
    $form['lists']['vr_list_enabled_' . $list] = array(
      '#title' => $name,
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => variable_get('vr_list_enabled_' . $list, FALSE),
      );
  }
  return system_settings_form($form);
}

/**
 * Allow administrators to map mailing list fields to user object properites.
 * The user object will be defined by a view that takes a uid as an argument.
 */
function vr_list_member_map($form_state, $list = NULL) {
  // Use default values if no list is provided
  if (empty($list)) {
    $list = _vr_default_list();
  }
  
  // Retrieve views
  $all_views = views_get_all_views();
  foreach ($all_views as $view) {
    // Only 'node' views that have fields will work for our purpose.
    if (!empty($view->display['default']->display_options['fields'])) {
      if ($view->type == 'Default') {
        $views[t('Default Views')][$view->name] = $view->name;
      }
      else {
        $views[t('Existing Views')][$view->name] = $view->name;
      }
    }
  }

  // Look for a view to map.
  $view = variable_get('vr_list_view_' . $list->id, NULL);
  // Allow administrator to select a view with to map list fields.
  $form['view'] = array(
    '#type' => 'fieldset',
    '#title' => t('Local User Object'),
    );
  $form['view']['vr_list_view_' . $list->id] = array(
    '#type' => 'select',
    '#title' => t('Select a view for the user to list field map.'),
    '#description' => t("The selected view should return all the fields required for 
      this list's field map.  It should take one argument uid."),
    '#options' => array(NULL => '-- Select One --') + $views,
    '#default_value' => $view,
    );
  $form['view']['vr_list_subcribe_throttle_' . $list->id] =   array(
    '#type' => 'textfield',
    '#title' => t('Subscription Throttle'),
    '#description' => t('Set a throttle for list members to subscribe in a single 
      batch execution.  This number can be much higher than the update throttle because
      users are subscribed in a single API transaction.'),
    '#default_value' => variable_get('vr_list_subscribe_throttle_' . $list->id, 100),
    );
  $form['view']['vr_list_throttle_' . $list->id] = array(
    '#type' => 'textfield',
    '#title' => t('Update Throttle'),
    '#description' => t('When requested, Drupal will update list member information 
      before sending out an email.  This field allows one to specify the number of records
      to be updated per batch execution.  This number will likely be much lower than
      the subscription throttle because one API call is required per record.'),
    '#default_value' => variable_get('vr_list_throttle_' . $list->id, 10),
    );
  // Provide field mapping objects if and only if a view has been selected and saved.
  $view = variable_get('vr_list_view_' . $list->id, NULL);
  if (!empty($view)) {
    $view = views_get_view($view);
    $form['fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field Map'),
      );
    // Retrieve fields from selected view.  TODO: there must be a better way of doing
    // this!
    $view = $view->load($view->name);
    $fields_raw = $view->display['default']->display_options['fields'];
    foreach ($fields_raw as $name => $field) {
      $fields[$name] = (empty($field['label'])) ? $name : $field['label'];
    }
    // Get default values
    $values = variable_get('vr_list_view_map_' . $list->id, array());
    foreach ($list->fields as $key => $field) {
      // TODO: manage skip fields with a constant.
      $skip = array(
        'id', 
        'optin_status', 
        'optin_status_last_updated', 
        'create_date', 
        'last_updated'
      );
      if (in_array($field, $skip)) {
        continue;
      }
      $title = str_replace('_', ' ', $field);
      $form['fields']['field_map'][$key] = array(
        '#type' => 'select',
        '#title' => ucwords($title),
        '#options' => array('-- Select One --') + $fields,
        '#default_value' => $values[$key],
        );
    }
    $form['fields']['field_map']['#tree'] = TRUE;
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    );
  return $form;
}

/**
 * Submit handler for field mapping utility.
 */
function vr_list_member_map_submit($form, &$form_state) {
  $list = $form['#parameters'][2];
  // Use default values if no list is provided
  if (empty($list)) {
    $list = _vr_default_list();
  }
  
  $values = $form_state['values'];

  $current_view = variable_get('vr_list_view_' . $list->id, NULL);
  $message = t('The settings have been saved. Please configure the fields below.');
  drupal_set_message($message);
  variable_set('vr_list_view_' . $list->id, $values['vr_list_view_' . $list->id]);
  variable_set('vr_list_throttle_' . $list->id, $values['vr_list_throttle_' . $list->id]);
  if ($current_view != $values['vr_list_view_' . $list->id]) {
    // A new view has been selected for this field map.  Clear stale settings.
    variable_set('vr_list_view_map_' . $list->id, array());
  }
  else {
    // A new view has been selected for this field map.  Clear stale settings.
    variable_set('vr_list_view_map_' . $list->id, $values['field_map']);
  }  
}

/**
 * Allow administrators to push local data to VerticalResponse withouth
 * launching a campaign.
 * 
 * Push local data to VerticalResponse.
 */
function vr_list_push($list) {
  $lists = array($list->id);
  vr_list_update_batch($lists);
}

/**
 * Administrators should have the ability to rename VerticalResponse
 * mailing lists.
 */
function vr_list_rename(&$form_state, $list) {
  $form['list_name'] = array(
    '#type' => 'textfield',
    '#title' => t('List Name'),
    '#default_value' => $list->name,
    '#required' => TRUE,
    '#description' => t('Change the name of this mailing list.'),
    );
  $form['list'] = array(
    '#type' => 'value',
    '#value' => $list,
    );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    );
  return $form;
}

/**
 * Push new list name to VerticalResponse.
 */
function vr_list_rename_submit($form, &$form_state) {
  $list = $form_state['values']['list'];
  $param['attribute_name'] = 'name';
  $param['attribute_value'] = $form_state['values']['list_name'];
  $param['list_id'] = $list->id;
  $success = vr_api('editListAttribute', $param);
  if ($success) {
    $message = t('The list name has been updated.');
    drupal_set_message($message);
  }
  else {
    $message = t('The list name was not changed due to an error. Please try again.');
    drupal_set_message($message, 'error');
  }
  $form_state['redirect'] = url('admin/build/vr/list/' . $list->id . '/edit');
}

/**
 * Return a basic VerticalResponse list. With only an id of 'default'
 * and the default fields.  This list can be used when one wants to manage
 * lists generally, without regard to specific lists.
 * 
 * @return object
 *  mock VerticalResponse list object.
 */
function _vr_default_list() {
  $list = (object) NULL;
  $list->id = 'default';
  $list->fields = array(
    'id',
    'create_date',
    'last_updated',
    'optin_status',
    'optin_status_last_updated',
    'email_address',
    'first_name',
    'last_name',
    'address_1',
    'address_2',
    'city',
    'state',
    'postalcode',
    'country',
    'work_phone',
    'home_phone',
    'mobile_phone',
    'fax',
    'gender',
    'marital_status',
    'ip_address',
    'company_name',
    'title',
    'hash',
    'address_hash',
    );
  return $list;
}