<?php
/**
 * @file
 * metrocatch-connect-profile-featured-teaser.tpl.php
 *
 * Displays user profile on the Today's Catch page.
 *
 * Available variables:
 * - huge_userpic: Huge userpic.
 * - username: User's first name.
 * - interest: A string containing user's interest (e.g. 'Interested in women').
 * - share: An array of share links.
 *
 * Additional variables:
 * - trimmed_marketing_text: Contains trimmed custom marketing text drafted by the site administrator.
 * - learn_more: Contains link on the Feature landing page.
 * - count_connections: Contains number of user's connections if this is public field.
 * - connect: Contains connect link if current user can connect to the Catch.
 * - time_left: Contains remaining time left until user can connect to the Catch.
 *
 * @see template_preprocess_metrocatch_connect_profile_featured_teaser()
 */
?>

<div class="user-profile-wrapper user-profile user-profile-featured-teaser clearfix">
  <div class="user-profile-left-column">
    <div class="user-profile-userpic">
      <?php print $huge_userpic; ?>
    </div>
    <h3 class="user-profile-interest"><?php print $interest;?></h3>
  </div>
  <div class="user-profile-right-column">
    <h2><?php print $username; ?></h2>
    <div class="user-profile-marketing-text">
      <?php print $trimmed_marketing_text; ?>
      <span class="profile-more">
        <?php print $more_link; ?>
      </span>
    </div>
    <div class="user-profile-connect-link">
      <?php print $connect; ?>
    </div>
    <div class="user-profile-share-links">
      <?php print $share; ?>
    </div>
    <div class="user-profile-connections-count-and-time-left">
      <div class="user-profile-connections-count">
        <?php if (!empty($count_connections)) : ?>
          <?php print $count_connections; ?>
        <?php endif; ?>
      </div>
      <div class="user-profile-time-left">
        <?php if (!empty($time_left)) : ?>
          <?php print $time_left; ?>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>
