<?php
/**
 * @file
 * One "The Conversation" entry template
 *
 * variables:
 *   - $node: populated entry's node object (standart)
 *   - $body: rendered node
 *   - $entry_links: themed bottom links of the entry (not node!)
 *   - $comments: standart comments area
 *   - $comment_form: form for commenting this node
 */
?>
<div class="metrocatch-conversation-entry entry-<?php print $node->nid; ?>">
  <?php print $author; ?>
  <?php print $body; ?>
  <?php print $entry_links; ?>
  <?php print $comments; ?>
  <?php print $comment_form; ?>
</div>
