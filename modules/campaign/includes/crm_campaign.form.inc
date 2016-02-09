<?php

/**
 * @file
 * Default entity form.
 */

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

  form_load_include($form_state, 'inc', 'crm_campaign', 'includes/crm_campaign.form');
  $form_state['crm_campaign'] = $campaign;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    // @todo: There is no need to test it here. Default value has been added.
    '#default_value' => isset($campaign->title) ? $campaign->title : '',
  );

  field_attach_form('crm_campaign', $campaign, $form, $form_state);
  $form['crm_channel']['#access'] = FALSE;

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 98,
  );
  $form['settings']['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['options']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#default_value' => $campaign->status,
    '#options' => crm_campaign_status_options_list(),
    '#access' => user_access('crm administer'),
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

  if (!empty($campaign->data['manage_recipients']['methods'])) {
    $form['settings']['manage_recipients'] = array(
      '#type' => 'fieldset',
      '#title' => t('Manage recipients'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );
    foreach ($campaign->data['manage_recipients']['methods'] as $method => $options) {
      $form['settings']['manage_recipients']['methods'][$method] = crm_campaign_method_get_form($method, $options);
    }

    $form['settings']['manage_recipients']['rebuild'] = array(
      '#type' => 'checkbox',
      '#title' => t('Rebuild recipient list.'),
      '#default_value' => isset($campaign->data['manage_recipients']['rebuild']) ? $campaign->data['manage_recipients']['rebuild'] : TRUE,
      '#description' => t('If selected new channels will be added on form submit (if available).'),
    );

    $form['settings']['manage_recipients']['remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove all existing channels and add it one more time.'),
      '#default_value' => isset($campaign->data['manage_recipients']['remove']) ? $campaign->data['manage_recipients']['remove'] : FALSE,
      '#description' => t('If selected all channels will be removed and added again. All channels added manually will be removed!'),
      '#states' => array(
        'invisible' => array(
          ':input[name="manage_recipients[rebuild]"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#ief_submit_all' => TRUE,
    '#weight' => -1,
  );
  $form['actions']['send'] = array(
    '#type' => 'submit',
    '#value' => t('Save & send'),
    '#ief_submit_all' => TRUE,
    '#weight' => 99,
  );

  // IEF workaround.
  $form['#submit'][] = 'inline_entity_form_trigger_submit';
  $form['#submit'][] = 'crm_campaign_form_submit';

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
  $campaign->data['manage_recipients'] = $form_state['values']['manage_recipients'];

  if (isset($form_state['values']['status'])) {
    $campaign->status = $form_state['values']['status'];
  }
  if (!isset($campaign->revision)) {
    $campaign->revision = TRUE;
  }
  try {
    // Attach fields values to campaign object.
    field_attach_submit('crm_campaign', $campaign, $form, $form_state);
    // Prepopulate channel field.
    // Be really careful about that - this function can remove all channels from
    // crm_channel field.
    _crm_campaign_method_prepopulate($campaign);

    entity_save('crm_campaign', $campaign);
    $uri = entity_uri('crm_campaign', $campaign);
    if ($form_state['triggering_element']['#value'] == t('Save & send')) {
      if (isset($_GET['destination'])) {
        unset($_GET['destination']);
      }
      $form_state['redirect'] = $uri['path'] . '/send';
    }
    else {
      $form_state['redirect'] = $uri['path'];
    }
    drupal_set_message(t('Campaign has been saved'));
  }
  catch (Exception $e) {
    watchdog_exception('crm_campaign', $e);
    $form_state['rebuild'] = TRUE;
    drupal_set_message(t('Unable to save campaign'), 'error');
  }

}
