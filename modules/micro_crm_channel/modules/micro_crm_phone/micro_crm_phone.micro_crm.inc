<?php

/**
 * Implements hook_micro_crm_message_send().
 *
 * @see micro_crm_message_send().
 */
function micro_crm_phone_micro_crm_message_send($message) {
  $wrapper = entity_metadata_wrapper('micro_crm_message', $message);
  if (!$wrapper->__isset('crm_message_phone') || !$wrapper->crm_message_phone->value()) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_phone_body') || !($message = $wrapper->crm_message_phone_body->value())) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_phone_body') || !($number = $wrapper->crm_message_phone->field_micro_crm_phone->value())) {
    return FALSE;
  }
  if (!function_exists('sms_send')) {
    return FALSE;
  }
  return sms_send($number, $message, array('message_id' => $wrapper->getIdentifier()));
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

/**
 * Implements hook_micro_crm_newsletter_prepare_message().
 */
function micro_crm_phone_micro_crm_newsletter_prepare_message($channel, $type = 'unsubscribe') {
  $message = entity_create('micro_crm_message', array('type' => 'phone'));
  $wrapper = entity_metadata_wrapper('micro_crm_message', $message);
  if (!$wrapper->__isset('crm_message_phone')) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_phone_body')) {
    return FALSE;
  }
  $wrapper->crm_message_phone->set($channel);
  switch ($type) {
    case 'subscribe':
      $wrapper->crm_message_phone_body->set(t("Click to confirm @button", array('@button' => url('micro_crm/newsletter/subscribe/' . $channel->token, array('absolute' => TRUE)))));
      break;

    case 'unsubscribe':
      $wrapper->crm_message_phone_body->set(t("Thank you for being with us. Click to confirm @button", array('@button' => url('micro_crm/newsletter/unsubscribe/' . $channel->token, array('absolute' => TRUE)))));
      break;

    default:
      return FALSE;
  }
  return $message;
}
