<?php

/**
 * Field handler to present a product's operations links.
 */
class crm_contact_handler_field_contact_operations extends crm_ui_handler_operations {
  function render($values) {
    return parent::render_element($values, 'crm_contact', 'admin/crm/contact', 'crm_contact_ui');
  }
}
