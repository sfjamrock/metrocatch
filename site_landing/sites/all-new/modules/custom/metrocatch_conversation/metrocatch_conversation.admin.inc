<?php
/**
 * @file
 * Module page callbacks for administation pages only
 */


/**
 * Form constructor for the metrocatch_conversation_settings_form form.
 *
 * Path: admin/metrocatch/metrocatch-conversation
 *
 * Constructs form with module settings.
 */
function metrocatch_conversation_settings_form($form_state) {
  $form = array(
    'metrocatch_conversation_posts_initial' => array(
      '#type' => 'textfield',
      '#title' => t('Posts to display on "conversation" page initially.'),
      '#default_value' => variable_get('metrocatch_conversation_posts_initial', 8),
    ),
    'metrocatch_conversation_posts_more_number' => array(
      '#type' => 'textfield',
      '#title' => t('Posts to load by "more" link on "conversation" page.'),
      '#default_value' => variable_get('metrocatch_conversation_posts_more_number', 8),
    ),
    'social_integration' => array(
      '#type' => 'fieldset',
      '#title' => t('Social networks integrations'),
      'facebook' => array(
        '#type' => 'fieldset',
        '#title' => t('Facebook Application'),
        'metrocatch_conversation_facebook_app_id' => array(
          '#type' => 'textfield',
          '#title' => t('Facebook application ID'),
          '#default_value' => variable_get('metrocatch_conversation_facebook_app_id', ''),
        ),
        'metrocatch_conversation_facebook_secret' => array(
          '#type' => 'textfield',
          '#title' => t('Facebook application secret key'),
          '#default_value' => variable_get('metrocatch_conversation_facebook_secret', ''),
        ),
        'facebook_subscribtion_actions' => array(
          'update_subscribtion' => array(
            '#type' => 'submit',
            '#value' => t('Update Subscription'),
            '#submit' => array('_metrocatch_conversation_add_subscription'),
          ),
          'view_subscribtions' => array(
            '#value' => l(t('Checkout app subscriptions'), 'admin/metrocatch/metrocatch-conversation/view-facebook-subscribtions'),
          ),
        ),
      ),
      'twitter' => array(
        '#type' => 'fieldset',
        '#title' => t('Twitter Application'),
        'metrocatch_conversation_twitter_app_id' => array(
          '#type' => 'textfield',
          '#title' => t('Consumer key'),
          '#default_value' => variable_get('metrocatch_conversation_twitter_app_id', ''),
        ),
        'metrocatch_conversation_twitter_secret' => array(
          '#type' => 'textfield',
          '#title' => t('Consumer secret'),
          '#default_value' => variable_get('metrocatch_conversation_twitter_secret', ''),
        ),
      ),
    ),
  );
  return system_settings_form($form);
}

/**
 * Page callback: allows site administrator to overview Facebook subscribtions.
 *
 * Path: admin/metrocatch/metrocatch-conversation/view-facebook-subscribtions
 */
function metrocatch_conversation_overview_facebook_subscribtions_page() {
  $subscriptions = metrocatch_conversation_get_subscriptions();
  $header = array(
    t('Object'),
    t('Callback URL'),
    t('Fields'),
    t('Active'),
  );
  $rows = array();
  foreach ($subscriptions as $subscribtion) {
    $rows[] = array(
      check_plain($subscribtion['object']),
      check_plain($subscribtion['callback_url']),
      check_plain(implode(', ', $subscribtion['fields'])),
      ($subscribtion['active'] == TRUE ? t('Yes') : t('No')),
    );
  }
  return empty($rows) ? t('No subscribtions are found') : theme('table', $header, $rows);
}

/**
 * Form constructor for the metrocatch_conversation_post_broadcast_entry_form form.
 *
 * Path: admin/metrocatch/metrocatch-conversation/send-broadcast
 *
 * Using this form site administrators can post broadcast Conversation entries on user's walls.
 */
function metrocatch_conversation_post_broadcast_entry_form(&$form_state) {
  return array(
    'body' => array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#required' => TRUE,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Post broadcast message'),
    ),
  );
}

/**
 * Form submission handler for the metrocatch_conversation_post_broadcast_entry_form form.
 *
 * Saves broadcast conversation entry in database.
 *
 * @see metrocatch_conversation_post_broadcast_entry_form()
 */
function metrocatch_conversation_post_broadcast_entry_form_submit($form, &$form_state) {
  global $user;
  $node = (object) array(
    'type' => 'conversation_entry',
    'title' => t('Conversation entry'),
    'body' => $form_state['values']['body'],
    'uid' => $user->uid,
    'status' => 1,
    'field_conversation_entry_conv_id' => array(
      array(
        'uid' => NULL,
      ),
    ),
  );
  node_save($node);
  drupal_set_message('Broadcast message has been successfully published.');
}

/**
 * Helper function: get a list of Facebook application subscriptions.
 *
 * @return
 * 	An array of subscriptions with following keys:
 * 	  - object: The type of the object you want to receive updates about,
 *      e.g. user, page or permissions.
 * 	  - fields: A comma-separated list. This is a list of properties or connections on the specified object.
 *    - callback_url: A callback URL to which Facebook will post subscription updates.__PHP_Incomplete_Class
 *
 * @link http://developers.facebook.com/docs/reference/api/realtime/
 */
function metrocatch_conversation_get_subscriptions() {
  $facebook = metrocatch_conversation_facebook_init();
  $params = array(
    'access_token' => $facebook->getAppId() .'|'. $facebook->getAppSecret(), // this method must be called with App access token
  );
  try {
    $ret = $facebook->api($facebook->getAppId() . '/subscriptions', 'GET', $params);
    return (isset($ret['data']) ? $ret['data'] : array());
  }
  catch (FacebookApiException $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}
