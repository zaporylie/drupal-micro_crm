<?php

/**
 * Implements hook_micro_crm_message_send().
 *
 * @see micro_crm_message_send().
 */
function micro_crm_email_micro_crm_message_send($message) {
  $wrapper = entity_metadata_wrapper('micro_crm_message', $message);
  // @todo ensure all fields exists and was filled.
  if (!$wrapper->__isset('crm_message_email') || !$wrapper->crm_message_email->value()) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_email_subject') || !($subject = $wrapper->crm_message_email_subject->value())) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_email_body') || !($body = $wrapper->crm_message_email_body->value())) {
    return FALSE;
  }
  $params['subject'] = $subject;
  $params['body'] = $body;
  $result = drupal_mail('micro_crm_email', 'message', $wrapper->crm_message_email->field_micro_crm_email->value(), $wrapper->language->value(), $params);
  return $result['result'];
}

/**
 * Implements hook_micro_crm_channel_label().
 */
function micro_crm_email_micro_crm_channel_label($channel) {
  $wrapper = entity_metadata_wrapper('micro_crm_channel', $channel);
  if (!$wrapper->__isset('field_micro_crm_email')) {
    return NULL;
  }
  return $wrapper->field_micro_crm_email->value();
}

/**
 * Implements hook_micro_crm_newsletter_prepare_message().
 */
function micro_crm_email_micro_crm_newsletter_prepare_message($channel) {
  $message = entity_create('micro_crm_message', array('type' => 'email'));
  $wrapper = entity_metadata_wrapper('micro_crm_message', $message);
  if (!$wrapper->__isset('crm_message_email')) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_email_subject')) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_email_body')) {
    return FALSE;
  }
  $wrapper->crm_message_email->set($channel);
  $wrapper->crm_message_email_subject->set(t('Cancel your newsletter subscription'));
  $wrapper->crm_message_email_body->set(array(
    'format' => 'plain_text',
    'value' => t("Thank you for being with us. Click the button below to confirm.\n @button", array('@button' => url('micro_crm/newsletter/unsubscribe/' . $channel->token, array('absolute' => TRUE)))),
  ));
  return $message;
}
