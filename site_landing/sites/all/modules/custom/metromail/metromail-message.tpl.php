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
 * - $type:
 *   Machine name of this message type.
 * - $was_read:
 *   Boolean indicating whether the message was read by the recipient.
 * - $sent:
 *   Formatted date when this message was sent.
 * - $sender:
 *   Full loaded stdClass object of the user, who has sent this message.
 * - $username:
 *   Themed username of $sender, links on his profile.
 * - $userpic:
 *   Themed userpick of $sender.
 * - $account:
 *   Full loaded stdClass object of the recipient.
 * - $message:
 *   The message object.
 *
 * Additional variables:
 * - $zebra:
 *   Zebra stripping: 'odd' or 'even'.
 *
 *   @see template_preprocess_metromail_message()
 */
?>

<div id="metromail-message-<?php print $message->mid;?>" class="<?php print implode(' ', $classes); ?>">
  <div class="metromail-message-userpic">
    <?php print $userpic;?>
  </div>
  <div class="metromail-message-container clearfix">
    <div class="metromail-message-header">
      <div class="metromail-message-username">
        <?php print $username;?>
        <?php if ($self_reply) :  ?>
          <span class="recipient-separator">&nbsp;&gt;&nbsp;</span>
          <?php print $recipient_name;?>
        <?php endif;?>
      </div>
      <div class="metromail-message-date">
        <?php print $sent;?>
      </div>
    </div>
    <div class="metromail-message-body">
    <?php print $body;?>
    <?php if (!empty($additions)) :?>
      <div class="metromail-message-additions">
        <?php print $additions; ?>
      </div>
    <?php endif;?>
    </div>
    <?php if (!empty($actions)) :?>
    <div class="metromail-message-actions">
      <?php print implode('', $actions);?>
    </div>
    <?php endif;?>
  </div>
  <div class="metromail-reply-wrapper"></div>
  <?php if ($message->has_children || !$message->thread_id) :?>
    <div class="children first-child">
      <?php if ($thread) :?>
        <?php print $thread; ?>
      <?php endif;?>
    </div>
  <?php endif;?>
</div>
