<?php
/**
 * @file
 * metrocatch-conversation-facebook-post.tpl.php
 *
 * Formats Facebook post.
 *
 * Available variables:
 *   - message: The text of Facebook post.
 *   - attachments: Themed attachments for this post, if present.
 *
 * @see template_preprocess_metrocatch_conversation_facebook_post()
 */
?>

<div class="facebook-post-wrapper">
  <?php print $message; ?>
  <?php if (!empty($attachment)) : ?>
    <?php print $attachment; ?>
  <?php endif; ?>
</div>
