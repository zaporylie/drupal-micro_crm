<?php

/**
 * @file
 */

/**
 * Act on an entity before it is about to be created or updated.
 *
 * @param $message
 *   The entity object.
 * @param $type
 *   The type of entity being saved (i.e. node, user, comment).
 */
function hook_entity_presave($message, $type) {
  $message->changed = REQUEST_TIME;
}

/**
 * Triggered on message update.
 *
 * @param object $message
 *   CRM Message object.
 */
function hook_crm_message_update($message) {
  // @todo: Add example.
}

/**
 * Triggered on message insert.
 *
 * @param object $message
 *   CRM Message object.
 */
function hook_crm_message_insert($message) {
  // @todo: Add example.
}

/**
 * List of status messages.
 *
 * @return array
 *   List of available statuses.
 */
function hook_crm_message_status_info() {
  $status = array();

  $status[CRM_MESSAGE_STATUS_DRAFT] = array(
    'title' => t('Draft'),
    'description' => t('Draft message'),
    'weight' => 0,
  );

  return $status;
}

/**
 * List of status messages.
 *
 * @return array
 *   List of available statuses.
 */
function hook_crm_message_status_info_alter(&$status) {

  $status['canceled'] = array(
    'title' => t('Canceled'),
    'description' => t('MEssage cancelled'),
    'weight' => -99,
  );

}

/**
 * Send message.
 *
 * @param object $message
 *   Message object.
 *
 * @return bool
 *   TRUE if sent FALSE if not.
 */
function hook_crm_message_send($message) {
  // Perform some action.
  return TRUE;
}
