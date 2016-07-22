<?php

/**
 * Field handler to present a product's operations links.
 */
class crm_campaign_handler_field_campaign_operations extends crm_ui_handler_operations {
  function render($values) {
    return parent::render_element($values, 'crm_campaign', 'admin/crm/campaign', 'crm_campaign_ui');
  }
}
