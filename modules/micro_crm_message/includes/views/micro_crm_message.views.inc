<?php

/**
 * Export CRM Message to Views.
 */

/**
 * Implements hook_views_data()
 */
function micro_crm_message_views_data() {
  $data = array();

  $data['micro_crm_message']['table']['group']  = t('Message');

  $data['micro_crm_message']['table']['base'] = array(
    'field' => 'message_id',
    'title' => t('Message', array(), array('context' => 'micro crm')),
    'help' => t('Message stored in system.'),
    'access query tag' => 'micro_crm_message_access',
  );
  $data['micro_crm_message']['table']['entity type'] = 'micro_crm_message';

  $data['micro_crm_message']['table']['default_relationship'] = array(
    'micro_crm_message_revision' => array(
      'table' => 'micro_crm_message_revision',
      'field' => 'revision_id',
    ),
  );
  $data['micro_crm_message']['message_id'] = array(
    'title' => t('Message ID', array(), array('context' => 'micro crm')),
    'help' => t('The unique internal identifier of the message.'),
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
      'name field' => 'message_id',
      'numeric' => TRUE,
      'validate type' => 'message_id',
    ),
  );

  $data['micro_crm_message']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the message was created.'),
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

  $data['micro_crm_message']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the message was last updated.'),
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

  // Expose links to operate on the product.
  $data['micro_crm_message']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the message.'),
      'handler' => 'micro_crm_message_handler_field_message_operations',
    ),
  );

  // Expose links to operate on the product.
  $data['micro_crm_message']['bundle'] = array(
    'field' => array(
      'title' => t('Bundle'),
      'help' => t('Display all the available operations links for the message.'),
      'handler' => 'micro_crm_message_handler_field_message_operations',
    ),
  );

  $data['micro_crm_message']['empty_text'] = array(
    'title' => t('Empty text'),
    'help' => t('Displays an appropriate empty text message for message lists.'),
    'area' => array(
      'handler' => 'micro_crm_message_handler_area_empty_text',
    ),
  );

  /**
   * Integrate the message revision table.
   */
  $data['micro_crm_message_revision']['table']['entity type'] = 'micro_crm_message';
  $data['micro_crm_message_revision']['table']['group']  = t('Message revision');

  // Advertise this table as a possible base table
  $data['micro_crm_message_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Message revision'),
    'help' => t('Message revision is a history of changes to an message.'),
    'defaults' => array(
      'field' => 'message_id',
    ),
  );

  $data['micro_crm_message_revision_revision']['table']['join'] = array(
    'micro_crm_message' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  $data['micro_crm_message_revision_revision']['table']['default_relationship'] = array(
    'micro_crm_message_revision' => array(
      'table' => 'micro_crm_message_revision',
      'field' => 'revision_id',
    ),
  );

  return $data;
}
