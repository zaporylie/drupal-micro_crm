<?php

/**
 * @file
 * Contact configuration.
 */

function crm_contact_ui_settings_form($form, &$form_state) {

  $form['crm_contact_default_channel_method'] = array(
    '#type' => 'radios',
    '#title' => t('Select default channel selecting method'),
    '#default_value' => variable_get('crm_contact_default_channel_method', CRM_CONTACT_DEFAULT_CHANNEL_METHOD_LAST),
    '#options' => array(
      CRM_CONTACT_DEFAULT_CHANNEL_METHOD_LAST => t('Last active'),
      CRM_CONTACT_DEFAULT_CHANNEL_METHOD_FIRST => t('First active'),
      CRM_CONTACT_DEFAULT_CHANNEL_METHOD_RANDOM => t('Random active'),
    ),
  );

  $form['crm_contact_preferable_channel_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select preferable channel type'),
    '#default_value' => variable_get('crm_contact_preferable_channel_type', CRM_CONTACT_PREFERABLE_CHANNEL_TYPE),
    '#options' => array(CRM_CONTACT_PREFERABLE_CHANNEL_TYPE => t('None')) + crm_channel_type_option_list(),
  );

  return system_settings_form($form);
}