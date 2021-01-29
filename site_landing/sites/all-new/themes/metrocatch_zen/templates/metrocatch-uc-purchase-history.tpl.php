<?php
/**
 * @file
 * Template for "Purchase History" page
 * Varibles:
 *   - $orders: array of order objects
 *   - $purchases: themed order list
 *   - $account: user account object
 */
?>
<div class="metrocatch-uc-purchase-history">
  <?php if (FALSE) : ?>
  <?php // Temporary hide 'Connections left' text from Purchase history page. ?>
  <div class="metrocatch-purahses-connections-left"><span class="label"><?php print t('Connections Left'); ?>: </span><?php print $connections_left; ?></div>
  <?php endif; ?>
  <?php print $purchases; ?>
</div>
