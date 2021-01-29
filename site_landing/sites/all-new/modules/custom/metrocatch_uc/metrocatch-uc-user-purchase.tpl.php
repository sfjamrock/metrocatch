<?php
/**
 * @file
 * Template for one record on "purchase history" page
 * Variables:
 *   - $order: order object
 *   - $title: title of the product
 *   - $type: order "type" - 'single' or '3 months' plan
 *   - $total: order total, already themed
 *   - $date: order date (themed)
 */
?>
<div class="metrocatch-uc-user-purchase">
  <div class="purchse-title"><?php print $title; ?></div>
  <div class="purchase-date"><span class="purchase-label"><?php print t('Transaction Date'); ?>:</span><?php print $date; ?></div>
  <div class="transaction-number"><span class="purchase-label"><?php print t('Transaction Number'); ?>:</span><?php print $order->order_id; ?></div>
  <div class="purchase-promotion"><span class="purchase-label"><?php print t('Promotion'); ?>:</span><?php print $promotion; ?></div>
  <div class="purchase-type"><span class="purchase-label"><?php print t('Type'); ?>:</span><?php print $type; ?></div>
  <div class="purchase-total"><span class="purchase-label"><?php print t('Purchase Total'); ?>:</span><?php print $total; ?></div>
</div>
