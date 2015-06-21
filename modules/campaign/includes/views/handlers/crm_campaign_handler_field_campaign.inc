<?php

/**
 * @file
 * Contains the basic campaign field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a campaign.
 */
class crm_campaign_handler_field_campaign extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_campaign'])) {
      $this->additional_fields['campaign_id'] = 'campaign_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_campaign'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to campaign option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_campaign'] = array(
      '#title' => t("Link this field to the campaign's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_campaign']),
    );
  }

  /**
   * Render whatever the data is as a link to the campaign.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_campaign']) && $data !== NULL && $data !== '') {
      $campaign_id = $this->get_value($values, 'campaign_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/crm/campaign/' . $campaign_id;
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
