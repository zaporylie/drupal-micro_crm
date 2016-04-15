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
    $message = $wrapper->crm_message_phone_body->value();
    $number = $wrapper->crm_channel_single->crm_phone->value();
    return sms_send($number, $message, array('message_id' => $wrapper->getIdentifier()));
  }
}