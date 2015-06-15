<?php

/**
 * Filter by product type.
 */
class crm_message_template_handler_filter_message_template_type extends views_handler_filter_in_operator {
  // Display a list of product types in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Message template type');
      $this->value_options = crm_message_template_types();
    }
  }
}
