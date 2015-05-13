<?php

/**
 * @file
 */

function micro_crm_channel_entity_property_info() {
  $info = array();
  $properties = &$info['micro_crm_channel']['properties'];
  $default_properties = array(
    'setter callback' => 'entity_property_verbatim_set',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['is_new'] = array(
    'label' => t('Is new'),
    'type' => 'boolean',
    'description' => t('Whether the entity is new and not saved to the database yet.'),
  ) + $default_properties;
//  $properties['created'] = array(
//      'label' => t('Date created'),
//      'type' => 'date',
//      'description' => t('The date when channel was posted.'),
//      'schema field' => 'created',
//    ) + $default_properties;
//  $properties['changed'] = array(
//      'label' => t('Date changed'),
//      'type' => 'date',
//      'schema field' => 'changed',
//      'description' => t('The date when channel was most recently updated.'),
//    ) + $default_properties;

  return $info;
}