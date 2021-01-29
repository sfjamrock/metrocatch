<?php
/**
 * @file
 * Template of 'failed tests' page
 * 
 * Variables: 
 *   - $form - rendered from with failed tests
 *   - $module - tested module, info as fetched from DB
 */
?>
<div class="db-consistency-failed-tests">
  <?php if ($form): ?>
    <?php print $form; ?>
  <?php else: ?>
    <div class="db-consistency-no-failed-tests">
      <?php print t('No failed tests found.'); ?>
    </div>
  <?php endif; ?>
</div>
