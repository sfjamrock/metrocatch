<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function metrocatch_connect_user_default_permissions() {
  $permissions = array();

  // Exported permission: edit field_profile_connections_left
  $permissions['edit field_profile_connections_left'] = array(
    'name' => 'edit field_profile_connections_left',
    'roles' => array(),
  );

  // Exported permission: view field_profile_connections_left
  $permissions['view field_profile_connections_left'] = array(
    'name' => 'view field_profile_connections_left',
    'roles' => array(),
  );

  return $permissions;
}
