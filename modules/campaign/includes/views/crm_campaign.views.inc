<?php

/**
 * Export CRM Campaign to Views.
 */

/**
 * Implements hook_views_data()
 */
function crm_campaign_views_data() {
  $data = array();

  $data['crm_campaign']['table']['group']  = t('Campaign');

  $data['crm_campaign']['table']['base'] = array(
    'field' => 'campaign_id',
    'title' => t('Campaign', array(), array('context' => 'crm')),
    'help' => t('Campaign stored in system.'),
    'access query tag' => 'crm_campaign_access',
  );
  $data['crm_campaign']['table']['entity type'] = 'crm_campaign';

  $data['crm_campaign']['table']['default_relationship'] = array(
    'crm_campaign_revision' => array(
      'table' => 'crm_campaign_revision',
      'field' => 'revision_id',
    ),
  );
  $data['crm_campaign']['campaign_id'] = array(
    'title' => t('Campaign ID', array(), array('context' => 'crm')),
    'help' => t('The unique internal identifier of the campaign.'),
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
      'name field' => 'campaign_id',
      'numeric' => TRUE,
      'validate type' => 'campaign_id',
    ),
  );

  // Expose the message type.
  $data['crm_campaign']['title'] = array(
    'title' => t('Title'),
    'help' => t('Title.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['crm_campaign']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the campaign was created.'),
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

  $data['crm_campaign']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the campaign was last updated.'),
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
  $data['crm_campaign']['status'] = array(
    'title' => t('Status'),
    'help' => t('The human-readable campaign status.'),
    'field' => array(
      'handler' => 'crm_campaign_handler_field_campaign_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'crm_campaign_handler_filter_campaign_status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['crm_campaign']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the campaign.'),
      'handler' => 'crm_campaign_handler_field_campaign_operations',
    ),
  );

  /**
   * Integrate the campaign revision table.
   */
  $data['crm_campaign_revision']['table']['entity type'] = 'crm_campaign';
  $data['crm_campaign_revision']['table']['group']  = t('Campaign revision');

  // Advertise this table as a possible base table
  $data['crm_campaign_revision']['table']['base'] = array(
    'field' => 'revision_id',
    'title' => t('Campaign revision'),
    'help' => t('Campaign revision is a history of changes to an campaign.'),
    'defaults' => array(
      'field' => 'campaign_id',
    ),
  );

  $data['crm_campaign_revision_revision']['table']['join'] = array(
    'crm_campaign' => array(
      'left_field' => 'revision_id',
      'field' => 'revision_id',
    ),
  );

  $data['crm_campaign_revision_revision']['table']['default_relationship'] = array(
    'crm_campaign_revision' => array(
      'table' => 'crm_campaign_revision',
      'field' => 'revision_id',
    ),
  );

  return $data;
}
