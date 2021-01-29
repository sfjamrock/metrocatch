<?php
/**
 * @file
 * "The Conversation" page template
 *
 * Available variables:
 *   - $records: array of rendered records.
 *   - $add_new_entry_form: themed form for creationg new posts.
 *   - $feed_facebook: Add/remove Facebook feed link.
 *   - $feed_twitter: Add/remove Twitter feed link.
 *
 * @see template_preprocess_metrocatch_conversation_page()
 */
?>
<div class="metrocatch-conversation-page">
<?php print $profile_teaser; ?>
<div class="metrocatch-conversation-feeds">
  <?php
    if (isset($feed_facebook)) {
      print $feed_facebook;
    }
    if (isset($feed_twitter)) {
      print $feed_twitter;
    }

  ?>
  </div>
  <?php print $add_new_entry_form; ?>
  <?php print $records; ?>
  <?php print $more_link; ?>
</div>
