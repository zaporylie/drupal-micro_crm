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
