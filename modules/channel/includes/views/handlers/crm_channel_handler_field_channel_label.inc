<?php

/**
 * Field handler to present a link to a channel.
 *
 * @todo: load all needed fields and build label from it.
 */
class crm_channel_handler_field_channel_label extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['channel_id'] = 'channel_id';
  }

  function option_definition() {
    return parent::option_definition();
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $channel_id = $this->get_value($values, 'channel_id');
    return entity_label('crm_channel', crm_channel_load($channel_id));
  }
}
