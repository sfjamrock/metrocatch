<?php
/**
 * @file
 * menu callbacks for administration pages
 */

/**
 * "Delete all pages" form callback.
 */
function cyd_utils_delete_all_pages_form($form_state) {
  return array(
    '#submit' => array('cyd_utils_delete_all_pages_form_submit'),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Clear all page data'),
    ),
  );
}

/**
 * Submit handler for "Delete all pages" form.
 * Deletes all page data from database.
 */
function cyd_utils_delete_all_pages_form_submit($form, &$form_state) {
  db_query('TRUNCATE TABLE {cyd_utils_pages}');
  db_query('TRUNCATE TABLE {cyd_utils_page_info}');
  db_query('TRUNCATE TABLE {cyd_utils_page_tags}');
  db_query('TRUNCATE TABLE {cyd_utils_prints}');
  drupal_set_message(t('All page request records has been deleted.'));
}

/**
 * Menu callback for 'admin/settings/cyd-utils/pages'.
 * Prints a list of recent opened pages.
 */
function cyd_utils_pages_list() {
  $filter_form = drupal_get_form('cyd_utils_pages_filter_form');

  drupal_add_css(drupal_get_path('module', 'cyd_utils') .'/cyd_utils.css');
  drupal_set_title(t('Pages list'));
  $table_header = array(
    array('data' => t('Path'), 'field' => 'path'),
    array('data' => t('User')),
    array('data' => t('Language'), 'field' => 'language'),
    array('data' => t('Request time'), 'field' => 'request_time', 'sort' => 'desc'),
    array('data' => t('Page time, s'), 'field' => 'execution_time'),
    array('data' => t('IP address'), 'field' => 'ip_address'),
    array('data' => t('Actions')),
  );
  $include_conditions = array();
  $exclude_conditions = array();
  $query_args = array();
  $page_args = $_GET;

  if (isset($page_args['path'])) {
    $include_conditions[] = "cup.path LIKE '%%%s%%'";
    $query_args[] = $_GET['path'];
  }

  if (!empty($page_args['itags'])) {
    $subconditions = array();
    if (($empty_key = array_search('not_set', $page_args['itags'], TRUE)) !== FALSE) {
      $subconditions[] = "ti.tag IS NULL";
      unset($page_args['itags'][$empty_key]);
    }
    if (!empty($page_args['itags'])) {
      $subconditions[] = "ti.tag IN (". db_placeholders($page_args['itags'], 'varchar') .")";
      $query_args = array_merge($query_args, $page_args['itags']);
    }
    $include_conditions[] = ' ('. implode(' OR ', $subconditions) .') ';
  }

  if (!empty($page_args['etags'])) {
    $subconditions = array();
    if (($empty_key = array_search('not_set', $page_args['etags'], TRUE)) !== FALSE) {
      $include_conditions[] = "ti.tag IS NOT NULL";
      unset($page_args['etags'][$empty_key]);
    }
    if (!empty($page_args['etags'])) {
      $subconditions[] = " (te.tag IN (". db_placeholders($page_args['etags'], 'varchar') ."))";
      $query_args = array_merge($page_args['etags'], $query_args);
      $exclude_conditions[] = ' ('. implode(' OR ', $subconditions) .') ';
      $include_conditions[] = "te.tag IS NULL";
    }
  }

  $query = pager_query(
    "SELECT cup.path, cup.full_path, u.name, cup.language, cup.uid, cup.pid, cup.request_time, cup.execution_time, cup.ip_address
    FROM {cyd_utils_pages} cup
    LEFT JOIN {cyd_utils_page_tags} ti ON ti.pid = cup.pid\n".
    (empty($exclude_conditions) ? '' : (" LEFT JOIN {cyd_utils_page_tags} te ON te.pid = cup.pid AND ". implode(' AND ', $exclude_conditions) .' ')).
    "\nLEFT JOIN {users} u ON cup.uid=u.uid".
    (empty($include_conditions) ? '' : ("\nWHERE ". implode(' AND ', $include_conditions))).
    " GROUP BY cup.pid ".
    tablesort_sql($table_header),
    50,
    0,
    "SELECT COUNT(*) FROM (
      SELECT cup.path, cup.full_path, u.name, cup.language, cup.uid, cup.pid, cup.request_time, cup.execution_time, cup.ip_address
      FROM {cyd_utils_pages} cup
      LEFT JOIN {cyd_utils_page_tags} ti ON ti.pid = cup.pid\n".
      (empty($exclude_conditions) ? '' : (" LEFT JOIN {cyd_utils_page_tags} te ON te.pid = cup.pid AND ". implode(' AND ', $exclude_conditions) .' ')).
      "LEFT JOIN {users} u ON cup.uid=u.uid".
      (empty($include_conditions) ? '' : ("\nWHERE ". implode(' AND ', $include_conditions))).
      " GROUP BY cup.pid
      ) tmp",
    $query_args
  );
  ////////////////
  $debug_query =
    "SELECT cup.path, cup.full_path, u.name, cup.language, cup.uid, cup.pid, cup.request_time, cup.execution_time, cup.ip_address
    FROM {cyd_utils_pages} cup
    LEFT JOIN {cyd_utils_page_tags} ti ON ti.pid = cup.pid\n".
    (empty($exclude_conditions) ? '' : (" LEFT JOIN {cyd_utils_page_tags} te ON te.pid = cup.pid AND ". implode(' AND ', $exclude_conditions) .' ')).
    "\nLEFT JOIN {users} u ON cup.uid=u.uid".
    (empty($include_conditions) ? '' : ("\nWHERE ". implode(' AND ', $include_conditions))).
    " GROUP BY cup.pid ".
    tablesort_sql($table_header);
  _db_query_callback($query_args, TRUE);
  dpm(preg_replace_callback(DB_QUERY_REGEXP, '_db_query_callback', $debug_query));
  ////////////////
  $rows = array();
  $user_access = user_access('administer users');
  while ($result = db_fetch_array($query)) {
    if (!$result['uid']) {
      $result['name'] = variable_get('anonymous', 'Anonymous');
    }
    elseif ($user_access) {
      $result['name'] = l($result['name'], 'user/'. $result['uid']);
    }
    $result['path'] = l(truncate_utf8($result['path'], 75, FALSE, TRUE), $result['path'], array('attributes' => array('title' => $result['full_path'])));
    $page_tags = cyd_utils_get_page_tags($result['pid']);
    if (!empty($page_tags)) {
      $result['path'] .= theme('cyd_utils_page_tags', $page_tags);
    }
    $result['execution_time'] = cyd_utils_format_time($result['execution_time']);
    $result['action'] = l('view', 'admin/settings/cyd-utils/pages/'. $result['pid']);
    $result['request_time'] = cyd_utils_format_date($result['request_time']);
    unset($result['uid'], $result['full_path'], $result['pid']);
    $rows[] = $result;
  }

  global $pager_total_items;
  return theme(
    'cyd_utils_pages_list_page',
    $filter_form,
    theme('table', $table_header, $rows) . theme('pager', array(), 50),
    drupal_get_form('cyd_utils_delete_all_pages_form'),
    t('Found @number pages.', array('@number' => $pager_total_items[0]))
  );
}

/**
 * Callback for filter form on "pages" page.
 */
function cyd_utils_pages_filter_form($form_state) {
  // fetch all tags
  $existing_tags = array();
  $rows = db_query("SELECT DISTINCT(tag) FROM {cyd_utils_page_tags}");
  while ($tag = db_result($rows)) {
    $existing_tags[$tag] = $tag;
  }
  $tag_options = array();
  $tag_options['not_set'] = t('- untagged -');
  $tag_options = array_merge($tag_options, $existing_tags);
  return array(
    'path' => array(
      '#type' => 'textfield',
      '#title' => t('Request path'),
      '#default_value' => isset($_GET['path']) ? $_GET['path'] : '',
    ),
    'itags' => array(
      '#title' => t('Include tags'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $tag_options,
      '#default_value' => isset($_GET['itags']) ? $_GET['itags'] : array(),
    ),
    'etags' => array(
      '#title' => t('Exclude tags'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $tag_options,
      '#default_value' => isset($_GET['etags']) ? $_GET['etags'] : array(),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Filter'),
    ),
    'reset' => array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#name' => 'reset',
    ),
  );
}

/**
 * Submit handler for "pages" filter form.
 */
function cyd_utils_pages_filter_form_submit(&$form, &$form_state) {
  if ($form_state['clicked_button']['#name'] == 'reset') {
    $form['#redirect'] = $_GET['q'];
    return;
  }
  $keys = array('path', 'itags', 'etags');
  $args = array();
  foreach ($keys as $key) {
    if (!empty($form_state['values'][$key])) {
      if (is_array($form_state['values'][$key])) {
        foreach ($form_state['values'][$key] as $value) {
          $args[] = urlencode($key) .'[]='. urlencode($value);
        }
      }
      else {
        $args[] = urlencode($key) .'='. urlencode($form_state['values'][$key]);
      }
    }
  }
  unset($args['q']);
  $form_state['redirect'] = array($_GET['q'], implode('&', $args));
}

/**
 * Menu callback for 'admin/settings/cyd-utils/pages/%cyd_utils_page' path.
 * Display one record of page data.
 *
 * @param array $form_state
 *   form state
 * @param object $page
 *   loaded cyd_utils page object
 */
function cyd_utils_page_view_form($form_state, $page) {
  drupal_add_css(drupal_get_path('module', 'cyd_utils') .'/cyd_utils.css');
  if (!$page->uid) {
    $user = variable_get('anonymous', 'Anonymous');
  }
  elseif (user_access('administer users')) {
    $user = l($page->name, 'user/'. $page->uid);
  }
  else {
    $user = $page->name;
  }
  $page_tags = cyd_utils_get_page_tags($page->pid);
  $form = array(
    'path' => array(
      '#type' => 'item',
      '#title' => t('Path'),
      '#value' => l($page->path, $page->path),
    ),
    'full_path' => array(
      '#type' => 'item',
      '#title' => t('Full path'),
      '#value' => l($page->full_path, $page->full_path, array('absolute' => TRUE)),
    ),
    'user' => array(
      '#type' => 'item',
      '#title' => t('User'),
      '#value' => $user,
    ),
    'tags' => array(
      '#title' => t('Page tags'),
      '#type' => 'item',
      '#value' => empty($page_tags) ? t('No tags.') : implode(', ', $page_tags),
    ),
    'language' => array(
      '#type' => 'item',
      '#title' => t('Language'),
      '#value' => $page->language,
    ),
    'request_time' => array(
      '#type' => 'item',
      '#title' => t('Request time'),
      '#value' => cyd_utils_format_date($page->request_time, 'medium'),
    ),
    'execution_time' => array(
      '#type' => 'item',
      '#title' => t('Page time, s'),
      '#value' => cyd_utils_format_time($page->execution_time),
    ),
    'ip_address' => array(
      '#type' => 'item',
      '#title' => t('IP address'),
      '#value' => $page->ip_address,
    ),
  );
  $page_prints = cyd_utils_get_page_prints($page->pid);
  if (!empty($page_prints)) {
    $form['page_prints'] = array(
      '#type' => 'fieldset',
      '#title' => t('Page prints'),
    );
    foreach ($page_prints as $print) {
      $print_item_title = '['. cyd_utils_format_date($print->time) .']';
      if ($print->title) {
        $print_item_title .= ' '. $print->title;
      }
      $form['page_prints'][] = array(
        '#type' => 'item',
        '#title' => $print_item_title,
        '#value' => cyd_utils_print($print->data),
      );
    }
  }
  $info_types = cyd_utils_get_page_info_types();
  $contexts = array();
  foreach ($page->info as $info_item) {
    $contexts[$info_item->context][] = $info_item;
  }
  $info_form = array();
  foreach ($contexts as $context_name => $context) {
    $context_element = array(
      '#type' => 'fieldset',
      '#title' => t('Information collected on @context_name', array('@context_name' => $context_name)),
    );
    foreach ($context as $info_item) {
      $context_element['info_item_'. $info_item->iid] = array(
        '#type' => 'item',
        '#title' => $info_types[$info_item->type]['name'],
        '#value' => cyd_utils_print($info_item->data),
      );
    }
    $form['context_'. $context_name] = $context_element;
  }
  // fetch related log records
  $logs = cyd_utils_get_page_log_records($page->pid);
  if (!empty($logs)) {
    $form['log_records'] = array(
      '#type' => 'fieldset',
      '#title' => t('Logs collected on the page'),
      'table' => array(
        '#type' => 'item',
        '#value' => theme('cyd_utils_log_list', $logs),
      ),
    );
  }
  return $form;
}

/**
 * Page callback for 'admin/settings/cyd-utils/logs' path.
 * Display list of recent logs.
 */
function cyd_utils_logs() {
  $form = drupal_get_form('cyd_utils_logs_list_form');
  $clear_form = drupal_get_form('cyd_utils_logs_clear_form');
  drupal_set_title(t('Logs list'));
  $filters = array();
  foreach (array('source', 'type', 'subtype') as $key) {
    if (isset($_GET[$key])) {
      $filters[$key] = $_GET[$key];
    }
  }
  if (isset($_GET['is_warning']) && $_GET['is_warning'] != 'both') {
    $filters['is_warning'] = array(intval($_GET['is_warning']));
  }
  $result = cyd_utils_logs_list($filters);
  return theme('cyd_utils_log_list_page', $form, $result['list'], $clear_form, t('Found @number log entries.', array('@number' => $result['total'])));
}

/**
 * Get themed log list, filtered by conditions.
 *
 * @param array $filters
 *   Array with structure ($db_column => $value).
 * @return string
 *   Themed table representation.
 */
function cyd_utils_logs_list($filters = array()) {
  //$log_types = module_invoke_all('log_sources');
  $conditions = array();
  $args = array();
  foreach ($filters as $column => $values) {
    $subconditions = array();
    if (($null_key = array_search('not_set', $values, TRUE)) !== FALSE) {
      unset($values[$null_key]);
      $subconditions[] = 'ISNULL('. $column .')';
    }
    if (!empty($values)) {
      $subconditions[] = $column .' IN('. db_placeholders($values, ($column == 'is_warning' ? 'int' : 'varchar')) .')';
    }
    $conditions[] = '('. implode(' OR ', $subconditions) . ')';
    $args = array_merge($args, $values);
  }
  $query = pager_query("SELECT lid, source, is_warning, log_time, message, page, type, subtype FROM {cyd_utils_logs} ". (!empty($conditions) ? ' WHERE '. implode(' AND ', $conditions) : '') ." ORDER BY lid DESC", 50, 0, NULL, $args);
  ////////////////
  $debug_query = "SELECT lid, source, is_warning, log_time, message, page, type, subtype FROM {cyd_utils_logs} ". (!empty($conditions) ? ' WHERE '. implode(' AND ', $conditions) : '') ." ORDER BY lid DESC";
  _db_query_callback($args, TRUE);
  dpm(preg_replace_callback(DB_QUERY_REGEXP, '_db_query_callback', $debug_query));
  ////////////////
  $log_records = array();
  while ($record = db_fetch_object($query)) {
    $log_records[] = $record;
  }
  global $pager_total_items;
  return array(
    'list' => theme('cyd_utils_log_list', $log_records) . theme('pager'),
    'total' => $pager_total_items[0],
  );
}

/**
 * Filter form callback for "logs" page.
 */
function cyd_utils_logs_list_form($form_state) {
  drupal_add_css(drupal_get_path('module', 'cyd_utils') .'/cyd_utils.css');
  // select available sources
  $log_sources = module_invoke_all('log_sources');;
  $rows = db_query("SELECT DISTINCT(source) FROM {cyd_utils_logs}");
  $available_sources = array();
  while ($source = db_fetch_object($rows)) {
    if (!isset($source->source)) {
      $available_sources['not_set'] = t('- not set -');
    }
    elseif (isset($log_sources[$source->source])) {
      $available_sources[$source->source] = t($log_sources[$source->source]['description']);
    }
    else {
      $available_sources[$source->source] = t($source->source);
    }
  }

  // '_' suffix needs, because html select can't distinguish zero-value from <not-selected> case
  $available_severities = array('both' => t('Both'), '0_' => t('Error'), '1_' => t('Warning'));
  // select available types
  $rows = db_query("SELECT DISTINCT(type) FROM {cyd_utils_logs}");
  $available_types = array();
  while ($type = db_fetch_object($rows)) {
    if (isset($type->type)) {
      $available_types[$type->type] = $type->type;
    }
    else {
      $available_types['not_set'] = t('- not set -');
    }
  }

  // select available subtype
  $rows = db_query("SELECT DISTINCT(subtype) FROM {cyd_utils_logs}");
  $available_subtypes = array();
  while ($subtype = db_fetch_object($rows)) {
    if (isset($subtype->subtype)) {
      $available_subtypes[$subtype->subtype] = $subtype->subtype;
    }
    else {
      $available_subtypes['not_set'] = t('- not set -');
    }

  }

  $values = array();
  foreach (array('source', 'type', 'subtype', 'is_warning') as $key) {
    $values[$key] = isset($_GET[$key]) ? $_GET[$key] : '';
  }
  return array(
    'source' => array(
      '#title' => t('Source'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $available_sources,
      '#default_value' => $values['source'],
    ),
    'type' => array(
      '#title' => t('Type'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $available_types,
      '#default_value' => $values['type'],
    ),
    'subtype' => array(
      '#title' => t('Subtype'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $available_subtypes,
      '#default_value' => $values['subtype'],
    ),
    'is_warning' => array(
      '#title' => t('Severity'),
      '#type' => 'radios',
      '#multiple' => TRUE,
      '#options' => $available_severities,
      '#default_value' => isset($values['is_warning']) ? $values['is_warning'] : 'both',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Filter'),
    ),
    'reset' => array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#name' => 'reset',
    ),
  );
}

/**
 * Submit handler for log list filter form.
 */
function cyd_utils_logs_list_form_submit(&$form, &$form_state) {
  if ($form_state['clicked_button']['#name'] == 'reset') {
    $form['#redirect'] = $_GET['q'];
    return;
  }
  $args = array();
  if (isset($_GET['page'])) {
    $args['pager'] = $_GET['page'];
  }
  $args = array();
  foreach (array('source', 'type', 'subtype') as $key) {
    $value = $form_state['values'][$key];
    if (!empty($value)) {
      foreach ($value as $val) {
        $args[] = $key .'[]='. $val;
      }
    }
  }
  if (!empty($form_state['values']['is_warning'])) {
    $args[] = 'is_warning='. $form_state['values']['is_warning'];
  }
  $form['#redirect'] = array(
    $_GET['q'],
    implode('&', $args),
  );
}

/**
 * Form callback for "clear log data" form.
 */
function cyd_utils_logs_clear_form() {
  return array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Clear all log data'),
    ),
  );
}

/**
 * Submit handler for "clear log data" form.
 */
function cyd_utils_logs_clear_form_submit() {
  db_query("TRUNCATE TABLE {cyd_utils_logs}");
  drupal_set_message(t('All log records have been deleted.'));
}

/**
 * "View log record" form callback - used on "log" page.
 * ('/admin/settings/cyd-utils/logs/%cyd_utils_log' path).
 *
 * @param array $form_state
 *   Form state.
 * @param object $log
 *   Loaded cyd_utils log record.
 */
function cyd_utils_log_view_form($form_state, $log) {
  drupal_set_title(t('Log details'));
  $log_sources = module_invoke_all('log_sources');
  $backtrace = $log->data['backtrace'];
  unset($log->data['backtrace']);
  if (!isset($log_sources[$log->source])) {
    $form = array(
      'type' => array(
        '#type' => 'markup',
        '#value' => t('Unable to display log of type %type.', array('%type' => $log->type)),
      ),
      'time' => array(
        '#type' => 'item',
        '#title' => t('Time'),
        '#value' => cyd_utils_format_date($log->log_time),
      ),
      'description' => array(
        '#type' => 'item',
        '#title' => t('Description'),
        '#value' => $log->message,
      ),
      'contents' => array(
        '#type' => 'item',
        '#title' => t('Logged data'),
        '#value' => cyd_utils_print($log->data),
      ),
    );
  }
  else {
    $form = array(
      'type' => array(
        '#type' => 'item',
        '#title' => t('Type'),
        '#value' => $log_sources[$log->source]['description'],
      ),
      'time' => array(
        '#type' => 'item',
        '#title' => t('Time'),
        '#value' => cyd_utils_format_date($log->log_time),
      ),
      'contents' => array(
        '#type' => 'item',
        '#title' => t('Log'),
        '#value' => theme($log_sources[$log->source]['theme callback'], $log->data),
      ),
    );
    if (isset($backtrace)) {
      $form['backtrace'] = array(
        '#type' => 'item',
        '#title' => t('Backtrace'),
        '#value' => cyd_utils_print(cyd_utils_trace_remove_args($backtrace)),
      );
    }
    if (!empty($log->page) && $log_page = cyd_utils_page_load($log->page)) {
      $form['page'] = array(
        '#type' => 'item',
        '#title' => t('Page info'),
        '#value' => $log_page->path .' - '. l(t('view page info'), 'admin/settings/cyd-utils/pages/'. $log_page->pid),
      );
    }
  }
  return $form;
}

/**
 * "Page tags" form callback.
 */
function cyd_utils_page_tags_form($form_state) {
  if (!empty($form_state['storage']['delete_tags'])) {
    $tag_list = array(
      '#type' => 'item',
      '#title' => t('Tags'),
      '#value' => theme('item_list', $form_state['storage']['delete_tags']),
    );
    return confirm_form(array($tag_list), t('Are you sure that you want to delete tags?'), 'admin/settings/cyd-utils/page-tags', NULL, t('Delete'));
  }

  // fetch all tags from DB
  $tag_rows = db_query("SELECT tag, COUNT(tag) AS number FROM {cyd_utils_page_tags} GROUP BY tag");
  $form = array();
  while ($row = db_fetch_object($tag_rows)) {
    $form['tags'][$row->tag] = array(
      'delete' => array(
        '#type' => 'checkbox',
        '#title' => '',
      ),
      'tag' => array(
        '#type' => 'markup',
        '#value' => $row->tag,
      ),
      'count' => array(
        '#type' => 'markup',
        '#value' => $row->number,
      ),
    );
  }
  // sort tags
  if  (!empty($form['tags'])) {
    uasort($form['tags'], 'cyd_utils_sort_page_tags');
  }
  $form['tags']['#tree'] = TRUE;
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete tags'),
  );
  return $form;
}

/**
 * Submit callback for 'page tags' form.
 */
function cyd_utils_page_tags_form_submit($form, &$form_state) {
  if (!empty($form_state['storage']['delete_tags'])) {
    // delete tags
    db_query("DELETE FROM {cyd_utils_page_tags} WHERE tag IN (". db_placeholders($form_state['storage']['delete_tags'], 'varchar') .')', $form_state['storage']['delete_tags']);
    drupal_set_message(t('@num tags has been deleted.', array('@num' => count($form_state['storage']['delete_tags']))));
    unset($form_state['storage']);
    return;
  }
  $tags_to_delete = array();
  foreach ($form_state['values']['tags'] as $tag => $values) {
    if ($values['delete']) {
      $tags_to_delete[] = $tag;
    }
  }
  if (empty($tags_to_delete)) {
    drupal_set_message(t('Not tags selected.'));
  }
  else {
    $form_state['storage']['delete_tags'] = $tags_to_delete;
  }
}

/**
 * Theme callback for form 'cyd_utils_page_tags_form'.
 */
function theme_cyd_utils_page_tags_form($form) {
  // prepare header
  $header = array('', t('Tag'), t('Page count'));
  // build rows
  $rows = array();
  foreach (element_children($form['tags']) as $tag) {
    $rows[] = array(
      drupal_render($form['tags'][$tag]['delete']),
      drupal_render($form['tags'][$tag]['tag']),
      drupal_render($form['tags'][$tag]['count']),
    );
  }
  unset($form['tags']);

  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Sort callback, used in cyd_utils_page_tags_form().
 *
 * @see cyd_utils_page_tags_form().
 */
function cyd_utils_sort_page_tags($a, $b) {
  return ($a['tag']['#value'] < $b['tag']['#value']) ? -1 : 1;
}

/**
 * Module settings form.
 * Used on '/admin/settings/cyd-utils/settings' page.
 */
function cyd_utils_settings_form($form_state) {
  drupal_set_title(t('CYD Utilities settings'));
  $info_types = array();
  foreach (cyd_utils_get_page_info_types() as $type => $type_info) {
    $info_types[$type] = $type_info['name'];
  }
  $table_limits = variable_get('cyd_utils_table_limits', array('cyd_utils_logs' => array('time_limit' => 0, 'records_limit' => 0), 'cyd_utils_pages' => array('time_limit' => 0, 'records_limit' => 0)));

  $log_sources = module_invoke_all('log_sources');
  $source_options = array();
  foreach ($log_sources as $key => $data) {
    $source_options[$key] = $data['description'];
  }
  $form = array(
    'cyd_utils_page_info' => array(
      '#type' => 'fieldset',
      '#title' => t('Collect page info'),
      'cyd_utils_store_page_information' => array(
        '#type' => 'checkbox',
        '#title' => t('Write page request information to the database'),
        '#default_value' => isset($form_state['cyd_utils_store_page_information'])?$form_state['cyd_utils_store_page_information']:variable_get('cyd_utils_store_page_information', FALSE),
      ),
      'cyd_utils_info_types' => array(
        '#title' => t('Type of information to collect'),
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
      'cyd_utils_exclude_collect' => array(
        '#type' => 'textarea',
        '#title' => t('Exclude pages'),
        '#default_value' => isset($form_state['cyd_utils_exclude_collect'])?$form_state['cyd_utils_exclude_collect']:variable_get('cyd_utils_exclude_collect', ''),
        '#description' => t('Information won\'t be collected on these internal paths.'),
      ),
    ),
    'cyd_utils_bot_tags_container' => array(
      '#type' => 'fieldset',
      '#title' => t('Search engines tags'),
      'cyd_utils_enable_bot_tags' => array(
        '#type' => 'checkbox',
        '#title' => t('Enable tags of search engines'),
        '#default_value' => isset($form_state['values']['cyd_utils_enable_bot_tags']) ? $form_state['values']['cyd_utils_enable_bot_tags'] : variable_get('cyd_utils_enable_bot_tags', FALSE),
      ),
      'cyd_utils_bot_tags' => array(
        '#type' => 'textarea',
        '#title' => t('Bot tags'),
        '#description' => t('enter one tag per line, in format [botstring]|[bottag], where [botstring] is looked in User-Agent string.'),
        '#default_value' => isset($form_state['values']['cyd_utils_bot_tags']) ? $form_state['values']['cyd_utils_bot_tags'] : variable_get('cyd_utils_bot_tags', "UnisterBot|UnisterBot\nbingbot|bingbot\nGooglebot|Googlebot\nMSNBot|MSNBot\nAhrefsBot|AhrefsBot\nBaiduspider|Baiduspider\nMail.Ru bot|Mail.Ru\nYandexBot|Yandex\nPurebot|Purebot\nMJ12bot|MJ12bot"),
      ),
    ),
    'cyd_utils_php_file_tags_container' => array(
      '#type' => 'fieldset',
      '#title' => t('Php file tags'),
      'cyd_utils_enable_php_file_tags' => array(
        '#type' => 'checkbox',
        '#title' => t('Enable php file tagging'),
        '#default_value' => isset($form_state['values']['cyd_utils_enable_php_file_tags']) ? $form_state['values']['cyd_utils_enable_php_file_tags'] : variable_get('cyd_utils_enable_php_file_tags', FALSE),
      ),
      'cyd_utils_exclude_php_file_tags' => array(
        '#type' => 'textarea',
        '#title' => t('Exclude files'),
        '#description' => t('Enter one file per line without leading slash.'),
        '#default_value' => isset($form_state['values']['cyd_utils_exclude_php_file_tags']) ? $form_state['values']['cyd_utils_exclude_php_file_tags'] : variable_get('cyd_utils_exclude_php_file_tags', "index.php"),
      ),
    ),
    'cyd_utils_watchdog_unclean_script_exit' => array(
      '#type' => 'checkbox',
      '#title' => t('Log unclean script termination (w/o running hook_exit()) to watchdog'),
      '#default_value' => isset($form_state['cyd_utils_watchdog_unclean_script_exit']) ? $form_state['cyd_utils_watchdog_unclean_script_exit'] : variable_get('cyd_utils_watchdog_unclean_script_exit', FALSE),
    ),
    'cyd_utils_store_time_information' => array(
      '#type' => 'checkbox',
      '#title' => t('Store and output page execution time info'),
      '#default_value' => isset($form_state['cyd_utils_store_time_information'])?$form_state['cyd_utils_store_time_information']:variable_get('cyd_utils_store_time_information', FALSE),
    ),
    'cyd_utils_log_mail' => array(
      '#type' => 'checkbox',
      '#title' => t('Log mail sending'),
      '#default_value' => isset($form_state['cyd_utils_log_mail']) ? $form_state['cyd_utils_log_mail'] : variable_get('cyd_utils_log_mail', FALSE),
    ),
    'cyd_utils_log_source_without_args' => array(
      '#type' => 'checkboxes',
      '#title' => t('Disable logging of backtrace function arguments for the following log types'),
      '#options' => $source_options,
      '#default_value' => cyd_utils_get_log_sources_without_args(),
    ),
    'cyd_utils_table_limits' => array(
      '#type' => 'fieldset',
      '#title' => t('Limits for cyd_utils tables'),
      '#tree' => TRUE,
      'cyd_utils_logs' => array(
        'time_limit' => array(
          '#type' => 'textfield',
          '#title' => t('Delete records from @table table, older that N seconds', array('@table' => '{cyd_utils_logs}')),
          '#default_value' => isset($form_state['values']['cyd_utils_logs']['time_limit']) ? $form_state['values']['cyd_utils_logs']['time_limit'] : $table_limits['cyd_utils_logs']['time_limit'],
        ),
        'records_limit' => array(
          '#type' => 'textfield',
          '#title' => t('Maximum number of records in @table', array('@table' => '{cyd_utils_logs}')),
          '#default_value' => isset($form_state['values']['cyd_utils_logs']['records_limit']) ? $form_state['values']['cyd_utils_logs']['records_limit'] : $table_limits['cyd_utils_logs']['records_limit'],
        ),
      ),
      'cyd_utils_pages' => array(
        'time_limit' => array(
          '#type' => 'textfield',
          '#title' => t('Delete records from @table table, older that N seconds', array('@table' => '{cyd_utils_pages}')),
          '#default_value' => isset($form_state['values']['cyd_utils_pages']['time_limit']) ? $form_state['values']['cyd_utils_pages']['time_limit'] : $table_limits['cyd_utils_pages']['time_limit'],
        ),
        'records_limit' => array(
          '#type' => 'textfield',
          '#title' => t('Maximum number of records in @table', array('@table' => '{cyd_utils_pages}')),
          '#default_value' => isset($form_state['values']['cyd_utils_pages']['records_limit']) ? $form_state['values']['cyd_utils_pages']['records_limit'] : $table_limits['cyd_utils_pages']['records_limit'],
        ),
      ),
    ),
  );
  $context_types = array('init', 'exit' , 'script end');
  $info_settings = variable_get('cyd_utils_info_types', array());
  foreach ($context_types as $context) {
    if (empty($info_settings[$context])) {
      $info_settings[$context] = array();
    }
    $form['cyd_utils_page_info']['cyd_utils_info_types'][$context] = array(
      '#title' => t('"@context" context', array('@context' => $context)),
      '#type' => 'checkboxes',
      '#options' => $info_types,
      '#default_value' => isset($form_state['values']['cyd_utils_info_types'][$context]) ? $form_state['values']['cyd_utils_info_types'][$context] : $info_settings[$context],
    );
  }
  return system_settings_form($form);
}
