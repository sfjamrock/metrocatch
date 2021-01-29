<?php
/**
 * @file
 * Contains page and form callbacks.
 */

/**
 * Page callback: handles AJAX form submission.
 *
 * Path: ajax-metrocatch-gallery-manage-form
 *
 * @param $op
 *   The ID of performed action.
 */
function metrocatch_gallery_manage_form_ajax($op) {
  $output = '';
  if ($form = metrocatch_gallery_ahah_helper('metrocatch_gallery_manage_form')) {
    switch ($op) {
      case 'save':
        $element = $form['areas']['edit_area'];
        // Remove selected class from all items.
        $embed_prefix = "\n<!--//--><![CDATA[//><!--\n";
        $embed_suffix = "\n//--><!]]>\n";
        $output .= '<script type="text/javascript">' . $embed_prefix . "jQuery('.gallery-item.selected').removeClass('selected');" . $embed_suffix . "</script>\n";
        break;
      case 'upload':
      case 'delete':
        $element = $form;
        break;
      default:
        $element = $form;
    }
    unset($element['#prefix'], $element['#suffix']);
    $output .= drupal_render($element);
  }
  else {
    drupal_set_message(t('Unknown error occurred. Please try again later.'), 'error');
  }
  $output = theme('status_messages') . $output;
  drupal_set_header('Content-Type: text/html; charset=utf-8');
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
}

/**
 * Page callback: change picture ordering in a user gallery.
 *
 * Path: ajax-metrocatch-gallery-change-weights
 */
function metrocatch_gallery_change_weights_ajax() {
  $nodes = (isset($_POST['nodes']) ? $_POST['nodes'] : array());
  $token = (isset($_POST['token']) ? $_POST['token'] : NULL);
  if (!drupal_valid_token($token, 'metrocatch_gallery_manage_form')) {
    return drupal_json(array('status' => FALSE));
  }
  foreach ($nodes as $nid => $weight) {
    if (($node = node_load($nid)) && node_access('update', $node) && is_numeric($weight)) {
      $node->field_picture_weight[0]['value'] = $weight;
      node_save($node);
    }
  }
  return drupal_json(array('status' => TRUE));
}

/**
 * Page callback: display manage gallery page.
 *
 * Path: metrocatch-gallery/manage/%user
 *
 * @param $account
 *   stdClass object of a user whose gallery to manage.
 * @param $edit
 *   (Optional) stdClass object of a picture to edit.
 */
function metrocatch_gallery_manage_page($account, $edit = NULL) {
  $form = drupal_get_form('metrocatch_gallery_manage_form', $account, $edit);
  if (!empty($_GET['ajax'])) {
    return drupal_json(array('status' => TRUE, 'data' => theme('status_messages') . $form));
  }
  return $form;
}

/**
 * Page callback: display a user gallery in popup.
 *
 * Path: ajax-metrocatch-gallery-browse
 *
 * @param $account
 *   stdClass object of a user whose gallery to browse.
 */
function metrocatch_gallery_browse_ajax($account) {
  global $metrocatch_gallery_browse_page;
  $metrocatch_gallery_browse_page = TRUE;
  return views_embed_view('metrocatch_gallery', 'page_1', $account->uid);
}

/**
 * Form construction handler for the Manage gallery form.
 *
 * @param $account
 *   stdClass object of a user whose gallery to manage
 *   or the user's UID (if form is opened in popup).
 * @param $edit
 *   (Optional) stdClass object of a picture to edit.
 */
function metrocatch_gallery_manage_form(&$form_state, $account, $edit = NULL) {
  drupal_set_title(t('Manage gallery'));
  // Form is opened in popup.
  if (is_numeric($account)) {
    $account = user_load($account);
  }
  $storage = &$form_state['storage'];
  $edit = (isset($storage['edit']) ? $storage['edit'] : $edit);
  $storage['edit'] = $edit;
  $nodes = metrocatch_gallery_load_gallery($account->uid);
  foreach ($nodes as $nid => $node) {
    if (isset($form_state['values']['gallery']['weights'][$node->nid])) {
      $nodes[$nid]->field_picture_weight[0]['value'] = $form_state['values']['gallery']['weights'][$node->nid];
    }
  }
  $form = array(
    // When form is opened in Popup the menu item access callback is not called.
    // So check access here too.
    '#access' => metrocatch_gallery_manage_access($account, $edit),
    '#after_build' => array('metrocatch_gallery_fix_form_action'),
    '#attributes' => array(
      'enctype' => 'multipart/form-data',
    ),
    '#account' => $account,
    '#edit' => $edit,
    '#prefix' => '<div id="manage-form-wrapper">',
    '#suffix' => '</div>',
    'areas' => array(
      '#prefix' => '<div id="areas-wrapper" class="manage-gallery-areas clearfix">',
      '#suffix' => '</div>',
      'edit_area' => array(
        '#prefix' => '<div id="edit-area" class="manage-gallery-area">',
        '#suffix' => '</div>',
        '#type' => 'item',
        'edit' => array(
          '#access' => !empty($edit),
          'preview' => array(
            '#value' => (!empty($edit) ? theme('imagecache', 'metrocatch_gallery_manage_upload_preview', $edit->field_picture_file[0]['filepath']) : ''),
          ),
          'description' => array(
            '#type' => 'textarea',
            '#title' => '',
            '#rows' => 3,
            '#cols' => 20,
            '#default_value' => (!empty($edit) ? $edit->field_picture_file[0]['data']['description'] : ''),
          ),
          'actions' => array(
            'save' => array(
              '#access' => (!empty($edit)),
              '#type' => 'submit',
              '#value' => t('Save'),
              '#submit' => array('metrocatch_gallery_manage_form_edit_picture_submit'),
              '#ahah' => array(
                'path' => 'ajax-metrocatch-gallery-manage-form/save',
                'wrapper' => 'edit-area',
                'method' => 'replace',
                'effect' => 'fade',
              ),
            ),
            'delete' => array(
              '#access' => (!empty($edit)),
              '#type' => 'submit',
              '#value' => t('Delete'),
              '#submit' => array('metrocatch_gallery_manage_form_delete_picture_submit'),
              '#ahah' => array(
                'path' => 'ajax-metrocatch-gallery-manage-form/delete',
                'wrapper' => 'manage-form-wrapper',
                'method' => 'replace',
                'effect' => 'fade',
              ),
            ),
          ),
        ),
      ),
      'upload_area' => array(
        '#prefix' => '<div id="upload-area" class="manage-gallery-area">',
        '#suffix' => '</div>',
        'upload' => array(
          'description' => array(
            '#type' => 'item',
            '#value' => t('Select an image on your computer'),
            '#description' => t('100px by 75px recommended'),
          ),
          'file' => array(
            '#type' => 'file',
            '#title' => t('Choose file'),
            '#size' => 15,
          ),
          'actions' => array(
            'upload' => array(
              '#type' => 'submit',
              '#value' => t('Add'),
              '#validate' => array('metrocatch_gallery_manage_form_upload_validate'),
              '#submit' => array('metrocatch_gallery_manage_form_edit_picture_submit'),
              '#ahah' => array(
                'path' => 'ajax-metrocatch-gallery-manage-form/upload',
                'wrapper' => 'manage-form-wrapper',
                'method' => 'replace',
                'effect' => 'fade',
              ),
            ),
          ),
        ),
      ),
    ),
    'gallery' => array(
      '#tree' => TRUE,
      '#theme' => 'metrpocatch_gallery_manage_form_gallery',
      '#nodes' => $nodes,
      '#account' => $account,
      '#edit' => $edit,
      'weights' => array(),
    ),
    'actions' => array(
      'okay' => array(
        '#type' => 'submit',
        '#value' => t('Change ordering'),
        '#name' => 'okay',
        '#submit' => array('metrocatch_gallery_manage_form_change_weights_submit', 'metrocatch_gallery_manage_form_clear_storage', 'metrocatch_gallery_manage_form_submit'),
      ),
    ),
  );
  $weights = range(-50, 50);
  foreach ($form['gallery']['#nodes'] as $node) {
    $form['gallery']['weights'][$node->nid] = array(
      '#type' => 'select',
      '#title' => '',
      '#options' => array_combine($weights, $weights),
      '#default_value' => $node->field_picture_weight[0]['value'],
    );
  }
  // During AHAH submission do not clear storage after editing single picture
  // Because in this case picture will be still available in $form['#parameters']
  // And form will be reduilded with it.
  if (empty($_GET['ajax']) && empty($form_state['ahah_submission'])) {
    $form['actions']['save']['#submit'][] = 'metrocatch_gallery_manage_form_clear_storage';
    $form['actions']['delete']['#submit'][] = 'metrocatch_gallery_manage_form_clear_storage';
  }

  return $form;
}

/**
 * Submission handler for the Manage gallery form.
 */
function metrocatch_gallery_manage_form_submit($form, &$form_state) {

}

/**
 * Validation handler for the Manage gallery form.
 *
 * Validates and uploads image.
 */
function metrocatch_gallery_manage_form_upload_validate($form, &$form_state) {
  $account = $form['#account'];
  $validators = array(
    'file_validate_is_image' => array(),
    'file_validate_size' => array(variable_get('metrocatch_gallery_max_file_size', '1024') * 1024),
  );
  if (!isset($_FILES['files']) || empty($_FILES['files']['name']['file'])) {
    form_set_error('file', t('Please attach file to upload.'));
  }
  elseif ($file = file_save_upload('file', $validators)) {
    // Get the max weight of gallery item.
    foreach ($form['gallery']['#nodes'] as $node) {
      $weight = $node->field_picture_weight[0]['value'];
      if (!isset($max_weight) || $weight > $max_weight) {
        $max_weight = $weight;
      }
    }
    $file->data = array(
      'description' => '',
    );
    $node = array(
      'uid' => $account->uid,
      'title' => $file->filename,
      'new' => TRUE,
      'name' => (isset($account->name) ? $account->name : ''),
      'type' => 'metrocatch_gallery_picture',
      'language' => '',
      'field_picture_file' => array(),
      'field_picture_weight' => array(
        array('value' => $max_weight + 1),
      ),
    );
    $node['field_picture_file'][] = (array) $file;
    $form_state['storage']['edit'] = (object) $node;
  }
}

/**
 * Submission handler for the Manage gallery form.
 *
 * Add user upload to the gallery.
 */
function metrocatch_gallery_manage_form_edit_picture_submit($form, &$form_state) {
  $account = $form['#account'];
  $edit = &$form_state['storage']['edit'];
  $file = &$edit->field_picture_file[0];
  $file['data']['description'] = (isset($form_state['values']['description']) ? $form_state['values']['description'] : '');
  // Move file to the galleries folder.
  if (empty($edit->nid)) {
    $directory = variable_get('metrocatch_gallery_directory', 'galleries') .'/'. $account->uid;
    // Ensure that directory exist and is writable.
    $directory_path = file_create_path($directory);
    if (!file_check_directory($directory_path, FILE_CREATE_DIRECTORY, 'file')) {
      return;
    }
    $destination = $directory .'/'. $file['filename'];
    $source = (object) $file;
    if (!file_copy($source, $destination, FILE_EXISTS_RENAME)) {
      drupal_set_message(t('Failed to upload the picture image; please try again later.'), 'error');
    }
    $file['filepath'] = $source->filepath;
    // Update record in files table.
    drupal_write_record('files', $source, array('fid'));
  }
  node_save($edit);
  // New node is displayed in edit area after creating.
  if (!$edit->is_new) {
    // Node has been successfully saved, empty Edit area.
    $edit = 0;
  }
}

/**
 * Submission handler for the Manage gallery form.
 *
 * Delete picture from gallery.
 */
function metrocatch_gallery_manage_form_delete_picture_submit($form, &$form_state) {
  $edit = &$form_state['storage']['edit'];
  if (!empty($edit->nid)) {
    node_delete($edit->nid);
  }
  $edit = 0;
}

/**
 * Submission handler for the Manage gallery form.
 *
 * Updates picture weights.
 */
function metrocatch_gallery_manage_form_change_weights_submit($form, &$form_state) {
  foreach ($form['gallery']['#nodes'] as $node) {
    $original_weight = $node->field_picture_weight[0]['value'];
    $weight = (isset($form_state['values']['gallery']['weights'][$node->nid]) ? $form_state['values']['gallery']['weights'][$node->nid] : $original_weight);
    if ($weight != $original_weight) {
      $node->field_picture_weight[0]['value'] = $weight;
      node_save($node);
    }
  }
  // Clear static cache.
  node_load(NULL, NULL, $reset);
}

/**
 * Submission handler for the Manage gallery form.
 *
 * Clears form storage.
 */
function metrocatch_gallery_manage_form_clear_storage($form, &$form_state) {
  unset($form_state['storage']);
}

/**
 * Submission handler for the Manage gallery form.
 *
 * Fixes form action.
 */
function metrocatch_gallery_fix_form_action($form, &$form_state) {
  if (isset($_GET['action'])) {
    $form['#action'] = $_GET['action'];
  }
  if (!isset($form_state['storage']['action'])) {
    $form_state['storage']['action'] = $form['#action'];
  }
  else {
    $form['#action'] = $form_state['storage']['action'];
  }
  return $form;
}

/**
 * Helper function: get and process AHAH form.
 *
 * @param $form_id
 *   The ID of the form to get from cache.
 */
function metrocatch_gallery_ahah_helper($form_id) {
  if ($_POST['form_id'] != $form_id) {
    return FALSE;
  }
  $form_state = array(
    'storage' => NULL,
    'submitted' => FALSE,
    'ahah_submission' => TRUE,
  );
  $form_build_id = $_POST['form_build_id'];
  if (!$form = form_get_cache($form_build_id, $form_state)) {
    return FALSE;
  }

  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  return $form;
}
