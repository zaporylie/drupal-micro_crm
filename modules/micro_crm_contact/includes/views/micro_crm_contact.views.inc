<?php

/**
 * Export crm contact to Views.
 */

/**
 * Implements hook_views_data()
 */
function micro_crm_contact_views_data() {
  $data = array();

  $data['micro_crm_contact']['table']['group']  = t('Micro CRM');

  $data['micro_crm_contact']['table']['base'] = array(
    'field' => 'contact_id',
    'title' => t('Contact', array(), array('context' => 'micro crm')),
    'help' => t('Contact stored in system.'),
    'access query tag' => 'micro_crm_contact_access',
  );
  $data['micro_crm_contact']['table']['entity type'] = 'micro_crm_contact';

  $data['micro_crm_contact']['table']['default_relationship'] = array(
    'micro_crm_contact_revision' => array(
      'table' => 'micro_crm_contact_revision',
      'field' => 'revision_id',
    ),
  );
  $data['micro_crm_contact']['contact_id'] = array(
    'title' => t('Contact ID', array(), array('context' => 'micro crm')),
    'help' => t('The unique internal identifier of the contact.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'contact_id',
      'numeric' => TRUE,
      'validate type' => 'order_id',
    ),
  );

  /**
   * Integrate the contact revision table.
   */
  $data['micro_crm_contact_revision']['table']['entity type'] = 'micro_crm_contact';
  $data['micro_crm_contact_revision']['table']['group']  = t('Contact revision');

  // Advertise this table as a possible base table
  $data['micro_crm_contact_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Contact revision'),
    'help' => t('Contact revision is a history of changes to an contact.'),
    'defaults' => array(
      'field' => 'contact_id',
    ),
  );

  $data['micro_crm_contact_revision_revision']['table']['join'] = array(
    'micro_crm_contact' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    )
  );

  $data['micro_crm_contact_revision_revision']['table']['default_relationship'] = array(
    'micro_crm_contact_revision' => array(
      'table' => 'micro_crm_contact_revision',
      'field' => 'revision_id',
    ),
  );

  return $data;
}