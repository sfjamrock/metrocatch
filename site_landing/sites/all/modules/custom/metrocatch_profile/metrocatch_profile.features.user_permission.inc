<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function metrocatch_profile_user_default_permissions() {
  $permissions = array();

  // Exported permission: edit field_profile_birth_date
  $permissions['edit field_profile_birth_date'] = array(
    'name' => 'edit field_profile_birth_date',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_college
  $permissions['edit field_profile_college'] = array(
    'name' => 'edit field_profile_college',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_current_city
  $permissions['edit field_profile_current_city'] = array(
    'name' => 'edit field_profile_current_city',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_employment
  $permissions['edit field_profile_employment'] = array(
    'name' => 'edit field_profile_employment',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_favorite_books
  $permissions['edit field_profile_favorite_books'] = array(
    'name' => 'edit field_profile_favorite_books',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_favorite_movies
  $permissions['edit field_profile_favorite_movies'] = array(
    'name' => 'edit field_profile_favorite_movies',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_favorite_quotes
  $permissions['edit field_profile_favorite_quotes'] = array(
    'name' => 'edit field_profile_favorite_quotes',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_favorite_tv_shows
  $permissions['edit field_profile_favorite_tv_shows'] = array(
    'name' => 'edit field_profile_favorite_tv_shows',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_feature_question
  $permissions['edit field_profile_feature_question'] = array(
    'name' => 'edit field_profile_feature_question',
    'roles' => array(),
  );

  // Exported permission: edit field_profile_first_name
  $permissions['edit field_profile_first_name'] = array(
    'name' => 'edit field_profile_first_name',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_graduate_school
  $permissions['edit field_profile_graduate_school'] = array(
    'name' => 'edit field_profile_graduate_school',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_high_scool
  $permissions['edit field_profile_high_scool'] = array(
    'name' => 'edit field_profile_high_scool',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_hometown
  $permissions['edit field_profile_hometown'] = array(
    'name' => 'edit field_profile_hometown',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_last_name
  $permissions['edit field_profile_last_name'] = array(
    'name' => 'edit field_profile_last_name',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_neighborhood
  $permissions['edit field_profile_neighborhood'] = array(
    'name' => 'edit field_profile_neighborhood',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_public_connections
  $permissions['edit field_profile_public_connections'] = array(
    'name' => 'edit field_profile_public_connections',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_sex
  $permissions['edit field_profile_sex'] = array(
    'name' => 'edit field_profile_sex',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_sex_preference
  $permissions['edit field_profile_sex_preference'] = array(
    'name' => 'edit field_profile_sex_preference',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_state
  $permissions['edit field_profile_state'] = array(
    'name' => 'edit field_profile_state',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_birth_date
  $permissions['view field_profile_birth_date'] = array(
    'name' => 'view field_profile_birth_date',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_college
  $permissions['view field_profile_college'] = array(
    'name' => 'view field_profile_college',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_current_city
  $permissions['view field_profile_current_city'] = array(
    'name' => 'view field_profile_current_city',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_employment
  $permissions['view field_profile_employment'] = array(
    'name' => 'view field_profile_employment',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_books
  $permissions['view field_profile_favorite_books'] = array(
    'name' => 'view field_profile_favorite_books',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_movies
  $permissions['view field_profile_favorite_movies'] = array(
    'name' => 'view field_profile_favorite_movies',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_quotes
  $permissions['view field_profile_favorite_quotes'] = array(
    'name' => 'view field_profile_favorite_quotes',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_tv_shows
  $permissions['view field_profile_favorite_tv_shows'] = array(
    'name' => 'view field_profile_favorite_tv_shows',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_feature_question
  $permissions['view field_profile_feature_question'] = array(
    'name' => 'view field_profile_feature_question',
    'roles' => array(),
  );

  // Exported permission: view field_profile_first_name
  $permissions['view field_profile_first_name'] = array(
    'name' => 'view field_profile_first_name',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_graduate_school
  $permissions['view field_profile_graduate_school'] = array(
    'name' => 'view field_profile_graduate_school',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_high_scool
  $permissions['view field_profile_high_scool'] = array(
    'name' => 'view field_profile_high_scool',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_hometown
  $permissions['view field_profile_hometown'] = array(
    'name' => 'view field_profile_hometown',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_last_name
  $permissions['view field_profile_last_name'] = array(
    'name' => 'view field_profile_last_name',
    'roles' => array(),
  );

  // Exported permission: view field_profile_neighborhood
  $permissions['view field_profile_neighborhood'] = array(
    'name' => 'view field_profile_neighborhood',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_public_connections
  $permissions['view field_profile_public_connections'] = array(
    'name' => 'view field_profile_public_connections',
    'roles' => array(),
  );

  // Exported permission: view field_profile_sex
  $permissions['view field_profile_sex'] = array(
    'name' => 'view field_profile_sex',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_sex_preference
  $permissions['view field_profile_sex_preference'] = array(
    'name' => 'view field_profile_sex_preference',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_state
  $permissions['view field_profile_state'] = array(
    'name' => 'view field_profile_state',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  return $permissions;
}
