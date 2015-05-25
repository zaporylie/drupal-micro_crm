<?php

/**
 * @file
 * Admin views and forms.
 */

function micro_crm_newsletter_settings($form, &$form_state) {
  $form['micro_crm_newsletter_custom_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Custom fields'),
    '#description' => t('Which custom field should be available in newsletter form'),
    '#default_value' => variable_get('micro_crm_newsletter_custom_fields', array()),
    '#options' => micro_crm_newsletter_custom_fields(),
  );
  $form['micro_crm_newsletter_channels'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Channels'),
    '#description' => t('Which channels should be available in newsletter form'),
    '#default_value' => _micro_crm_newsletter_get_channels(),
    '#options' => micro_crm_channel_types(),
  );
  return system_settings_form($form);
}

/**
 * @return array
 */
function micro_crm_newsletter_custom_fields() {
  $custom_fields = array();

  foreach (field_info_instances('micro_crm_contact', 'micro_crm_contact') as $key => $instance) {
    if ($key === 'field_micro_crm_channel') {
      continue;
    }
    $custom_fields[$key] = $instance['label'];
  }

  return $custom_fields;
}
