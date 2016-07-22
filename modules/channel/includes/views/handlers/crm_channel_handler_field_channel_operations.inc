<?php

/**
 * Field handler to present a product's operations links.
 */
class crm_channel_handler_field_channel_operations extends crm_ui_handler_operations {
  function render($values) {
    return parent::render_element($values, 'crm_channel', 'admin/crm/channel', 'crm_channel_ui');
  }
}
