<?php
/**
 * @file
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $title: Block title.
 * - $content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: An ID for the block, unique within each module.
 * - $block->region: The block region embedding the current block.
 * - $edit_links: A list of contextual links for the block. It can be
 *   manipulated through the variable $edit_links_array from preprocess
 *   functions.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - block: The current template type, i.e., "theming hook".
 *   - block-[module]: The module generating the block. For example, the user
 *     module is responsible for handling the default user navigation block. In
 *     that case the class would be "block-user".
 *   - first: The first block in the region.
 *   - last: The last block in the region.
 *   - region-count-[x]: The position of the block in the list of blocks in the
 *     current region.
 *   - region-odd: An odd-numbered block of the list of blocks in the current
 *     region.
 *   - region-even: An even-numbered block of the list of blocks in the current
 *     region.
 *   - count-[x]: The position of the block in the list of blocks on the current
 *     page.
 *   - odd: An odd-numbered block of the list of blocks on the current page.
 *   - even: An even-numbered block of the list of blocks on the current page.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $edit_links_array: An array of contextual links for the block.
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $block_html_id: A valid HTML ID and guaranteed unique.
 *
 * @see template_preprocess()
 * @see zen_preprocess()
 * @see template_preprocess_block()
 * @see zen_preprocess_block()
 * @see zen_process()
 */
  global $base_path;
  $theme_path = $base_path . drupal_get_path('theme', 'metrocatch_zen');
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div id="metrocatch-footer-social-links">
    <a href="http://twitter.com/#!/metrocatch" target="_blank">
      <img src="<?php print $theme_path;?>/images/icons/twitter-icon-w24h24.png" alt="<?php print t('Follow Us'); ?>" title="<?php print t('Follow Us'); ?>"/>
    </a>
    <a href="http://www.facebook.com/metrocatch" target="_blank">
      <img src="<?php print $theme_path;?>/images/icons/facebook-icon-w24h24.png" alt="<?php print t('metrocatch on Facebook'); ?>" title="<?php print t('metrocatch on Facebook'); ?>"/>
    </a>
    <?php print $facebook_social_link; ?>
  </div>
  <div id="metrocatch-footer-title-prefix"></div>
  <?php if ($title): ?>
    <h2 class="title"><?php print $title; ?></h2>
  <?php endif; ?>

  <div class="content">
    <?php print $content; ?>
  </div>

  <?php print $edit_links; ?>
</div><!-- /.block -->
