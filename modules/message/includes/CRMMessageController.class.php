<?php

/**
 * @file
 * Defines contact message entity controller.
 */

/**
 * Class CRMMessageController
 */
class CRMMessageController extends EntityAPIController {

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
   * Overrides EntityAPIController::access().
   */
  public function access($op, $entity = NULL, $account = NULL) {
    if ($op !== 'create' && !$entity) {
      return CRM_ACCESS_IGNORE;
    }
    if (!in_array($op, array('create', 'view')) && $entity->status === CRM_MESSAGE_STATUS_FAILED) {
      return CRM_ACCESS_IGNORE;
    }
    if (!in_array($op, array('create', 'view', 'resend')) && $entity->status == CRM_MESSAGE_STATUS_SENT) {
      return CRM_ACCESS_IGNORE;
    }
    if (!in_array($op, array('create', 'view', 'cancel')) && $entity->status == CRM_MESSAGE_STATUS_QUEUED) {
      return CRM_ACCESS_IGNORE;
    }
    if (in_array($op, array('cancel')) && $entity->status !== CRM_MESSAGE_STATUS_QUEUED) {
      return CRM_ACCESS_IGNORE;
    }
    // The administer permission is a blanket override.
    if (user_access('crm bypass access')) {
      return CRM_ACCESS_ALLOW;
    }
    switch ($op) {
      case 'create':
        return user_access('crm message create') ? CRM_ACCESS_ALLOW : CRM_ACCESS_IGNORE;
      case 'send':
      case 'resend':
        return user_access('crm message send') ? CRM_ACCESS_ALLOW : CRM_ACCESS_IGNORE;
      case 'cancel':
        return user_access('crm message cancel') ? CRM_ACCESS_ALLOW : CRM_ACCESS_IGNORE;
      case 'view':
        return user_access('crm message view') ? CRM_ACCESS_ALLOW : CRM_ACCESS_IGNORE;
      case 'update':
        return user_access('crm message update') ? CRM_ACCESS_ALLOW : CRM_ACCESS_IGNORE;
      case 'delete':
        return user_access('crm message delete') ? CRM_ACCESS_ALLOW : CRM_ACCESS_IGNORE;
    }
  }

  /**
   * Overrides EntityAPIController::create().
   */
  public function create(array $values = array()) {
    $values += array(
      'status' => CRM_MESSAGE_STATUS_DRAFT,
      'language' => LANGUAGE_NONE,
    );
    return parent::create($values);
  }

  /**
   * Overrides EntityAPIController::save().
   */
  public function save($entity) {

    if (!empty($entity->is_new)) {
      if (empty($entity->created)) {
        $entity->created = REQUEST_TIME;
      }
      if (empty($entity->modified)) {
        $entity->modified = REQUEST_TIME;
      }
    }
    else {
      $entity->modified = REQUEST_TIME;
    }

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

}
