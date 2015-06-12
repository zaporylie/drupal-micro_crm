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

  foreach (crm_message_types() as $type => $name) {
    $type_arg = strtr($type, '_', '-');
    $items[] = array(
      'data' => l($name, 'admin/people/crm/message/add/' . $type_arg),
    );
  }

  $build['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
  return $build;
}