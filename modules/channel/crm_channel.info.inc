<?php

/**
 * @file
 */

/**
 * Implements hook_entity_property_info().
 */
function crm_channel_entity_property_info() {
  $info = array();
  $properties = &$info['crm_channel']['properties'];

  $default_properties = array(
    'setter callback' => 'entity_property_verbatim_set',
    'getter callback' => 'entity_property_verbatim_get',
  );

  $properties['is_new'] = array(
    'label' => t('Is new'),
    'type' => 'boolean',
    'description' => t('Whether the entity is new and not saved to the database yet.'),
  ) + $default_properties;
  $properties['channel'] = array(
    'label' => t('Channel'),
    'type' => 'text',
    'description' => t('General value.'),
    'getter callback' => 'crm_channel_channel',
  );
  $properties['channel_id'] = array(
    'label' => t('Channel ID'),
    'type' => 'integer',
    'schema field' => 'channel_id',
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
    'options list' => 'crm_channel_type_option_list',
    'required' => TRUE,
    'schema field' => 'type',
  ) + $default_properties;
  $properties['status'] = array(
    'label' => t('Status'),
    'description' => t('Entity status'),
    'type' => 'text',
    'options list' => 'crm_channel_status_option_list_flatten',
    'required' => TRUE,
    'schema field' => 'status',
  ) + $default_properties;
  $properties['token'] = array(
    'label' => t('Token'),
    'description' => t('One time token'),
    'type' => 'text',
    'schema field' => 'token',
  ) + $default_properties;
  $properties['token_created'] = array(
    'label' => t('Token created'),
    'description' => t('When one time token was generated'),
    'type' => 'date',
    'schema field' => 'token_created',
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
  $properties['created'] = array(
    'label' => t('Date created'),
    'type' => 'date',
    'description' => t('The date when channel was posted.'),
    'schema field' => 'created',
  ) + $default_properties;
  $properties['changed'] = array(
    'label' => t('Date changed'),
    'type' => 'date',
    'schema field' => 'changed',
    'description' => t('The date when channel was most recently updated.'),
  ) + $default_properties;

  return $info;
}
