<?php

/**
 * @file
 *
 */

/**
 * Implements hook_entity_info().
 */
function crm_message_entity_info() {
  $entities['crm_message'] = array(
    'label' => t('Message', array(), array('context' => 'crm')),
    'controller class' => 'CRMMessageController',
    'base table' => 'crm_message',
    'revision table' => 'crm_message_revision',
    'load hook' => 'crm_message_load',
    'uri callback' => 'crm_message_uri',
    'label callback' => 'crm_message_label',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'message_id',
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
    'access callback' => 'crm_message_access',
  );
  return $entities;
}

/**
 *
 */
function crm_message_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('crm_message')->access($op, $entity, $account);
}

/**
 *
 */
function crm_message_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = crm_message_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 *
 */
function crm_message_load_multiple($ids) {
  return entity_load('crm_message', $ids);
}

/**
 * @param $ids
 */
function crm_message_delete($id) {
  crm_message_delete(array($id));
}

/**
 * @param $ids
 */
function crm_message_delete_multiple($ids) {
  entity_delete_multiple('crm_message', $ids);
}

/**
 *
 */
function crm_message_view($message, $view_mode = 'customer') {
  if (!is_array($message)) {
    $message = array($message->message_id => $message);
  }
  return entity_get_controller('crm_message')->view($message, $view_mode);
}

/**
 * @todo: Move uri to UI module.
 */
function crm_message_uri($message) {
  return array(
    'path' => 'admin/people/crm/message/' . $message->message_id . '/view',
  );
}

/**
 *
 */
function crm_message_label($message) {

}
