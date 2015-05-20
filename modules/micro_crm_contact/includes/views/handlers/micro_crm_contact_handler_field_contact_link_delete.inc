<?php

/**
 * Field handler to present a link to delete a contact.
 */
class micro_crm_contact_handler_field_contact_link_delete extends micro_crm_contact_handler_field_contact_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    // Ensure the user has access to delete this contact.
    $contact = entity_create('micro_crm_contact', array());
    $contact->contact_id = $this->get_value($values, 'contact_id');

    if (!micro_crm_contact_access('delete', $contact)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');

    return l($text, 'admin/people/micro_crm/contact/' . $contact->contact_id . '/delete', array('query' => drupal_get_destination()));
  }
}
