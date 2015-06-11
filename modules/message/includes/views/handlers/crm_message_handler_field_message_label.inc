<?php

/**
 * Field handler to present a link to a message.
 *
 * @todo: load all needed fields and build label from it.
 */
class crm_message_handler_field_message_label extends views_handler_field {

  function construct() {
    parent::construct();

    $this->additional_fields['message_id'] = 'message_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $message_id = $this->get_value($values, 'message_id');
    return entity_label('crm_message', crm_message_load($message_id));
  }
}
