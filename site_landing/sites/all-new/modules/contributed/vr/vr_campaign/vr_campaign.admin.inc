<?php
// $Id: vr_campaign.admin.inc,v 1.1.2.2 2010/01/22 22:05:50 jbomb Exp $

/**
 * vr_campaign_admin
 * 
 * Provide administrative options for managing VerticalResponse 
 * campaigns.  Currently there is no functionality associated with
 * this form.
 * 
 * TODO: Allow administrators to delete a campaign and campaign node.
 * Add the ability for administrators to create a campaign node from
 * a VerticalResponse campaign.
 */
function vr_campaign_admin($form_state) {
  $param = vr_sort_param($_GET['order'], $_GET['sort'], vr_campaign_form_headers());
  $campaigns = vr_api('enumerateEmailCampaigns', $param);
  $total = count($campaigns);
  $list_size = (empty($_GET['size'])) ? 25 : $_GET['size'];
  $offset = $_GET['page'] * $list_size;
  $pager_end = (($offset + $list_size) < $total) ? $offset + $list_size : $total;
  for ($i = $offset; $i < $pager_end; $i++) {
    $campaign = $campaigns[$i];
    $sql = 'SELECT nid FROM {vr_campaign} WHERE vrid = %d';
    $nid = db_result(db_query($sql, $campaign->id));
    $name = (!empty($nid)) ? l($campaign->name, 'node/' . $nid) : $campaign->name;
    $rows[] = array(
      array(
        'data' => $name,
        ),
      array(
        'data' => $campaign->type,
        ),
      array(
        'data' => ucfirst($campaign->display_status),
        ),
      array(
        'data' => ($campaign->tested == FALSE) ? t('No') : t('Yes'),
        ),
      array(
        'data' => date('F j, Y', strtotime($campaign->creation_date)),
        ),
      );
  }
  $header = vr_campaign_form_headers();
  $form['campaings'] = array(
    '#type' => 'item',
    '#value' => theme('table', $header, $rows),
    );
  // Control table
  $controls[] = array(
    theme('vr_list_pager', $total, $list_size),
    array(
      'align' => 'right',
      'data' => theme('vr_list_controls'),
      )
    );
  $form['controls'] = array(
    '#type' => 'item',
    '#value' =>  theme('table', array(), $controls),
    );
  return $form;
}

/**
 * Provide header names and a tool to convert $_GET variables to
 * field_names.
 * 
 * @param string
 *  The value of $_GET['order'] from a sortable theme_table header.
 *  This value will be equal to one of the 'data' attributes listed
 *  below.
 * @return mixed
 *  Returns an array of column headings when no argument is passed, or
 *  the field_name for a sorting.
 * 
 * TODO: Examine using this method for sorting other administration
 * forms.
 */
function vr_campaign_form_headers($sort = NULL) {
  $header = array(
    array(
      'field' => 'name',
      'data' => t('Name'),
      ),
    array(
      'field' => 'type',
      'data' => t('Type'),
      ),
    array(
      'field' => 'status',
      'data' => t('Status'),
      ),
    array(
      'field' => 'tested',
      'data' => t('Tested'),
      ),
    array(
      'field' => 'creation_date',
      'data' => t('Created'),
      ),
    );
  return $header;
}
