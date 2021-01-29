<?php
/**
 * @file
 * Theme implementation for page tags.
 *
 * Variables:
 * - $tags: array of tags (strings)
 */
?>
<div class="cyd-utils-page-tags"><?php print t('<b>Tags:</b> @tag_list', array('@tag_list' => implode(', ', $tags))); ?></div>
