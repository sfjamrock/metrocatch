<?php
/**
 * @file
 * Tempalte for a User gallery in the Manage Gallery form.
 *
 * Available variables:
 *  - element: An array representing Forms API element containing this gallery.
 *  - nodes: An array of gallery nodes.
 *
 * @see template_preprocess_metrpocatch_gallery_manage_form_gallery()
 */
?>
<?php
  $query = array('destination' => ltrim($_SERVER['REQUEST_URI'], '/'));
?>

<div class="gallery-items clearfix">
  <?php foreach ($nodes as $node) : ?>
    <div id="gallery-item-<?php print $node->nid; ?>" class="gallery-item<?php print (!empty($edit->nid) && $node->nid == $edit->nid) ? ' selected' : ''; ?>">
      <?php print theme('imagecache', 'metrocatch_gallery_manage_picture', $node->field_picture_file[0]['filepath']); ?>
      <?php print drupal_render($element['weights'][$node->nid]); ?>
      <?php print l(t('Edit'), 'metrocatch-gallery/manage/'. $account->uid .'/'. $node->nid, array('attributes' => array('id' => 'edit-picture-'. $node->nid, 'class' => 'edit-picture'), 'query' => $query)); ?>
    </div>
  <?php endforeach; ?>
</div>
