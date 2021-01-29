<?php
/**
 * @file
 * metrocatch-connect-random-catch-page.tpl.php
 *
 * Template for a random Catch of the Day page.
 *
 * Availabel variables:
 *   - catch: stdClass object of fully loaded catch in a Feature slot.
 *   - catches_tabs: A string containing themed tabs. Each tab links on today's Catch of the Day.
 *   - featured_slot: A string containing themed featured slot with
 *     random Featured Catch of the Day.
 *   - featured_slot_empty_text: A string containing empty text if there is no Catches today.
 *   - other_catches_block: A string containing all the others today's Catches.
 *   - other_catches_empty_text: A string containing empty text if there is no other's Catches.
 *
 * @see template_preprocess_metrocatch_connect_random_catch_page()
 */
?>
<?php print $catches_tabs; ?>
<div id="feature-slot-<?php print isset($catch->uid) ? $catch->uid : '0'; ?>" class="feature-slot<?php print empty($catch->uid) ? ' no-catches' : ''; ?>">
  <?php print isset($featured_slot) ? $featured_slot : $featured_slot_empty_text; ?>
</div>
<div id="become-metrocatch-streak"></div>
<div id="featured-catches-block">
<?php print isset($other_catches_block) ? $other_catches_block : $other_catches_empty_text; ?>
</div>
