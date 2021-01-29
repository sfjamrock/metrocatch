<?php
/**
 * @file
 * Template for a login and profile creation page.
 *
 * Available variables:
 *  - login_text: The text over login form.
 *  - login_form: The login form itseft.
 *  - profile_question: 'Don\'t have profile?' text.
 *  - profile_text: 'It's free and only takes a minute.' text.
 *  - profile_link: Link to a profile creation form.
 *  - profile_benifits: 'Benefits include:' text.
 *  - profile_benefits_list: Text below 'Benefits include:'.
 *
 * @see template_preprocess_metrocatch_connect_login()
 */
?>

<div class='connect-login-wrapper'>
  <div class='login-wrapper'>
    <?php print $login_text; ?>
    <div class="connect-login-form">
      <?php print $login_form; ?>
    </div>
  </div>
  <div class='create-profile-wrapper'>
    <p><?php print $profile_question; ?></p>
    <p><?php print $profile_text; ?></p>
    <div class="create-profile-link">
      <p><?php print $profile_link; ?></p>
    </div>
    <p><?php print $profile_benefits; ?></p>
    <p><?php print $profile_benefits_list; ?></p>
  </div>
</div>
