<?php

/**
 * @file
 * Declaration of campaign entity and entity callbacks.
 */

/**
 * Implements hook_entity_info().
 */
function crm_campaign_entity_info() {
  $entities['crm_campaign'] = array(
    'label' => t('Campaign', array(), array('context' => 'crm')),
    'controller class' => 'CRMCampaignController',
    'base table' => 'crm_campaign',
    'revision table' => 'crm_campaign_revision',
    'load hook' => 'crm_campaign_load',
    'uri callback' => 'crm_campaign_uri',
    'label callback' => 'crm_campaign_label',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'campaign_id',
      'label' => 'title',
      'revision' => 'revision_id',
    ),
    'bundles' => array(
      'crm_campaign' => array(
        'label' => t('Campaign', array(), array('context' => 'crm')),
      ),
    ),
    'view modes' => array(
      'administrator' => array(
        'label' => t('Administrator'),
        'custom settings' => FALSE,
      ),
    ),
    'access callback' => 'crm_campaign_access',
  );
  return $entities;
}

/**
 * Campaign access callback.
 *
 * @param $op
 * @param $entity
 * @param null $account
 */
function crm_campaign_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('crm_campaign')->access($op, $entity, $account);
}

/**
 * @param $id
 * @return bool|mixed
 */
function crm_campaign_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = crm_campaign_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 * @param $id
 */
function crm_campaign_delete($id) {
  crm_campaign_delete_multiple(array($id));
}

/**
 * @param $ids
 */
function crm_campaign_delete_multiple($ids) {
  entity_delete_multiple('crm_campaign', $ids);
}

/**
 * @param $id
 * @return bool|mixed
 */
function crm_campaign_view($campaign, $view_mode = 'administrator', $language = NULL, $page = NULL) {
  if (!is_array($campaign)) {
    $campaign = array($campaign->campaign_id => $campaign);
  }
  return entity_get_controller('crm_campaign')->view($campaign, $view_mode, $language, $page);
}

/**
 * @param $ids
 * @return mixed
 */
function crm_campaign_load_multiple($ids) {
  return entity_load('crm_campaign', $ids);
}

/**
 * @param $campaign
 * @return string
 */
function crm_campaign_label($campaign) {
  return $campaign->title;
}

/**
 * @param $campaign
 * @return array|null
 */
function crm_campaign_uri($campaign) {
  return array(
    'path' => 'admin/crm/campaign/' . $campaign->campaign_id,
  );
}
