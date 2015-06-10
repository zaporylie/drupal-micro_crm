<?php

/**
 * Implements hook_crm_message_send().
 *
 * @see crm_message_send().
 */
function crm_email_crm_message_send($message) {
  $wrapper = entity_metadata_wrapper('crm_message', $message);
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
  $result = drupal_mail('crm_email', 'message', $wrapper->crm_message_email->field_crm_email->value(), $wrapper->language->value(), $params);
  return $result['result'];
}

/**
 * Implements hook_crm_channel_label().
 */
function crm_email_crm_channel_label($channel) {
  $wrapper = entity_metadata_wrapper('crm_channel', $channel);
  if (!$wrapper->__isset('field_crm_email')) {
    return NULL;
  }
  return $wrapper->field_crm_email->value();
}

/**
 * Implements hook_crm_newsletter_prepare_message().
 */
function crm_email_crm_newsletter_prepare_message($channel, $type = 'unsubscribe') {
  $message = entity_create('crm_message', array('type' => 'email'));
  $wrapper = entity_metadata_wrapper('crm_message', $message);
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
  switch ($type) {
    case 'subscribe':
      $wrapper->crm_message_email_subject->set(t('Confirm your subscription'));
      $wrapper->crm_message_email_body->set(array(
        'format' => 'plain_text',
        'value' => t("Click the button below to confirm.\n @button", array('@button' => url('crm/newsletter/subscribe/' . $channel->token, array('absolute' => TRUE)))),
      ));
      break;

    case 'unsubscribe':
      $wrapper->crm_message_email_subject->set(t('Cancel your newsletter subscription'));
      $wrapper->crm_message_email_body->set(array(
        'format' => 'plain_text',
        'value' => t("Thank you for being with us. Click the button below to confirm.\n @button", array('@button' => url('crm/newsletter/unsubscribe/' . $channel->token, array('absolute' => TRUE)))),
      ));
      break;

    default:
      return FALSE;
  }
  return $message;
}
