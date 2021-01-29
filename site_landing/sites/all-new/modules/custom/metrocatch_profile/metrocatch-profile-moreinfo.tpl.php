<?php
/**
 * @file
 *
 * Template of "more info" user profile area.
 *
 * Variables:
 * - $uid: User id.
 * - $fields - Array of fields to display.
 *   Each element contains:
 *   - label: The label of this field.
 *   - output: Rendered HTML output of this field.
 *
 */
?>
<div class="profile-moreinfo-wrapper">
  <?php foreach ($fields as $field): ?>
    <div class="profile-moreinfo-field">
      <div class="profile-moreinfo-field-label group-title">
        <?php print $field['label']; ?>
      </div>
      <div class="profile-moreinfo-field-value-trimmed">
        <?php print $field['output']; ?>
      </div>
    </div>
  <?php endforeach; ?>
</div>
