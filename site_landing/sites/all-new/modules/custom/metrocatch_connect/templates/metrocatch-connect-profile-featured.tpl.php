<?php
/**
 * @file
 * metrocatch-connect-profile-featured.tpl.php
 *
 * Displays user profile on th Catch of the Day page.
 *
 * Available variables:
 * - huge_userpic: A string containing themed user picture.
 * - username: A string containing user's first name.
 * - interest: A string containing user's interest (e.g. 'Interested in women').
 * - baseinfo: A string containing all the public base profile information,
 *   such as 'born on', 'studied at', etc.
 * - moreinfo: A string containing themed in-depth profile information.
 * - photos: A link on user's photos.
 * - share: An array of share links.
 *
 * Additional variables:
 * - marketing_text: Contains custom marketing text drafted by the site administrator.
 * - count_connections: Contains number of user's connections if this is public field.
 * - connect: Contains connect link if current user can connect to the Catch.
 * - time_left: Contains remaining time left until user can connect to the Catch.
 *
 * @see template_preprocess_metrocatch_connect_profile_featured()
 */
?>

<h3 class="user-profile-interest"><?php print $interest;?></h3>
<div class="user-profile-wrapper user-profile user-profile-featured clearfix">
  <div class="user-profile-left-column">
    <div class="user-profile-userpic">
      <?php print $userpic; ?>
    </div>
    <div class="user-profile-photos">
      <?php print $photos; ?>
    </div>
    <div class="user-profile-baseinfo">
      <?php print $baseinfo; ?>
    </div>
  </div>
  <div class="user-profile-right-column">
    <h2><?php print $username; ?></h2>
    <div class="user-profile-marketing-text">
      <?php print $marketing_text; ?>
    </div>
    <div class="user-profile-connect-link">
      <?php print $connect; ?>
    </div>
    <div class="user-profile-share-links addthis_toolbox addthis_default_style">
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
<div class="user-profile-moreinfo">
  <?php print $moreinfo; ?>
</div>
