<?php

/**
 * Export CRM Message to Views.
 */

/**
 * Implements hook_views_data()
 */
function crm_message_views_data() {
  $data = array();

  $data['crm_message']['table']['group']  = t('Message');

  $data['crm_message']['table']['base'] = array(
    'field' => 'message_id',
    'title' => t('Message', array(), array('context' => 'crm')),
    'help' => t('Message stored in system.'),
    'access query tag' => 'crm_message_access',
  );
  $data['crm_message']['table']['entity type'] = 'crm_message';

  $data['crm_message']['table']['default_relationship'] = array(
    'crm_message_revision' => array(
      'table' => 'crm_message_revision',
      'field' => 'revision_id',
    ),
  );
  $data['crm_message']['message_id'] = array(
    'title' => t('Message ID', array(), array('context' => 'crm')),
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

  $data['crm_message']['created'] = array(
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

  $data['crm_message']['changed'] = array(
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
  $data['crm_message']['operations'] = array(
    'title' => t('Operations links'),
    'help' => t('Display all the available operations links for the message.'),
    'field' => array(
      'handler' => 'crm_message_handler_field_message_operations',
    ),
  );

  // Expose the message type.
  $data['crm_message']['type'] = array(
    'title' => t('Type'),
    'help' => t('The human-readable name of the type of the message.'),
    'field' => array(
      'handler' => 'crm_message_handler_field_message_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'crm_message_handler_filter_message_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_message']['status'] = array(
    'title' => t('Status'),
    'help' => t('The human-readable name of the status of the message.'),
    'field' => array(
      'handler' => 'crm_message_handler_field_message_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'crm_message_handler_filter_message_status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['crm_message']['empty_text'] = array(
    'title' => t('Empty text'),
    'help' => t('Displays an appropriate empty text message for message lists.'),
    'area' => array(
      'handler' => 'crm_message_handler_area_empty_text',
    ),
  );

  /**
   * Integrate the message revision table.
   */
  $data['crm_message_revision']['table']['entity type'] = 'crm_message';
  $data['crm_message_revision']['table']['group']  = t('Message revision');

  // Advertise this table as a possible base table
  $data['crm_message_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Message revision'),
    'help' => t('Message revision is a history of changes to an message.'),
    'defaults' => array(
      'field' => 'message_id',
    ),
  );

  $data['crm_message_revision_revision']['table']['join'] = array(
    'crm_message' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  $data['crm_message_revision_revision']['table']['default_relationship'] = array(
    'crm_message_revision' => array(
      'table' => 'crm_message_revision',
      'field' => 'revision_id',
    ),
  );

  return $data;
}
