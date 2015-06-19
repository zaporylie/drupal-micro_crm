<?php

/**
 * Export CRM Contact to Views.
 */

/**
 * Implements hook_views_data()
 */
function crm_contact_views_data() {
  $data = array();

  $data['crm_contact']['table']['group']  = t('Contact');

  $data['crm_contact']['table']['base'] = array(
    'field' => 'contact_id',
    'title' => t('Contact', array(), array('context' => 'crm')),
    'help' => t('Contact stored in system.'),
    'access query tag' => 'crm_contact_access',
  );
  $data['crm_contact']['table']['entity type'] = 'crm_contact';

  $data['crm_contact']['table']['default_relationship'] = array(
    'crm_contact_revision' => array(
      'table' => 'crm_contact_revision',
      'field' => 'revision_id',
    ),
  );
  $data['crm_contact']['contact_id'] = array(
    'title' => t('Contact ID', array(), array('context' => 'crm')),
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
      'validate type' => 'contact_id',
    ),
  );

  $data['crm_contact']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the contact was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['crm_contact']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the contact was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['crm_contact']['status'] = array(
    'title' => t('Status'),
    'help' => t('The human-readable contact status.'),
    'field' => array(
      'handler' => 'crm_contact_handler_field_contact_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'crm_contact_handler_filter_contact_status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose links to operate on the product.
  $data['crm_contact']['view_contact'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the administrator view of the contact.'),
      'handler' => 'crm_contact_handler_field_contact_link',
    ),
  );
  $data['crm_contact']['edit_contact'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the contact.'),
      'handler' => 'crm_contact_handler_field_contact_link_edit',
    ),
  );
  $data['crm_contact']['delete_contact'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the contact.'),
      'handler' => 'crm_contact_handler_field_contact_link_delete',
    ),
  );

  $data['crm_contact']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the contact.'),
      'handler' => 'crm_contact_handler_field_contact_operations',
    ),
  );

  $data['crm_contact']['empty_text'] = array(
    'title' => t('Empty text'),
    'help' => t('Displays an appropriate empty text message for contact lists.'),
    'area' => array(
      'handler' => 'crm_contact_handler_area_empty_text',
    ),
  );

  /**
   * Integrate the contact revision table.
   */
  $data['crm_contact_revision']['table']['entity type'] = 'crm_contact';
  $data['crm_contact_revision']['table']['group']  = t('Contact revision');

  // Advertise this table as a possible base table
  $data['crm_contact_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Contact revision'),
    'help' => t('Contact revision is a history of changes to an contact.'),
    'defaults' => array(
      'field' => 'contact_id',
    ),
  );

  $data['crm_contact_revision_revision']['table']['join'] = array(
    'crm_contact' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  $data['crm_contact_revision_revision']['table']['default_relationship'] = array(
    'crm_contact_revision' => array(
      'table' => 'crm_contact_revision',
      'field' => 'revision_id',
    ),
  );

  return $data;
}
