<?php

/**
 * @file
 */

/**
 * Implements hook_crm_channel_label().
 */
function crm_email_crm_channel_label($channel) {
  $wrapper = entity_metadata_wrapper('crm_channel', $channel);
  if (!$wrapper->__isset('crm_email')) {
    return NULL;
  }
  return $wrapper->crm_email->value();
}

/**
 * Implements hook_crm_message_send().
 *
 * @see crm_message_send().
 */
function crm_email_crm_message_send($message) {
  $wrapper = entity_metadata_wrapper('crm_message', $message);
  $subject = $wrapper->crm_message_email_subject->value();
  $body = $wrapper->crm_message_email_body->value();
  $key = 'crm_message';
  $params['subject'] = $subject;
  $params['body'] = $body;

  $result = drupal_mail('crm_email', $key, $wrapper->crm_channel_single->crm_email->value(), $wrapper->language->value(), $params);
  return $result['result'];
}
