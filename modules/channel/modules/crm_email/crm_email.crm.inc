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
  // @todo ensure all fields exists and was filled.
  if (!$wrapper->__isset('crm_channel_single') || !$wrapper->crm_channel_single->value() || !$wrapper->crm_channel_single->__isset('crm_email')) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_email_subject') || !($subject = $wrapper->crm_message_email_subject->value())) {
    return FALSE;
  }
  if (!$wrapper->__isset('crm_message_email_body') || !($body = $wrapper->crm_message_email_body->value())) {
    return FALSE;
  }
  $key = 'crm_message';
  if ($wrapper->__isset('crm_message_template_single') && $template = $wrapper->crm_message_template_single->label()) {
    $key = $template;
  }
  $params['subject'] = $subject;
  $params['body'] = $body;

  $result = drupal_mail('crm_email', $key, $wrapper->crm_channel_single->crm_email->value(), $wrapper->language->value(), $params);
  return $result['result'];
}
