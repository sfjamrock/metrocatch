<?php
/**
 * @file
 * Template for the Metrocatch Gallery.
 *
 * Available variables:
 *   - display_controls: Identifier where to display carousel control buttons.
 *     Valid options are: 'above', 'below'. Empty means do not display controls.
 *   - controls: A string containing rendered carousel control or empty string
 *     if display_controls is empty.
 *
 *  @see template_preprocess_metrocatch_gallery_jcarousel()
 */
?>

<?php if ($display_controls == 'above') : ?>
  <?php print $controls; ?>
<?php endif; ?>
<ul class="<?php print $jcarousel_classes; ?>">
  <?php foreach ($rows as $id => $row): ?>
    <li class="<?php print $classes[$id]; ?>"><?php print $row; ?></li>
  <?php endforeach; ?>
</ul>
<?php if ($display_controls == 'below') : ?>
  <?php print $controls; ?>
<?php endif; ?>
