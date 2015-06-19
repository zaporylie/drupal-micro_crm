<?php

/**
 * Field handler to present a link to delete a contact.
 */
class crm_contact_handler_field_contact_link_delete extends crm_contact_handler_field_contact_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    // Ensure the user has access to delete this contact.
    $contact = entity_create('crm_contact', array());
    $contact->contact_id = $this->get_value($values, 'contact_id');

    if (!crm_contact_access('delete', $contact)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');

    return l($text, 'admin/crm/contact/' . $contact->contact_id . '/delete', array('query' => drupal_get_destination()));
  }
}
