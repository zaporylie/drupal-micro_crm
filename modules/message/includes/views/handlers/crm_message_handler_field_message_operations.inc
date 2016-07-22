<?php

/**
 * Field handler to present a operations links.
 */
class crm_message_handler_field_message_operations extends crm_ui_handler_operations {

  function render($values) {
    return parent::render_element($values, 'crm_message', 'admin/crm/message', 'crm_message_ui');
  }
}
