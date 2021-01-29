<?php
/**
 * @file
 * metrocatch-notifications-dailymail-preview.tpl.php
 *
 * Preview Daily Mail.
 *
 * Availablle variables:
 *   - language: An object of the user prefered language.
 *   - styles: A string containing rendered CSS tags.
 *   - content: A string containing Daily Mail rendered content.
 *
 * @see template_preprocess_metrocatch_notifications_dailymail_preview()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
<title><?php print $title; ?></title>
<?php print $styles; ?>
</head>
<body>
<div id="metrocatch-dailymail-preview">
  <?php print $content; ?>
</div>
</body>
</html>
