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
  if (isset($form_state['values']['status'])) {
    $campaign->status = $form_state['values']['status'];
  }
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
  $campaign = $form_state['values']['campaign'];
  $wrapper = entity_metadata_wrapper('crm_campaign', $campaign);
  $channels = $wrapper->crm_channel->value();
  $batch_operations = array();

  foreach ($channels as $channel) {
    $batch_operations[] = array(
      'crm_campaign_prepare_batch_item',
      array(
        $campaign,
        $channel,
      ),
    );
  }

  try {
    $campaign->log = 'Campaign has been queued';
    $campaign->revision = TRUE;
    $campaign->status = CRM_CAMPAIGN_STATUS_QUEUED;
    entity_save('crm_campaign', $campaign);
    drupal_set_message(t('Campaign has been queued'));
  }
  catch (Exception $e) {
    watchdog_exception('crm_campaign', $e);
  }

  $batch = array(
    'operations' => $batch_operations,
    'finished' => 'crm_campaign_send_batch_finished',
    'file' => drupal_get_path('module', 'crm_campaign') . '/includes/crm_campaign.form.inc',
  );
  $uri = entity_uri('crm_campaign', $campaign);
  $form_state['redirect'] = $uri['path'];
  batch_set($batch);
}

/**
 * @param $campaign
 * @param $channel
 * @param $context
 */
function crm_campaign_prepare_batch_item($campaign, $channel, &$context) {
  $context['results']['campaign'] = $campaign->campaign_id;
  $queue_mode = variable_get('crm_campaign_queue', FALSE);

  if ($queue_mode) {
    $queue = DrupalQueue::get('crm_message_queue');
    $queue->createQueue();
  }

  $message = _crm_campaign_message_exists($campaign, $channel);
  if (is_object($message) && $message->status == CRM_MESSAGE_STATUS_SENT) {
    return;
  }

  if (is_object($message) && $message->status <> CRM_MESSAGE_STATUS_QUEUED) {
    try {
      $message->status = CRM_MESSAGE_STATUS_QUEUED;
      $message->revision = TRUE;
      $message->log = format_string('Message (message_id: @message_id) has been queued.', array('@message_id' => $message->message_id));
      entity_save('crm_message', $message);
    }
    catch (Exception $e) {
      // @todo Do something with exceptions!
    }
  }
  elseif (!$message && $message = crm_campaign_channel_prepare_message($campaign, $channel, FALSE)) {
    try {
      $wrapper = entity_metadata_wrapper('crm_message', $message);
      $wrapper->crm_campaign_single->set($campaign);
      $message->status = CRM_MESSAGE_STATUS_QUEUED;
      $message->revision = TRUE;
      $message->log = format_string('Message (message_id: @message_id) has been queued.', array('@message_id' => $message->message_id));
      entity_save('crm_message', $message);
    }
    catch (Exception $e) {
      // @todo Do something with exceptions!
    }
  }
  else {
    watchdog('crm_message', 'Unable to create message for channel_id @channel_id, campaign_id @campaign_id.', array('@channel_id' => $channel->channel_id, '@campaign_id' => $campaign->campaign_id), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($queue_mode) {
    $queue->createItem($message->message_id);
    $context['results']['iterations'][] = array(
      'message_id' => $message->message_id,
      'status' => CRM_MESSAGE_STATUS_QUEUED,
    );
  }
  else {
    crm_campaign_send_batch_item($message->message_id, $campaign->campaign_id, $context);
  }

}

/**
 * @param $message_id
 * @param $campaign_id
 * @param $context
 */
function crm_campaign_send_batch_item($message_id, $campaign_id, &$context) {
  crm_message_send_worker_batch_wrapper($message_id, $context);
}

/**
 * @param $success
 * @param $results
 * @param $operations
 */
function crm_campaign_send_batch_finished($success, $results, $operations) {
  if ($success) {
    if (isset($results['iterations'])) {
      $sent = array_filter($results['iterations'], function ($value) {
        return $value['status'] == CRM_MESSAGE_STATUS_SENT;
      });
      $queued = array_filter($results['iterations'], function ($value) {
        return $value['status'] == CRM_MESSAGE_STATUS_QUEUED;
      });
      drupal_set_message(t('@sent messages has been sent. @queued messages has been queued and will be sent later.', array(
        '@sent' => count($sent),
        '@queued' => count($queued),
      )));
    }
    else {
      drupal_set_message(t('Nothing to send'));
    }
    $campaign = entity_load_single('crm_campaign', $results['campaign']);
    if (_crm_campaign_is_sent($campaign->campaign_id)) {
      $campaign->log = 'Campaign has been sent';
      $campaign->revision = TRUE;
      $campaign->status = CRM_CAMPAIGN_STATUS_SENT;
      entity_save('crm_campaign', $campaign);
      drupal_set_message(t('Campaign has been sent'));
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

/**
 * @param $campaign
 * @param $channel
 * @param bool $send
 * @return bool|mixed
 */
function crm_campaign_channel_prepare_message($campaign, $channel, $send = FALSE) {
  $type = $channel->type;
  $wrapper = entity_metadata_wrapper('crm_campaign', $campaign);
  $templates = $wrapper->crm_message_template->raw();
  if (!is_array($templates) && is_numeric($templates)) {
    if ($wrapper->crm_message_template->getBundle() == $type) {
      $template = $wrapper->crm_message_template->value();
    }
  }
  elseif (is_array($templates)) {
    foreach ($wrapper->crm_message_template as $template_wrapper) {
      if ($template_wrapper->getBundle() == $type) {
        $template = $template_wrapper->value();
        break;
      }
    }
  }
  if (!isset($template)) {
    return FALSE;
  }
  return crm_message_template_apply($template, $channel, $send);
}

/**
 * @param $campaign
 * @param $channel
 * @return bool|mixed
 */
function _crm_campaign_message_exists($campaign, $channel) {
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'crm_message')
    ->fieldCondition('crm_campaign_single', 'target_id', $campaign->campaign_id, '=')
    ->fieldCondition('crm_channel_single', 'target_id', $channel->channel_id, '=')
    ->range(0, 1);

  $result = $query->execute();

  if (isset($result['crm_message'])) {
    $ids = array_keys($result['crm_message']);
    return entity_load_single('crm_message', reset($ids));
  }
  return FALSE;
}
