<?php

/**
 * @file
 * Contains the basic channel field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a message.
 */
class crm_message_handler_field_message extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_message'])) {
      $this->additional_fields['message_id'] = 'message_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_message'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to message option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_message'] = array(
      '#title' => t("Link this field to the message's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_message']),
    );
  }

  /**
   * Render whatever the data is as a link to the message.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_message']) && $data !== NULL && $data !== '') {
      $message_id = $this->get_value($values, 'message_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/people/crm/message/' . $message_id;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
