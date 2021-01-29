<?php
/**
 * @file
 * mailcreator_abstract.inc
 */

class TestMailCreator extends MailCreator {
  public function __construct() {
    parent::__construct('test');
  }

  protected function getMailSettings(&$params) {
    $account =& $params['user'];
    $setting = $this->getDefaultMailSettings();
    $setting['to'] = $account->mail;
    $setting['language'] = user_preferred_language($account);
    $setting['placeholders'] += array(
      '@name' => $account->name,
      '@email' => $account->email,
      '!account_page_url' => url('user/'. $account->uid, array('absolute' => TRUE)),
    );
    return array(
      'user' => $setting,
    );
  }

  protected function getDefaultSubjectTemplate() {
    return 'Hello, @name';
  }

  protected function getDefaultBodyTemplate() {
    return 'A message from site @site:
Hello!!!';
  }

  protected function getDescription() {
    return 'Send one user some email.';
  }

  protected function getPlaceholderList() {
    return array_merge(array('@name', '@email', '!account_page_url'), parent::getPlaceholderList());
  }
}
