<?php

/**
 * Field handler to translate a channel type into its readable form.
 */
class crm_message_template_handler_field_message_template_name extends crm_message_template_handler_field_message_template {

  function render($values) {
    if ($name = $this->get_value($values)) {
      return $this->sanitize_value($name);
    }

    return '';
  }
}
