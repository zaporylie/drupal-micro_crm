<?php

/**
 * @file
 */

/**
 * Settings form.
 */
function crm_campaignmonitor_settings($form, &$form_state) {
  $form['#tree'] = TRUE;

  // Create a block for each list.
  $cm = CampaignMonitor::getConnector();
  $lists_options = _crm_campaignmonitor_options_get_lists();
  $form['crm_campaignmonitor_lists'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Lists'),
    '#description' => t('Choose available lists'),
    '#options' => $lists_options,
    '#default_value' => variable_get('crm_campaignmonitor_lists', array()),
    '#ajax' => array(
      'callback' => 'crm_campaignmonitor_list_callback',
      'method' => 'replace',
      'wrapper' => 'crm-campaignmonitor-settings-form',
    ),
  );
  if (isset($form_state['values']['crm_campaignmonitor_lists'])) {
    $available_lists = array_values($form_state['values']['crm_campaignmonitor_lists']);
  }
  else {
    $available_lists = variable_get('crm_campaignmonitor_lists', array());
  }
  $form['crm_campaignmonitor']['#prefix'] = '<div id="crm-campaignmonitor-settings-form">';
  $form['crm_campaignmonitor']['#suffix'] = '</div>';
  $lists = $cm->getLists();
  foreach ($lists as $list_id => $list) {
    if (in_array($list_id, $available_lists, TRUE)) {
      $list = $cm->getExtendedList($list_id);
      $form['crm_campaignmonitor'][$list_id] = array(
        '#type' => 'fieldset',
        '#title' => $lists_options[$list_id],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $values = variable_get('crm_campaignmonitor_list_' . $list_id, array());
      $form['crm_campaignmonitor'][$list_id]['Name']['field'] = array(
        '#type' => 'select',
        '#title' => 'Name',
        '#description' => 'Text',
        '#default_value' => isset($values['Name']['field']) ? $values['Name']['field'] : NULL,
        '#options' => array(NULL => t('- Choose -')) + _crm_newsletter_get_custom_fields(),
      );
      foreach ($list['CustomFields'] as $custom_field) {
        $form['crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])] = array(
          '#type' => 'container',
          '#attributes' => array(),
        );
        $form['crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['field'] = array(
          '#type' => 'select',
          '#title' => $custom_field['FieldName'],
          '#description' => $custom_field['DataType'],
          '#default_value' => isset($values[urlencode($custom_field['Key'])]['field']) ? $values[urlencode($custom_field['Key'])]['field'] : NULL,
          '#options' => array(NULL => t('- Choose -')) + _crm_newsletter_get_custom_fields(),
        );
        if (in_array($custom_field['DataType'], array('MultiSelectMany', 'MultiSelectOne')) && (isset($form_state['values']['crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['field']) || isset($values[urlencode($custom_field['Key'])]['field']))) {

          if (isset($form_state['values']['crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])])) {
            $field = field_info_field($form_state['values']['crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['field']);
          }
          else {
            $field = field_info_field($values[urlencode($custom_field['Key'])]['field']);
          }

          if (!in_array($field['module'], array('list', 'taxonomy'))) {
            continue;
          }

          $instance = field_info_instance('crm_contact', $field['field_name'], 'crm_contact');
          $type = str_replace('options_', '', $instance['widget']['type']);
          $multiple = $field['cardinality'] > 1 || $field['cardinality'] == FIELD_CARDINALITY_UNLIMITED;
          $required = FALSE;
          $has_value = FALSE;
          $properties = _options_properties($type, $multiple, $required, $has_value);

          $form['crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['map'] = array(
            '#type' => 'fieldset',
            '#title' => t('@field: Values map', array('@field' => $custom_field['FieldName'])),
            '#attributes' => array(
              'id' => 'crm_campaignmonitor_list_' . $list_id . '_field_' . urlencode($custom_field['Key']),
            ),
          );
          foreach ($custom_field['FieldOptions'] as $option) {
            $form['crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['map'][urlencode($option)] = array(
              '#type' => 'select',
              '#title' => $option,
              '#default_value' => isset($values[urlencode($custom_field['Key'])]['map'][urlencode($option)]) ? $values[urlencode($custom_field['Key'])]['map'][urlencode($option)] : NULL,
              '#options' => array(NULL => t('- Choose -')) + _options_get_options($field, $instance, $properties, 'crm_contact', NULL),
            );
          }
        }
      }
    }
  }


  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settigns'),
  );
  $form['settings']['crm_campaignmonitor_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug webhook'),
    '#description' => t('Log all webhook requests'),
    '#default_value' => variable_get('crm_campaignmonitor_debug', FALSE),
  );
  $form['settings']['crm_campaignmonitor_queue_incoming'] = array(
    '#type' => 'checkbox',
    '#title' => t('Queue incoming requests'),
    '#description' => t('Add incoming requests to queue. It makes system more stable but sometimes you can lost data integrity. Recommended value - FALSE'),
    '#default_value' => variable_get('crm_campaignmonitor_queue_incoming', FALSE),
  );
  $form['settings']['crm_campaignmonitor_queue_outgoing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Queue outgoing requests'),
    '#description' => t('Add outgoing requests to queue. It makes system more stable but sometimes you can lost data integrity. Recommended value - TRUE'),
    '#default_value' => variable_get('crm_campaignmonitor_queue_outgoing', TRUE),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Settings form submit callback.
 */
function crm_campaignmonitor_settings_submit($form, &$form_state) {
  // Lists.
  variable_set('crm_campaignmonitor_lists', $form_state['values']['crm_campaignmonitor_lists']);
  // List map.
  foreach ($form_state['values']['crm_campaignmonitor_lists'] as $list) {
    if ($list !== 0) {
      variable_set('crm_campaignmonitor_list_' . $list, $form_state['values']['crm_campaignmonitor'][$list]);
    }
  }
  // Settings.
  variable_set('crm_campaignmonitor_debug', $form_state['values']['settings']['crm_campaignmonitor_debug']);
  variable_set('crm_campaignmonitor_queue_incoming', $form_state['values']['settings']['crm_campaignmonitor_queue_incoming']);
  variable_set('crm_campaignmonitor_queue_outgoing', $form_state['values']['settings']['crm_campaignmonitor_queue_outgoing']);
}

/**
 * Regenerates form if something will change.
 */
function crm_campaignmonitor_list_callback($form, $form_state) {
  return $form['crm_campaignmonitor'];
}

/**
 * @return array
 */
function crm_campaignmonitor_webhooks($form, &$form_state) {
  $account = variable_get('campaignmonitor_account', array());

  if (!($library = libraries_load('campaignmonitor')) || empty($library['loaded']) || !isset($account['api_key'])) {
    $form['markup'] = array(
      '#type' => 'markup',
      '#markup' => t('Unable to load CampaignMonitor library'),
    );
    return $form;
  }
  $header = array(
    'list' => array('data' => t('List')),
    'url' => array('data' => t('URL')),
    'events' => array('data' => t('Events')),
    'status' => array('data' => t('Status')),
  );
  $options = array();

  $lists = _crm_campaignmonitor_get_lists();
  foreach ($lists as $list_id => $list) {
    $wrap = new CS_REST_Lists($list_id, $account['api_key']);
    $results = $wrap->get_webhooks();
    if (!$results->was_successful()) {
      drupal_set_message(t('Unable to load webhook for list @list', array('@list' => $list_id)), 'warning');
      continue;
    }
    foreach ($results->response as $webhook) {
      $options[$webhook->WebhookID] = array();
      $options[$webhook->WebhookID]['list'] = array();
      $options[$webhook->WebhookID]['list']['data'] = $list_id;
      $options[$webhook->WebhookID]['url'] = array();
      $options[$webhook->WebhookID]['url']['data'] = $webhook->Url;
      $options[$webhook->WebhookID]['events'] = array();
      $options[$webhook->WebhookID]['events']['data'] = implode(', ', $webhook->Events);
      $options[$webhook->WebhookID]['status'] = array();
      $options[$webhook->WebhookID]['status']['data'] = $webhook->Status;
    }
  }

  $form['list']['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No webhook found'),
  );
  $form['list']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('crm_campaignmonitor_webhook_delete'),
  );

  $form['new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add new webhook'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['new']['list'] = array(
    '#type' => 'select',
    '#title' => t('List'),
    '#options' => array_intersect_key(_crm_campaignmonitor_options_get_lists(), $lists),
  );
  $form['new']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
  );
  $form['new']['events'] = array(
    '#type' => 'select',
    '#title' => t('Events'),
    '#multiple' => TRUE,
    '#options' => array(
      CS_REST_LIST_WEBHOOK_DEACTIVATE => t('Deactivate'),
      CS_REST_LIST_WEBHOOK_UPDATE => t('Update'),
      CS_REST_LIST_WEBHOOK_SUBSCRIBE => t('Subscribe'),
    ),
  );
  $form['new']['actions'] = array('#type' => 'actions');
  $form['new']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add new'),
    '#submit' => array('crm_campaignmonitor_webhook_add'),
  );

  $form['#tree'] = TRUE;
  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function crm_campaignmonitor_webhook_delete(&$form, &$form_state) {
  $account = variable_get('campaignmonitor_account', array());
  if (!($library = libraries_load('campaignmonitor')) || empty($library['loaded']) || !isset($account['api_key'])) {
    drupal_set_message(t('Unable to load library'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  else {
    foreach (array_filter($form_state['values']['list']['table']) as $webhook_id => $value) {
      $list = $form['list']['table']['#options'][$webhook_id]['list']['data'];
      $wrap = new CS_REST_Lists($list, $account['api_key']);
      $result = $wrap->delete_webhook($webhook_id);
      if (!$result->was_successful()) {
        drupal_set_message(t('Unable to delete webhook'), 'error');
        $form_state['rebuild'] = TRUE;
      }
    }
  }
}

/**
 * New webhook form callback.
 */
function crm_campaignmonitor_webhook_add(&$form, &$form_state) {
  $account = variable_get('campaignmonitor_account', array());
  if (empty($form_state['values']['new']['url'])) {
    drupal_set_message(t('Missing URL'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  elseif (empty($form_state['values']['new']['list'])) {
    drupal_set_message(t('Missing list'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  elseif (empty($form_state['values']['new']['events'])) {
    drupal_set_message(t('Missing event'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  elseif (!($library = libraries_load('campaignmonitor')) || empty($library['loaded']) || !isset($account['api_key'])) {
    drupal_set_message(t('Unable to load library'), 'error');
    $form_state['rebuild'] = TRUE;
  }
  else {
    $wrap = new CS_REST_Lists($form_state['values']['new']['list'], $account['api_key']);
    $result = $wrap->create_webhook(array(
      'Events' => array_values($form_state['values']['new']['events']),
      'Url' => $form_state['values']['new']['url'],
      'PayloadFormat' => CS_REST_WEBHOOK_FORMAT_JSON,
    ));
    if (!$result->was_successful()) {
      drupal_set_message(t('Unable to create new webhook'), 'error');
      $form_state['rebuild'] = TRUE;
    }
  }
}

/**
 * @return array
 */
function _crm_campaignmonitor_options_get_lists() {
  $cm = CampaignMonitor::getConnector();
  $lists = $cm->getLists();
  $lists_options = array();
  if ($lists) {
    foreach ($lists as $list_id => $list) {
      // Get local configuration options and check if the list is enabled.
      if (campaignmonitor_is_list_enabled($list_id)) {
        $lists_options[$list_id] = $list['name'];
      }
    }
  }
  return $lists_options;
}
