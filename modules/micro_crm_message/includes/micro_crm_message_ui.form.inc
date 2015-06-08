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
function micro_crm_message_ui_form_wrapper($message = array(), $delete = FALSE, $type = NULL) {
  if ($delete == TRUE && isset($message->message_id)) {
    return drupal_get_form('micro_crm_message_ui_delete_form', $message);
  }
  if (empty($message))  {
    $message = entity_create('micro_crm_message', array(
      'type' => $type,
    ));
  }
  return drupal_get_form('micro_crm_message_ui_form', $message);
}

/**
 * Message form.
 */
function micro_crm_message_ui_form($form, &$form_state, $message) {

  $form['message'] = array(
    '#type' => 'value',
    '#value' => $message,
  );

  field_attach_form('micro_crm_message', $message, $form, $form_state);

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
    '#options' => micro_crm_message_status_options_list(),
  );

  $form['settings']['options']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#required' => TRUE,
    '#default_value' => $message->language,
    '#options' => array(LANGUAGE_NONE => t('Undefined')) + micro_crm_language_list(),
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
 * Message form validation
 */
function micro_crm_message_ui_form_validate($form, &$form_state) {
  field_attach_form_validate('micro_crm_message', $form_state['values']['message'], $form, $form_state);
}

/**
 * Message form submit callback.
 */
function micro_crm_message_ui_form_submit($form, &$form_state) {
  $message = $form_state['values']['message'];

  $message->status = $form_state['values']['status'];
  $message->language = $form_state['values']['language'];
  $message->log = $form_state['values']['log'];
  if (!isset($message->revision)) {
    $message->revision = TRUE;
  }
  try {
    field_attach_submit('micro_crm_message', $message, $form, $form_state);
    entity_save('micro_crm_message', $message);
    $uri = entity_uri('micro_crm_message', $message);
    $form_state['redirect'] = $uri['path'];
    drupal_set_message(t('Message has been saved'));
  }
  catch (Exception $e) {
    watchdog_exception('micro_crm_channel', $e);
    $form_state['rebuild'] = TRUE;
    drupal_set_message(t('Unable to save message'), 'error');
  }

}

/**
 * Send message confirmation form.
 */
function micro_crm_message_ui_send($form, &$form_state, $message) {
  $form['message'] = array(
    '#type' => 'value',
    '#value' => $message,
  );
  return confirm_form(
    $form,
    t('Do you want to send the message?'),
    'admin/people/micro_crm/message'
  );
}

/**
 * Delete form submit handler.
 */
function micro_crm_message_ui_send_submit($form, &$form_state) {
  $message_id = entity_id('micro_crm_message', $form_state['values']['message']);
  if (micro_crm_message_send($form_state['values']['message']) !== FALSE) {
    drupal_set_message(t('Message @message has been sent', array('@message' => $message_id)));
  }
}

/**
 * Delete confirmation form.
 */
function micro_crm_message_ui_delete_form($form, &$form_state, $message) {
  $form['message'] = array(
    '#type' => 'value',
    '#value' => $message,
  );
  return confirm_form(
    $form,
    t('Do you want to delete message?'),
    'admin/people/micro_crm/message'
  );
}

/**
 * Delete form submit handler.
 */
function micro_crm_message_ui_delete_form_submit($form, &$form_state) {
  $message_id = entity_id('micro_crm_message', $form_state['values']['message']);
  if (entity_delete('micro_crm_message', $message_id) !== FALSE) {
    drupal_set_message(t('Message @message has been deleted', array('@message' => $message_id)));
  }
}
