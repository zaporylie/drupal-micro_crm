<?php

/**
 * @file
 * Builds placeholder replacement tokens for contact-related data.
 */

/**
 * Implements hook_token_info().
 */
function crm_contact_token_info() {
  $type = array(
    'name' => t('Contact'),
    'description' => t('Tokens related to individual crm contact items.'),
    'needs-data' => 'crm_contact',
  );

  // Core tokens for nodes.
  $contact['contact_id'] = array(
    'name' => t("Contact ID"),
    'description' => t('The unique ID of the contact.'),
  );
  $contact['revision_id'] = array(
    'name' => t("Revision ID"),
    'description' => t("The unique ID of the contact's latest revision."),
  );
  $contact['status'] = array(
    'name' => t("Status"),
    'description' => t("Contact status."),
  );
  $contact['status-name'] = array(
    'name' => t("Status name"),
    'description' => t("The human-readable status of the contact."),
  );
  $contact['token'] = array(
    'name' => t("Token"),
    'description' => t("One-time confirmation token."),
  );
  $contact['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the contact."),
  );
  $contact['created'] = array(
    'name' => t("Date created"),
    'description' => t("The date the node was posted."),
    'type' => 'date',
  );
  $contact['changed'] = array(
    'name' => t("Date changed"),
    'description' => t("The date the node was most recently updated."),
    'type' => 'date',
  );

  return array(
    'types' => array('crm_contact' => $type),
    'tokens' => array('crm_contact' => $contact),
  );
}

/**
 * Implements hook_tokens().
 */
function crm_contact_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'crm_contact' && !empty($data['crm_contact'])) {
    $contact = $data['crm_contact'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the node.
        case 'contact_id':
          $replacements[$original] = $contact->contact_id;
          break;

        case 'revision_id':
          $replacements[$original] = $contact->revision_id;
          break;

        case 'status':
          $replacements[$original] = $sanitize ? check_plain($contact->status) : $contact->status;
          break;

        case 'status-name':
          $status_name = crm_contact_status_get_name($contact->status);
          $replacements[$original] = $sanitize ? check_plain($status_name) : $status_name;
          break;

        case 'token':
          $replacements[$original] = $contact->token;
          break;

        case 'url':
          $uri = entity_uri('crm_contact', $contact);
          $replacements[$original] = url($uri['path'], $url_options);
          break;

        case 'created':
          $replacements[$original] = format_date($contact->created);
          break;

        case 'changed':
          $replacements[$original] = format_date($contact->changed);
          break;
      }
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $created_tokens, array('date' => $contact->created), $options);
    }

    if ($changed_tokens = token_find_with_prefix($tokens, 'changed')) {
      $replacements += token_generate('date', $changed_tokens, array('date' => $contact->changed), $options);
    }
  }

  return $replacements;
}
