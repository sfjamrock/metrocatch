<?php
/**
 * @file
 * Create template for available fixes to the test.
 *
 * Available variables:
 * - $fix_info: Array with test fix information.
 *   -- $fix_info['description']: Description of the test fix.
 *   -- $fix_info['query']: Query to the database to fix it.
 * - $apply_fix_button: Html of button to apply fix.
 */
?>
<div class="db-consistency-available-fixes-content">
  <div class="db-consistency-available-fixes-content-header">
    <div class="db-consistency-available-fixes-content-title"><?php print $fix_info['description']; ?></div>
    <div class="db-consistency-available-fixes-content-button"><?php print $apply_fix_button; ?></div>
  </div>
  <div class="db-consistency-available-fixes-content-description"><?php print $fix_info['query']; ?></div>
</div>
