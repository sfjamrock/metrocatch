<?php
/**
 * @file
 * metrocatch-email-other-catches.tpl.php
 *
 * Template for the Other Catches block in the DailyMail.
 *
 * Available variables:
 *   - $catches: An array of catches to theme.
 *   - $per_line: An integer number of catches per line.
 *   - $imagecache_preset: A string containing machine-readable name of the Imagecache preset
 *     to use for themin user pictures.
 *
 * @see template_preprocess_metrocatch_email_other_catches()
 */
?>

<table class="other-catches-block">
  <?php if (!empty($catches)) : ?>
    <tbody>
      <?php foreach (array_chunk($catches, $per_line) as $chunk) : ?>
        <tr>
          <?php foreach ($chunk as $catch) : ?>
            <?php $catch->account->path = 'catches/'. $catch->sex .'-interested-in-'. $catch->sex_preference .'/'. $catch->uid; ?>
            <td>
              <div id="other_catch_<?php print $catch->uid; ?>" class="catch-preview-wrapper">
                <div class="catch-preview-userpic">
                  <?php print theme('metrocatch_profile_picture', $imagecache_preset, $catch->account, array('absolute' => TRUE)); ?>
                </div>
                <div class="catch-preview-name">
                  <?php
                    // Link path must be absolute:
                    $catch->account->path_options = array('absolute' => TRUE);
                    print theme('username', $catch->account);
                  ?>
                </div>
              </div>
            </td>
          <?php endforeach; ?>
          <?php for ($cells_in_last_row = count($chunk); $cells_in_last_row < $per_line; $cells_in_last_row++) : ?>
            <td>
              <div class="empty-cell">&nbsp;</div>
            </td>
          <?php endfor; ?>
        </tr>
      <?php endforeach; ?>
    </tbody>
  <?php endif; ?>
</table>
