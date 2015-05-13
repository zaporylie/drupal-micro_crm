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
