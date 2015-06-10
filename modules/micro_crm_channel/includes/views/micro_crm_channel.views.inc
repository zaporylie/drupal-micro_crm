<?php

/**
 * Export crm channel to Views.
 */

/**
 * Implements hook_views_data()
 */
function micro_crm_channel_views_data() {
  $data = array();

  $data['micro_crm_channel']['table']['group']  = t('Channel');

  $data['micro_crm_channel']['table']['base'] = array(
    'field' => 'channel_id',
    'title' => t('Channel', array(), array('context' => 'micro crm')),
    'help' => t('Channel stored in system.'),
    'access query tag' => 'micro_crm_channel_access',
  );
  $data['micro_crm_channel']['table']['entity type'] = 'micro_crm_channel';

  $data['micro_crm_channel']['table']['default_relationship'] = array(
    'micro_crm_channel_revision' => array(
      'table' => 'micro_crm_channel_revision',
      'field' => 'revision_id',
    ),
  );
  $data['micro_crm_channel']['channel_id'] = array(
    'title' => t('Channel ID', array(), array('context' => 'micro crm')),
    'help' => t('The unique internal identifier of the channel.'),
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
      'name field' => 'channel_id',
      'numeric' => TRUE,
      'validate type' => 'channel_id',
    ),
  );

  $data['micro_crm_channel']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the channel was created.'),
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

  $data['micro_crm_channel']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the channel was last updated.'),
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

  $data['micro_crm_channel']['label'] = array(
    'title' => t('Label'),
    'help' => t('Provides a rendered channel label.'),
    'field' => array(
      'handler' => 'micro_crm_channel_handler_field_channel_label',
    ),
  );

  // Expose links to operate on the product.
  $data['micro_crm_channel']['operations'] = array(
    'title' => t('Operations links'),
    'help' => t('Display all the available operations links for the contact.'),
    'field' => array(
      'handler' => 'micro_crm_channel_handler_field_channel_operations',
    ),
  );

  /**
   * Integrate the channel revision table.
   */
  $data['micro_crm_channel_revision']['table']['entity type'] = 'micro_crm_channel';
  $data['micro_crm_channel_revision']['table']['group']  = t('Channel revision');

  // Advertise this table as a possible base table
  $data['micro_crm_channel_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Channel revision'),
    'help' => t('Channel revision is a history of changes to an channel.'),
    'defaults' => array(
      'field' => 'channel_id',
    ),
  );

  $data['micro_crm_channel_revision_revision']['table']['join'] = array(
    'micro_crm_channel' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  $data['micro_crm_channel_revision_revision']['table']['default_relationship'] = array(
    'micro_crm_channel_revision' => array(
      'table' => 'micro_crm_channel_revision',
      'field' => 'revision_id',
    ),
  );

  return $data;
}
