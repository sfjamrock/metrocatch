<?php
/**
 * @file
 * Template for the user registration form.
 *
 * Available variables:
 */
?>

<div class="metrocatch-profile-form-group required-fields">
  <div class="metrocatch-profile-form-row clearfix">
    <?php print $field_profile_first_name; ?>
    <?php print $field_profile_last_name; ?>
  </div>

  <div class="metrocatch-profile-form-row clearfix">
    <?php print $mail; ?>
    <?php print $conf_mail; ?>
  </div>

  <?php print $pass; ?>

  <div class="metrocatch-profile-form-row inline-labels clearfix">
    <?php print $field_profile_sex; ?>
    <?php print $field_profile_sex_preference; ?>
  </div>
</div>

<div class="metrocatch-profile-form-group additional-fields">
  <p class="metrocatch-profile-help-text">
    <?php print $help_message; ?>
  </p>
  <div class="metrocatch-profile-form-row field-birthday metrocatch-profile-private-field clearfix">
    <?php print $field_profile_birth_date; ?>
    <?php if (isset($field_profile_birth_date_make_private)) : ?>
      <?php print $field_profile_birth_date_make_private; ?>
    <?php endif; ?>
  </div>

  <?php if (isset($field_profile_hometown) || isset($field_profile_state)) : ?>
    <div class="metrocatch-profile-form-row metrocatch-profile-private-field clearfix">
        <div class="metrocatch-profile-form-inline">
          <?php print $field_profile_hometown; ?>
          <?php print $field_profile_state; ?>
        </div>
        <?php if (isset($field_profile_hometown_make_private)) : ?>
          <?php print $field_profile_hometown_make_private; ?>
        <?php endif; ?>
    </div>
  <?php endif; ?>

  <?php if (isset($field_profile_current_city) || isset($field_profile_neighborhood_state) || isset($field_profile_neighborhood)) : ?>
  <div class="metrocatch-profile-form-row metrocatch-profile-private-field clearfix">
    <div class="metrocatch-profile-form-inline">
      <?php print $field_profile_current_city; ?>
      <?php print $field_profile_neighborhood_state; ?>
      <?php print $field_profile_neighborhood; ?>
    </div>
    <?php if (isset($field_profile_current_city_make_private)) : ?>
      <?php print $field_profile_current_city_make_private; ?>
    <?php endif; ?>
  </div>
  <?php endif; ?>

  <?php if (isset($field_profile_favorite_books) || isset($field_profile_favorite_movies)) : ?>
  <div class="metrocatch-profile-favorites">
    <?php if (isset($field_profile_favorite_books)) : ?>
      <div class="metrocatch-profile-form-row metrocatch-profile-private-field clearfix">
        <?php print $field_profile_favorite_books; ?>
        <?php if (isset($field_profile_favorite_books_make_private)) : ?>
          <?php print $field_profile_favorite_books_make_private; ?>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <?php if (isset($field_profile_favorite_movies)) : ?>
      <div class="metrocatch-profile-form-row metrocatch-profile-private-field clearfix">
        <?php print $field_profile_favorite_movies; ?>
        <?php if (isset($field_profile_favorite_movies_make_private)) : ?>
          <?php print $field_profile_favorite_movies_make_private; ?>
        <?php endif; ?>
      </div>
    <?php endif; ?>

  </div>
  <?php endif; ?>
</div>

<div class="metrocatch-profile-form-group other-fields">
  <?php if (isset($picture)) : ?>
    <?php print $picture; ?>
  <?php endif; ?>

  <?php if (isset($metrocatch_conversation_flag)) : ?>
    <?php print $metrocatch_conversation_flag; ?>
  <?php endif; ?>

  <?php if (isset($field_profile_public_connections)) : ?>
    <?php print $field_profile_public_connections; ?>
  <?php endif; ?>

  <?php if (isset($field_profile_free_feature_agree)) : ?>
    <?php print $field_profile_free_feature_agree; ?>
  <?php endif; ?>

  <?php if (isset($agree_terms)) : ?>
    <?php print $agree_terms; ?>
  <?php endif; ?>
</div>

<?php if ($is_admin) : ?>
  <div class="metrocatch-profile-form-group admin-fields">
    <?php print $field_profile_feature_question; ?>
    <?php print $field_marketing_text; ?>
    <?php print $field_profile_connections_left; ?>
    <?php print $menu; ?>
    <?php print $author; ?>
    <?php print $revision_information; ?>
    <?php print $options; ?>
    <?php print $comment_settings; ?>
    <?php print $path; ?>
    <?php print $attachments; ?>
  </div>
<?php endif; ?>

<?php if (isset($captcha)) : ?>
  <?php print $captcha; ?>
<?php endif; ?>

<div class="metrocatch-profile-form-group actions">
  <?php print $submit; ?>
  <?php print $save_and_proceed; ?>
</div>

<?php print $form_id; ?>
<?php print $form_build_id; ?>
<?php print $form_token; ?>
