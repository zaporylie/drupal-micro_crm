<?php

/**
 * @file
 * Default entity form.
 */

/**
 * Form wrapper for entity addition.
 *
 * @param null $type
 *
 * @return array|mixed
 */
function crm_campaign_add_form_wrapper() {
  $campaign = entity_create('crm_campaign', array());
  return drupal_get_form('crm_campaign_form', $campaign);
}

/**
 * Form wrapper for entity deletion.
 *
 * @param null $type
 *
 * @return array|mixed
 */
function crm_campaign_delete_form_wrapper($campaign) {
  return drupal_get_form('crm_campaign_delete_form', $campaign);
}

/**
 * Form wrapper for entity update.
 *
 * @param array $campaign
 *   Campaign object or array of default values.
 *
 * @return array
 */
function crm_campaign_form_wrapper($campaign) {
  return drupal_get_form('crm_campaign_form', $campaign);
}

/**
 * Campaign form.
 */
function crm_campaign_form($form, &$form_state, $campaign) {

  $form_state['crm_campaign'] = $campaign;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => isset($campaign->title) ? $campaign->title : '',
  );

  field_attach_form('crm_campaign', $campaign, $form, $form_state);

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 98,
  );
  $form['settings']['revision'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['revision']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Revision log'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Campaign form validation
 */
function crm_campaign_form_validate($form, &$form_state) {
  field_attach_form_validate('crm_campaign', $form_state['crm_campaign'], $form, $form_state);
}

/**
 * Campaign form submit callback.
 */
function crm_campaign_form_submit($form, &$form_state) {
  $campaign = $form_state['crm_campaign'];
  $campaign->title = $form_state['values']['title'];
  $campaign->log = $form_state['values']['log'];
  if (!isset($campaign->revision)) {
    $campaign->revision = TRUE;
  }
  try {
    field_attach_submit('crm_campaign', $campaign, $form, $form_state);
    entity_save('crm_campaign', $campaign);
    $uri = entity_uri('crm_campaign', $campaign);
    $form_state['redirect'] = $uri['path'];
    drupal_set_message(t('Campaign has been saved'));
  }
  catch (Exception $e) {
    watchdog_exception('crm_campaign', $e);
    $form_state['rebuild'] = TRUE;
    drupal_set_message(t('Unable to save campaign'), 'error');
  }

}

/**
 * Delete confirmation form.
 */
function crm_campaign_delete_form($form, &$form_state, $campaign) {
  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );
  return confirm_form(
    $form,
    t('Do you want to delete campaign?'),
    'admin/crm/campaign'
  );
}

/**
 * Delete form submit handler.
 */
function crm_campaign_delete_form_submit($form, &$form_state) {
  $campaign_id = entity_id('crm_campaign', $form_state['values']['campaign']);
  if (entity_delete('crm_campaign', $campaign_id) !== FALSE) {
    drupal_set_message(t('Campaign @campaign has been deleted', array('@campaign' => $campaign_id)));
  }
}
