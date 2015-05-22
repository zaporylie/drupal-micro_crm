<?php

/**
 * @file
 */

/**
 * Implements hook_entity_property_info().
 */
function micro_crm_contact_entity_property_info() {
  $info = array();
  $properties = &$info['micro_crm_contact']['properties'];
  $default_properties = array(
    'setter callback' => 'entity_property_verbatim_set',
    'getter callback' => 'entity_property_verbatim_get',
  );
  $properties['is_new'] = array(
      'label' => t('Is new'),
      'type' => 'boolean',
      'description' => t('Whether the entity is new and not saved to the database yet.'),
    ) + $default_properties;
  $properties['log'] = array(
    'label' => t("Revision log message"),
    'type' => 'text',
    'description' => t("In case a new revision is to be saved, the log entry explaining the changes for this version."),
    'setter callback' => 'entity_property_verbatim_set',
  );
//  $properties['created'] = array(
//      'label' => t('Date created'),
//      'type' => 'date',
//      'description' => t('The date when contact was posted.'),
//      'schema field' => 'created',
//    ) + $default_properties;
//  $properties['changed'] = array(
//      'label' => t('Date changed'),
//      'type' => 'date',
//      'schema field' => 'changed',
//      'description' => t('The date when contact was most recently updated.'),
//    ) + $default_properties;

  return $info;
}
