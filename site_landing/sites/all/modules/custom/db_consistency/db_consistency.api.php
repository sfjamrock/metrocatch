<?php

/**
 * @file
 * Hooks provided by the db_consistency module.
 */

/**
 * Provide information about database consistency tests and fixes.
 *
 * @return array
 *   An associative array of provided tests, keyed by test machine name. Machine
 *   name should be like "{module_name}.{test_name}", where {module_name} is
 *   module, which database tables will be checked by this test. {test_name} is
 *   an unique test name within {module_name} module. Values of returned array
 *   must be arrays with following keys:
 *   - title: A string to be used as human readable title of the test.
 *   - description: A string to be used as description of the test.
 *   - table: A string, which contains table name, which will be checked by the
 *     test.
 *   - severity: A string, which represents an importance of the test. Can
 *     be one of the following:
 *       - 'warning': minor database inconsistency. This type of error can
 *     sometimes occur when Drupal settings are changed.
 *       - 'error': database data consitency is broken. This type of error
 *     should never occur during normal work of Drupal.
 *   - test_query: A string with test query, which will check appropriate module
 *     table.
 *   - test_type: (optional) A string, which represents an expectation what test
 *     query should return on test failure. Can be one of the following:
 *       - 'positive': (default) Test had failed if test query returned positive
 *         number.
 *       - 'negative': Test had failed if test query returned FALSE.
 *   - error_list_query: (optional) A string with query, which returns
 *     identificators of non-consistent rows.
 *   - error_list_description: (optional) A string, which represents a template
 *     of message about wich rows are wrong. Use @list placeholder for determine
 *     where list os identificators of non-consistent rows should be inserted.
 *     This array item should be specified if 'error_list_query' item is
 *     specified.
 *   - fixes: (optional) An array of available fixes of wring rows, keyed by fix
 *     machine name. Values of this array must be arrays with following keys:
 *     - description: A string to be used as description of the fix.
 *     - query: A string with query, which performs the fix.
 */
function hook_db_test() {
  return array(
    'node.authors_exist' => array(
      'title' => t('Missing node authors'),
      'description' => t('Check if nodes belong to nonexistent users.'),
      'table' => 'node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node} n LEFT JOIN {users} u ON n.uid = u.uid WHERE u.uid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT n.nid FROM {node} n LEFT JOIN {users} u ON n.uid = u.uid WHERE u.uid IS NULL",
      'error_list_description' => 'Nids of nodes which belong to missing users: @list',
      'fixes' => array(
        'node_set_admin_author' => array(
          'description' => t('Set superuser as author of wrong nodes.'),
          'query' => "UPDATE {node} n LEFT JOIN {users} u ON n.uid = u.uid SET n.uid = 1 WHERE u.uid IS NULL",
        ),
        'node_set_anonymous_author' => array(
          'description' => t('Set anonymous user as author of wrong nodes.'),
          'query' => "UPDATE {node} n LEFT JOIN {users} u ON n.uid = u.uid SET n.uid = 0 WHERE u.uid IS NULL",
        ),
      ),
    ),
  );
}

