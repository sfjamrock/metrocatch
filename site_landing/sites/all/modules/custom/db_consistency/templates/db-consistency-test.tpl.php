<?php
/**
 * @file
 * Template for database test page.
 *
 * Available variables:
 * - $style: State of test, can be 'error', 'ok', 'warning' or 'not-check'.
 * - $classes: array of additional classes
 * - $identifiers_list: Row with $info['error_list_description'] with placeholder replaced on list of identifiers.
 * - $info: Array with information about test for database, described in hook_db_test().
 *   -- $info['title']: Test name.
 *   -- $info['description']: Description of a test.
 *   -- $info['test_query']: Query to the database to check it.
 *   -- $info['error_list_query']: Query to the database, which finds ids of bad rows.
 *   -- $info['error_list_description']: Text to show ids of bad rows.
 *   -- $info['fixes']: Available fixes.
 * - $test_button: Html of button to test or retest single test.
 * - $test_check: Html of checkbox, which indicate which test should be run.
 * - $result: Array of test results.
 *   -- $result['result_passed']: Result of the passing test: TRUE, FALSE.
 *   -- $result['identifiers_list']: Array of bad row ids in database.
 *   -- $result['time']: Time when test has been executed.
 * - $test_fixes: HTML of rendered fixes.
  */
?>
<div class="db-consistency-test-result db-consistency-test-result-<?php print $style; ?> <?php print implode(' ', $classes); ?>">
  <div class="db-consistency-test-result-header">
    <div class="db-consistency-test-result-status">
      <div class="db-consistency-result-name"><?php print $info['title']; ?></div>
      <div class="db-consistency-result-time"><?php print $result['time']; ?></div>
    </div>
    <div class="db-consistency-test-button"><?php print $test_button; ?></div>
  </div>
  <div class="db-consistency-test-result-content-wrapper">
    <div class="db-consistency-check-test"><?php print $test_check; ?></div>
    <div class="db-consistency-test-result-content">
      <div class="db-consistency-test-description"><?php print $info['description']; ?></div>
      <div class="db-consistency-test-query"><?php print $info['test_query']; ?></div>
      <?php if (isset($identifiers_list)): ?>
        <div class="db-consistency-test-result-id-list"><?php print $identifiers_list; ?></div>
      <?php endif; ?>
      <?php if (isset($test_fixes)): ?>
        <div class="db-consistency-available-fixes">
          <div class="db-consistency-available-fixes-header"><?php print t('Available fixes'); ?></div>
          <?php print $test_fixes; ?>
        </div>
      <?php endif; ?>
    </div>
  </div>
</div>