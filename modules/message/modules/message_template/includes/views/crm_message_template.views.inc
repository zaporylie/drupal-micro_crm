<?php

/**
 * Export CRM Message to Views.
 */

/**
 * Implements hook_views_data()
 */
function crm_message_template_views_data() {
  $data = array();

  $data['crm_message_template']['table']['group']  = t('Message template');

  $data['crm_message_template']['table']['base'] = array(
    'field' => 'message_id',
    'title' => t('Message template', array(), array('context' => 'crm')),
    'help' => t('Message template stored in system.'),
    'access query tag' => 'crm_message_template_access',
  );
  $data['crm_message_template']['table']['entity type'] = 'crm_message_template';

  $data['crm_message_template']['table']['default_relationship'] = array(
    'crm_message_template_revision' => array(
      'table' => 'crm_message_template_revision',
      'field' => 'revision_id',
    ),
  );
  $data['crm_message_template']['template_id'] = array(
    'title' => t('Message template ID', array(), array('context' => 'crm')),
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

  $data['crm_message_template']['created'] = array(
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

  $data['crm_message_template']['changed'] = array(
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
  $data['crm_message_template']['operations'] = array(
    'title' => t('Operations links'),
    'help' => t('Display all the available operations links for the message.'),
    'field' => array(
      'handler' => 'crm_message_template_handler_field_message_template_operations',
    ),
  );

  // Expose the message type.
  $data['crm_message_template']['name'] = array(
    'title' => t('Name'),
    'help' => t('Message template unique machine name.'),
    'field' => array(
      'handler' => 'crm_message_template_handler_field_message_template_name',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the message type.
  $data['crm_message_template']['type'] = array(
    'title' => t('Type'),
    'help' => t('The human-readable name of the type of the message.'),
    'field' => array(
      'handler' => 'crm_message_template_handler_field_message_template_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'crm_message_template_handler_filter_message_template_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  /**
   * Integrate the message revision table.
   */
  $data['crm_message_template_revision']['table']['entity type'] = 'crm_message_template';
  $data['crm_message_template_revision']['table']['group']  = t('Message template revision');

  // Advertise this table as a possible base table
  $data['crm_message_template_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Message revision'),
    'help' => t('Message revision is a history of changes to an message.'),
    'defaults' => array(
      'field' => 'message_id',
    ),
  );

  $data['crm_message_template_revision_revision']['table']['join'] = array(
    'crm_message_template' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  $data['crm_message_template_revision_revision']['table']['default_relationship'] = array(
    'crm_message_template_revision' => array(
      'table' => 'crm_message_template_revision',
      'field' => 'revision_id',
    ),
  );

  return $data;
}
