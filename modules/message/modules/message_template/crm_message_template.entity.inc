<?php

/**
 * @file
 *
 */

/**
 * Implements hook_entity_info().
 */
function crm_message_template_entity_info() {
  $entities['crm_message_template'] = array(
    'label' => t('Message template', array(), array('context' => 'crm')),
    'controller class' => 'CRMMessageTemplateController',
    'base table' => 'crm_message_template',
    'revision table' => 'crm_message_template_revision',
    'load hook' => 'crm_message_template_load',
    'uri callback' => 'crm_message_template_uri',
    'label callback' => 'crm_message_template_label',
    'fieldable' => TRUE,
    'entity keys' => array(
      'id' => 'template_id',
      'bundle' => 'type',
      'revision' => 'revision_id',
      'language' => 'language',
    ),
    'bundle keys' => array(
      'bundle' => 'type',
    ),
    'bundles' => array(),
    'view modes' => array(
      'administrator' => array(
        'label' => t('Administrator'),
        'custom settings' => FALSE,
      ),
    ),
//    'translation' => array(
//      'entity_translation' => array(
//        'class' => 'EntityTranslationDefaultHandler',
//        'bundle callback' => 'crm_message_template_entity_translation_supported_type',
//        'default settings' => array(
//          'default_language' => LANGUAGE_NONE,
//          'hide_language_selector' => TRUE,
//          'lock_language' => TRUE,
//        ),
//      ),
//    ),
    'access callback' => 'crm_message_template_access',
  );
  return $entities;
}

/**
 *
 */
function crm_message_template_access($op, $entity = NULL, $account = NULL) {
  return entity_get_controller('crm_message_template')->access($op, $entity, $account);
}

/**
 *
 */
function crm_message_template_load($id) {
  if (!is_numeric($id)) {
    return FALSE;
  }
  $entities = crm_message_template_load_multiple(array($id));
  if ($entities) {
    return reset($entities);
  }
}

/**
 * @param $machine_name
 */
function crm_channel_template_name_load($machine_name) {
  $id = db_query('SELECT template_id FROM {crm_message_template} WHERE name = :name', array(':name' => $machine_name))->fetchField();
  return crm_message_template_load($id);
}

/**
 *
 */
function crm_message_template_load_multiple($ids) {
  return entity_load('crm_message_template', $ids);
}

/**
 * @param $ids
 */
function crm_message_template_delete($id) {
  crm_message_template_delete(array($id));
}

/**
 * @param $ids
 */
function crm_message_template_delete_multiple($ids) {
  entity_delete_multiple('crm_message_template', $ids);
}

/**
 *
 */
function crm_message_template_view($template, $view_mode = 'administrator') {
  if (!is_array($template)) {
    $template = array($template->template_id => $template);
  }
  return entity_get_controller('crm_message_template')->view($template, $view_mode);
}

/**
 * @todo: Move uri to UI module.
 */
function crm_message_template_uri($template) {
  return array(
    'path' => 'admin/crm/message/template/' . $template->template_id,
  );
}

/**
 *
 */
function crm_message_template_label($template) {
  return $template->name;
}
