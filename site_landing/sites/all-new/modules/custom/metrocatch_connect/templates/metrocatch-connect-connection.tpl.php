<?php
/**
 * @file
 * Displays single connection between users.
 *
 * Available variables:
 * - $connection: An array of fully-loaded connection.
 * - $account: StdClass object of connector.
 *   Connector means user who is connected to user who views this connection.
 * - $classes: String containing all classes for the wrapper div.
 * - $userpic: Themed userpic of connector.
 * - $username: Themed username of connector.
 * - $fields: Themed public base fields of connector"s profile.
 * - $actions: Themed links of connection actions. E.g. Break connection, Make permanent, etc.
 *
 * @see template_preprocess_metrocatch_connection()
 */
?>

<div id="connection-<?php print $connection["cid"];?>" class="<?php print $classes;?>">
  <div class="connection-profile clearfix">
    <div class="user-info">
      <?php print $userpic;?>
    </div>
    <div class="username-wrapper">
      <?php print $username;?>
    </div>
    <div class="profile-fields">
      <?php print $fields;?>
    </div>
  </div>
  <div class="connection-actions">
    <?php print $actions;?>
  </div>
</div>
