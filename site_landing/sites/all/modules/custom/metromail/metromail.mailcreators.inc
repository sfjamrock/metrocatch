<?php
/**
 * @file
 * metromail.mailcreators.inc
 *
 * Contains 2 classes for sending notification e-mails.
 */

/**
 * This class is used for sending notifications about new private or system MetroMail messages.
 * It doesn't check if user wants to receive notifications.
 *
 * @see metromail_settings_form()
 * @see metromail_notify_recipient()
 */
class MetroMailCreator extends MailCreator {

  public function __construct() {
    parent::__construct(METROMAIL_CREATOR_NAME);
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
    $additional_settings = array(
      'accountFrom' => $params['accountFrom'],
      'accountTo' => $params['accountTo'],
      'messageObject' => $params['messageObject'],
    );
    $additional_settings += parent::getDefaultMailSettings();
    $additional_placeholders = array(
      '@username' => $params['accountTo']->name,
      '@username_from' => $params['accountFrom']->uid ? $params['accountFrom']->name : variable_get('metromail_system_name', t('MetroCatch')),
      '!link_to_metromail' => url('user/' . $params['accountTo']->uid . '/metromail', array('absolute' => TRUE)),
      '!user_from' => $params['accountFrom']->uid ? theme('username', $params['accountFrom']) : variable_get('metromail_system_name', t('MetroCatch')),
      '!text' => $params['messageObject']->body,
      '!sent_date' => format_date($params['messageObject']->sent),
      '!site_link' => l(variable_get('site_name', ''), $base_url, array('absolute' => TRUE)),
    );
    $additional_settings['placeholders'] += $additional_placeholders;
    $required_settings = array(
      'to' => $params['accountTo']->mail,
      'language' => user_preferred_language($params['accountTo']),
    );
    $settings = array_merge($additional_settings, $required_settings);
    return array(
      'notification' => $settings,
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
    return 'You have received new message on @site from @username_from.';
  }

  /**
   * Get default email body text for this MailCreator.
   *
   * @return
   *   A string with default message body template.
   */
  protected function getDefaultBodyTemplate() {
    return "Hi @username,\r\n
You have received new message from !user_from on @site_url.\r\n";
  }

  /**
   * Get description text for this MailCreator (to use in admin ui, etc).
   */
  protected function getDescription() {
    return t('Notifies users about new MetroMails.');
  }

  /**
   * Get placeholder list for this MailCreator (to use in admin ui, etc).
   *
   * @return
   *   An array with all available placeholders.
   */
  protected function getPlaceholderList() {
    $newPlaceholders = array(
      '@username',
      '@username_from',
      '!link_to_metromail',
      '!user_from',
      '!text',
      '!sent_date',
      '!site_link',
    );
    return parent::getPlaceholderList() + $newPlaceholders;
  }
}

/**
 * Second class is used for sending notifications about new broadcast messages.
 *
 * It also doesn't check if users want to receive notifications.
 *
 * @see metromail_settings_form()
 * @see metromail_notify_everyone()
 */
class BroadcastMetroMailCreator extends MailCreator {

  public function __construct() {
    parent::__construct(METROMAIL_BROADCAST_CREATOR_NAME);
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
    $additional_settings = array(
      'accounts' => $params['accounts'],
      'messageObject' => $params['messageObject'],
    );
    $additional_settings += parent::getDefaultMailSettings();
    $additional_settings['placeholders'] += array(
      '@site' => variable_get('site_name', ''),
      '@site_url' => $base_url,
      '!site_link' => l(variable_get('site_name', ''), $base_url, array('absolute' => TRUE)),
      '!text' => $params['messageObject']->body,
      '!sent_date' => format_date($params['messageObject']->sent),
    );
    $emails = array();
    foreach ($params['accounts'] as $account) {
      $emails[] = $account->mail;
    }
    $emails = implode(', ', $emails);
    $required_settings = array(
      'to' => $emails,
      'language' => language_default(),
    );
    $settings = array_merge($additional_settings, $required_settings);
    return array(
      'broadcast_notification' => $settings,
    );
  }

  /**
   * Get placeholder list for this MailCreator (to use in admin ui, etc).
   *
   * @return
   *   An array with all available placeholders.
   */
  protected function getPlaceholderList() {
    return array(
      '@site',
      '@site_url',
      '!text',
      '!sent_date',
      '!site_link',
    );
  }

  /**
   * Get default email subject text for this MailCreator.
   */
  protected function getDefaultSubjectTemplate() {
    return 'You have received a new system message on @site.';
  }

  /**
   * Get default email body text for this MailCreator.
   */
  protected function getDefaultBodyTemplate() {
    return "You have received a new system message on @site_url.\r\n";
  }

  /**
   * Get description text for this MailCreator (to use in admin ui, etc).
   */
  protected function getDescription() {
    return t('Notifies users about new broadcast MetroMails.');
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

}
