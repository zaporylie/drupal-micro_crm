<?php

/**
 * @file
 * Contains the basic channel field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a channel.
 */
class crm_channel_handler_field_channel extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_channel'])) {
      $this->additional_fields['channel_id'] = 'channel_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_channel'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to channel option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_channel'] = array(
      '#title' => t("Link this field to the channel's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_channel']),
    );
  }

  /**
   * Render whatever the data is as a link to the channel.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_channel']) && $data !== NULL && $data !== '') {
      $channel_id = $this->get_value($values, 'channel_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/people/crm/channel/' . $channel_id;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
