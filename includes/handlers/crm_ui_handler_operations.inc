<?php

/**
 * Displays operations in a ctools dropbutton.
 *
 * @ingroup views_field_handlers
 */
class crm_ui_handler_operations extends views_handler_field_entity {

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

  /**
   * @param $values
   * @return string
   * // i.e. return render_element($values, 'crm_channel', 'admin/crm/channel', 'crm_channel_ui');
   */
  function render($values) {
    return '';
  }

  protected function render_element($values, $entity_type, $menu_parent_path, $module) {
    $info = entity_get_info($entity_type);
    $entity_id = $this->get_value($values, $info['entity keys']['id']);
    $menu_item = 'admin/crm/' . $entity_type . '/' . $entity_id . '/' . $this->view->current_display;
    $links = menu_contextual_links($module, $menu_parent_path, array($entity_id));
    if ($this->view->style_plugin->plugin_name == 'table_megarows') {
      // Unset default view menu item.
      $links = array_filter($links, function ($link) use ($entity_id, $menu_parent_path) {
        return $link['href'] !== $menu_parent_path . '/' . $entity_id . '/view';
      });

      // Add the quick-edit link which opens the megarow.
      $quick_edit['quick-edit'] = menu_get_item($menu_item);
      $quick_edit['quick-edit']['title'] = $quick_edit['quick-edit']['title'];
      $quick_edit['quick-edit']['attributes']['class'] = 'views-megarow-open';
      $links = array_merge($quick_edit, $links);
    }

    // Add the destination to the links if specified.
    if ($this->options['add_destination']) {
      foreach ($links as $entity_id => &$link) {
        $link['query'] = drupal_get_destination();
      }
    }

    $element = array(
      '#markup' => theme(
        'links__ctools_dropbutton',
        array(
          'links' => $links,
          'attributes' => array(
            'class' => array(
              'links',
              'inline',
            ),
          ),
        )
      ),
    );

    return $element;
  }
}
