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
    'path' => 'admin/crm/channel/' . $channel->channel_id,
  );
}

/**
 *
 */
function crm_channel_label($channel) {
  $entity = entity_get_info('crm_channel');
  if (!isset($channel->type) || !isset($entity['bundles'][$channel->type]['module'])) {
    return t('Undefined');
  }
  $module = $entity['bundles'][$channel->type]['module'];
  if ($value = module_invoke($module, 'crm_channel_label', $channel)) {
    return $value;
  }
  return NULL;
}

/**
 * Load channel for channel values (channel type independent).
 *
 * @param object $channel
 *   Load channel object for structured channel.
 *
 * @return bool|null|object
 *   FALSE if object cannot be loaded, NULL if object does not exist, object if
 *   matching object was founded.
 */
function crm_channel_values_load($channel) {
  if (!isset($channel->type)) {
    return FALSE;
  }
  $entity = entity_get_info('crm_channel');
  if (!isset($entity['bundles'][$channel->type]['module'])) {
    return FALSE;
  }
  $module = $entity['bundles'][$channel->type]['module'];
  if (!function_exists($module . '_crm_channel_values_load')) {
    return FALSE;
  }
  try {
    return module_invoke($module, 'crm_channel_values_load', $channel);
  }
  catch (Exception $e) {
    watchdog_exception('crm_channel', $e, NULL, array(), WATCHDOG_NOTICE);
    return FALSE;
  }
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
