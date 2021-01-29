<?php
/**
 * @file
 * metrocatch-connect-profile-full.tpl.php
 *
 * Available variables:
 *  - teaser: A string containing themed profile teaser.
 *  - moreinfo: A string containing themed in-depth profile information.
 *
 *  @see template_preprocess_metrocatch_connect_profile_teaser()
 *  @see template_preprocess_metrocatch_connect_profile_full()
 */
?>

<?php print $teaser; ?>
<div class="user-profile-moreinfo">
  <?php if (isset($metrocatch_badges)) : ?>
    <?php print $metrocatch_badges; ?>
  <?php endif; ?>
  <?php if (isset($metrocatch_moreinfo)) : ?>
    <?php print $metrocatch_moreinfo; ?>
  <?php endif; ?>
  <?php print $moreinfo; ?>
</div>
