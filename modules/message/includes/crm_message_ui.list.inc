<?php

/**
 * @file
 * Message list.
 */

/**
 *
 *
 * @return array
 */
function crm_message_ui_list() {
  $build = array();

  return $build;
}

/**
 *
 */
function crm_message_ui_add() {
  $build = array();
  $items = array();
  $options = array();
  if (isset($_GET['destination'])) {
    $options['query'] = drupal_get_destination();
  }

  foreach (crm_message_types() as $type => $name) {
    $type_arg = strtr($type, '_', '-');
    $items[] = array(
      'data' => l($name, 'admin/crm/message/add/' . $type_arg, $options),
      'class' => array('crm-channel', $type_arg),
    );
  }

  $build['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
    '#attributes' => array(
      'class' => array(
        'crm-channel-ui-type-list',
      ),
    ),
  );

  $build['#attached']['css'][] = drupal_get_path('module', 'crm_channel_ui') . '/assets/crm_channel_ui.types.css';
  return $build;
}