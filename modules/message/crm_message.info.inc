<?php

/**
 * @file
 */

/**
 * Implements hook_entity_property_info().
 */
function crm_message_entity_property_info() {
  $info = array();
  $properties = &$info['crm_message']['properties'];

  $default_properties = array(
    'setter callback' => 'entity_property_verbatim_set',
    'getter callback' => 'entity_property_verbatim_get',
  );

  $properties['is_new'] = array(
      'label' => t('Is new'),
      'type' => 'boolean',
      'description' => t('Whether the entity is new and not saved to the database yet.'),
    ) + $default_properties;
  $properties['message_id'] = array(
    'label' => t('Message ID'),
    'type' => 'integer',
    'schema field' => 'message_id',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['revision_id'] = array(
    'label' => t("Revision ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the node's revision."),
    'schema field' => 'revision_id',
  );
  $properties['type'] = array(
      'label' => t('Type'),
      'description' => t('Entity bundle'),
      'type' => 'text',
      'options list' => 'crm_message_types',
      'required' => TRUE,
      'schema field' => 'type',
    ) + $default_properties;
  $properties['created'] = array(
      'label' => t('Date created'),
      'type' => 'date',
      'description' => t('The date when message was posted.'),
      'schema field' => 'created',
    ) + $default_properties;
  $properties['changed'] = array(
      'label' => t('Date changed'),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t('The date when message was most recently updated.'),
    ) + $default_properties;
  $properties['status'] = array(
      'label' => t('Status'),
      'description' => t('Entity status'),
      'type' => 'text',
      'options list' => 'crm_message_status_options_list',
      'required' => TRUE,
      'schema field' => 'status',
    ) + $default_properties;
  $properties['language'] = array(
      'label' => t("Language"),
      'type' => 'token',
      'description' => t("The language the node is written in."),
      'options list' => 'entity_metadata_language_list',
      'schema field' => 'language',
    ) + $default_properties;

  $properties['log'] = array(
    'label' => t("Revision log message"),
    'type' => 'text',
    'description' => t("In case a new revision is to be saved, the log entry explaining the changes for this version."),
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['revision'] = array(
    'label' => t("Creates revision"),
    'type' => 'boolean',
    'description' => t("Whether saving this entity creates a new revision."),
    'setter callback' => 'entity_property_verbatim_set',
  );

  return $info;
}
