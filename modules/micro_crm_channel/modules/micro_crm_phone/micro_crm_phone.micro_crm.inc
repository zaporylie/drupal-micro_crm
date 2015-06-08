<?php

/**
 * Implements hook_micro_crm_message_send().
 *
 * @see micro_crm_message_send().
 */
function micro_crm_email_micro_crm_message_send($message) {
  $wrapper = entity_metadata_wrapper('micro_crm_message', $message);
  if (!$wrapper->__isset('crm_message_phone') || !$wrapper->crm_message_phone->value()) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_phone_body') || !($body = $wrapper->crm_message_phone_body->value())) {
    return FALSE;
  }
  // @todo: Send message here.

  return FALSE;
}

/**
 * Implements hook_micro_crm_channel_label().
 */
function micro_crm_phone_micro_crm_channel_label($channel) {
  $wrapper = entity_metadata_wrapper('micro_crm_channel', $channel);
  if (!$wrapper->__isset('field_micro_crm_phone')) {
    return NULL;
  }
  return $wrapper->field_micro_crm_phone->value();
}
