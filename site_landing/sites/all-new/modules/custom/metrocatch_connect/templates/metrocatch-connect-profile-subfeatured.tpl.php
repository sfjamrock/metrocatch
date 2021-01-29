<?php
/**
 * @file
 * metrocatch-connect-profile-subfeatured.tpl.php
 *
 * Displays user profile on the Subfeature Catch of the day Landing page.
 *
 * Available variables:
 * - huge_userpic: A string containing themed user picture.
 * - username: A string containing user's first name.
 * - interest: A string containing user's interest (e.g. 'Interested in women').
 * - share: An array of share links.
 * - baseinfo: A string with basic public profile information.
 *
 * Additional variables:
 * - count_connections: Contains number of user's connections if this is public field.
 * - connect: Contains connect link if current user can connect to the Catch.
 * - time_left: Contains remaining time left until user can connect to the Catch.
 *
 * @see template_preprocess_metrocatch_connect_profile_subfeatured()
 */
?>

<h2 class="user-profile-interest"><?php print $interest;?></h2>
<div class="user-profile-wrapper user-profile user-profile-subfeatured clearfix">
  <div class="user-profile-left-column">
    <div class="user-profile-userpic">
      <?php print $huge_userpic; ?>
    </div>
  </div>
  <div class="user-profile-right-column">
    <h3><?php print $username; ?></h3>
    <div class="user-profile-baseinfo">
      <?php print $baseinfo; ?>
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
