<?php
/**
 * @file
 * metrocatch-connect-catch-subfeature-page.tpl.php
 *
 * Template for a metrocatch of the Day Sub-Feature Landing Page.
 *
 * Available variables:
 *   - catch: stdClass object of fully loaded catch in a Feature slot.
 *   - featured_slot: A string containing themed Featured slot.
 *     Featured slot may contain the person someone clicked through on in an email
 *     or Catch of the Day otherwise.
 *   - featured_slot_empty_text: A string containg text displayed if there is not Catch of the Day.
 *   - other_catches_block: A string containing themed Other Catches of the Day.
 *   - other_catches_empty_text: A string containing text to display if there is not available
 *     Other Catches of the Day.
 *   - view_archive_link: A string containing link to the archive in current segment.
 *     Is empty if user is already viewing arhive or there is no available catches in it.
 *
 * @see template_preprocess_metrocatch_connect_catch_subfeature_page()
 */
?>

<div id="feature-slot-<?php print isset($catch->uid) ? $catch->uid : '0'; ?>" class="feature-slot">
  <?php print isset($featured_slot) ? $featured_slot : $featured_slot_empty_text; ?>
</div>
<div id="other-catches-block">
  <?php print isset($other_catches_block) ? $other_catches_block : $other_catches_empty_text; ?>
</div>
<?php if (!empty($view_archive_link)) : ?>
  <?php print $view_archive_link; ?>
<?php endif;?>
<?php if (!empty($close_archive_link)) : ?>
  <?php print $close_archive_link; ?>
<?php endif; ?>
