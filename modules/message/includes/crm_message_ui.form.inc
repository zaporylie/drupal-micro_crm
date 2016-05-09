<?php

/**
 * @file
 * Default entity form.
 */

/**
 * Form wrapper for create, update and delete.
 *
 * @param array $message
 *   Message object or array of default values.
 * @param bool $delete
 *   Boolean indicator if create/edit or delete form should be displayed.
 *
 * @return array
 */
function crm_message_ui_form_wrapper($message = array(), $delete = FALSE, $type = NULL) {
  if ($delete == TRUE && isset($message->message_id)) {
    return drupal_get_form('crm_message_ui_delete_form', $message);
  }
  if (empty($message))  {
    $message = entity_create('crm_message', array(
      'type' => $type,
    ));
  }
  return drupal_get_form('crm_message_ui_form', $message);
}

/**
 * @param $channel
 */
function crm_message_ui_send_channel_form_wrapper($channel) {
  $message = crm_message_create(array(
    'channel' => $channel,
  ), 'crm_message');
  if (!$message) {
    drupal_set_message(t('Unable to create a new message'), 'error');
    drupal_goto();
  }
  return drupal_get_form('crm_message_ui_form', $message);
}

/**
 * @param $contact
 */
function crm_message_ui_send_contact_form_wrapper($contact) {
  if ($channel = crm_contact_channel_get_default($contact)) {
    $message = crm_message_create(array(
      'channel' => $channel,
    ), 'crm_message');
  }
  if (!isset($message) || !$message) {
    drupal_set_message(t('Unable to create a new message'), 'error');
    drupal_goto();
  }
  return drupal_get_form('crm_message_ui_form', $message);
}

/**
 * Message form.
 */
function crm_message_ui_form($form, &$form_state, $message) {

  $form['message'] = array(
    '#type' => 'value',
    '#value' => $message,
  );

  field_attach_form('crm_message', $message, $form, $form_state);

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
    '#required' => TRUE,
    '#default_value' => $message->status,
    '#options' => crm_message_status_options_list(),
    '#disabled' => TRUE,
  );

  $form['settings']['options']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#required' => TRUE,
    '#default_value' => $message->language,
    '#options' => array(LANGUAGE_NONE => t('Undefined')) + crm_language_list(),
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
  $form['actions']['send'] = array(
    '#type' => 'submit',
    '#value' => t('Save & send'),
    '#ief_submit_all' => TRUE,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save as Draft'),
    '#attributes' => array(
      'class' => array(
        'text',
      ),
    ),
  );
  $form['actions']['queue'] = array(
    '#type' => 'submit',
    '#value' => t('Save & queue'),
    '#attributes' => array(
      'class' => array(
        'text',
      ),
    ),
  );
  return $form;
}

/**
 * Message form validation
 */
function crm_message_ui_form_validate($form, &$form_state) {
  field_attach_form_validate('crm_message', $form_state['values']['message'], $form, $form_state);
}

/**
 * Message form submit callback.
 */
function crm_message_ui_form_submit($form, &$form_state) {
  $message = &$form_state['values']['message'];

  $message->status = $form_state['values']['status'];
  $message->language = $form_state['values']['language'];
  $message->log = $form_state['values']['log'];
  if (!isset($message->revision)) {
    $message->revision = TRUE;
  }
  try {
    field_attach_submit('crm_message', $message, $form, $form_state);
    entity_save('crm_message', $message);
    $uri = entity_uri('crm_message', $message);

    $options = array();
    if (isset($_GET['destination'])) {
      $options['query'] = $_GET['destination'];
      unset($_GET['destination']);
    }
    if ($form_state['triggering_element']['#value'] == t('Save as Draft')) {
      drupal_set_message(t('Message saved as draft.'));
    }
    elseif ($form_state['triggering_element']['#value'] == t('Save & send')) {
      $form_state['values']['method'] = CRM_MESSAGE_STATUS_SENT;
      crm_message_ui_send_submit($form, $form_state);
    }
    elseif ($form_state['triggering_element']['#value'] == t('Save & queue')) {
      $form_state['values']['method'] = CRM_MESSAGE_STATUS_QUEUED;
      crm_message_ui_send_submit($form, $form_state);
    }

    if (isset($options['query'])) {
      $form_state['redirect'] = $options['query'];
    }
    else {
      $form_state['redirect'] = $uri['path'];
    }
  }
  catch (Exception $e) {
    watchdog_exception('crm_message', $e);
    $form_state['rebuild'] = TRUE;
    drupal_set_message(t('Unable to save message'), 'error');
  }

}

/**
 * Cancel message confirmation form.
 */
function crm_message_ui_cancel($form, &$form_state, $message) {
  $form['message'] = array(
    '#type' => 'value',
    '#value' => $message,
  );
  $uri = entity_uri('crm_message', $message);
  return confirm_form(
    $form,
    t('Do you want to cancel this message?'),
    $uri['path']
  );
}

/**
 * Cancel message submit handler.
 */
function crm_message_ui_cancel_submit($form, &$form_state) {
  $message = $form_state['values']['message'];
  crm_message_status_update($message, CRM_MESSAGE_STATUS_DRAFT);
  $uri = entity_uri('crm_message', $message);
  $form_state['redirect'] = $uri['path'];
}

/**
 * Send message confirmation form.
 */
function crm_message_ui_send($form, &$form_state, $message) {
  $form['message'] = array(
    '#type' => 'value',
    '#value' => $message,
  );
  $form['method'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#default_value' => CRM_MESSAGE_STATUS_SENT,
    '#options' => array(
      CRM_MESSAGE_STATUS_SENT => t('Send instantly'),
      CRM_MESSAGE_STATUS_QUEUED => t('Queue, send with other queued messages'),
    ),
  );
  $uri = entity_uri('crm_message', $message);
  return confirm_form(
    $form,
    t('Do you want to send the message?'),
    $uri['path']
  );
}

/**
 * Delete form submit handler.
 */
function crm_message_ui_send_submit($form, &$form_state) {
  $message = $form_state['values']['message'];
  $message_id = entity_id('crm_message', $message);

  $message->status = CRM_MESSAGE_STATUS_QUEUED;
  $message->revision = TRUE;
  $message->log = 'Message has been queued.';
  entity_save('crm_message', $message);

  switch ($form_state['values']['method']) {
    case CRM_MESSAGE_STATUS_QUEUED:
      drupal_set_message(t('Message (message_id: @message_id) has been queued', array('@message_id' => $message_id)));
      break;

    case CRM_MESSAGE_STATUS_SENT:
      // Try to send message.
      $result = crm_message_send($message);

      if ($result === TRUE) {
        drupal_set_message(t('Message (message_id: @message_id) has been sent', array('@message_id' => $message_id)));
      }
      else {
        drupal_set_message(t('Unable to send message. Message (message_id: @message_id) has been queued', array('@message_id' => $message_id)), 'error');
      }
      break;
  }
  $uri = entity_uri('crm_message', $message);
  $form_state['redirect'] = $uri['path'];
}

/**
 * Delete confirmation form.
 */
function crm_message_ui_delete_form($form, &$form_state, $message) {
  $form['message'] = array(
    '#type' => 'value',
    '#value' => $message,
  );
  return confirm_form(
    $form,
    t('Do you want to delete message?'),
    'admin/crm/message'
  );
}

/**
 * Delete form submit handler.
 */
function crm_message_ui_delete_form_submit($form, &$form_state) {
  $message_id = entity_id('crm_message', $form_state['values']['message']);
  if (entity_delete('crm_message', $message_id) !== FALSE) {
    drupal_set_message(t('Message @message has been deleted', array('@message' => $message_id)));
  }
}
