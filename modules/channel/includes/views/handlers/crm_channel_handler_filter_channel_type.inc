<?php

/**
 * Filter by product type.
 */
class crm_channel_handler_filter_channel_type extends views_handler_filter_in_operator {
  // Display a list of product types in the filter's options.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Channel type');
      $this->value_options = crm_channel_type_option_list();
    }
  }
}
