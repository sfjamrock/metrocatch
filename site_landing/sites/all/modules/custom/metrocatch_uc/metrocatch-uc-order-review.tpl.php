<?php
/**
 * @file
 * Template for "review order" step of purchase form
 * Variables:
 *   - $order: the whole ubercart order object
 *   - $products: rendered products table
 *   - $billing_info: billing_info area (rendered standart order pane)
 *   - $payment_method: same as above, but payment method checkout pane
 */
?>
<div class="metrocatch-uc-order-review">
  <h3 class="header"><?php print t('Products'); ?>:</h3>
  <?php print $products; ?>
  <h3 class="header"><?php print t('Billing information'); ?>:</h3>
  <?php print $billing_info; ?>
  <h3 class="header"><?php print t('Payment Method'); ?>:</h3>
  <?php print $payment_method; ?>
</div>
