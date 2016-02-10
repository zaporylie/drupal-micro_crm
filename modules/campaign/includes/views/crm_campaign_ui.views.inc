<?php

/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_default_views_alter().
 */
function crm_campaign_ui_views_default_views_alter(&$views) {
  if (isset($views['crm_channel'])) {
    $handler =& $views['crm_channel']->display['default']->handler;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations']['action::crm_campaign_channel_send_message']['skip_confirmation'] = 1;
  }
  if (isset($views['crm_contact'])) {
    $handler =& $views['crm_contact']->display['default']->handler;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations']['action::crm_campaign_contact_send_message']['skip_confirmation'] = 1;
  }
}