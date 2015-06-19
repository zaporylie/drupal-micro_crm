<?php

/**
 * Field handler to present a product's operations links.
 */
class crm_channel_handler_field_channel_operations extends views_handler_field {

  function construct() {
    parent::construct();

    $this->additional_fields['channel_id'] = 'channel_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['add_destination'] = TRUE;

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['add_destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a destination parameter to operations links so users return to this View on form submission.'),
      '#default_value' => $this->options['add_destination'],
    );
  }

  function render($values) {
    $channel_id = $this->get_value($values, 'channel_id');

    // Get the operations links.
    $links = menu_contextual_links('crm-channel', 'admin/crm/channel', array($channel_id));

    if (!empty($links)) {
      // Add the destination to the links if specified.
      if ($this->options['add_destination']) {
        foreach ($links as $id => &$link) {
          $link['query'] = drupal_get_destination();
        }
      }

      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
