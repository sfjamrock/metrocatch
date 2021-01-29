<?php
/**
 * @file
 * Module page callbacks and related functions.
 */

/**
 * Page callback for 'connect/buy' - "Become a MetroCatch" page
 * @todo: insert "select form" on place of NULL
 */
function metrocatch_connect_become_metrocatch() {
  return theme(
    'metrocatch_connect_become_metrocatch_page',
    drupal_get_form('metrocatch_connect_feature_purchase_form')
  );
}