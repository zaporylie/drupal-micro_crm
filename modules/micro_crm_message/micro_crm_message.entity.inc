<?php

/**
 * @file
 *
 */

/**
 * Implements hook_entity_info().
 */
function micro_crm_message_entity_info() {
  $entities['micro_crm_message'] = array(
    'label' => t('Message', array(), array('context' => 'micro crm')),
    'controller class' => 'MicroCRMMEssageController',
    'base table' => 'micro_crm_message',
    'revision table' => 'micro_crm_message_revision',
    'load hook' => 'micro_crm_message_load',
    'uri callback' => 'micro_crm_message_uri',
    'label callback' => 'micro_crm_message_label',
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
    'access callback' => 'micro_crm_message_access',
  );
  return $entities;
}

/**
 *
 */
function micro_crm_message_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('micro_crm_message')->access($op, $entity, $account);
}

/**
 *
 */
function micro_crm_message_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = micro_crm_message_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 *
 */
function micro_crm_message_load_multiple($ids) {
  return entity_load('micro_crm_message', $ids);
}

/**
 * @param $ids
 */
function micro_crm_message_delete($id) {
  micro_crm_message_delete(array($id));
}

/**
 * @param $ids
 */
function micro_crm_message_delete_multiple($ids) {
  entity_delete_multiple('micro_crm_message', $ids);
}

/**
 *
 */
function micro_crm_message_view($message, $view_mode = 'customer') {
  if (!is_array($message)) {
    $message = array($message->message_id => $message);
  }
  return entity_get_controller('micro_crm_message')->view($message, $view_mode);
}

/**
 * @todo: Move uri to UI module.
 */
function micro_crm_message_uri($message) {
  return array(
    'path' => 'admin/people/micro_crm/message/' . $message->message_id . '/view',
  );
}

/**
 *
 */
function micro_crm_message_label($message) {

}
