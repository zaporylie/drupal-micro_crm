<?php

/**
 * Views for the default message UI.
 */

/**
 * Implements hook_views_default_views().
 */
function crm_message_ui_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'crm_message';
  $view->description = 'Message admin view';
  $view->tag = 'crm';
  $view->base_table = 'crm_message';
  $view->human_name = 'CRM Message';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Message';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'crm message overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table_megarows';
  $handler->display->display_options['style_options']['columns'] = array(
    'message_id' => 'message_id',
    'label' => 'label',
    'type' => 'type',
    'status' => 'status',
    'created' => 'created',
    'changed' => 'changed',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'message_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'label' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['scroll_padding'] = '120';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'This view is empty.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['crm_channel_single_target_id']['id'] = 'crm_channel_single_target_id';
  $handler->display->display_options['relationships']['crm_channel_single_target_id']['table'] = 'field_data_crm_channel_single';
  $handler->display->display_options['relationships']['crm_channel_single_target_id']['field'] = 'crm_channel_single_target_id';
  /* Field: Message: Message ID */
  $handler->display->display_options['fields']['message_id']['id'] = 'message_id';
  $handler->display->display_options['fields']['message_id']['table'] = 'crm_message';
  $handler->display->display_options['fields']['message_id']['field'] = 'message_id';
  /* Field: Channel: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'crm_channel';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['relationship'] = 'crm_channel_single_target_id';
  $handler->display->display_options['fields']['label']['label'] = 'To';
  /* Field: Message: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'crm_message';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['link_to_message'] = 0;
  $handler->display->display_options['fields']['type']['use_raw_value'] = 0;
  /* Field: Message: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'crm_message';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['link_to_message'] = 0;
  $handler->display->display_options['fields']['status']['use_raw_value'] = 0;
  /* Field: Message: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'crm_message';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Message: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'crm_message';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'long';
  /* Field: Message: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'crm_message';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['add_destination'] = 0;
  /* Filter criterion: Message: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'crm_message';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Message: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'crm_message';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Messages */
  $handler = $view->new_display('page', 'Messages', 'page');
  $handler->display->display_options['path'] = 'admin/crm/message/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Message';
  $handler->display->display_options['menu']['description'] = 'List of all messages registered in CRM system.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Message';
  $handler->display->display_options['tab_options']['description'] = 'List of all messages registered in CRM system.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Display: Channel messages */
  $handler = $view->new_display('page', 'Channel messages', 'page_1');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Channel: Channel ID */
  $handler->display->display_options['arguments']['channel_id']['id'] = 'channel_id';
  $handler->display->display_options['arguments']['channel_id']['table'] = 'crm_channel';
  $handler->display->display_options['arguments']['channel_id']['field'] = 'channel_id';
  $handler->display->display_options['arguments']['channel_id']['relationship'] = 'crm_channel_single_target_id';
  $handler->display->display_options['arguments']['channel_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['channel_id']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['channel_id']['default_argument_options']['index'] = '3';
  $handler->display->display_options['arguments']['channel_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['channel_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['channel_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'admin/crm/channel/%/message';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Messages';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 1;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['crm_message'] = array(
    t('Master'),
    t('Message'),
    t('more'),
    t('Filter'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('This view is empty.'),
    t('Channel entity referenced from crm_channel_single'),
    t('Message ID'),
    t('To'),
    t('Type'),
    t('Status'),
    t('Created date'),
    t('Updated date'),
    t('Operations links'),
    t('Messages'),
    t('Channel messages'),
    t('All'),
  );

  $views[$view->name] = $view;

  return $views;
}
