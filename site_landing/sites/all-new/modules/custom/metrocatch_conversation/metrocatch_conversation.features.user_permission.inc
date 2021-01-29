<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function metrocatch_conversation_user_default_permissions() {
  $permissions = array();

  // Exported permission: create conversation_entry content
  $permissions['create conversation_entry content'] = array(
    'name' => 'create conversation_entry content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: post comments
  $permissions['post comments'] = array(
    'name' => 'post comments',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: post comments without approval
  $permissions['post comments without approval'] = array(
    'name' => 'post comments without approval',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view imagecache metrocatch_conversation_comment
  $permissions['view imagecache metrocatch_conversation_comment'] = array(
    'name' => 'view imagecache metrocatch_conversation_comment',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  return $permissions;
}
