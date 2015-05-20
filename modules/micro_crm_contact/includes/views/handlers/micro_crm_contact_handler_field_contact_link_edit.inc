<?php

/**
 * Field handler to present a product edit link.
 */
class micro_crm_contact_handler_field_contact_link_edit extends micro_crm_contact_handler_field_contact_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    // Ensure the user has access to edit this product.
    $contact = entity_create('micro_crm_contact', array());
    $contact->contact_id = $this->get_value($values, 'contact_id');

    if (!micro_crm_contact_access('update', $contact)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');

    return l($text, 'admin/people/micro_crm/contact/' . $contact->contact_id . '/edit', array('query' => drupal_get_destination()));
  }
}
