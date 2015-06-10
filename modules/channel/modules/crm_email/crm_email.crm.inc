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