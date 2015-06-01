<?php

/**
 * @file
 */

/**
 * Settings form.
 */
function micro_crm_campaignmonitor_settings($form, &$form_state) {
  $form['#tree'] = TRUE;

  // Create a block for each list.
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
  $form['micro_crm_campaignmonitor_lists'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Lists'),
    '#description' => t('Choose available lists'),
    '#options' => $lists_options,
    '#default_value' => variable_get('micro_crm_campaignmonitor_lists', array()),
    '#ajax' => array(
      'callback' => 'micro_crm_campaignmonitor_list_callback',
      'method' => 'replace',
      'wrapper' => 'micro-crm-campiagnmonitor-settings-form',
    ),
  );
  if (isset($form_state['values']['micro_crm_campaignmonitor_lists'])) {
    $available_lists = array_values($form_state['values']['micro_crm_campaignmonitor_lists']);
  }
  else {
    $available_lists = variable_get('micro_crm_campaignmonitor_lists', array());
  }
  $form['micro_crm_campaignmonitor']['#prefix'] = '<div id="micro-crm-campiagnmonitor-settings-form">';
  $form['micro_crm_campaignmonitor']['#suffix'] = '</div>';
  foreach ($lists as $list_id => $list) {
    if (in_array($list_id, $available_lists, TRUE)) {
      $list = $cm->getExtendedList($list_id);
      $form['micro_crm_campaignmonitor'][$list_id] = array(
        '#type' => 'fieldset',
        '#title' => $lists_options[$list_id],
      );

      $values = variable_get('micro_crm_campaignmonitor_list_' . $list_id, array());
      $form['micro_crm_campaignmonitor'][$list_id]['Name']['field'] = array(
        '#type' => 'select',
        '#title' => 'Name',
        '#description' => 'Text',
        '#default_value' => isset($values['Name']['field']) ? $values['Name']['field'] : NULL,
        '#options' => array(NULL => t('- Choose -')) + _micro_crm_newsletter_get_custom_fields(),
      );
      foreach ($list['CustomFields'] as $custom_field) {
        $form['micro_crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])] = array(
          '#type' => 'container',
          '#attributes' => array(),
        );
        $form['micro_crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['field'] = array(
          '#type' => 'select',
          '#title' => $custom_field['FieldName'],
          '#description' => $custom_field['DataType'],
          '#default_value' => isset($values[urlencode($custom_field['Key'])]['field']) ? $values[urlencode($custom_field['Key'])]['field'] : NULL,
          '#options' => array(NULL => t('- Choose -')) + _micro_crm_newsletter_get_custom_fields(),
        );
        if (in_array($custom_field['DataType'], array('MultiSelectMany', 'MultiSelectOne')) && (isset($form_state['values']['micro_crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['field']) || isset($values[urlencode($custom_field['Key'])]['field']))) {

          if (isset($form_state['values']['micro_crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])])) {
            $field = field_info_field($form_state['values']['micro_crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['field']);
          }
          else {
            $field = field_info_field($values[urlencode($custom_field['Key'])]['field']);
          }

          if (!in_array($field['module'], array('list', 'taxonomy'))) {
            continue;
          }

          $instance = field_info_instance('micro_crm_contact', $field['field_name'], 'micro_crm_contact');
          $type = str_replace('options_', '', $instance['widget']['type']);
          $multiple = $field['cardinality'] > 1 || $field['cardinality'] == FIELD_CARDINALITY_UNLIMITED;
          $required = FALSE;
          $has_value = FALSE;
          $properties = _options_properties($type, $multiple, $required, $has_value);

          $form['micro_crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['map'] = array(
            '#type' => 'fieldset',
            '#title' => t('@field: Values map', array('@field' => $custom_field['FieldName'])),
            '#attributes' => array(
              'id' => 'micro_crm_campaignmonitor_list_' . $list_id . '_field_' . urlencode($custom_field['Key']),
            ),
          );
          foreach ($custom_field['FieldOptions'] as $option) {
            $form['micro_crm_campaignmonitor'][$list_id][urlencode($custom_field['Key'])]['map'][urlencode($option)] = array(
              '#type' => 'select',
              '#title' => $option,
              '#default_value' => isset($values[urlencode($custom_field['Key'])]['map'][urlencode($option)]) ? $values[urlencode($custom_field['Key'])]['map'][urlencode($option)] : NULL,
              '#options' => array(NULL => t('- Choose -')) + _options_get_options($field, $instance, $properties, 'micro_crm_contact', NULL),
            );
          }
        }
      }
    }
  }

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
function micro_crm_campaignmonitor_settings_submit($form, &$form_state) {
  variable_set('micro_crm_campaignmonitor_lists', $form_state['values']['micro_crm_campaignmonitor_lists']);
  foreach ($form_state['values']['micro_crm_campaignmonitor_lists'] as $list) {
    if ($list !== 0) {
      variable_set('micro_crm_campaignmonitor_list_' . $list, $form_state['values']['micro_crm_campaignmonitor'][$list]);
    }
  }
}

/**
 * Regenerates form if something will change.
 */
function micro_crm_campaignmonitor_list_callback($form, $form_state) {
  return $form['micro_crm_campaignmonitor'];
}
