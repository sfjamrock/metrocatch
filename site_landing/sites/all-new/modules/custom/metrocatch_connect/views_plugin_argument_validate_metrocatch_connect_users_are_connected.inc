<?php
/**
 * @file
 * Contains Metrocatch argument validation plugin which cheks whether current user is
 * connected with a user presented in arguments.
 */

class views_plugin_argument_validate_metrocatch_connect_users_are_connected extends views_plugin_argument_validate {

  function validate_form(&$form, &$form_state) {
    $form['validate_connection_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Connection types'),
      '#options' => metrocatch_connect_get_all_connection_types(),
      '#default_value' => $this->argument->options['validate_connection_types'],
      '#process' => array('expand_checkboxes', 'views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
      '#prefix' => '<div id="edit-options-validate-connection-types-wrapper">',
      '#suffix' => '</div>',
    );
  }

  function validate_argument($argument) {
    global $user;
    $uid_to = $argument;
//     if (is_array($uid_to)) {
//       $uid_to = array_shift($argument);
//     }
    if (is_object($uid_to)) {
      $uid_to = $argument->uid;
    }
    $types = $this->argument->options['validate_connection_types'];
    return user_access('administer metrocatch connect') || ($uid_to && $user->uid && ($uid_to == $user->uid || metrocatch_connect_users_are_connected($uid_to, $user->uid, $types)));
  }
}
