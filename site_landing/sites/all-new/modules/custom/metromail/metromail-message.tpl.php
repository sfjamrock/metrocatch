<?php
/**
 * @file
 * metromail-message.tpl.php
 *
 * Available variables:
 *
 * - $body:
 *   The body of this message. Formatted text.
 * - $additions:
 *   Formatted additions made by other modules.
 * - $sent:
 *   Formatted date when this message was sent.
 * - $sender:
 *   Full loaded stdClass object of the user, who has sent this message.
 * - $sender_username:
 *   Themed username of $sender, links on his profile.
 * - $sender_userpic:
 *   Themed userpick of $sender.
 * - $recipient:
 *   Full loaded stdClass object of the recipient. Anonymous user for broadcast messages.
 * - $message:
 *   The message object.
 *
 *   @see template_preprocess_metromail_message()
 */
?>

<div id="metromail-message-<?php print $message->mid; ?>" class="clearfix metromail-message <?php print $zebra; ?>">
  <div class="metromail-message-userpic">
    <?php print $sender_userpic; ?>
  </div>
  <div class="metromail-message-container clearfix">
    <div class="metromail-message-header">
      <div class="metromail-message-username">
        <?php print $sender_username; ?>
        <?php if (empty($message->pid)): ?>
          <span class="recipient-separator">&nbsp;&gt;&nbsp;</span>
          <?php print $recipient_name;?>
        <?php endif; ?>
      </div>
      <div class="metromail-message-date">
        <?php print $sent; ?>
      </div>
    </div>
    <div class="metromail-message-body">
    <?php print $body; ?>
    <?php if (!empty($additions)): ?>
      <div class="metromail-message-additions">
        <?php print $additions; ?>
      </div>
    <?php endif; ?>
    </div>
    <?php if (!empty($actions)): ?>
    <div class="metromail-message-actions">
      <?php print implode('', $actions); ?>
    </div>
    <?php endif; ?>
  </div>
  <div class="metromail-reply-wrapper"></div>
  <?php if ($message->has_children || !$message->thread_id): ?>
    <div class="children first-child">
      <?php if ($thread): ?>
        <?php print $thread; ?>
      <?php endif; ?>
    </div>
  <?php endif; ?>
</div>
