<?php

/**
 * @file
 * Hooks provided by the Micro CRM Contact Channel module.
 */


function hook_micro_crm_channel_state_info() {

}

function hook_micro_crm_channel_status_info() {

}

function hook_micro_crm_channel_is_unique() {

}

/**
 * Allows you to prepare channel data before it is saved.
 *
 * @param $channel
 *   The channel object to be saved.
 *
 * @see rules_invoke_all()
 */
function hook_micro_crm_channel_presave($channel) {
  // No example.
}

function hook_micro_crm_channel_update($channel) {
  // No example.
}