<?php

/**
 * @file
 *
 */

/**
 * Settings form.
 */
function crm_campaign_settings($form, &$form_state) {
  $form['crm_campaign_queue'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Drupal Queue API'),
    '#description' => t('If this settings is set to TRUE all messages generated for campaign will be queued and send by cron job runner. If FALSE - Batch API.'),
    '#default_value' => variable_get('crm_campaign_queue', FALSE),
  );
  return system_settings_form($form);
}
