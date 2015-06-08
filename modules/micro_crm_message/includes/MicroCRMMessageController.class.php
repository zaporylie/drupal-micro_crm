<?php

/**
 * @file
 * Defines contact message entity controller.
 */

/**
 * Class MicroCRMMessageController
 */
class MicroCRMMessageController extends EntityAPIController {

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
   * Overrides EntityAPIController::buildContent().
   */
  public function buildContent($entity, $view_mode = 'customer', $langcode = NULL) {
    return parent::buildContent($entity, $view_mode, $langcode);
  }

  /**
   * Overrides EntityAPIController::attachLoad().
   */
  public function attachLoad(&$queried_entities, $revision_id = FALSE) {
    parent::attachLoad($queried_entities, $revision_id);
  }

  /**
   * Overrides EntityAPIController::load().
   */
  public function load($ids = array(), $conditions = array()) {
    return parent::load($ids, $conditions);
  }

  /**
   * Overrides EntityAPIController::access().
   */
  public function access($op, $entity = NULL, $account = NULL) {
    if ($op !== 'create' && !$entity) {
      return FALSE;
    }
    if ($op == 'send' && $entity->status == MICRO_CRM_MESSAGE_STATUS_SENT) {
      return FALSE;
    }
    // The administer permission is a blanket override.
    if (user_access('micro crm bypass access')) {
      return TRUE;
    }
    switch ($op) {
      case 'create':
        return user_access('micro crm message create');
      case 'send':
        return user_access('micro crm message send');
      case 'view':
        return user_access('micro crm message view');
      case 'update':
        return user_access('micro crm message update');
      case 'delete':
        return user_access('micro crm message delete');
    }
    return FALSE;
  }

  /**
   * Overrides EntityAPIController::view().
   */
  public function view($entities, $view_mode = 'summary', $langcode = NULL, $page = NNULL) {
    return parent::view($entities, $view_mode, $langcode, $page);
  }

  /**
   * Overrides EntityAPIController::create().
   */
  public function create(array $values = array()) {
    $values += array(
      'status' => MICRO_CRM_MESSAGE_STATUS_REGISTERED,
      'language' => LANGUAGE_NONE,
    );
    return parent::create($values);
  }

  /**
   * Overrides EntityAPIController::save().
   */
  public function save($entity) {

    // If we are going to save new message data needs to be validated against
    // uniqueness.
    if (isset($entity->is_new) && $entity->is_new == TRUE && !isset($entity->order_id)) {
      $entity->created = REQUEST_TIME;
    }
    // Set changed time arbitrary.
    $entity->changed = REQUEST_TIME;

    // Create new revision by default.
    if (!isset($entity->revision)) {
      $entity->revision = TRUE;
    }

    // Save message.
    return parent::save($entity);
  }

  /**
   * Overrides DrupalDefaultEntityController::saveRevision().
   */
  protected function saveRevision($entity) {
    $entity->revision_timestamp = REQUEST_TIME;
    $entity->revision_uid = $GLOBALS['user']->uid;
    return parent::saveRevision($entity);
  }

  /**
   * Overrides DrupalDefaultEntityController::delete().
   */
  public function delete($ids) {
    return parent::delete($ids);
  }
}