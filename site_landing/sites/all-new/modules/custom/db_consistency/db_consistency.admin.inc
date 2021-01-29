<?php
/**
 * @file
 * DB consistency administrative pages
 */

/**
 * Form callback for 'admin/reports/db-consistency' menu item.
 */
function db_consistency_test_form($form_state, $show_tests = array(), $group_tests = TRUE) {
  if (!$form_state['submitted']) {
    db_consistency_actualize_test_result();
    // Check for duplicate fix names.
    $duplicate_fix_names = _db_consistency_check_fix_unique_names();
    if (!empty($duplicate_fix_names)) {
      drupal_set_message(t('Following fixes have duplicate names: @names', array('@names' => implode(', ', $duplicate_fix_names))), 'warning');
    }
  };
  $tests_info = db_consistency_get_test_info();
  $tests_results = db_consistency_get_result();
  if (!empty($show_tests)) {
    $tests_info = array_intersect_key($tests_info, $show_tests);
    $tests_results = array_intersect_key($tests_results, $show_tests);
  }
  // fill form by tests
  $i = 0;
  $form = array('#db_consistency_group_tests' => $group_tests);
  foreach ($tests_results as $test_name => $test_result) {
    $form['tests']['test_'. $i] = array(
      '#prefix' => '<div id="db_consistency_test_'. $i .'">',
      '#suffix' => '</div>',
      '#theme' => 'db_consistency_test_element',
      '#tree' => TRUE,
      '#db_consistency_test_info' =>  $tests_info[$test_name],
      '#db_consistency_test_name' => $test_name,
      '#db_consistency_test_result' => $test_result,
      'test_check' => array(
        '#type' => 'checkbox',
      ),
      'test_button' => array(
        '#type' => 'submit',
        '#value' => is_bool($test_result['result_passed']) ? 'retest' : 'test',
        '#submit' => array('db_consistency_retest_test_submit'),
        '#name' => 'retest_button_'. $i,
        '#ahah' => array(
          'path' => 'admin/reports/db-consistency/ahah-retest-test',
          'event' => 'click',
          'wrapper' => 'db_consistency_test_'. $i,
          'effect' => 'fade',
        )
      ),
      'db_consistency_test_fixes' => array(),
    );
    if (is_bool($test_result['result_passed']) && !$test_result['result_passed'] && !empty($tests_info[$test_name]['fixes']) ) {
      foreach ($tests_info[$test_name]['fixes'] as $fix_name => $fix_info) {
        $form['tests']['test_'. $i]['db_consistency_test_fixes'][$fix_name] = array(
          '#theme' => 'db_consistency_test_element_fix',
          '#fix_info' => $fix_info,
          'apply_fix_button' => array(
            '#type' => 'submit',
            '#value' => 'apply',
            '#submit' => array('db_consistency_apply_test_fix_submit'),
            '#name' => 'apply_fix_button_'. $fix_name,
            '#ahah' => array(
              'path' => 'admin/reports/db-consistency/ahah-retest-test',
              'event' => 'click',
              'wrapper' => 'db_consistency_test_'. $i,
            ),
          ),
        );
      };
    };
    $i++;
  };
  $form['check_button'] = array(
    '#value' => '<input type="button" id="edit-check-button" value="Select all"  class="form-submit" />',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Run selected tests'),
  );
  return $form;
}

/**
 * Page callback for 'admin/reports/db-consistency/ahah-retest-test' menu item.
 */
function db_consistency_ahah_retest_test() {
  $form = db_consistency_ahah_callback_helper();
  $test_name = $form['#parameters'][1]['clicked_button']['#array_parents'][1];
  $test_info = $form['tests'][$test_name];
  unset($test_info['#prefix'], $test_info['#suffix']);
  $output = theme('status_messages') . drupal_render($test_info);
  return drupal_json(
    array(
      'status' => TRUE,
      'data' => $output,
    )
  );
}

/**
 * Form submit callback for db_consistency_test_form form.
 */
function db_consistency_test_form_submit($form, $form_state) {
  $batch_operations = array();
  foreach ($form_state['values'] as $item => $test_result) {
    if (is_array($form_state['values'][$item]) && $form_state['values'][$item]['test_check']) {
      $batch_operations[] = array(
        'db_consistency_batch_operation',
        array(
          $form['tests'][$item]['#db_consistency_test_info'],
          $form['tests'][$item]['#db_consistency_test_name'],
        ),
      );
    };
  };
  $batch = array(
    'operations' => $batch_operations,
    'finished' => 'db_consistency_batch_finished',
    'title' => t('Checking database'),
    'init_message' => t('Prepare data'),
    'progress_message' => t('Done @current of @total.'),
    'error_message' => t('Error.'),
    'file' => drupal_get_path('module', 'db_consistency') .'/db_consistency.admin.inc',
  );
  batch_set($batch);
}

/**
 * Implementation of batch_finished.
 */
function db_consistency_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('Run @count_tests tests.', array('@count_tests' => count($results) )));
  }
  else {
    drupal_set_message(t('Makes with errors.'), 'error');
  }
}

/**
 * Batch_api run tests from hook_db_test.
 */
function db_consistency_batch_operation($info, $test_name, &$context) {
  $result_test = db_consistency_run_test($info);
  $context['results'][] = $info['title'];
  $context['message'] = t('Test <em> %test</em> runing', array('%test' => $info['title']));
  db_consistency_save_test_result($test_name, $result_test);
}

/**
 * Submit handler for test_button button in db_consistency_test_form form, tests or retests single test.
 */
function db_consistency_retest_test_submit($form, $form_state) {
  $test_name = $form_state['clicked_button']['#parents'][0];
  $info_test = $form['tests'][$test_name]['#db_consistency_test_info'];
  $test_name = $form['tests'][$test_name]['#db_consistency_test_name'];
  $result_test = db_consistency_run_test($info_test);
  db_consistency_save_test_result($test_name, $result_test);
}

/**
 * Submit handler for apply_fix_button button in db_consistency_test_form form, applies fix from test.
 */
function db_consistency_apply_test_fix_submit($form, $form_state) {
  $test_name = $form_state['clicked_button']['#parents'][0];
  $test_info = $form['tests'][$test_name];
  $fix_name = $form_state['clicked_button']['#parents'][2];
  $fix_info = $form['tests'][$test_name]['db_consistency_test_fixes'][$fix_name]['#fix_info'];
  $test_name = $form['tests'][$test_name]['#db_consistency_test_name'];
  if (is_array($fix_info['query'])) {
    foreach ($fix_info['query'] as $query) {
      $result_query = db_query($query);
    };
  }
  else {
    $result_query = db_query($fix_info['query']);
  };
  $result_test = db_consistency_run_test($test_info['#db_consistency_test_info']);
  db_consistency_save_test_result($test_name, $result_test);
}

/**
 * AHAH callback helper.
 */
function db_consistency_ahah_callback_helper() {
  $form_state = array(
    'storage' => NULL,
    'submitted' => FALSE,
  );
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form_state['ahah_submission'] = TRUE;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  return $form;
}

/**
 * Theme callback for form with DB tests
 */
function theme_db_consistency_test_form($form) {
  $out = '';
  if ($form['#db_consistency_group_tests']) {
    // group tests by module
    $test_groups = array(
      'all' => l(t('All'), '', array('fragment' => 'all', 'external' => TRUE)),
    );
    foreach (element_children($form['tests']) as $key) {
      $test = $form['tests'][$key];
      if (!isset($test_groups[$test['#db_consistency_test_info']['module']])) {
        $test_groups[$test['#db_consistency_test_info']['module']] = $test['#db_consistency_test_info']['module'];
      }
    }
    $query_variables = array_keys($test_groups);
    $rows = db_query("SELECT name, info FROM {system} WHERE name IN (". db_placeholders(array_keys($query_variables), 'varchar') .")", $query_variables);
    while ($info = db_fetch_object($rows)) {
      if ($info->name !== 'all') {
        $data = unserialize($info->info);
        $test_groups[$info->name] = l($data['name'], '', array('fragment' => $info->name, 'external' => TRUE));
      }
    }
  }
  $out .= '<div class="db-consistency-tests-area">';
  if (!empty($test_groups)) {
    $out .= '<div class="db-consistency-test-groups">'. theme('item_list', $test_groups) .'</div>';
  }
  $out .= '<div class="db-consistency-tests">'. drupal_render($form['tests']) .'</div>';
  $out .= '</div>';
  unset($form['tests']);
  $out .= '<div class="db-consistency-another-form-part">'. drupal_render($form). '</div>';
  return $out;
}

/**
 * Page callback for "admin/reports/db-consistency/failed
 */
function db_consistency_failed_tests() {
  // restrict all test set to failed test
  db_consistency_actualize_test_result();
  $tests_keys = array();
  $tests_info = db_consistency_get_test_info();
  $tests_results = db_consistency_get_result();
  foreach ($tests_info as $key => $info) {
    if ($tests_results[$key]['result_passed'] === FALSE) {
      $tests_keys[$key] = $key;
    }
  }
  $form = empty($tests_keys) ? '' : drupal_get_form('db_consistency_test_form', $tests_keys, FALSE);
  return theme('db_consistency_failed_tests', $form, $module);
}
