<?php
/**
 * @file
 * metrocatch-profile-account-form.tpl.php
 *
 * Template for the user profile edition form.
 *
 * @see template_preprocess_metrocatch_profile_account_form()
 */
?>

<div class="metrocatch-profile-form-group required-fields">
  <div class="metrocatch-profile-form-row clearfix">
    <?php print $field_profile_first_name; ?>
    <?php print $field_profile_last_name; ?>
  </div>

  <div class="metrocatch-profile-form-row clearfix">
    <?php print $mail; ?>
    <?php if (isset($conf_mail)) : ?>
      <?php print $conf_mail; ?>
    <?php endif; ?>
  </div>

  <?php if (isset($pass)) : ?>
    <?php print $pass; ?>
  <?php endif; ?>

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

  <div class="metrocatch-profile-form-row metrocatch-profile-private-field clearfix">
      <div class="metrocatch-profile-form-inline">
        <?php print $field_profile_hometown; ?>
        <?php print $field_profile_state; ?>
      </div>
      <?php if (isset($field_profile_hometown_make_private)) : ?>
        <?php print $field_profile_hometown_make_private; ?>
      <?php endif; ?>
  </div>

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

  <div class="metrocatch-profile-favorites">
    <div class="profile-indepth-info">
      <?php $printed_fields = array('field_profile_hometown', 'field_profile_state', 'field_profile_current_city', 'field_profile_neighborhood_state', 'field_profile_neighborhood'); ?>
      <?php foreach (metrocatch_profile_second_step_fields() as $field_name) : ?>
        <?php if (in_array($field_name, $printed_fields)) {continue;} ?>
        <?php if (isset($$field_name)) : ?>
          <div class="metrocatch-profile-form-row metrocatch-profile-private-field metrocatch-profile-private-field-<?php print $field_name; ?> clearfix">
            <?php print $$field_name; ?>
            <?php if (isset(${$field_name .'_make_private'})) : ?>
              <?php print ${$field_name .'_make_private'}; ?>
            <?php endif; ?>
          </div>
        <?php endif; ?>
      <?php endforeach; ?>
    </div>
  </div>
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

<div class="metrocatch-profile-form-group actions">
  <?php print $changed; ?>
  <?php print $buttons; ?>
  <a class="metro-button" href="<?php print url('user/'. $form['#account']->uid .'/delete'); ?>"><?php print t('Delete'); ?></a>
</div>

<?php print $form_id; ?>
<?php print $form_build_id; ?>
<?php print $form_token; ?>
