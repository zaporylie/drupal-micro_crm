<?php

/**
 * @file
 * Defines contact entity controller.
 */

class CRMContactController extends EntityAPIController {

  /**
   * Overrides EntityAPIController::buildQuery().
   */
  public function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    // Add an alias to this query to ensure that we can tell if this is
    // the current revision or not.
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    $query->addField('base', $this->revisionKey, 'current_revision_id');
    return $query;
  }

  /**
   * Overrides EntityAPIController::attachLoad().
   */
  public function attachLoad(&$queried_entities, $revision_id = FALSE) {
    foreach ($queried_entities as $id => &$entity) {
      $entity->data = unserialize($entity->data);
    }

    parent::attachLoad($queried_entities, $revision_id);
  }

  /**
   * Overrides EntityAPIController::access().
   */
  public function access($op, $entity = NULL, $account = NULL) {
    if ($op !== 'create' && !$entity) {
      return FALSE;
    }
    // The administer permission is a blanket override.
    if (user_access('crm bypass access')) {
      return TRUE;
    }
    switch ($op) {
      case 'create':
        return user_access('crm contact create');
      case 'view':
        return user_access('crm contact view');
      case 'update':
        return user_access('crm contact update');
      case 'delete':
        return user_access('crm contact delete');
    }
    return FALSE;
  }

  /**
   * Overrides EntityAPIController::create().
   */
  public function create(array $values = array()) {
    $values += array(
      'status' => CRM_CONTACT_STATUS_REGISTERED,
      'language' => LANGUAGE_NONE,
    );
    return parent::create($values);
  }

  /**
   * Overrides EntityAPIController::save().
   */
  public function save($entity) {
    if (isset($entity->is_new) && $entity->is_new == TRUE && !isset($entity->client_id)) {
      $entity->created = REQUEST_TIME;
    }
    if (!isset($entity->language)) {
      $entity->language = LANGUAGE_NONE;
    }
    $entity->changed = REQUEST_TIME;

    $return = parent::save($entity);
    if (isset($entity->debug) && $entity->debug == TRUE) {
      $e = new Exception();
      watchdog('crm_contact', 'Contact (contact: @id) has been saved. Trace: <pre>@debug</pre>', array('@id' => $entity->contact_id, '@debug' => $e->getTraceAsString()), WATCHDOG_NOTICE);
    }
    else {
      watchdog('crm_contact', 'Contact (contact: @id) has been saved.', array('@id' => $entity->contact_id), WATCHDOG_NOTICE);
    }
    return $return;
  }

  /**
   * Overrides DrupalDefaultEntityController::saveRevision().
   */
  protected function saveRevision($entity) {
    $entity->revision_timestamp = REQUEST_TIME;
    $entity->revision_uid = $GLOBALS['user']->uid;
    return parent::saveRevision($entity);
  }

}