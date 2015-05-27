<?php

/**
 * @file
 */

/**
 * @param $form
 * @param $form_state
 */
function micro_crm_campaignmonitor_settings($form, &$form_state) {
  // Create a block for each list.
  $lists = CampaignMonitor::getConnector()->getLists();
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
      'callback' => 'mciro_crm_campaign_monitor_list_callback',
    ),
  );
  return system_settings_form($form);
}
