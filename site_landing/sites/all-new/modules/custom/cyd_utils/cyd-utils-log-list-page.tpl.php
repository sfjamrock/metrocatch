<?php
/**
 * @file
 * Theme implementation for log listing page.
 *
 * Variables:
 * - $filter_form: rendered filter form
 * - $list: themed log table
 * - $clear_form: form "clear all log data"
 * - $results_count: number of results (total), string
 */
?>
<div class="cyd-utils-log-list-page">
  <?php print $filter_form; ?>
  <div class="cyd-utils-total-found"><?php print $results_count; ?></div>
  <?php print $list; ?>
  <?php print $clear_form; ?>
</div>
