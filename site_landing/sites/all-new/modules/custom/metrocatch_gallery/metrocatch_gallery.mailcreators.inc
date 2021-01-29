<?php
/**
 * @file
 * Contains Mailcreatores for the metrocatch_gallery module.
 */

class metrocatchGalleryInappropriatePictureNotificationCreator extends MailCreator {

  public function __construct() {
    parent::__construct('metrocatch_gallery_inappropriate_picture_notification');
  }

  protected function getMailSettings(&$params) {
    global $base_url;
    $settings = parent::getDefaultMailSettings();
    $settings = array_merge($settings, array('to' => variable_get('site_mail', ''), 'language' => language_default()));
    $placeholders = array(
      '@username' => $params['account']->name,
      '@picture_url' => url('node/'. $params['picture']->nid, array('absolute' => TRUE)),
      '@gallery_owner' => $params['owner']->name,
    );
    $settings['placeholders'] = array_merge($settings['placeholders'], $placeholders);
    return array(
      'gallery_inappropriate_picture_notification' => $settings,
    );
  }

  protected function isEnabledByDefault() {
    return TRUE;
  }

  protected function getDefaultSubjectTemplate() {
    return 'Metrocatch: picture has been marked as inappropriate.';
  }

  protected function getDefaultBodyTemplate() {
    return '@username has marked a picture in <a href="@picture_url">@gallery_owner\'s gallery</a> as inappropriate.';
  }

  protected function getDescription() {
    return t('Notifies site adminstrator about inappropriate picture in a user gallery.');
  }

  protected function getPlaceholderList() {
    $new_placeholders = array('@username', '@picture_url', '@gallery_owner');
    return parent::getPlaceholderList() + $new_placeholders;
  }
}
