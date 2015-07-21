<?php

/**
 * @file
 * Admin settings callbacks.
 */

/**
 * Settings form callback.
 */
function crm_user_settings_form($form, $form_state) {

  $form['#tree'] = TRUE;
  $settings = variable_get('crm_user_map', array());

  foreach (crm_channel_types() as $key => $type) {
    $form['map'][$key] = array(
      '#type' => 'fieldset',
      '#title' => $key,
    );
    foreach (_crm_user_get_properties('crm_channel', $key) as $property_key => $property_label) {
      $form['map'][$key][$property_key] = array(
        '#type' => 'select',
        '#title' => $property_label,
        '#default_value' => isset($settings[$key][$property_key]) ? $settings[$key][$property_key] : NULL,
        '#options' => array(NULL => t('-- Choose --')) + _crm_user_get_properties(),
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler.
 */
function crm_user_settings_form_submit($form, $form_state) {
  variable_set('crm_user_map', $form_state['values']['map']);
}

/**
 * Make option list out of entity properties.
 *
 * @param bool|FALSE $flatten
 *   Should it be flatten or not.
 *
 * @return array
 *   One or two level array of key, value pair of options.
 */
function _crm_user_get_properties($entity_type = 'user', $bundle = 'user') {
  $array = array();

  // Get properties.
  $properties = entity_get_all_property_info($entity_type);
  $info = entity_get_property_info($entity_type);
  $properties = $info['properties'];
  if (isset($info['bundles'][$bundle])) {
    $properties += $info['bundles'][$bundle]['properties'];
  }

  // Build select list.
  foreach ($properties as $key => $property) {
    $type = isset($property['type']) ? entity_property_extract_innermost_type($property['type']) : 'text';
    $is_entity = ($type == 'entity') || (bool) entity_get_info($type);
    // Leave entities out of this.
    if (!$is_entity) {
      if (isset($property['field']) && $property['field'] && !empty($property['property info'])) {
        foreach ($property['property info'] as $sub_key => $sub_prop) {
          $array[$key . '|' . $sub_key] = $property['label'] . ' - ' . $sub_prop['label'];
        }
      }
      else {
        $array[$key] = $property['label'];
      }
    }
  }
  return $array;
}