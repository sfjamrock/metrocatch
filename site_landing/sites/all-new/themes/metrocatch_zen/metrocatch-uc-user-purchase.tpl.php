<?php
/**
 * @file
 * Template for one record on "purchase history" page
 * Variables:
 *   - $order order object
 *   - $title title of the product
 *   - $type order "type" - 'single' or '3 months' plan
 *   - $total order total, already themed
 *   - $date order date
 */
?>
<div class="metrocatch-uc-user-purchase">
  <div class="purchse-title"><?php print $title; ?></div>
  <div class="purchase-date"><span class="purchase-label"><?php print t('Transaction Date'); ?>: </span><?php print $date; ?></div>
  <div class="contact-us-about-transaction"><?php print $contact_us;?></div>
  <div class="transaction-number"><span class="purchase-label"><?php print t('Transaction Number'); ?>: </span><?php print $order->order_id; ?></div>
  <?php if (!empty($sheduled_date_label)): ?>
  <div class="sheduled-date">
    <span class="purchase-label"><?php print $sheduled_date_label; ?>: </span>
    <?php print $sheduled_date; ?>
  </div>
  <?php endif; ?>
  <?php if (!empty($promotion)) : ?>
    <div class="purchase-promotion"><span class="purchase-label"><?php print t('Discount'); ?>: </span><?php print $promotion; ?></div>
  <?php endif; ?>
  <div class="purchase-total"><span class="purchase-label"><?php print t('Purchase Total'); ?>: </span><?php print $total; ?></div>
</div>
