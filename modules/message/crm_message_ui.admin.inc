<?php

/**
 * @file
 */

/**
 * Settings form.
 */
function crm_message_ui_settings_form($form, $form_state) {

  $form['crm_message_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Threshold'),
    '#description' => t('Maximum number of message send attempts'),
    '#required' => TRUE,
    '#default_value' => variable_get('crm_message_threshold', 5),
    '#options' => _crm_message_ui_threshold_option_list(),
  );

  return system_settings_form($form);
}
