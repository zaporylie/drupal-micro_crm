<?php

/**
 * @file
 * Contains the basic contact field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a contact.
 */
class crm_contact_handler_field_contact extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_contact'])) {
      $this->additional_fields['contact_id'] = 'contact_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_contact'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to contact option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_contact'] = array(
      '#title' => t("Link this field to the contact's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_contact']),
    );
  }

  /**
   * Render whatever the data is as a link to the contact.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_contact']) && $data !== NULL && $data !== '') {
      $contact_id = $this->get_value($values, 'contact_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/crm/contact/' . $contact_id;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
