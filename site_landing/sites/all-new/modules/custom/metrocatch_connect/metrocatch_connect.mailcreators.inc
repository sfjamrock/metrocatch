<?php
/**
 * @file
 * metrocatch_connect.mailcreators.inc
 *
 * Contains MailCreator descendant class for sending Share emails.
 */

/**
 * This class is used for sending Daily Mails.
 */
class MetrocatchShareCreator extends MailCreator {

  public function __construct() {
    parent::__construct('metrocatch_connect_share_mail');
  }

  /**
   * Get a list of settings to send mail with. Each setting contains destination address,
   * language of email, attachments, etc.
   *
   * @param
   *   An array of arbitrary parameters for this MailCreator.
   *
   * @return
   *   An associative array in which a key is some string that represents one recipient
   *   and value is an array with structure defined in getDefaultMailSettings().
   */
  protected function getMailSettings(&$params) {
    global $base_url;
    return array(
      'share' => array_merge(parent::getDefaultMailSettings(), array(
        'to' => implode(', ', $params['recipients']),
        'from' => $params['from'],
        'language' => language_default(),
        'placeholders' => array(
          '!message' => check_plain($params['message']),
          '!url' => $params['url'],
        ),
      )),
    );
  }

  /**
   * Check if this mail creator is enabled by default (disabled mail creators do not send email).
   *
   * @return
   *   TRUE if this MailCreator is enabled by default.
   */
  protected function isEnabledByDefault() {
    return TRUE;
  }

  /**
   * Get default email subject text for this MailCreator.
   *
   * @return
   *   A string with default message subject template.
   */
  protected function getDefaultSubjectTemplate() {
    return 'metrocatch.';
  }

  /**
   * Get default email body text for this MailCreator.
   *
   * @return
   *   A string with default message body template.
   */
  protected function getDefaultBodyTemplate() {
    return "
    !message

    <a href=\"!url\">!url</a>
    ";
  }

  /**
   * Get description text for this MailCreator (to use in admin ui, etc).
   */
  protected function getDescription() {
    return t('This email will be sent when user clicks on "Email" share link.');
  }

  /**
   * Get placeholder list for this MailCreator (to use in admin ui, etc).
   *
   * @return
   *   An array with all available placeholders.
   */
  protected function getPlaceholderList() {
    return array_merge(array(
      '!message',
      '!url',
    ),
    parent::getPlaceholderList());
  }
}
