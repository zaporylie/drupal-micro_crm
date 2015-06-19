<?php

/**
 * Filter by product type.
 */
class crm_contact_handler_filter_contact_status extends views_handler_filter_in_operator {
  // Display a list of product types in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Status type');
      $this->value_options = crm_contact_status_options_list();
    }
  }
}
