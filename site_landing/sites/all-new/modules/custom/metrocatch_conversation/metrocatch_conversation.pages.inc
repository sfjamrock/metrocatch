<?php
/**
 * @file
 * Module page callbacks and related functions.
 */

/**
 * Page callback: displays user's conversation page.
 *
 * Path: user/%user/conversation
 *
 * @param object $account
 *   Target account, which conversation we need to display.
 */
function metrocatch_conversation_page($account) {
  global $user;
  $skip = arg(3);
  $skip = $skip ? $skip : 0;
  if ($user->uid != $account->uid) {
    // Check if user wants to display only his own posts:
    $flag = flag_get_flag('metrocatch_conversation_private');
    $only_own = $flag->is_flagged($account->uid);
  }
  else {
    // User views his own wall, show all post.
    $only_own = FALSE;
  }
  if (isset($_GET['ajax'])) {
    // ajax conversation loading
    $entries = metrocatch_conversation_get_entries($account, $only_own, FALSE, variable_get('metrocatch_conversation_posts_more_number', 8), $skip);
    $themed_entries = theme('metrocatch_conversation_entries', $entries, $skip);
    $more_link = theme('metrocatch_conversation_view_more_link', $account, $skip + count($entries));
    // Extend JS settings with new ones:
    $javascript = drupal_add_js(NULL, 'header');
    $js_settings = drupal_get_js('header', array('setting' => $javascript['setting']));
    return drupal_json(
      array(
        'status' => TRUE,
        'data' =>  $js_settings . $themed_entries . $more_link,
      )
    );
  }
  else {
    // view as usual page
    drupal_set_title(t('The Conversation'));
    $entries = metrocatch_conversation_get_entries($account, $only_own, FALSE, variable_get('metrocatch_conversation_posts_initial', 8), $skip);
    return theme('metrocatch_conversation_page', $entries, $account->uid, $skip + count($entries));
  }
}

/**
 * "add new conversation entry" form callback
 *
 * @param int $to_uid
 *   wall owner's user id
 */
function metrocatch_converstion_new_entry_form($form_state, $to_uid) {
  return array(
    'new_record' => array(
      '#js_text_placeholder' => t('Post new record'),
      '#type' => 'textarea',
      '#title' => t('What\'s the haps?'),
      '#deafult_value' => isset($form_state['values']['new_record']) ? $form_state['values']['new_record'] : '',
      '#required' => TRUE,
      '#resizable' => FALSE,
      '#rows' => 2,
    ),
    'wall_id' => array(
      '#type' => 'value',
      '#value' => $to_uid,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Post'),
    ),
  );
}

/**
 * Submit handler for the "add new conversation entry" form.
 */
function metrocatch_converstion_new_entry_form_submit($form, $form_state) {
  // create new conversation_entry node
  global $user;
  $node = new stdClass();
  $node->type = 'conversation_entry';
  $node->body = $form_state['values']['new_record'];
  $node->created = time();
  $node->title = 'Conversation entry';
  $node->uid = $user->uid;
  $node->status = 1;
  $node->comment = COMMENT_NODE_READ_WRITE;
  $node->field_conversation_entry_conv_id[0]['uid'] = $form_state['values']['wall_id'];
  node_save($node);
}

/**
 * Page callback: displays 'Add Facebook feed' page.
 *
 * Path: metrocatch-conversation/feed/facebook
 *
 * Gets and saves the Facebook server-side oAuth token in local database.
 */
function metrocatch_conversation_add_facebook_feed_page() {
  global $user;
  $facebook = metrocatch_conversation_facebook_init();
  $token = $facebook->getAccessToken();
  $fbuid = $facebook->getUser();
  if ($token && $fbuid) {
    metrocatch_conversation_add_feed($user->uid, $fbuid, 'facebook', $token);
  }
  $redirect = implode('/', array_slice(arg(), 3));
  $redirect = urldecode($redirect);
  drupal_goto($redirect);
}

/**
 * Page callback: redirects user to a Twitter authorization page..
 *
 * Path: metrocatch-conversation/feed/twitter
 *
 * Gets and saves in session the request token, redirects user to authorization page.
 */
function metrocatch_conversation_add_twitter_feed_page() {
  global $user;
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : '';
  if (isset($_REQUEST['destination'])) {
    // drupal_goto() works unproperly if something is stored in 'destination'.
    unset($_REQUEST['destination']);
  }
  $login_url = _metrocatch_conversation_get_twitter_login_url($redirect);
  if ($login_url !== FALSE) {
    drupal_goto($login_url);
  }
  return MENU_NOT_FOUND;
}

/**
 * Page callback: finalize Twitter authorization, redirects user to a page he came from.
 *
 * Path: metrocatch-conversation/feed/twitter/finalize
 *
 * Finalize Twitter authorization, gets and saves access_token in database.
 *
 * @see metrocatch_conversation_add_twitter_feed_page()
 */
function metrocatch_conversation_add_twitter_feed_finalize_page() {
  global $user;
  $redirect =  $_SESSION['metrocatch_conversation_twitter_oauth']['redirect'];
  unset($_SESSION['metrocatch_conversation_twitter_oauth']);
  if (isset($_GET['denied'])) {
    // User has denied application, just redirect him.
    drupal_goto($redirect);
  }
  $twitter = metrocatch_conversation_twitter_init();
  try {
    $ret = $twitter->oAuthAccessToken($_GET['oauth_token'], $_SESSION['metrocatch_conversation_twitter_oauth']['verifier']);
    if (isset($ret['oauth_token'])) {
      $twitter_user = $twitter->usersShow($ret['user_id']);
      // Get the last tweet id, this will be used in quickcron updates:
      $latest_tweet = $twitter->statusesUserTimeline($twitter_user['id'], NULL, NULL, NULL, 1);
      if (isset($latest_tweet[0]['id'])) {
        $latest_tweet_id = $latest_tweet[0]['id'];
      }
      else {
        $latest_tweet_id = 0;
      }
      metrocatch_conversation_add_feed($user->uid, $twitter_user['id'], 'twitter', $ret['oauth_token'], $ret['oauth_token_secret'], $latest_tweet_id);
      drupal_goto($redirect);
    }
  }
  catch (TwitterException $e) {
    watchdog('debug', 'Error occurred while getting Twitter access token: @message', array('@message' => $e->getMessage()), WATCHDOG_ERROR);
  }
  return MENU_NOT_FOUND;
}

/**
 * Page callback: removes user's Facebook feed.
 *
 * Path: metrocatch-conversation/unfeed/facebook
 *
 * Removes feed entry from Database, removes application from user's Facebook, clears up user's session.
 */
function metrocatch_conversation_remove_facebook_feed_page() {
  global $user;
  metrocatch_conversation_remove_feed($user->uid, 'facebook');
  $redirect = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : '';
  drupal_goto($redirect);
}

/**
 * Page callback: removes user's Twitter feed.
 *
 * Path: metrocatch-conversation/unfeed/twitter
 *
 * Removes feed entry from Database.
 */
function metrocatch_conversation_remove_twitter_feed_page() {
  global $user;
  metrocatch_conversation_remove_feed($user->uid, 'twitter');
  $redirect = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : '';
  drupal_goto($redirect);
}

/**
 * Page callback for Facebook real-time updates.
 *
 * Path: metrocatch/real-time-feed/facebook
 *
 * Facebook sends 2 requests on this page:
 * 1. If request method is GET, then Facebook validates this page,
 *    and we should return received random number stored in $_GET['hub_challenge'].
 * 2. If request method is POST, then we receive updates in JSON format.
 *    Each update is an associate array with following structure:
 *    - entry: an array of updates:
 *      - uid: the ID of the user on Facebook.
 *      - time: UNIX timestamp of when this update was made.
 *      - changes: an array of changes for this user. Each change contain:
 *        - field: Facebook graph object field. We are interested only in 'feed'.
 *        - value: an array with following structure:
 *          - item: the id of changed item, e.g. 'status' or 'photo'.
 *          - verb: text representation of this change, e.g. 'update' or 'add'.
 *   We walk through updates array looking for interesting updates. If user has updates
 *   we are interested in, then we save his Facebook UID and his earliest update time in database.
 *   Then quickcron.php script will invoke hook_quickcron() were we will get all the latest updates and
 *   handle them. This is done in order to process this request in 15 seconds.
 *
 *   @see metrocatch_conversation_quickcron()
 *   @link http://developers.facebook.com/docs/reference/api/realtime/
 */
function metrocatch_conversation_facebook_updates() {
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      print $_GET['hub_challenge'];
      break;
    case 'POST':
      $updates = json_decode(file_get_contents('php://input'), TRUE);
      watchdog('debug', 'Facebook updates received :'. print_r($updates, 1));
      $last_changes = array();
      // Save last update for each user.
      foreach ($updates['entry'] as $update) {
        $fbuid = $update['uid'];
        $valid_update = FALSE;
        // Support for new updates format:
        if (!isset($update['changes']) && isset($update['changed_fields'])) {
          foreach ($update['changed_fields'] as $changed_field) {
            switch ($changed_field) {
              case 'feed':
                $valid_update = TRUE;
                break;
            }
          }
        }
        else {
          foreach ($update['changes'] as $change) {
            if ($change['field'] != 'feed') {
              continue;
            }
            switch ($change) {
              case ($change['value']['item'] == 'status' && $change['value']['verb'] == 'update'):
              case ($change['value']['item'] == 'link'   && $change['value']['verb'] == 'share'):
              case ($change['value']['item'] == 'photo'  && $change['value']['verb'] == 'add'):
              case ($change['value']['item'] == 'story'  && $change['value']['verb'] == 'add'):
                // We are not interested in other updates.
                $valid_update = TRUE;
                break;
            }
          }
        }
        if ($valid_update && (!isset($last_changes[$fbuid]) || $last_changes[$fbuid] > $update['time'])) {
          $last_changes[$fbuid] = $update['time'];
        }
      }
      if (!empty($last_changes)) {
        $queue = array(
          'data' => $last_changes,
        );
        drupal_write_record('metrocatch_conversation_facebook_queue', $queue);
      }
      break;
  }
}

/**
 * Page callback: displays conversation entry delete form.
 *
 * Path: metrocatch-conversation-remove-entry/%node
 *
 * Delete one conversation entry form;
 *
 * @param object $node
 *   node to delete
 */
function metrocatch_conversation_delete_entry_page($node) {
  return drupal_get_form('metrocatch_conversation_delete_entry_form', $node->nid);
}

/**
 * Page callback: AJAXly submits comment form.
 *
 * Path: ajax-metrocatch-conversation-post-comment
 */
function metrocatch_conversation_ajax_comment_submit_page() {
  if (strpos($_POST['form_id'], 'comment_form') === 0 && ($form = metrocatch_conversation_ahah_helper())) {
    unset($form['#prefix'], $form['#suffix']);
    $output = array(
      'status' => TRUE,
      'data' => theme('status_messages') . drupal_render($form),
    );
    return drupal_json($output);
  }
  return MENU_NOT_FOUND;
}

/**
 * Page callback: Retrieve a JSON object containing comments for a conversation entry.
 *
 * Path: ajax-metrocatch-conversation-get-comments
 *
 * Here we get all comments for a specified node with ID greater than specified last_comment ID.
 *
 * @see metrocatch_conversation_ajax_get_comments_access()
 */
function metrocatch_conversation_ajax_get_comments_page() {
  $comments = array();
  $node = node_load($_POST['nid']);
  $order = _comment_get_display_setting('sort', $node);
  $query = "SELECT c.* FROM {comments} c WHERE c.nid = %d AND c.status = %d AND c.cid > %d";
  if ($order == COMMENT_ORDER_NEWEST_FIRST) {
    $query .= ' ORDER BY c.cid ASC';
  }
  else {
    $query .= ' ORDER BY c.cid DESC';
  }
  $result = db_query($query, $node->nid, COMMENT_PUBLISHED, $_POST['last_comment']);
  while ($comment = db_fetch_object($result)) {
    $comments[$comment->cid] = theme('comment_view', $comment, $node);
  }
  if ($order == COMMENT_ORDER_NEWEST_FIRST) {
    end($comments);
  }
  else {
    reset($comments);
  }
  $latest_comment = key($comments);
  $output = array(
    'comments' => implode("\r\n", $comments),
    'last_comment' => $latest_comment,
  );
  return drupal_json($output);
}
