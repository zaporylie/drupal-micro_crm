<?php

/**
 * @file
 *
 */

function crm_channel_entity_info() {
  $entities['crm_channel'] = array(
    'label' => t('Channel', array(), array('context' => 'crm')),
    'controller class' => 'CRMChannelController',
    'base table' => 'crm_channel',
    'revision table' => 'crm_channel_revision',
    'load hook' => 'crm_channel_load',
    'uri callback' => 'crm_channel_uri',
    'label callback' => 'crm_channel_label',
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
    'access callback' => 'crm_channel_access',

    // Inline entity controller support.
    'inline entity form' => array(
      'controller' => 'CRMChannelInlineFormController',
    ),
  );
  return $entities;
}

/**
 *
 */
function crm_channel_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('crm_channel')->access($op, $entity, $account);
}

/**
 *
 */
function crm_channel_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = crm_channel_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 *
 */
function crm_channel_load_multiple($ids) {
  return entity_load('crm_channel', $ids);
}

/**
 * @param $ids
 */
function crm_channel_delete($id) {
  crm_channel_delete(array($id));
}

/**
 * @param $ids
 */
function crm_channel_delete_multiple($ids) {
  entity_delete_multiple('crm_channel', $ids);
}

/**
 *
 */
function crm_channel_view($channel, $view_mode = 'administrator') {
  if (!is_array($channel)) {
    $channel = array($channel->channel_id => $channel);
  }
  return entity_get_controller('crm_channel')->view($channel, $view_mode);
}

/**
 *
 */
function crm_channel_uri($channel) {
  return array(
    'path' => 'admin/people/crm/channel/' . $channel->channel_id,
  );
}

/**
 *
 */
function crm_channel_label($channel) {
  $entity = entity_get_info('crm_channel');
  if (!isset($channel->type) || !isset($entity['bundles'][$channel->type]['module'])) {
    return NULL;
  }
  $module = $entity['bundles'][$channel->type]['module'];
  if ($value = module_invoke($module, 'crm_channel_label', $channel)) {
    return $value;
  }
  return NULL;
}

/**
 * @param $ids
 * @return bool|mixed
 */
function crm_channel_load_contact($ids) {
  // If it's not an array - make it an array.
  if (!is_array($ids)) {
    $ids = array($ids);
  }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'crm_contact')
    ->fieldCondition('crm_channel', 'target_id', $ids, 'IN');
  $result = $query->execute();

  if (!empty($result['crm_contact']) && count($result['crm_contact']) > 1) {
    return entity_load('crm_contact', array_keys($result['crm_contact']));
  }
  elseif (!empty($result['crm_contact']) && count($result['crm_contact']) == 1) {
    $entity = entity_load('crm_contact', array_keys($result['crm_contact']));
    return reset($entity);
  }
  return entity_create('crm_contact', array());
}

/**
 * @param $channel
 * @return mixed|null
 */
function crm_channel_values_load($channel) {
  if (!isset($channel->type)) {
    return NULL;
  }
  $entity = entity_get_info('crm_channel');
  if (!isset($entity['bundles'][$channel->type]['module'])) {
    return NULL;
  }
  $module = $entity['bundles'][$channel->type]['module'];
  if (!function_exists($module . '_crm_channel_values_load')) {
    return NULL;
  }
  return module_invoke($module, 'crm_channel_values_load', $channel);
}

/**
 * @param $token
 */
function crm_channel_token_load($token) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'crm_channel')
    ->propertyCondition('token', $token, '=');
  $result = $query->execute();
  if (!isset($result['crm_channel'])) {
    return NULL;
  }
  $entity = entity_load('crm_channel', array_keys($result['crm_channel']));

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
function crm_channel_send($channel) {
  if (!isset($channel->type)) {
    return FALSE;
  }
  $entity = entity_get_info('crm_channel');
  if (!isset($entity['bundles'][$channel->type]['module'])) {
    return FALSE;
  }
  $module = $entity['bundles'][$channel->type]['module'];
  if (!function_exists($module . '_crm_channel_send')) {
    return FALSE;
  }
  return module_invoke($module, 'crm_channel_send', $channel);
}
