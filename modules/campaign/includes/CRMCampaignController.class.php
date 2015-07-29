<?php

/**
 * @file
 * Defines campaign entity controller.
 */

class CRMCampaignController extends EntityAPIController {
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
      return FALSE;
    }
    if (!in_array($op, array('view', 'create')) && $entity->status != CRM_CAMPAIGN_STATUS_REGISTERED) {
      return FALSE;
    }
    // The administer permission is a blanket override.
    if (user_access('crm bypass access')) {
      return TRUE;
    }
    switch ($op) {
      case 'create':
        return user_access('crm campaign create');
      case 'view':
        return user_access('crm campaign view');
      case 'update':
        return user_access('crm campaign update');
      case 'delete':
        return user_access('crm campaign delete');
      case 'send':
        return user_access('crm campaign send');
    }
    return FALSE;
  }

  /**
   * Overrides EntityAPIController::create().
   */
  public function create(array $values = array()) {
    $values += array(
      'status' => CRM_CAMPAIGN_STATUS_REGISTERED,
      'data' => array(
        'manage_recipients' => array(
          'methods' => array(
            'crm_campaign_vbo' => array(),
          ),
        ),
      ),
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
    $entity->changed = REQUEST_TIME;

    $return = parent::save($entity);
    if (isset($entity->debug) && $entity->debug == TRUE) {
      $e = new Exception();
      watchdog('crm_campaign', 'Campaign (campaign: @id) has been saved. Trace: <pre>@debug</pre>', array('@id' => $entity->campaign_id, '@debug' => $e->getTraceAsString()), WATCHDOG_NOTICE);
    }
    else {
      watchdog('crm_campaign', 'Campaign (campaign: @id) has been saved.', array('@id' => $entity->campaign_id), WATCHDOG_NOTICE);
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