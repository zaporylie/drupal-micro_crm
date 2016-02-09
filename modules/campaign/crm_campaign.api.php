<?php

/**
 * @file
 */

/**
 * @return array
 */
function hook_crm_campaign_status_info() {
  $info['status'] = array(
    'title' => t('Status'),
    'description' => t('Example status'),
    'weight' => -99,
  );
  return $info;
}

/**
 * @return array
 */
function hook_crm_campaign_method_info() {
  $info['vbo'] = array(
    'name' => t('VBO'),
    'access path' => 'admin/crm/campaign/%/channel',
    'access path argument position' => 3,
    'auto' => FALSE,
  );
  return $info;
}
