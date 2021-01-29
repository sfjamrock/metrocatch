<?php
/**
 * @file
 * template for "cyd_utils_page_tags" theme handler.
 *
 * Variables:
 * - $tags: array of tags (strings)
 */
?>
<div class="cyd-utils-page-tags"><?php print t('<b>Tags:</b> @tag_list', array('@tag_list' => implode(', ', $tags))); ?></div>
