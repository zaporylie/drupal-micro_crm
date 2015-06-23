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
 * Form wrapper for sending campaign.
 *
 * @param null $type
 *
 * @return array|mixed
 */
function crm_campaign_send_form_wrapper($campaign) {
  return drupal_get_form('crm_campaign_send_form', $campaign);
}

/**
 * Form wrapper over entity form.
 *
 * @param $campaign
 *
 * @return array
 */
function crm_campaign_edit_form_wrapper($campaign) {
  return crm_campaign_form_wrapper($campaign);
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
  $form['crm_channel']['#access'] = FALSE;

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

/**
 * Delete confirmation form.
 */
function crm_campaign_send_form($form, &$form_state, $campaign) {
  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );
  return confirm_form(
    $form,
    t('Do you really want to send campaign?'),
    'admin/crm/campaign'
  );
}

/**
 * Delete form submit handler.
 */
function crm_campaign_send_form_submit($form, &$form_state) {
  $queue_mode = variable_get('crm_campaign_queue', FALSE);
  $campaign = $form_state['values']['campaign'];
  $wrapper = entity_metadata_wrapper('crm_campaign', $campaign);
  $channels = $wrapper->crm_channel->raw();
  // If queue mode - prepare queue.
  if ($queue_mode) {
    $queue = DrupalQueue::get('crm_campaign_send');
    $queue->createQueue();
  }
  else {
    $batch_operations = array();
  }

  foreach ($channels as $channel) {
    $data = array(
      'campaign' => $campaign,
      'channel_id' => $channel,
    );
    if ($queue_mode) {
      $queue->createItem($data);
    }
    else {
      $batch_operations[] = array(
        // Callback.
        'crm_campaign_send_batch_item',
        $data,
      );
    }
  }
  // If batch mode - prepare batch.
  if (!$queue_mode) {
    $batch = array(
      'operations' => $batch_operations,
      'finished' => 'crm_campaign_send_batch_finished',
      'file' => drupal_get_path('module', 'crm_campaign') . '/crm_campaign.form.inc',
    );
    batch_set($batch);
  }

  try {
    $campaign->log = 'Campaign has been queued';
    $campaign->revision = TRUE;
    $campaign->status = CRM_CAMPAIGN_STATUS_QUEUED;
    entity_save('crm_campaign', $campaign);
    drupal_set_message(t('Campaign @campaign has been queued', array('@campaign' => $campaign->campaign_id)));
  }
  catch (Exception $e) {
    watchdog_exception('crm_campaign', $e);
  }
}

function crm_campaign_send_batch_item($campaign, $channel_id, &$context) {
  $channel = entity_load_single('crm_channel', $channel_id);
  $result = crm_campaign_channel_send($campaign, $channel);
  if (!isset($context['results']['campaign'])) {
    $context['results']['campaign'] = $campaign;
  }
  $context['results']['iterations'][] = array(
    $result,
    $channel_id,
  );
}

function crm_campaign_send_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('@count messages has been sent.', array('@count' => count($results['iterations']))));
    $count = count(array_filter($results['iterations'], function ($value) { return !$value[0]; }));
    if ($count === 0) {
      $campaign = $results['campaign'];
      $campaign->log = 'Campaign has been sent';
      $campaign->revision = TRUE;
      $campaign->status = CRM_CAMPAIGN_STATUS_SENT;
      entity_save('crm_campaign', $campaign);
    }
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(
      t('An error occurred while processing @operation with arguments: @args',
        array(
          '@operation' => $error_operation[0],
          '@args' => print_r($error_operation[0], TRUE),
        )
      ),
      'error'
    );
  }
}

function crm_campaign_channel_prepare_message($campaign, $channel, $send = FALSE) {
  $type = $channel->type;
  $wrapper = entity_metadata_wrapper('crm_campaign', $campaign);
  foreach ($wrapper->crm_message_template->getIterator() as $template_wrapper) {
    if ($template_wrapper->bundle() == $type) {
      $template = $template_wrapper->value();
      break;
    }
  }
  if (!isset($template)) {
    return FALSE;
  }
  return FALSE;
  return crm_message_template_apply($template, $channel, $send);
}

function crm_campaign_channel_send($campaign, $channel) {
  return crm_campaign_channel_prepare_message($campaign, $channel, TRUE);
}
