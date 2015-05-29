<?php

/**
 * @file
 */

/**
 * @param $form
 * @param $form_state
 */
function micro_crm_campaignmonitor_settings($form, &$form_state) {
  $form['#prefix'] = '<div id="micro-crm-campiagnmonitor-settings-form">';
  $form['#suffix'] = '</div>';
  $form['#tree'] = TRUE;

  // Create a block for each list.
  $cm = CampaignMonitor::getConnector();
  $lists = $cm->getLists();
  $lists_options = array();
  if ($lists) {
    foreach ($lists as $list_id => $list) {
      // Get local configuration options and check if the list is enabled.
      if (campaignmonitor_is_list_enabled($list_id)) {
        $lists_options[$list_id] = $list['name'];
      }
    }
  }
  $form['micro_crm_campaignmonitor_lists'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Lists'),
    '#description' => t('Choose available lists'),
    '#options' => $lists_options,
    '#default_values' => variable_get('micro_crm_campaignmonitor_lists', NULL),
    '#ajax' => array(
      'callback' => 'micro_crm_campaignmonitor_list_callback',
      'method' => 'replace',
      'wrapper' => 'micro-crm-campiagnmonitor-settings-form',
    ),
  );
  if (isset($form_state['values']['micro_crm_campaignmonitor_lists'])) {
    $available_lists = array_values($form_state['values']['micro_crm_campaignmonitor_lists']);
  }
  else {
    $available_lists = array();
  }
  foreach ($lists as $list_id => $list) {
    $list = $cm->getExtendedList($list_id);
    if (in_array($list_id, $available_lists)) {
      $form['micro_crm_campaign_monitor'][$list_id] = array(
        '#type' => 'fieldset',
        '#title' => $lists_options[$list_id],
      );
      foreach ($list['CustomFields'] as $custom_field) {
        $form['micro_crm_campaign_monitor'][$list_id][$custom_field['Key']] = array(
          '#type' => 'select',
          '#title' => $custom_field['FieldName'],
          '#description' => $custom_field['DataType'],
          '#options' => array('_none' => t('- Choose -')) + _micro_crm_newsletter_get_custom_fields(),
        );
      }
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Regenerates form if something will change.
 */
function micro_crm_campaignmonitor_list_callback($form, $form_state) {
  return $form;
}

/**
 *
 */
function micro_crm_campaignmonitor_settigns_submit($form, &$form_state) {
  variable_set('micro_crm_campaignmonitor_settings', $form_state['values']);
}