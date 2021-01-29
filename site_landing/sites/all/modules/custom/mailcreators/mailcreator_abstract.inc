<?php
/**
 * @file
 * mailcreator_abstract.inc
 */

abstract class MailCreator {
  private $system_name = 'abstract_mail_creator';
  public $result;

  /**
   * Constructor.
   * @param $system_name A system name of this MailCreator. Should contain lowercase
   * English letters and underscores.
   */
  public function __construct($system_name) {
    $this->system_name = $system_name;
  }

  /**
   * Get system name of this MailCreator.
   * @return string
   */
  public function getSystemName() {
    return $this->system_name;
  }

  /**
   * Get Drupal variable name to store some data for this MailCreator.
   * @param string $type Type of data to store (lowercase letters only).
   * @return srting
   */
  public function getVariableName($type) {
    return 'mailcreators_'. $type .'_'. $this->system_name;
  }

  /**
   * Check if this mailcreator is enabled (value stored in Drupal variable).
   * @return string
   */
  public function isEnabled() {
    return variable_get($this->getVariableName('enabled'), $this->isEnabledByDefault());
  }

  /**
   * Get mail subject text from Drupal variable.
   * @return string
   */
  public function getSubjectTemplate() {
    return variable_get($this->getVariableName('subject'), $this->getDefaultSubjectTemplate());
  }

  /**
   * Get mail body text from Drupal variable.
   * @return string
   */
  public function getBodyTemplate() {
    return variable_get($this->getVariableName('body'), $this->getDefaultBodyTemplate());
  }

  /**
   * Delete all Drupal variables associated with this MailCreator (to perform cleanup
   * when uninstalling module, etc). Also may be redefined in children to perform other
   * cleanup tasks.
   */
  public function reset() {
    variable_del($this->getVariableName('enabled'));
    variable_del($this->getVariableName('subject'));
    variable_del($this->getVariableName('body'));
  }

  /**
   * Get settings form elements for this mail creator. Typically these elements should be added to
   * Drupal settings form.
   */
  public function getSettingsForm(&$form_state) {
    $name_enabled = $this->getVariableName('enabled');
    $name_subject = $this->getVariableName('subject');
    $name_body = $this->getVariableName('body');
    return array(
      $name_enabled => array(
        '#type' => 'checkbox',
        '#title' => t('Enabled'),
        '#default_value' => isset($form_state[$name_enabled])?$form_state[$name_enabled]:$this->isEnabled(),
      ),
      $name_subject => array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => isset($form_state[$name_subject])?$form_state[$name_subject]:$this->getSubjectTemplate(),
      ),
      $name_body => array(
        '#type' => 'textarea',
        '#title' => t('Body'),
        '#default_value' => isset($form_state[$name_body])?$form_state[$name_body]:$this->getBodyTemplate(),
        '#description' => $this->getDescription() .' '. t('Available placeholders are: %placeholder_list.', array('%placeholder_list' => implode(', ', $this->getPlaceholderList()))),
      ),
    );
  }

  /**
   * Prepare and send emails.
   * @param array $params An array of arbitrary parameters for this MailCreator.
   * @return array An associative map where each key represents one recipient (these
   * are the keys returned by getMailSettings() method) and a value contains mail send results
   * or null if this mail creator is disabled.
   */
  public function send($params) {
    if (!$this->isEnabled()) {
      return;
    }
    $settings = $this->getMailSettings($params);
    $result = array();
    foreach ($settings as $name => $setting) {
      if (!isset($setting['to']) || !isset($setting['language'])) {
        $result[$name] = array('status' => FALSE, 'reason' => 'Not enough data to send mail.');
        continue;
      }
      $message = $this->prepareMessage($setting['placeholders'], $setting['language'], $setting);
      $drupal_mail_result = drupal_mail(
        'mailcreators',
        $this->getSystemName(),
        $setting['to'],
        $setting['language'],
        array(
          'subject' => $message['subject'],
          'body' => drupal_html_to_text(nl2br($message['body'])),
        ),
        $setting['from']
      );
//      dpm($drupal_mail_result);
      $result[$name] = array('status' => $drupal_mail_result['result'], 'data' => $drupal_mail_result);
      /*
      // Alternative mimemail sender.
      $mimemail_result = mimemail
      (
        $setting['from'],
        $setting['to'],
        $message['subject'],
        _filter_autop($message['body']),
        NULL,
        array(),
        NULL,
        $setting['attachments']
      );
      $result[$name] = array('status' => TRUE, 'data' => $mimemail_result);
      */
    }
    return $this->result = $result;
  }

  /**
   * Get default mail setting. Use this method to merge with your own array to complete it.
   * @return keyed array
   */
  protected function getDefaultMailSettings() {
    global $base_url;
    return array(
      'to' => NULL,
      'from' => variable_get('site_mail', ini_get('sendmail_from')),
      'attachments' => array(),
      'language' => NULL,
      'placeholders' => array(
        '@site' => variable_get('site_name', ''),
        '@site_url' => $base_url,
      ),
    );
  }

  /**
   * Translate this message and replace placeholders with actual values for recipient.
   * @param array $placeholders A list of placeholders to replace tokens in this message.
   * @param unknown_type $language Language to translate message to.
   * @param array $additional_data A setting array for this message (with structure defined in getDefaultMailSettings()).
   * @return array with 'subject' and 'body' keys containing the subject and the body of the mail respectively.
   */
  protected function prepareMessage($placeholders, $language, $additional_data) {
    return array(
      'subject' => t($this->getSubjectTemplate(), $placeholders, $language->language),
      'body' => t($this->getBodyTemplate(), $placeholders, $language->language),
    );
  }

  /**
   * Get a list of settings to send mail with. Each setting contains destination address,
   * language of email, attachments, etc.
   * This method should be redefined in children of this class.
   * @param array of arbitrary parameters for this MailCreator.
   * @return array An associative map in which a key is some string that represents one recipient
   * and value is an array with structure defined in getDefaultMailSettings().
   */
  protected function getMailSettings(&$params) {
    return array(
      'test_destination' => getDefaultMailSettings(),
    );
  }

  /**
   * Check if this mail creator is enabled by default (disabled mail creators do not send email).
   * This method should be redefined in children of this class.
   * @return string
   */
  protected function isEnabledByDefault() {
    return FALSE;
  }

  /**
   * Get default email subject text for this MailCreator.
   * This method should be redefined in children of this class.
   * @return string
   */
  protected function getDefaultSubjectTemplate() {
    return 'Subject';
  }

  /**
   * Get default email body text for this MailCreator.
   * This method should be redefined in children of this class.
   * @return string
   */
  protected function getDefaultBodyTemplate() {
    return 'Body';
  }

  /**
   * Get description text for this MailCreator (to use in admin ui, etc).
   * This method should be redefined in children of this class.
   * @return string
   */
  protected function getDescription() {
    return 'Abstract mail creator class.';
  }

  /**
   * Get placeholder list for this MailCreator (to use in admin ui, etc).
   * This method should be redefined in children of this class.
   * @return string
   */
  protected function getPlaceholderList() {
    return array('@site', '@site_url');
  }
}
