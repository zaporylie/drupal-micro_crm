<?php

/**
 * @file
 * Forms exposed to user.
 */


/**
 * Form wrapper for entity addition.
 *
 * @return array
 *   Form.
 */
function crm_campaign_ui_add_form_wrapper() {
  $campaign = entity_create('crm_campaign', array(
    'data' => array(
      'manage_recipients' => array(
        'methods' => array(
          'crm_campaign_vbo' => array(),
        ),
      ),
    ),
  ));
  module_load_include('inc', 'crm_campaign', 'includes/crm_campaign.form');
  return drupal_get_form('crm_campaign_form', $campaign);
}

/**
 * Form wrapper for entity clone.
 *
 * @param object $campaign
 *   Original campaign.
 *
 * @return array
 *   Form.
 */
function crm_campaign_ui_clone($campaign) {
  return drupal_get_form('crm_campaign_ui_clone_form', $campaign);
}

/**
 * Clone confirmation form.
 */
function crm_campaign_ui_clone_form($form, $form_state, $campaign) {

  $form['campaign'] = array(
    '#type' => 'value',
    '#value' => $campaign,
  );

  return confirm_form($form, t('Do you really want to clone campaign %title?', array('%title' => $campaign->title)), 'admin/crm/campaign');
}

/**
 * Clone confirmation form: submit handler.
 */
function crm_campaign_ui_clone_form_submit($form, &$form_state) {
  $campaign = $form_state['values']['campaign'];
  $transaction = db_transaction();
  try {
    $campaign_wrapper = entity_metadata_wrapper('crm_campaign', $campaign);
    $templates = array();
    foreach ($campaign_wrapper->crm_message_template->getIterator() as $delta => $template_wrapper) {
      $template = $template_wrapper->value();
      unset($template->template_id);
      unset($template->revision_id);
      unset($template->revision_uid);
      unset($template->revision_timestamp);
      $template->created = REQUEST_TIME;
      unset($template->current_revision_id);
      unset($template->default_revision);

      do {
        $template->name = $template->name . '_clone';
      } while(crm_message_template_menu_name_exists($template->name));
      $template->log = 'Template cloned along with campaign';

      $template_wrapper->save();
      $templates[] = $template;
    }
    $campaign_wrapper->crm_message_template->set($templates);

    $campaign = entity_create('crm_campaign', array(
      'title' => $campaign->title,
      'crm_channel' => $campaign->crm_channel,
      'crm_message_template' => $campaign->crm_message_template,
      'data' => $campaign->data,
      'log' => 'Campaign cloned',
    ));
    entity_save('crm_campaign', $campaign);
    $uri = crm_campaign_uri($campaign);
    $form_state['redirect'] = $uri['path'];
  }
  catch (Exception $e) {
    $transaction->rollback();
    watchdog_exception('crm_campaign_ui', $e);
    drupal_set_message(t('Unable to clone campaign. Check admin log.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Form wrapper for entity deletion.
 *
 * @param null $type
 *
 * @return array|mixed
 */
function crm_campaign_ui_delete_form_wrapper($campaign) {
  return drupal_get_form('crm_campaign_ui_delete_form', $campaign);
}

/**
 * Form wrapper for sending campaign.
 *
 * @param null $type
 *
 * @return array|mixed
 */
function crm_campaign_ui_send_form_wrapper($campaign) {
  return drupal_get_form('crm_campaign_ui_send_form', $campaign);
}

/**
 * Form wrapper over entity form.
 *
 * @param $campaign
 *
 * @return array
 */
function crm_campaign_ui_edit_form_wrapper($campaign) {
  module_load_include('inc', 'crm_campaign', 'includes/crm_campaign.form');
  return crm_campaign_form_wrapper($campaign);
}

/**
 * Delete confirmation form.
 */
function crm_campaign_ui_delete_form($form, &$form_state, $campaign) {
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
function crm_campaign_ui_delete_form_submit($form, &$form_state) {
  $campaign_id = entity_id('crm_campaign', $form_state['values']['campaign']);
  if (entity_delete('crm_campaign', $campaign_id) !== FALSE) {
    drupal_set_message(t('Campaign @campaign has been deleted', array('@campaign' => $campaign_id)));
  }
}

/**
 * Delete confirmation form.
 */
function crm_campaign_ui_send_form($form, &$form_state, $campaign) {
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
function crm_campaign_ui_send_form_submit($form, &$form_state) {
  $campaign = $form_state['values']['campaign'];
  $wrapper = entity_metadata_wrapper('crm_campaign', $campaign);
  $channels = $wrapper->crm_channel->value();
  $batch_operations = array();

  foreach ($channels as $channel) {
    $batch_operations[] = array(
      'crm_campaign_ui_prepare_batch_item',
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
    'finished' => 'crm_campaign_ui_send_batch_finished',
    'file' => drupal_get_path('module', 'crm_campaign') . '/includes/crm_campaign_ui.form.inc',
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
function crm_campaign_ui_prepare_batch_item($campaign, $channel, &$context) {
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
  elseif (!$message && $message = crm_campaign_channel_create_message($campaign, $channel)) {
    // Message created and queued.
  }
  else {
    watchdog('crm_message', 'Unable to create message for channel_id @channel_id, campaign_id @campaign_id.', array('@channel_id' => $channel->channel_id, '@campaign_id' => $campaign->campaign_id), WATCHDOG_ERROR);
    return FALSE;
  }

  if ($queue_mode) {
    $queue->createItem($message->message_id);
    $context['results']['iterations'][] = array(
      'message_id' => $message->message_id,
      'status' => $message->status,
    );
  }
  else {
    crm_campaign_ui_send_batch_item($message->message_id, $context);
  }

}

/**
 * @param $message_id
 * @param $context
 */
function crm_campaign_ui_send_batch_item($message_id, &$context) {
  crm_message_send_worker_callback_batch_wrapper($message_id, $context);
}

/**
 * @param $success
 * @param $results
 * @param $operations
 */
function crm_campaign_ui_send_batch_finished($success, $results, $operations) {
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
