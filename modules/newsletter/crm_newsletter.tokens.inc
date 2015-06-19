<?php

/**
 * @file
 */

/**
 * Implements hook_token_info_alter().
 */
function crm_newsletter_token_info_alter(&$data) {

  $data['tokens']['crm_channel']['subscribe-url'] = array(
    'name' => t("Subscribe url"),
    'description' => t("Displays double opt newsletter subscription link with one-time token."),
  );
  $data['tokens']['crm_channel']['unsubscribe-url'] = array(
    'name' => t("Unubscribe url"),
    'description' => t("Displays double opt newsletter cancellation with one-time token."),
  );
}

/**
 * Implements hook_tokens().
 */
function crm_newsletter_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  if ($type == 'crm_channel' && !empty($data['crm_channel'])) {
    $url_options = array(
      'absolute' => TRUE,
    );
    $channel = $data['crm_channel'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the node.
        case 'subscribe-url':
          $replacements[$original] = url('crm/newsletter/subscribe/' . $channel->token, $url_options);
          break;
        case 'unsubscribe-url':
          $replacements[$original] = url('crm/newsletter/unsubscribe/' . $channel->token, $url_options);
          break;
      }
    }
  }
  return $replacements;
}
