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

  // Exported permission: edit field_profile_body_type
  $permissions['edit field_profile_body_type'] = array(
    'name' => 'edit field_profile_body_type',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_college
  $permissions['edit field_profile_college'] = array(
    'name' => 'edit field_profile_college',
    'roles' => array(),
  );

  // Exported permission: edit field_profile_current_city
  $permissions['edit field_profile_current_city'] = array(
    'name' => 'edit field_profile_current_city',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_education
  $permissions['edit field_profile_education'] = array(
    'name' => 'edit field_profile_education',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_employment
  $permissions['edit field_profile_employment'] = array(
    'name' => 'edit field_profile_employment',
    'roles' => array(),
  );

  // Exported permission: edit field_profile_ethnicity
  $permissions['edit field_profile_ethnicity'] = array(
    'name' => 'edit field_profile_ethnicity',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_eye_color
  $permissions['edit field_profile_eye_color'] = array(
    'name' => 'edit field_profile_eye_color',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_favorite_books
  $permissions['edit field_profile_favorite_books'] = array(
    'name' => 'edit field_profile_favorite_books',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_favorite_movies
  $permissions['edit field_profile_favorite_movies'] = array(
    'name' => 'edit field_profile_favorite_movies',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
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
      '0' => 'anonymous user',
      '1' => 'authenticated user',
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

  // Exported permission: edit field_profile_free_feature_agree
  $permissions['edit field_profile_free_feature_agree'] = array(
    'name' => 'edit field_profile_free_feature_agree',
    'roles' => array(
      '0' => 'anonymous user',
    ),
  );

  // Exported permission: edit field_profile_graduate_school
  $permissions['edit field_profile_graduate_school'] = array(
    'name' => 'edit field_profile_graduate_school',
    'roles' => array(),
  );

  // Exported permission: edit field_profile_hair_color
  $permissions['edit field_profile_hair_color'] = array(
    'name' => 'edit field_profile_hair_color',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_headline
  $permissions['edit field_profile_headline'] = array(
    'name' => 'edit field_profile_headline',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_height_feet
  $permissions['edit field_profile_height_feet'] = array(
    'name' => 'edit field_profile_height_feet',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_height_inches
  $permissions['edit field_profile_height_inches'] = array(
    'name' => 'edit field_profile_height_inches',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_high_scool
  $permissions['edit field_profile_high_scool'] = array(
    'name' => 'edit field_profile_high_scool',
    'roles' => array(),
  );

  // Exported permission: edit field_profile_hometown
  $permissions['edit field_profile_hometown'] = array(
    'name' => 'edit field_profile_hometown',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
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

  // Exported permission: edit field_profile_motive
  $permissions['edit field_profile_motive'] = array(
    'name' => 'edit field_profile_motive',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_neighborhood
  $permissions['edit field_profile_neighborhood'] = array(
    'name' => 'edit field_profile_neighborhood',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_neighborhood_state
  $permissions['edit field_profile_neighborhood_state'] = array(
    'name' => 'edit field_profile_neighborhood_state',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_public_connections
  $permissions['edit field_profile_public_connections'] = array(
    'name' => 'edit field_profile_public_connections',
    'roles' => array(),
  );

  // Exported permission: edit field_profile_religion
  $permissions['edit field_profile_religion'] = array(
    'name' => 'edit field_profile_religion',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
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

  // Exported permission: edit field_profile_smoke
  $permissions['edit field_profile_smoke'] = array(
    'name' => 'edit field_profile_smoke',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_state
  $permissions['edit field_profile_state'] = array(
    'name' => 'edit field_profile_state',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: edit field_profile_uniqueness
  $permissions['edit field_profile_uniqueness'] = array(
    'name' => 'edit field_profile_uniqueness',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_birth_date
  $permissions['view field_profile_birth_date'] = array(
    'name' => 'view field_profile_birth_date',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_body_type
  $permissions['view field_profile_body_type'] = array(
    'name' => 'view field_profile_body_type',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_college
  $permissions['view field_profile_college'] = array(
    'name' => 'view field_profile_college',
    'roles' => array(),
  );

  // Exported permission: view field_profile_current_city
  $permissions['view field_profile_current_city'] = array(
    'name' => 'view field_profile_current_city',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_education
  $permissions['view field_profile_education'] = array(
    'name' => 'view field_profile_education',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_employment
  $permissions['view field_profile_employment'] = array(
    'name' => 'view field_profile_employment',
    'roles' => array(),
  );

  // Exported permission: view field_profile_ethnicity
  $permissions['view field_profile_ethnicity'] = array(
    'name' => 'view field_profile_ethnicity',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_eye_color
  $permissions['view field_profile_eye_color'] = array(
    'name' => 'view field_profile_eye_color',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_books
  $permissions['view field_profile_favorite_books'] = array(
    'name' => 'view field_profile_favorite_books',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_movies
  $permissions['view field_profile_favorite_movies'] = array(
    'name' => 'view field_profile_favorite_movies',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_quotes
  $permissions['view field_profile_favorite_quotes'] = array(
    'name' => 'view field_profile_favorite_quotes',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_favorite_tv_shows
  $permissions['view field_profile_favorite_tv_shows'] = array(
    'name' => 'view field_profile_favorite_tv_shows',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
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
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_free_feature_agree
  $permissions['view field_profile_free_feature_agree'] = array(
    'name' => 'view field_profile_free_feature_agree',
    'roles' => array(),
  );

  // Exported permission: view field_profile_graduate_school
  $permissions['view field_profile_graduate_school'] = array(
    'name' => 'view field_profile_graduate_school',
    'roles' => array(),
  );

  // Exported permission: view field_profile_hair_color
  $permissions['view field_profile_hair_color'] = array(
    'name' => 'view field_profile_hair_color',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_headline
  $permissions['view field_profile_headline'] = array(
    'name' => 'view field_profile_headline',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_height_feet
  $permissions['view field_profile_height_feet'] = array(
    'name' => 'view field_profile_height_feet',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_height_inches
  $permissions['view field_profile_height_inches'] = array(
    'name' => 'view field_profile_height_inches',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_high_scool
  $permissions['view field_profile_high_scool'] = array(
    'name' => 'view field_profile_high_scool',
    'roles' => array(),
  );

  // Exported permission: view field_profile_hometown
  $permissions['view field_profile_hometown'] = array(
    'name' => 'view field_profile_hometown',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_last_name
  $permissions['view field_profile_last_name'] = array(
    'name' => 'view field_profile_last_name',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_motive
  $permissions['view field_profile_motive'] = array(
    'name' => 'view field_profile_motive',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_neighborhood
  $permissions['view field_profile_neighborhood'] = array(
    'name' => 'view field_profile_neighborhood',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_neighborhood_state
  $permissions['view field_profile_neighborhood_state'] = array(
    'name' => 'view field_profile_neighborhood_state',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
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
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_sex_preference
  $permissions['view field_profile_sex_preference'] = array(
    'name' => 'view field_profile_sex_preference',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_smoke
  $permissions['view field_profile_smoke'] = array(
    'name' => 'view field_profile_smoke',
    'roles' => array(),
  );

  // Exported permission: view field_profile_state
  $permissions['view field_profile_state'] = array(
    'name' => 'view field_profile_state',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view field_profile_uniqueness
  $permissions['view field_profile_uniqueness'] = array(
    'name' => 'view field_profile_uniqueness',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  return $permissions;
}
