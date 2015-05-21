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

/**
 * Load channel by form values.
 *
 * @param $bundle
 * @param $form
 * @param $form_state
 * @return mixed|null
 */
function micro_crm_channel_load_by_form_values($bundle, $form, $form_state) {
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'micro_crm_channel')
    ->entityCondition('bundle', $bundle)
    ->addTag('micro_crm_channel_load_' . $bundle . '_by_values')
    ->range(0, 0)
    ->addMetaData('form', $form)
    ->addMetaData('form_state', $form_state);

  $result = $query->execute();
  if (!empty($result['micro_crm_channel'])) {
    $entity = entity_load('micro_crm_channel', array_keys($result['micro_crm_channel']));
    return reset($entity);
  }
  return NULL;
}

/**
 * @param $ids
 * @return null
 */
function micro_crm_channel_load_contact($ids) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'micro_crm_contact')
    ->fieldCondition('field_micro_crm_channel', 'target_id', $ids, 'IN');
  $result = $query->execute();

  if (!empty($result['micro_crm_contact'])) {
    return entity_load('micro_crm_contact', array_keys($result['micro_crm_contact']));
  }
  return NULL;
}