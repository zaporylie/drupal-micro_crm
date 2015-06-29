<?php
/**
 * Implements hook_drush_command()
 */
function crm_message_drush_command() {
  $items = array();

  $items['crm-message-queue'] = array(
    'callback' => 'drush_crm_message_run_queue',
    'description' => 'Run message queue',
    'arguments' => array(
      'concurrency level' => 'The amount of background processes to spin-off',
    ),
    'required-arguments' => TRUE,
  );
  return $items;
}

/**
 * Command callback for drush queue-run-concurrent.
 *
 * Queue runner that is compatible with queues declared using both
 * hook_queue_info() and hook_cron_queue_info().
 *
 * @param $queue_name
 *   Arbitrary string. The name of the queue to work with.
 * @param $concurrency_level
 *   Amount of background
 */
function drush_crm_message_run_queue($concurrency_level) {
  $invocations = array();
  $queue = DrupalQueue::get('crm_message_queue');
  for ($i = 0; $queue->numberOfItems() > $i; $i++) {
    $invocations[] = array('command' => 'queue-run crm_message_queue', 'site' => '@self');
  }
  $common_options = array(
    'concurrency' => $concurrency_level,
  );
  drush_backend_invoke_concurrent($invocations, array(), $common_options);
}
