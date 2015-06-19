<?php

/**
 * @file
 */

/**
 * Implements hook_crm_channel_label().
 */
function crm_phone_crm_channel_label($channel) {
  $wrapper = entity_metadata_wrapper('crm_channel', $channel);
  if (!$wrapper->__isset('crm_phone')) {
    return NULL;
  }
  return $wrapper->crm_phone->value();
}

if (module_exists('sms')) {
  /**
   * Implements hook_crm_message_send().
   *
   * @see crm_message_send().
   */
  function crm_phone_crm_message_send($message) {
    $wrapper = entity_metadata_wrapper('crm_message', $message);
    if (!$wrapper->__isset('crm_message_phone') || !$wrapper->crm_message_phone->value()) {
      return FALSE;
    }
    if (!$wrapper->__isset('crm_message_phone_body') || !($message = $wrapper->crm_message_phone_body->value())) {
      return FALSE;
    }
    if (!$wrapper->__isset('crm_message_phone_body') || !($number = $wrapper->crm_message_phone->crm_phone->value())) {
      return FALSE;
    }
    return sms_send($number, $message, array('message_id' => $wrapper->getIdentifier()));
  }
}