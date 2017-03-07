<?php

/**
 * @file
 * Declaration of contact entity and entity callbacks.
 */

/**
 * Implements hook_entity_info().
 */
function crm_contact_entity_info() {
  $entities['crm_contact'] = array(
    'label' => t('Contact', array(), array('context' => 'crm')),
    'controller class' => 'CRMContactController',
    'base table' => 'crm_contact',
    'revision table' => 'crm_contact_revision',
    'load hook' => 'crm_contact_load',
    'label callback' => 'crm_contact_label',
    'fieldable' => TRUE,
    'module' => 'crm_contact',
    'entity keys' => array(
      'id' => 'contact_id',
      'revision' => 'revision_id',
    ),
    'bundles' => array(
      'crm_contact' => array(
        'label' => t('Contact', array(), array('context' => 'crm')),
      ),
    ),
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
    'access callback' => 'crm_contact_access',
  );
  return $entities;
}

/**
 * Contact access callback.
 *
 * @param $op
 * @param $entity
 * @param null $account
 */
function crm_contact_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('crm_contact')->access($op, $entity, $account);
}

/**
 * @param $id
 * @return bool|mixed
 */
function crm_contact_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = crm_contact_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 * @param $id
 */
function crm_contact_delete($id) {
  crm_contact_delete_multiple(array($id));
}

/**
 * @param $ids
 */
function crm_contact_delete_multiple($ids) {
  entity_delete_multiple('crm_contact', $ids);
}

/**
 * @param $id
 * @return bool|mixed
 */
function crm_contact_view($contact, $view_mode = 'administrator') {
  if (!is_array($contact)) {
    $contact = array($contact->contact_id => $contact);
  }
  return entity_get_controller('crm_contact')->view($contact, $view_mode);
}

/**
 * @param $ids
 * @return mixed
 */
function crm_contact_load_multiple($ids) {
  return entity_load('crm_contact', $ids);
}

/**
 * @param $ids
 * @return bool|mixed
 *
 * @todo This function should return NULL if there is nothing to show instead of
 * creating new, empty contact.
 */
function crm_contact_load_by_channels($ids) {
  // If it's not an array - make it an array.
  if (!is_array($ids)) {
    $ids = array($ids);
  }
  if (empty($ids)) {
    return entity_create('crm_contact', array());
  }
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'crm_contact')
    ->fieldCondition('crm_channel', 'target_id', $ids, 'IN');
  $result = $query->execute();

  if (!empty($result['crm_contact']) && count($result['crm_contact']) > 1) {
    return entity_load('crm_contact', array_keys($result['crm_contact']));
  }
  elseif (!empty($result['crm_contact']) && count($result['crm_contact']) == 1) {
    return entity_load_single('crm_contact', key($result['crm_contact']));
  }
  return entity_create('crm_contact', array());
}

/**
 * @param $contact
 * @return string
 */
function crm_contact_label($contact) {
  $fields_info = field_info_instances('crm_contact', 'crm_contact');
  foreach ($fields_info as $field_name => $value) {

  }
  return '';
}

/**
 * @param array $contacts
 *   Array of contact entities to merge.
 */
function crm_contact_merge($contacts) {
  if (!is_array($contacts)) {
    return FALSE;
  }

  // Oldest contact should be taken as a base.
  ksort($contacts);
  $entity = array_shift($contacts);
  $wrapper = entity_metadata_wrapper('crm_contact', $entity);

  // Get all current channels ids to flat array.
  $tmp = array();
  foreach ($wrapper->crm_channel->getIterator() as $channel) {
    $tmp[] = $channel->getIdentifier();
  }

  // Iterate all contacts...
  foreach ($contacts as $contact_id => $contact) {
    $contact_wrapper = entity_metadata_wrapper('crm_contact', $contact);

    // ...and channels and add it to contact we are merging to.
    foreach ($contact_wrapper->crm_channel->getIterator() as $channel_wrapper) {
      if (!in_array($channel_wrapper->getIdentifier(), $tmp)) {
        $wrapper->crm_channel[] = $channel_wrapper->getIdentifier();
      }
    }
    // Reset channel field in deprecated contact.
    $contact_wrapper->crm_channel->set(NULL);

    // Add deprecated contact to parent one.
    $wrapper->crm_deprecated[] = $contact_wrapper->getIdentifier();

    // Save changed contact.
    $contact_wrapper->log->set('Contact has been deprecated after merging to ' . $wrapper->getIdentifier());
    $contact_wrapper->save();
  }

  // Save and return.
  $wrapper->log->set('Contacts has been merged into this one.');
  $wrapper->save();
  return $entity;
}

/**
 * @param $contact
 * @return array|null
 */
function crm_contact_uri($contact) {
  return array(
    'path' => 'admin/crm/contact/' . $contact->contact_id,
  );

}
