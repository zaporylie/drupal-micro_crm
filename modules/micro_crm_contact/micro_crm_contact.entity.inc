<?php

/**
 * @file
 * Declaration of contact entity and entity callbacks.
 */

/**
 * Implements hook_entity_info().
 */
function micro_crm_contact_entity_info() {
  $entities['micro_crm_contact'] = array(
    'label' => t('Contact', array(), array('context' => 'micro crm')),
    'controller class' => 'MicroCRMContactController',
    'base table' => 'micro_crm_contact',
    'revision table' => 'micro_crm_contact_revision',
    'load hook' => 'micro_crm_contact_load',
    'label callback' => 'micro_crm_contact_label',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'contact_id',
      'revision' => 'revision_id',
    ),
    'bundles' => array(
      'micro_crm_contact' => array(
        'label' => t('Contact', array(), array('context' => 'micro crm contact')),
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
    'access callback' => 'micro_crm_contact_access',
  );
  return $entities;
}

/**
 * Contact ccess callback.
 *
 * @param $op
 * @param $entity
 * @param null $account
 */
function micro_crm_contact_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('micro_crm_contact')->access($op, $entity, $account);
}

/**
 * @param $id
 * @return bool|mixed
 */
function micro_crm_contact_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = micro_crm_contact_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 * @param $id
 */
function micro_crm_contact_delete($id) {
  micro_crm_contact_delete_multiple(array($id));
}

/**
 * @param $ids
 */
function micro_crm_contact_delete_multiple($ids) {
  entity_delete_multiple('micro_crm_contact', $ids);
}

/**
 * @param $id
 * @return bool|mixed
 */
function micro_crm_contact_view($contact, $view_mode = 'administrator') {
  if (!is_array($contact)) {
    $contact = array($contact->contact_id => $contact);
  }
  return entity_get_controller('micro_crm_contact')->view($contact, $view_mode);
}

/**
 * @param $ids
 * @return mixed
 */
function micro_crm_contact_load_multiple($ids) {
  return entity_load('micro_crm_contact', $ids);
}

/**
 * @param $contact
 * @return string
 */
function micro_crm_contact_label($contact) {
  $fields_info = field_info_instances('micro_crm_contact', 'micro_crm_contact');
  foreach ($fields_info as $field_name => $value) {

  }
  return '';
}

/**
 * @param array $contacts
 *   Array of contact entities to merge.
 */
function micro_crm_contact_merge($contacts) {
  if (!is_array($contacts)) {
    return FALSE;
  }

  // Oldest contact should be taken as a base.
  ksort($contacts);
  $entity = array_shift($contacts);
  $wrapper = entity_metadata_wrapper('micro_crm_contact', $entity);

  // Get all current channels ids to flat array.
  $tmp = array();
  foreach ($wrapper->field_micro_crm_channel->getIterator() as $channel) {
    $tmp[] = $channel->getIdentifier();
  }

  // Iterate all contacts...
  foreach ($contacts as $contact_id => $contact) {
    $contact_wrapper = entity_metadata_wrapper('micro_crm_contact', $contact);

    // ...and channels and add it to contact we are merging to.
    foreach ($contact_wrapper->field_micro_crm_channel->getIterator() as $channel_wrapper) {
      if (!in_array($channel_wrapper->getIdentifier(), $tmp)) {
        $wrapper->field_micro_crm_channel[] = $channel_wrapper->getIdentifier();
      }
    }
    // Reset channel field in deprecated contact.
    $contact_wrapper->field_micro_crm_channel->set(NULL);

    // Add deprecated contact to parent one.
    $wrapper->field_micro_crm_deprecated[] = $contact_wrapper->getIdentifier();

    // Save changed contact.
    $contact_wrapper->log->set('Contact has been deprecated after merging to ' . $wrapper->getIdentifier());
    $contact_wrapper->save();
  }

  // Save and return.
  $wrapper->log->set('Contacts has been merged into this one.');
  $wrapper->save();
  return $entity;
}
