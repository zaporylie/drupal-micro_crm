<?php

/**
 * @file
 * Hooks provided by the Micro CRM Contact module.
 */

/**
 * @return array
 *   An array of contact status arrays keyed by name.
 */
function hook_micro_crm_contact_status_info() {
  $status = array();

  $status[MICRO_CRM_CONTACT_STATUS_REGISTERED] = array(
    'title' => t('Registered'),
    'description' => t('Registered, unprocessed contact'),
  );

  return $status;
}

/**
 * Allows modules to alter contact status definitions.
 *
 * @param $statuses
 *   An array of conact status arrays keyed by name.
 */
function hook_micro_crm_contact_status_info_alter(&$statuses) {
  $statuses[MICRO_CRM_CONTACT_STATUS_REGISTERED]['title'] = t('Unprocessed');
}


/**
 * Allows modules to specify a uri for a contact.
 *
 * @param $contact
 *   The contact object whose uri is being determined.
 *
 * @return
 *  The uri elements of an entity as expected to be returned by entity_uri()
 *  matching the signature of url().
 *
 * @see micro_crm_contact_uri()
 * @see hook_module_implements_alter()
 * @see entity_uri()
 * @see url()
 */
function hook_micro_crm_contact_uri($contact) {
  // No example.
}

/**
 * Allows you to prepare contact data before it is saved.
 *
 * @param $contact
 *   The contact object to be saved.
 *
 * @see rules_invoke_all()
 */
function hook_micro_crm_contact_presave($contact) {
  // No example.
}
