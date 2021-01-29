<?php
/**
 * @file metrocatch-connect-profile-teaser.tpl.php
 *
 * Available variables:
 *  - userpic: A string containing themed user picture.
 *  - count_photos: Number of user's photos.
 *  - username: User's first name.
 *  - count_connections: Number of user's connections, if current user can view it.
 *  - count_metropoints: Number of user's metropoints.
 *  - baseinfo: A string containing themed public base profile information.
 *  - more_link: A string containing link on full profile display.
 */

?>
<div class="user-profile user-profile-teaser">
  <div class="metrocatch-profile-user-short-info clearfix">
    <div class="user-profile-userpic-pin">
        <?php print $userpic_pin; ?>
    </div>
    <div class="metrocatch-profile-user-avatar">
      <?php print $userpic;?>
    </div>
    <div class="metrocatch-profile-baseinfo">
      <div class="metrocatch-profile-username-wrapper">
        <?php print $preference;?>
        <?php print $username;?>
      </div>
      <?php if ($user->uid == $account->uid && !empty($count_connections)) :?>
        <div class="metrocatch-profile-connections-count">
          <?php print $count_connections; ?>
        </div>
      <?php endif;?>
      <?php if (!empty($count_metropoints)) :?>
        <div class="profile-metropoints">
          <?php print $count_metropoints; ?>
        </div>
      <?php endif;?>
      <div class="metrocatch-profile-fields">
        <?php print $baseinfo; ?>
        <?php if (isset($more_link)) : ?>
          <span class="profile-more">
            <?php print $more_link; ?>
          </span>
        <?php endif; ?>
      </div>
      <div class="profile-photos">
        <?php if (!empty($count_photos)) : ?>
          <?php print $count_photos;?>
        <?php endif;?>
        <?php if (!empty($count_photos) && !empty($add_photos)) : ?>
          <span class="photos-separator"> | </span>
        <?php endif; ?>
        <?php if (!empty($add_photos)) : ?>
          <?php print $add_photos; ?>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>