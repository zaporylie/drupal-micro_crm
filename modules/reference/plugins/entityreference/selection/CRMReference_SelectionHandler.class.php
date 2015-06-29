<?php

/**
 * A Entity handler for DynamicBundle filter.
 */
class CRMReference_SelectionHandler extends EntityReference_SelectionHandler_Generic {
  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new CRMReference_SelectionHandler($field, $instance, $entity_type, $entity);
  }

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    $form['view']['crm_reference'] = array(
      '#markup' => t('Entity will look for a entities with matching bundle.'),
    );

    return $form;
  }

  /**
   * Build an EntityFieldQuery to get referenceable entities.
   */
  public function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->field['settings']['target_type']);
    $entities = entity_get_info();
    if (isset($entities[$this->field['settings']['target_type']]['bundles'][$this->instance['bundle']]) && isset($entities[$this->instance['entity_type']]['bundles'][$this->instance['bundle']])) {
      $query->entityCondition('bundle', array($this->instance['bundle']), 'IN');
    }

//    if (isset($match)) {
//      $fields = array();
//      if ($fields = crm_reference_get_search_field($this->field['settings']['target_type'], $this->instance['bundle'])) {
//        // Do nothig.
//      }
//      elseif ($tmp = crm_reference_get_search_field($this->field['settings']['target_type'])) {
//        // Flatten array.
//        foreach ($tmp as $field) {
//          $fields += $field;
//        }
//      }
//      foreach ($fields as $field) {
//        $query->fieldCondition($field['name'], $field['column'], $match, $match_operator);
//      }
//    }

    // Add a generic entity access tag to the query.
    $query->addTag($this->field['settings']['target_type'] . '_access');
    $query->addTag('entityreference');
    $query->addTag('crm_reference');
    $query->addMetaData('crm_reference_match', array(
      'match' => $match,
      'match_operator' => $match_operator,
    ));
    $query->addMetaData('field', $this->field);
    $query->addMetaData('entityreference_selection_handler', $this);


    return $query;
  }
}
