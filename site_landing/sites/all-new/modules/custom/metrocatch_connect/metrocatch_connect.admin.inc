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
  $rows = pager_query(
    "SELECT c.*, ctp.field_profile_first_name_value AS first_name, ctp.field_profile_last_name_value AS last_name
    FROM {metrocatch_connect_catches} c
    INNER JOIN {users} u ON u.uid=c.uid
    INNER JOIN {node} n ON n.type='profile' AND n.uid=u.uid
    INNER JOIN {content_type_profile} ctp ON ctp.nid=n.nid",
    64
  );
  $table_rows = array();
  while ($row = db_fetch_object($rows)) {
    $table_rows[] = array(
      date("d-m-Y", strtotime($row->date)),
      l($row->name, 'user/'. $row->uid),
      $row->first_name,
      $row->last_name,
      $row->type,
      $row->sex,
      $row->sex_preference,
      l(t('Delete'), 'admin/metrocatch/catches/delete/'. $row->fid),
    );
  }
  if (empty($table_rows)) {
    $out = t('No metrocatch features found.');
  }
  else {
    $out = theme('table', array(t('Date'), t('User'), t('First Name'), t('Last Name'), t('Type'), t('Sex'), t('Sexual preferences'), t('Actions')), $table_rows) . theme('pager', array(), 64);
  }
  return $out;
}

/**
 * Page callback for 'admin/metrocatch/catches/delete/%' path - delete one feature
 * @param int $fid feature id
 */
function metrocatch_connect_admin_delete_catch($fid) {
  db_query("DELETE FROM {metrocatch_connect_catches} WHERE fid=%d", $fid);
  drupal_set_message(format_plural(db_affected_rows(), '1 row deleted.', '@count rows deleted.'));
  drupal_goto('admin/metrocatch/catches/list');
}


/**
 * "Add new catch" form callback
 */
function metrocatch_connect_admin_add_catch_form($form_state) {
  $now = time();
  return array(
    'user' => array(
      '#type' => 'textfield',
      '#title' => t('Select user'),
      '#default_value' => isset($form_state['values']['user']) ? $form_state['values']['user'] : '',
      '#autocomplete_path' => 'user/autocomplete',
    ),
    'new_account' => array(
      '#type' => 'fieldset',
      '#title' => t('Or create new account'),
      'email' => array(
        '#type' => 'textfield',
        '#title' => t('Email'),
      ),
      'create_for_each_feature_day' => array(
        '#type' => 'checkbox',
        '#title' => t('Create new user for each feature day'),
        '#default_value' => FALSE,
      ),
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
      '#default_value' => isset($form_state['values']['date']) ? $form_state['values']['date'] : array('year' => date('Y', $now), 'month' => date('m', $now), 'day' => date('d', $now)),
      '#required' => TRUE,
      '#date_format' => 'm/d/Y',
    ),
    'date_end' => array(
      '#type' => 'date_select',
      '#title' => t('Feature ends on'),
      '#default_value' => isset($form_state['values']['date_end']) ? $form_state['values']['date_end'] : array('year' => date('Y', $now), 'month' => date('m', $now), 'day' => date('d', $now)),
      '#date_format' => 'm/d/Y',
    ),
    'sex' => array(
      '#type' => 'radios',
      '#title' => t('Sex'),
      '#options' => array(METROCATCH_SEX_MAN => t('Man'), METROCATCH_SEX_WOMAN => t('Woman')),
      '#default_value' => isset($form_state['values']['sex']) ? $form_state['values']['sex'] : METROCATCH_SEX_MAN,
      '#required' => TRUE,
    ),
    'sex_preference' => array(
      '#type' => 'radios',
      '#title' => t('Sexual preferences'),
      '#options' => array(METROCATCH_SEX_PREFERENCE_MEN => t('Interested in men'), METROCATCH_SEX_PREFERENCE_WOMEN => t('Interested in women')),
      '#default_value' => isset($form_state['values']['sex_preference']) ? $form_state['values']['sex_preference'] : METROCATCH_SEX_PREFERENCE_WOMEN,
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
  if (!empty($v['user'])) {
    $account = user_load(array('name' => $v['user']));
  }
  else {
    $account = _metrocatch_connect_create_featured_user($v);
  }
  $data = array(
    'uid' => $account ? $account->uid : 0,
    'type' => $v['type'],
    'sex' => $v['sex'],
    'sex_preference' => $v['sex_preference'],
    'date' => $v['date'],
  );
  $from = strtotime($v['date']);
  $till = strtotime($v['date_end']);
  if ($till < $from) {
    form_error($form['date_end'], t('Feature can not end before it starts.'));
  }
  else {
    for ($date = $from; $date <= $till; $date = $date + 86400) {
      $data['date'] = date('Y-m-d H:i:s', $date);
      if (!empty($v['create_for_each_feature_day']) && !isset($account)) {
        $account = _metrocatch_connect_create_featured_user($v);
        $data['uid'] = $account->uid;
      }
      if ($error = metrocatch_connect_create_catch_feature($data)) {
        form_error($form, metrocatch_connect_get_last_error());
      }
      unset($account);
    }
  }
}

/**
 * Submit handler for Add new catch" form
 */
function metrocatch_connect_admin_add_catch_form_submit(&$form, $form_state) {
  drupal_set_message(t('New "Catch of the day feature" created'));
  $form['#redirect'] = 'admin/metrocatch/catches/list';
}

/**
 * Page callback.
 *
 * Path: metrocatch/connections, metrocatch/connections/overview
 *
 * Returns themed table to overview all connections between users.
 */
function metrocatch_connect_connections_overview_admin() {
  $header = array(
    'cid' => array(
      'data' => '#',
      'field' => 'c.cid',
      'sort' => 'desc',
    ),
    'from' => array(
      'data' => t('From user'),
    ),
    'to' => array(
      'data' => t('To user'),
    ),
    'type' => array(
      'data' => t('Type'),
      'field' => 'c.type',
    ),
    'created' => array(
      'data' => t('Initiated'),
      'field' => 'c.created',
    ),
    'actions' => array(
      'data' => t('Actions'),
    ),
  );
  $result = pager_query(
    'SELECT c.*, u.name AS name_from, u2.name AS name_to FROM {metrocatch_connect_connection} c
     LEFT JOIN {users} u ON u.uid = c.uid_from
     LEFT JOIN {users} u2 ON u2.uid = c.uid_to'. tablesort_sql($header), 32);
  $rows = array();
  while ($connection = db_fetch_object($result)) {
    $rows[] = array(
      'cid' => $connection->cid,
      'from' => l($connection->name_from, 'user/' . $connection->uid_from),
      'to' => l($connection->name_to, 'user/' . $connection->uid_to),
      'type' => metrocatch_connect_get_connection_type_name($connection->type),
      'created' => format_date($connection->created),
      'actions' => l(t('Delete'), "admin/metrocatch/connections/{$connection->cid}/delete", array('query' => drupal_get_destination())),
    );
  }
  return empty($rows)
    ? t('No connections found yet.')
    : (theme('table', $header, $rows) . theme('pager'));
}

/**
 * Form constructor for the metrocatch_connect_add_connection_form.
 *
 * Path: metrocatch/connections/add
 */
function metrocatch_connect_add_connection_form(&$form_state) {
  return array(
    'connection' => array(
      '#tree' => TRUE,
      'from' => array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => t('User from'),
        '#autocomplete_path' => 'user/autocomplete',
      ),
      'to' => array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => t('User to'),
        '#autocomplete_path' => 'user/autocomplete',
      ),
      'type' => array(
        '#type' => 'select',
        '#title' => t('Type of connection'),
        '#options' => metrocatch_connect_get_all_connection_types(),
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Add connection'),
      ),
    ),
  );
}

/**
 * Form submission handler for metrocatch_connect_add_connection_form().
 *
 * @see metrocatch_connect_add_connection_form()
 */
function metrocatch_connect_add_connection_form_submit($form, &$form_state) {
  $values = $form_state['values']['connection'];
  $uid_to = db_result(db_query("SELECT uid FROM {users} WHERE name = '%s'", $values['to']));
  $uid_from = db_result(db_query("SELECT uid FROM {users} WHERE name = '%s'", $values['from']));
  // Expiration time will be calculated in metrocatch_connect_save_connection().
//   $feature_duration = metrocatch_connect_get_longest_feature_expiration($connection['uid_to']);
//   $time_to_approve  = time() + (variable_get('metrocatch_connect_days_to_approve_connection', 3) * 86400);
//   if (!empty($feature_duration)) {
//     $expiration_time = min(array($feature_duration, $time_to_approve));
//   }
//   else {
//     $expiration_time = $time_to_approve;
//   }
  $connection = array(
    'uid_to' => $uid_to,
    'uid_from' => $uid_from,
    'requester' => $uid_from,
    'type' => $values['type'],
//     'expiration_time' => $expiration_time,
  );
  metrocatch_connect_save_connection($connection);
  drupal_set_message(t('Connection saved'));
}

/**
 * Form constructor for the metrocatch_connect_delete_connection_form form.
 *
 * Path: admin/metrocatch/connections/%metrocatch_connect_connection/delete
 *
 * Deletes a connection between users.
 * @param $form_state
 * @param $connection
 */
function metrocatch_connect_delete_connection_form(&$form_state, $connection) {
  $form['connection'] = array(
    '#type' => 'value',
    '#value' => $connection,
  );
  $redirect = $form['#redirect'] = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : 'admin/metrocatch/connections/overview';
  return confirm_form($form, t('Are you sure you want to delete this connection?'), $redirect, t('This action can\'t be undone.'), t('Delete connection'));
}

/**
 * Form submit handler for a metrocatch_connect_delete_connection_form form.
 *
 * @see metrocatch_connect_delete_connection_form()
 */
function metrocatch_connect_delete_connection_form_submit($form, &$form_state) {
  $connection = $form_state['values']['connection'];
  metrocatch_connect_remove_connection($connection);
  drupal_set_message(t('Connection was deleted.'));
}

/**
 * Form constructor for the metrocatch_connect_connections_settings_form form.
 *
 * Path: admin/metrocatch/connections/settings
 */
function metrocatch_connect_connections_settings_form(&$form_state) {
  $form = array(
    'metrocatch_connect_feature_text' => array(
      '#type' => 'textarea',
      '#title' => t('Marketing text displayed on the Catch of the Day page'),
      '#default_value' => variable_get('metrocatch_connect_feature_text', ''),
    ),
//     'metrocatch_connect_catches_per_line' => array(
//       '#type' => 'textfield',
//       '#title' => t('Catches per line in the Other Catches block'),
//       '#default_value' => variable_get('metrocatch_connect_catches_per_line', 3),
//     ),
    'metrocatch_connect_archive_period' => array(
      '#type' => 'textfield',
      '#title' => t('The number of days for which the archive catches will be displayed'),
      '#default_value' => variable_get('metrocatch_connect_archive_period', 30),
    ),
//     'metrocatch_connect_catches_archieve_per_page' => array(
//       '#type' => 'textfield',
//       '#title' => t('Catches per page in the Other Catches Archieve'),
//       '#default_value' => variable_get('metrocatch_connect_catches_archieve_per_page', 10),
//     ),
    'metrocatch_connect_feature_duration' => array(
      '#type' => 'textfield',
      '#description' => t('In seconds'),
      '#title' => t('Catch of the day feature duration'),
      '#default_value' => variable_get('metrocatch_connect_feature_duration', 2592000),
    ),
    'metrocatch_connect_subfeature_duration' => array(
      '#type' => 'textfield',
      '#description' => t('In seconds'),
      '#title' => t('Catch of the day subfeature duration'),
      '#default_value' => variable_get('metrocatch_connect_subfeature_duration', 1296000),
    ),
    'metrocatch_connect_purchase_feature_text' => array(
      '#type' => 'textarea',
      '#title' => t('Top text on "Become a metrocatch" page'),
      '#default_value' => variable_get('metrocatch_connect_purchase_feature_text', ''),
    ),
    'metrocatch_connect_purchase_connect_text' => array(
      '#type' => 'textarea',
      '#title' => t('Top text on "Purchase Connect" page'),
      '#default_value' => variable_get('metrocatch_connect_purchase_connect_text', ''),
    ),
    'metrocatch_connect_days_to_approve_connection' => array(
      '#type' => 'textfield',
      '#title' => t('Number of days to approve connection request'),
      '#default_value' => variable_get('metrocatch_connect_days_to_approve_connection', 3),
    ),
    'metrocatch_connect_featured_slot_empty_text' => array(
      '#type' => 'textarea',
      '#title' => t('Marketing text when no users are featured'),
      '#description' => t('This text will be displayed in the featured slot on the front page if no catches are available today.'),
      '#default_value' => variable_get('metrocatch_connect_featured_slot_empty_text', 'No Catches are available today.'),
    ),
    'facebook_share_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Facebook share settings'),
      'metrocatch_connect_facebook_share_text' => array(
        '#type' => 'textarea',
        '#title' => t('Summary of a shared catch'),
        '#description' => t('This text will be shared when user clicks a Share button.<br/>For the Featured users this setting will be ignored and theirs marketing text will be shared.'),
        '#default_value' => variable_get('metrocatch_connect_facebook_share_text', ''),
      ),
    ),
    'tweet_settings' => array(
      '#type' => 'fieldset',
      '#title' => t('Tweet settings'),
      'metrocatch_connect_tweet_text' => array(
        '#type' => 'textarea',
        '#title' => t('Default tweet text'),
        '#description' => t('This text will be tweeted when user clicks a Tweet button.'),
        '#default_value' => variable_get('metrocatch_connect_tweet_text', ''),
      ),
      'metrocatch_connect_tweet_via' => array(
        '#type' => 'textfield',
        '#title' => t('Via'),
        '#description' => t('Name of the user to attribute the Tweet to'),
        '#default_value' => variable_get('metrocatch_connect_tweet_via', variable_get('site_name', 'metrocatch')),
      ),
    ),
  );
  return system_settings_form($form);
}

/**
 * Helper function: get random first name.
 */
function _metrocatch_connect_get_user_first_name($sex) {
  $first_names = array(
    'man' => array(
      'Alexandr', 'Aleksey', 'Avraam', 'Adolf', 'Boris', 'Bogdan', 'German', 'Gregoryi', 'Garry', 'Daniil', 'Dmitry', 'Egor', 'Evgenyi', 'Emelya', 'Zahar',
      'Ivan', 'Josef', 'Kirill', 'Konstantin', 'Leonid', 'Mikhail', 'Matvey', 'Nikita', 'Oleg', 'Pavel',
    ),
    'woman' => array(
      'Alexandra', 'Anya', 'Anastasya', 'Veronika', 'Viktorya', 'Galina', 'Dasha', 'Diana', 'Elena', 'Ekaterina', 'Evgenya', 'Elizaveta', 'Janna', 'Zinaida',
      'Irina', 'Inna', 'Ksenya', 'Klara', 'Kristina', 'Lubov', 'Ludmila', 'Marina', 'Nina', 'Nadejda', 'Oksana', 'Olga',
    ),
  );
  $random = array_rand($first_names[$sex]);
  return $first_names[$sex][$random];
}

/**
 * Helper function: get random last name.
 */
function _metrocatch_connect_get_user_last_name($sex) {
  $last_names = array(
    'Aleskandrov', 'Bogdanov', 'Varlamov', 'Gerasimov', 'Darimov', 'Emelyanov', 'Jilov', 'Zarimov', 'Ivlev', 'Karimov', 'Larin', 'Mariinov', 'Novikov', 'Obukhov', 'Petrov',
  );
  $suffix = ($sex == 'woman' ? 'a' : '');
  $random = array_rand($last_names);
  return $last_names[$random] . $suffix;
}

/**
 * Helper function: creates user account.
 *
 * @param $v
 *   An array of $form_state values.
 */
function _metrocatch_connect_create_featured_user($v) {
  $username = $v['sex'][0] . $v['sex_preference'][0];
  $last_in_this_category = db_result(db_query_range("SELECT name FROM {users} WHERE name LIKE '%s%%' ORDER BY CAST(SUBSTR(name, 3) AS unsigned) DESC", $username, 0, 1));
  $suffix = ($last_in_this_category ? str_replace($username, '', $last_in_this_category) + 1 : 1);
  $email_parts = explode('@', $v['email']);
  $data = array(
    'name' => $username . $suffix,
    'mail' =>  $email_parts[0] .'+'. $username . $suffix .'@'. $email_parts[1],
    'pass' => '123',
    'status' => 1,
  );
  if ($account = user_save(NULL, $data)) {
    db_query("UPDATE {users} SET name = '%s' WHERE uid = %d", $data['name'], $account->uid);
    $profile = (object) array(
      'type' => 'profile',
      'title' => $data['name'],
      'uid' => $account->uid,
      'status' => 1,
      'format' => FILTER_FORMAT_DEFAULT,
      'field_profile_first_name' => array(
        array('value' => _metrocatch_connect_get_user_first_name($v['sex'])),
      ),
      'field_profile_last_name' => array(
        array('value' => _metrocatch_connect_get_user_last_name($v['sex'])),
      ),
      'field_profile_sex' => array(
        array('value' => $v['sex']),
      ),
      'field_profile_sex_preference' => array(
        array('value' => $v['sex_preference']),
      ),
      'field_profile_birth_date' => array(
        array('value' => strtotime(implode('.', array(rand(1, 28), rand(1, 12), rand(1970, 1990))))),
      ),
      'field_profile_connections_left' => array(
        array('value' => array_rand(range(1, 10))),
      ),
      'field_profile_public_connections' => array(
        array('value' => rand(0, 1)),
      ),
    );
    node_save($profile);
  }
  return $account;
}