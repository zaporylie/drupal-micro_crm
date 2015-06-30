<?php

/**
 * @file
 * Default entity form.
 */

/**
 * @return array
 */
function crm_message_template_ui_add() {
  $build = array();
  $items = array();

  foreach (crm_message_template_types() as $type => $name) {
    $type_arg = strtr($type, '_', '-');
    $items[] = array(
      'data' => l($name, 'admin/crm/message/template/add/' . $type_arg),
    );
  }

  $build['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
  return $build;
}

/**
 * Form wrapper for entity addition.
 *
 * @param null $type
 *
 * @return array|mixed
 */
function crm_message_template_ui_add_form_wrapper($type) {
  $template = entity_create('crm_message_template', array(
    'type' => $type,
  ));
  return drupal_get_form('crm_message_template_ui_form', $template);
}

/**
 * Form wrapper for entity deletion.
 *
 * @param null $type
 *
 * @return array|mixed
 */
function crm_message_template_ui_delete_form_wrapper($template) {
  return drupal_get_form('crm_message_template_ui_delete_form', $template);
}

/**
 * Form wrapper for entity update.
 *
 * @param array $template
 *   Message object or array of default values.
 *
 * @return array
 */
function crm_message_template_ui_form_wrapper($template) {
  return drupal_get_form('crm_message_template_ui_form', $template);
}

/**
 * Message form.
 */
function crm_message_template_ui_form($form, &$form_state, $template) {

  $form_state['crm_message_template'] = $template;

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Template name'),
    '#required' => TRUE,
    '#default_value' => isset($template->name) ? $template->name : NULL,
    '#disabled' => $template->locked ? TRUE : FALSE,
    '#machine_name' => array(
      'exists' => 'crm_message_template_menu_name_exists',
    ),
  );

  $langcode = entity_language('crm_message_template', $template);

  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  field_attach_form('crm_message_template', $template, $form, $form_state, $langcode);

  $form['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('crm_channel', 'crm_contact'),
    '#weight' => 97,
  );

  $form['language'] = array(
    '#type' => 'value',
    '#value' => $langcode,
  );

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 98,
  );
  $form['settings']['revision'] = array(
    '#type' => 'fieldset',
    '#title' => t('Revision'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['revision']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Revision log'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Message form validation
 */
function crm_message_template_ui_form_validate($form, &$form_state) {
  field_attach_form_validate('crm_message_template', $form_state['crm_message_template'], $form, $form_state);
}

/**
 * Message form submit callback.
 */
function crm_message_template_ui_form_submit($form, &$form_state) {
  $template = $form_state['crm_message_template'];

  $template->name = $form_state['values']['name'];
  $template->language = $form_state['values']['language'];
  $template->log = $form_state['values']['log'];
  if (!isset($template->revision)) {
    $template->revision = TRUE;
  }
  try {
    field_attach_submit('crm_message_template', $template, $form, $form_state);
    entity_save('crm_message_template', $template);
    $uri = entity_uri('crm_message_template', $template);
    $form_state['redirect'] = $uri['path'];
    drupal_set_message(t('Message template has been saved'));
  }
  catch (Exception $e) {
    watchdog_exception('crm_message_template', $e);
    $form_state['rebuild'] = TRUE;
    drupal_set_message(t('Unable to save message template'), 'error');
  }

}

/**
 * Delete confirmation form.
 */
function crm_message_template_ui_delete_form($form, &$form_state, $template) {
  $form['message_template'] = array(
    '#type' => 'value',
    '#value' => $template,
  );
  return confirm_form(
    $form,
    t('Do you want to delete message template?'),
    'admin/crm/message/template'
  );
}

/**
 * Delete form submit handler.
 */
function crm_message_template_ui_delete_form_submit($form, &$form_state) {
  $template_id = entity_id('crm_message_template', $form_state['values']['message_template']);
  if (entity_delete('crm_message_template', $template_id) !== FALSE) {
    drupal_set_message(t('Message template @template has been deleted', array('@template' => $template_id)));
  }
}
