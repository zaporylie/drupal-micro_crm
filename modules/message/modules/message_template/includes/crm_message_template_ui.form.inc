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
      'data' => l($name, 'admin/people/crm/message/template/add/' . $type_arg),
    );
  }

  $build['list'] = array(
    '#theme' => 'item_list',
    '#items' => $items,
  );
  return $build;
}

/**
 * Form wrapper for create, update and delete.
 *
 * @param array $template
 *   Message object or array of default values.
 * @param bool $delete
 *   Boolean indicator if create/edit or delete form should be displayed.
 *
 * @return array
 */
function crm_message_template_ui_form_wrapper($template = array(), $delete = FALSE, $type = NULL) {
  if ($delete == TRUE && isset($template->template_id)) {
    return drupal_get_form('crm_message_template_ui_delete_form', $template);
  }
  if (empty($template))  {
    $template = entity_create('crm_message_template', array(
      'type' => $type,
    ));
  }
  return drupal_get_form('crm_message_template_ui_form', $template);
}

/**
 * Message form.
 */
function crm_message_template_ui_form($form, &$form_state, $template) {

  $form['message_template'] = array(
    '#type' => 'value',
    '#value' => $template,
  );

  field_attach_form('crm_message_template', $template, $form, $form_state);

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 98,
  );
  $form['settings']['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['options']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#required' => TRUE,
    '#default_value' => $template->language,
    '#options' => array(LANGUAGE_NONE => t('Undefined')) + crm_language_list(),
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
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Message form validation
 */
function crm_message_template_ui_form_validate($form, &$form_state) {
  field_attach_form_validate('crm_message_template', $form_state['values']['message_template'], $form, $form_state);
}

/**
 * Message form submit callback.
 */
function crm_message_template_ui_form_submit($form, &$form_state) {
  $template = $form_state['values']['message_template'];

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
    'admin/people/crm/message/template'
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
