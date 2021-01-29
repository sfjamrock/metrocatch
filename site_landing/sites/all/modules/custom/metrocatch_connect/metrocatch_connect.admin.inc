<?php
/**
 * @file
 * Module page callbacks for administation pages only
 */

/**
 * Page callback for 'admin/settings/metrocatch/catches' path - list all catches
 */
function metrocatch_connect_admin_list_catches() {
  // fetch all catches
  $rows = pager_query("SELECT * FROM {metrocatch_connect_catches} c INNER JOIN {users} u ON u.uid=c.uid ", 64);
  $table_rows = array();
  while ($row = db_fetch_object($rows)) {
    $table_rows[] = array(
      date("d-m-Y", strtotime($row->date)),
      l($row->name, 'user/'. $row->uid),
      $row->type,
      $row->sex,
      $row->sex_preference,
      l(t('Delete'), 'admin/metrocatch/catches/delete/'. $row->fid),
    );
  }
  if (empty($table_rows)) {
    $out = t('No MetroCatch features found.');
  }
  else {
    $out = theme('table', array(t('Date'), t('User'), t('Type'), t('Sex'), t('Sexual preferences'), t('Actions')), $table_rows) . theme('pager', array(), 64);
  }
  return $out;
}

/**
 * Page callback for 'admin/metrocatch/catches/delete/%' path - delete one feature
 * @param int $fid feature id
 */
function metrocatch_connect_admin_delete_catch($fid = NULL) {
  if ($fid) {
    db_query("DELETE FROM {metrocatch_connect_catches} WHERE fid=%d", $fid);
    if (($num = db_affected_rows()) > 0) {
      drupal_set_message(t('One record deleted.'));
    }
  }
  drupal_goto('admin/metrocatch/catches/list');
}


/**
 * "Add new catch" form callback
 */
function metrocatch_connect_admin_add_catch_form($form_state) {
  return array(
    'user' => array(
      '#type' => 'textfield',
      '#title' => t('User'),
      '#default_value' => isset($form_state['values']['user']) ? $form_state['values']['user'] : '',
      '#required' => TRUE,
      '#autocomplete_path' => 'user/autocomplete',
    ),
    'type' => array(
      '#type' => 'radios',
      '#title' => t('Type'),
      '#options' => array(METROCATCH_CATCH_TYPE_FEATURE => t('Feature'), METROCATCH_CATCH_TYPE_SUBFEATURE => t('Subfeature')),
      '#default_value' => isset($form_state['values']['type']) ? $form_state['values']['type'] : 'feature',
      '#required' => TRUE,
    ),
    'date' => array(
      '#type' => 'date_select',
      '#title' => t('Date'),
      '#default_value' => isset($form_state['values']['date']) ? $form_state['values']['date'] : NULL,
      '#required' => TRUE,
      '#date_format' => 'm/d/Y',
    ),
    'sex' => array(
      '#type' => 'radios',
      '#title' => t('Sex'),
      '#options' => array(METROCATCH_SEX_MALE => t('Male'), METROCATCH_SEX_FEMALE => t('Female')),
      '#default_value' => isset($form_state['values']['sex']) ? $form_state['values']['sex'] : METROCATCH_SEX_MALE,
      '#required' => TRUE,
    ),
    'sex_preference' => array(
      '#type' => 'radios',
      '#title' => t('Sexual preferences'),
      '#options' => array(METROCATCH_SEX_PREFERENCE_MEN => t('Interested in man'), METROCATCH_SEX_PREFERENCE_WOMEN => t('Interested in women')),
      '#default_value' => isset($form_state['values']['sex_preference']) ? $form_state['values']['sex_preference'] : 'women',
      '#required' => TRUE,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
}

/**
 * Validate callback for Add new catch" form
 */
function metrocatch_connect_admin_add_catch_form_validate($form, $form_state) {
  $v = $form_state['values'];
  $account = user_load(array('name' => $v['user']));
  $data = array(
    'uid' => $account ? $account->uid : 0,
    'type' => $v['type'],
    'sex' => $v['sex'],
    'sex_preference' => $v['sex_preference'],
    'date' => $v['date'],
  );
  if ($error = metrocatch_connect_create_catch_feature($data)) {
    form_error($form, metrocatch_connect_get_last_error());
  }

}

/**
 * Submit handler for Add new catch" form
 */
function metrocatch_connect_admin_add_catch_form_submit(&$form, $form_state) {
  drupal_set_message(t('New "Catch of the day feature" created'));
  $form['#redirect'] = 'admin/metrocatch/catches/list';
}