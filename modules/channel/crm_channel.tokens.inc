<?php

/**
 * @file
 * Builds placeholder replacement tokens for channel-related data.
 */

/**
 * Implements hook_token_info().
 */
function crm_channel_token_info() {
  $type = array(
    'name' => t('Channel'),
    'description' => t('Tokens related to individual crm channel items.'),
    'needs-data' => 'crm_channel',
  );

  // Core tokens for nodes.
  $channel['channel_id'] = array(
    'name' => t("Channel ID"),
    'description' => t('The unique ID of the channel.'),
  );
  $channel['revision_id'] = array(
    'name' => t("Revision ID"),
    'description' => t("The unique ID of the channel's latest revision."),
  );
  $channel['type'] = array(
    'name' => t("Channel type"),
    'description' => t("The type of the node."),
  );
  $channel['type-name'] = array(
    'name' => t("Channel type name"),
    'description' => t("The human-readable name of the channel type."),
  );
  $channel['status'] = array(
    'name' => t("Status"),
    'description' => t("Channel status."),
  );
  $channel['status-name'] = array(
    'name' => t("Status name"),
    'description' => t("The human-readable status of the channel."),
  );
  $channel['token'] = array(
    'name' => t("Token"),
    'description' => t("One-time confirmation token."),
  );
  $channel['url'] = array(
    'name' => t("URL"),
    'description' => t("The URL of the channel."),
  );
  $channel['created'] = array(
    'name' => t("Date created"),
    'description' => t("The date the node was posted."),
    'type' => 'date',
  );
  $channel['changed'] = array(
    'name' => t("Date changed"),
    'description' => t("The date the node was most recently updated."),
    'type' => 'date',
  );

  return array(
    'types' => array('crm_channel' => $type),
    'tokens' => array('crm_channel' => $channel),
  );
}

/**
 * Implements hook_tokens().
 */
function crm_channel_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'crm_channel' && !empty($data['crm_channel'])) {
    $channel = $data['crm_channel'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the node.
        case 'channel_id':
          $replacements[$original] = $channel->channel_id;
          break;

        case 'revision_id':
          $replacements[$original] = $channel->revision_id;
          break;

        case 'type':
          $replacements[$original] = $sanitize ? check_plain($channel->type) : $channel->type;
          break;

        case 'type-name':
          $type_name = crm_channel_type_get_name($channel->type);
          $replacements[$original] = $sanitize ? check_plain($type_name) : $type_name;
          break;

        case 'status':
          $replacements[$original] = $sanitize ? check_plain($channel->status) : $channel->status;
          break;

        case 'status-name':
          $status_name = crm_channel_status_get_name($channel->status);
          $replacements[$original] = $sanitize ? check_plain($status_name) : $status_name;
          break;

        case 'token':
          $replacements[$original] = $channel->token;
          break;

        case 'url':
          $uri = entity_uri('crm_channel', $channel);
          $replacements[$original] = url($uri['path'], $url_options);
          break;

        case 'created':
          $replacements[$original] = format_date($channel->created);
          break;

        case 'changed':
          $replacements[$original] = format_date($channel->changed);
          break;
      }
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $created_tokens, array('date' => $channel->created), $options);
    }

    if ($changed_tokens = token_find_with_prefix($tokens, 'changed')) {
      $replacements += token_generate('date', $changed_tokens, array('date' => $channel->changed), $options);
    }
  }

  return $replacements;
}
