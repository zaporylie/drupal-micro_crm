<?php

/**
 * @file
 */

/**
 * Implements hook_crm_message_send().
 *
 * @see crm_message_send().
 */
function crm_phone_message_crm_message_send($message) {
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

/**
 * Implements hook_crm_newsletter_prepare_message().
 */
function crm_phone_message_crm_newsletter_prepare_message($channel, $type = 'unsubscribe') {
  $message = entity_create('crm_message', array('type' => 'phone'));
  $wrapper = entity_metadata_wrapper('crm_message', $message);
  if (!$wrapper->__isset('crm_message_phone')) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_phone_body')) {
    return FALSE;
  }
  $wrapper->crm_message_phone->set($channel);
  switch ($type) {
    case 'subscribe':
      $wrapper->crm_message_phone_body->set(t("Click to confirm @button", array('@button' => url('crm/newsletter/subscribe/' . $channel->token, array('absolute' => TRUE)))));
      break;

    case 'unsubscribe':
      $wrapper->crm_message_phone_body->set(t("Thank you for being with us. Click to confirm @button", array('@button' => url('crm/newsletter/unsubscribe/' . $channel->token, array('absolute' => TRUE)))));
      break;

    default:
      return FALSE;
  }
  return $message;
}
