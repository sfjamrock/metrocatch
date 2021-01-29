<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to STARTERKIT_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: STARTERKIT_breadcrumb()
 *
 *   where STARTERKIT is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */


/**
 * Implementation of HOOK_theme().
 */
function metrocatch_zen_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Returns HTML output of themed status messages.
 */
function metrocatch_zen_status_messages($display = NULL) {
  $messages = drupal_get_messages($display);
  if (empty($messages)) {
    return '';
  }
  $output = '<div id="messages-wrapper">';
  foreach ($messages as $type => $messages) {
    $output .= "<div class=\"messages $type\">\n";
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>'. $message ."</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div>\n";
  }
  $output .= '</div>';
  return $output;
}

/**
 * Theme implementation of theme('textfield')
 */
function metrocatch_zen_textfield($element) {
  if (isset($element['#js_text_placeholder'])) {
    metrocatch_zen_add_textfield_paceholder('#'. $element['#id'], $element['#js_text_placeholder']);
  }
  return theme_textfield($element);
}

/**
 * Theme implementation of theme('textarea')
 */
function metrocatch_zen_textarea($element) {
  if (isset($element['#js_text_placeholder'])) {
    metrocatch_zen_add_textfield_paceholder('#'. $element['#id'], $element['#js_text_placeholder']);
  }
  return theme_textarea($element);
}

/**
 * Add texfield placeholder setting to js
 *
 * @staticvar boolean $js_added
 *
 * @param string $selector
 *   textarea's selector
 * @param string $value
 *   text to display
 */
function metrocatch_zen_add_textfield_paceholder($selector, $value) {
  static $js_added;
  if (!$js_added) {
    drupal_add_js(drupal_get_path('theme', 'metrocatch_zen') .'/js/textfield_placeholders.js');
    drupal_add_js(array('textfield_placeholders' => array(array('selector' => ''))), 'setting');
    $js_added = TRUE;
  }
  drupal_add_js(array('textfield_placeholders' => array(array('selector' => $selector, 'text' => $value))), 'setting');
}

/**
 * Preprocess function for 'metrocatch_uc_user_purchase' theme handler
 *
 * @see metrocatch-uc-user-purchase.tpl.php
 */
function metrocatch_zen_preprocess_metrocatch_uc_user_purchase(&$vars) {
  $order = $vars['order'];
  $vars['contact_us'] = l(t('Contact us about this transaction'), 'contact-us-about-transaction', array('query' => array('order_id' => $order->order_id)));
}

/**
 * Preoprocess function for 'metrocatch_uc_purchase_history' theme item.
 */
function metrocatch_zen_preprocess_metrocatch_uc_purchase_history(&$vars) {
  $vars['connections_left'] = metrocatch_connect_get_connections_number_text($vars['account']->uid);
}

/**
 * Preprocess additional variables for the metrocatch of the Day Feature Landing Page.
 */
function metrocatch_zen_preprocess_metrocatch_connect_profile_featured(&$vars) {
  $account = $account = $vars['account'];
  $profile = metrocatch_profile_get_profile($account->uid);
  $class = $profile->field_profile_sex[0]['value'] .'-interested-in-'. $profile->field_profile_sex_preference[0]['value'];
  $vars['preference'] = '<div class="'. $class .' icon-small"></div>';
}

/**
 * Preprocess additional variables for the metrocatch of the Profile page.
 */
function metrocatch_zen_preprocess_metrocatch_connect_profile_teaser(&$vars) {
  $account = $account = $vars['account'];
  $profile = metrocatch_profile_get_profile($account->uid);
  $class = $profile->field_profile_sex[0]['value'] .'-interested-in-'. $profile->field_profile_sex_preference[0]['value'];
  $vars['preference'] = '<div class="'. $class .' icon-small"></div>';
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function metrocatch_zen_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */

function metrocatch_zen_preprocess_page(&$vars, $hook) {
  global $user;
  $vars['site_slogan'] = 'Washington, DC. <span class="date">'. date('F jS Y') .'</span>';
  $profile_page = (arg(0) == 'user' && ((!empty($user->uid) && !arg(1)) || (is_numeric(arg(1)) && (!arg(2) || (arg(2) == 'profile' && !arg(3))))));
  $connections_page = (arg(0) == 'user' && is_numeric(arg(1)) && arg(2) == 'connections');
  $catches_page = (arg(0) == 'catches');
  $conversation_page = (arg(0) == 'user' && is_numeric(arg(1)) && arg(2) == 'conversation');
  if ($catches_page || $profile_page || $connections_page || $conversation_page) {
    $vars['classes_array'][] = 'with-user-profile-block';
  }
  else {
    $vars['classes_array'][] = 'without-user-profile-block';
  }
  // Add some extra classes for all pages.
  $args = explode('/', $_GET['q']);
  $class_string = '';
  foreach ($args as $arg) {
    if (is_numeric($arg)) { // Skip dynamic arguments, such as node nid or similar.
      continue;
    }
    $class_string = empty($class_string) ? 'page-'. $arg : $class_string .'-'. $arg;
    $vars['classes_array'][] = $class_string;
  }
}

/**
 * Theme's implementation of theme('menu_item_link').
 */
function metrocatch_zen_menu_item_link($link) {
  if ($link['menu_name'] == 'menu-metromenu') {
    $link['localized_options']['attributes'] = array('class' => str_replace('/', '-', $link['href']));
    $first_space = strpos($link['title'], ' ');
    $first_word  = drupal_substr($link['title'], 0, $first_space);
    $link['title'] = '<b>'. $first_word .'</b>'. drupal_substr($link['title'], $first_space);
    $link['localized_options']['html'] = TRUE;
  }
  return zen_menu_item_link($link);
}

/**
 * Theme function for 'metrocatch_connect_connect_link'
 * Displays "connect" link
 *
 * @param int $to
 *   uid of the user to connect to
 * @param bool $absolute
 *   Boolean indicating whether link should be absolute.
 */
function metrocatch_zen_metrocatch_connect_connect_link($to, $absolute = FALSE) {
  static $js_added;
  global $user;
  if (!isset($js_added)) {
    popup_forms_parent_js();
    drupal_add_js(drupal_get_path('module', 'metrocatch_connect') .'/metrocatch_connect.js');
    $settings = array(
      'metrocatch_connect' => array(
        'uid_from' => $user->uid,
        'uid_to' => (isset($_GET['connect']) ? $_GET['connect'] : $to->uid),
        // When user logs in we show dialog with connect form.
        'show_form' => isset($_GET['connect']),
      ),
    );
    drupal_add_js($settings, 'setting');
    $js_added = TRUE;
  }
  $connect_icon = theme('image', drupal_get_path('theme', 'metrocatch_zen') .'/images/connect-icon-medium.png');
  $connect_text = '<div class="connect-text">'. t('Connect') .'</div>';
  return l('<div class="connect">'. $connect_icon . $connect_text .'</div>', 'connect/new/'. $to->uid, array('attributes' => array('class' => 'metrocatch-connect-link', 'id' => 'metrocatch-connect-to-'. $to->uid), 'absolute' => $absolute, 'html' => TRUE));
}

function metrocatch_zen_preprocess_flag__metromail_notify_flag(&$vars) {
  $checkbox = array(
    '#type' => 'checkbox',
    '#name' => 'checkbox_for_flag_'. $vars['flag']->fid,
    '#id' => 'checkbox-for-flag-'. $vars['flag']->fid,
    '#value' => ($vars['status'] == 'flagged'),
    '#attributes' => array(
      'class' => 'metromail-flag-checbox',
    ),
  );
  $vars['checkbox'] = drupal_render($checkbox);
}

function metrocatch_zen_preprocess_metromail_message(&$vars) {
  static $counter = 1;
  $message = $vars['message'];
  if (!$message->pid) {
    $vars['zebra'] = ($counter % 2 ? 'odd' : 'even');
    $vars['classes'] = array(
      'metromail-message',
      $vars['zebra']
    );
    $counter++;
  }
}

/**
 * Return HTML output of user's connections count.
 *
 * @param $account
 *   StdClass object representing account whose connections count is being displayed.
 * @param $with_colon
 *   (optional) Indicating whether to add colon after 'Connections' text.
 */
function metrocatch_zen_metrocatch_connect_count_connections($account, $count, $with_colon = FALSE) {
  $output = '<div class="user-count-connections-label">';
  $output .= t('Connections');
  if ($with_colon) {
    $output .= ':';
  }
  $output .= '</div>';
  $output .= '<div class="user-count-connections-value">';
  $output .= $count;
  $output .= '</div>';
  return $output;
}

/**
 * Return HTML output of time left to connect.
 *
 * @param $catch
 *   StdClass object representing Catch of the Day.
 * @param $with_colon
 *   (optional) Indicator whether to add colon after 'Time left' text.
 */
function metrocatch_zen_metrocatch_connect_time_left($catch, $with_colon = FALSE) {
  $output = '<div class="connect-time-left-label">';
  $output .= t('Time Left');
  if ($with_colon) {
    $output .= ':';
  }
  $output .= '</div>';
  $output .= '<div class="connect-time-left-value">';
  $output .= _metrocatch_connect_days_left_text($catch->expire, '1 Day', '@count Days');
  $output .= '</div>';
  return $output;
}

/**
 * Returns HTML output of a themed Catch preview.
 *
 * @param $catch
 *   StdClass object representing themed Catch.
 * @param $show_preference
 *   (optional) Flag whether display user's sexual preference or not.
 * @param $imagecache_preset
 *   (optional) String identifier of Imagecache preset to user for theming user's picture.
 */
function metrocatch_zen_metrocatch_connect_catch_preview($catch, $show_preference = FALSE, $imagecache_preset = 'userpic_large') {
  $output = '<div class="catch-preview-wrapper">';
  $output .= '<div class="catch-preview-preference ';
  $output .= $catch->sex .'-interested-in-'. $catch->sex_preference;
  $output .= '">';
  $output .= '<span class="catch-preview-preference-text element-invisible">'. metrocatch_profile_get_preference_text($catch->account) .'</span>';
  $output .= '</div>';
  $output .= '<div class="catch-preview-userpic">';
  $output .= theme('metrocatch_profile_picture', $imagecache_preset, $catch->account, array(), NULL, FALSE);
  $output .= '</div>';
  $output .= '<div class="catch-preview-name">';
  $output .= check_plain($catch->account->name);
  $output .= '</div>';
  $output .= '</div>';
  return l($output, $catch->account->path, array('html' => TRUE, 'attributes' => array('class' => 'other-catch')));
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function metrocatch_zen_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // Smetrocatch_zen_preprocess_node_page() or metrocatch_zen_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $vars['node']->type;
  if (function_exists($function)) {
    $function($vars, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function metrocatch_zen_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override theming of Facebook posts.
 */
function metrocatch_zen_metrocatch_conversation_facebook_post_attachment($post) {
  $output = '';
  $output .= '<div class="facebook-attachment clearfix">';
  $attachment = $post['attachment'];
  // Display first media (if present).
  if (!empty($attachment['media'])) {
    $output .= '<div class="attachment-media-wrapper">';
    $media = array_shift($attachment['media']);
    $output .= '<div class="attachment-media">';
    if (isset($media['src'])) {
      $preview = theme('image', $media['src'], $media['alt'], $media['alt'], array(), FALSE);
      if (isset($media['href'])) {
        $output .= l($preview, $media['href'], array('absolute' => TRUE, 'html' => TRUE));
      }
      else {
        $output .= $preview;
      }
    }
    $output .= '</div>';
    $output .= '</div>';
  }
  if (!empty($attachment['name'])) {
    $output .= '<div class="attachment-title">';
    $attachment['name'] = check_plain($attachment['name']);
    if (!empty($attachment['href'])) {
      $output .= l($attachment['name'], $attachment['href'], array('absolute' => TRUE));
    }
    else {
      $output .= $attachment['name'];
    }
    $output .= '</div>';
  }
  if (!empty($attachment['caption'])) {
    $output .= '<div class="attachment-caption">';
    $output .= check_plain($attachment['caption']);
    $output .= '</div>';
  }
  if (!empty($attachment['description'])) {
    $output .= '<div class="attachment-description">';
    $output .= check_plain($attachment['description']);
    $output .= '</div>';
  }
  // Display other media.
  if (!empty($attachment['media'])) {
    $output .= '<div class="attachment-other-media clearfix">';
    foreach ($attachment['media'] as $media) {
      $output .= '<div class="attachment-media">';
      if (isset($media['src'])) {
        $preview = theme('image', $media['src'], $media['alt'], $media['alt'], array(), FALSE);
        if (isset($media['href'])) {
          $output .= l($preview, $media['href'], array('absolute' => TRUE, 'html' => TRUE));
        }
        else {
          $output .= $preview;
        }
      }
      $output .= '</div>';
    }
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}

/**
 *  Theme the label for the empty value for options that are not required.
 *  The default theme will display N/A for a radio list and blank for a select.
 */
function metrocatch_zen_optionwidgets_none($field) {
  switch ($field['widget']['type']) {
    case 'optionwidgets_buttons':
    case 'nodereference_buttons':
    case 'userreference_buttons':
      return t('N/A');
    case 'optionwidgets_select':
    case 'nodereference_select':
    case 'userreference_select':
      if (empty($field['widget']['label'])) {
        return t('- None -');
      }
      return '- '. check_plain($field['widget']['label']) .' -';
    default :
      return '';
  }
}

/**
 * Override theming of foemt elemetns.
 *
 * Remove colons from element labels.
 */
function metrocatch_zen_form_element($element, $value) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  $output = '<div class="form-item"';
  if (!empty($element['#id'])) {
    $output .= ' id="'. $element['#id'] .'-wrapper"';
  }
  $output .= ">\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="'. $t('This field is required.') .'">*</span>' : '';

  if (!empty($element['#title'])) {
    $title = $element['#title'];
    if (!empty($element['#id'])) {
      $output .= ' <label for="'. $element['#id'] .'">'. $t('!title !required', array('!title' => filter_xss_admin($title), '!required' => $required)) ."</label>\n";
    }
    else {
      $output .= ' <label>'. $t('!title !required', array('!title' => filter_xss_admin($title), '!required' => $required)) ."</label>\n";
    }
  }

  $output .= " $value\n";

  if (!empty($element['#description'])) {
    $output .= ' <div class="description">'. $element['#description'] ."</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}