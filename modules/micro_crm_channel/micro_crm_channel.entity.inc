<?php

/**
 * @file
 *
 */

function micro_crm_channel_entity_info() {
  $entities['micro_crm_channel'] = array(
    'label' => t('Channel', array(), array('context' => 'micro crm')),
    'controller class' => 'MicroCRMChannelController',
    'base table' => 'micro_crm_channel',
    'revision table' => 'micro_crm_channel_revision',
    'load hook' => 'micro_crm_channel_load',
    'uri callback' => 'micro_crm_channel_uri',
    'label callback' => 'micro_crm_channel_label',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'channel_id',
      'bundle' => 'type',
      'revision' => 'revision_id',
    ),
    'bundle keys' => array(
      'bundle' => 'type',
    ),
    'bundles' => array(),
    'view modes' => array(
      'administrator' => array(
        'label' => t('Administrator'),
        'custom settings' => FALSE,
      ),
      'customer' => array(
        'label' => t('Customer'),
        'custom settings' => FALSE,
      ),
    ),
    'access callback' => 'micro_crm_channel_access',

    // Inline entity controller support.
    'inline entity form' => array(
      'controller' => 'MicroCRMChannelInlineFormController',
    ),
  );
  return $entities;
}

/**
 *
 */
function micro_crm_channel_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('micro_crm_contact')->access($op, $entity, $account);
}

/**
 *
 */
function micro_crm_channel_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = micro_crm_channel_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 *
 */
function micro_crm_channel_load_multiple($ids) {
  return entity_load('micro_crm_channel', $ids);
}

/**
 * @param $ids
 */
function micro_crm_channel_delete($id) {
  micro_crm_channel_delete(array($id));
}

/**
 * @param $ids
 */
function micro_crm_channel_delete_multiple($ids) {
  entity_delete_multiple('micro_crm_channel', $ids);
}

/**
 *
 */
function micro_crm_channel_view($channel) {

}

/**
 *
 */
function micro_crm_channel_uri($channel) {

}

/**
 *
 */
function micro_crm_channel_label($channel) {

}

/**
 * @param $ids
 * @return bool|mixed
 */
function micro_crm_channel_load_contact($ids) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'micro_crm_contact')
    ->fieldCondition('field_micro_crm_channel', 'target_id', $ids, 'IN');
  $result = $query->execute();

  if (!empty($result['micro_crm_contact']) && count($result['micro_crm_contact']) > 1) {
    return entity_load('micro_crm_contact', array_keys($result['micro_crm_contact']));
  }
  elseif (!empty($result['micro_crm_contact']) && count($result['micro_crm_contact']) == 1) {
    $entity = entity_load('micro_crm_contact', array_keys($result['micro_crm_contact']));
    return reset($entity);
  }
  return entity_create('micro_crm_contact', array());
}

/**
 * @param $channel
 * @return mixed|null
 */
function micro_crm_channel_values_load($channel) {
  if (!isset($channel->type)) {
    return NULL;
  }
  $entity = entity_get_info('micro_crm_channel');
  if (!isset($entity['bundles'][$channel->type]['module'])) {
    return NULL;
  }
  $module = $entity['bundles'][$channel->type]['module'];
  if (!function_exists($module . '_micro_crm_channel_values_load')) {
    return NULL;
  }
  return module_invoke($module, 'micro_crm_channel_values_load', $channel);
}

/**
 * @param $token
 */
function micro_crm_channel_token_load($token) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'micro_crm_channel')
    ->propertyCondition('token', $token, '=');
  $result = $query->execute();
  if (!isset($result['micro_crm_channel'])) {
    return NULL;
  }
  $entity = entity_load('micro_crm_channel', array_keys($result['micro_crm_channel']));

  if (count($entity) <> 1) {
    return NULL;
  }
  return reset($entity);
}

/**
 * Send message callback, module independent.
 *
 * @param $bundle
 * @param $channel
 */
function micro_crm_channel_send($channel) {
  if (!isset($channel->type)) {
    return FALSE;
  }
  $entity = entity_get_info('micro_crm_channel');
  if (!isset($entity['bundles'][$channel->type]['module'])) {
    return FALSE;
  }
  $module = $entity['bundles'][$channel->type]['module'];
  if (!function_exists($module . '_micro_crm_channel_send')) {
    return FALSE;
  }
  return module_invoke($module, 'micro_crm_channel_send', $channel);
}
