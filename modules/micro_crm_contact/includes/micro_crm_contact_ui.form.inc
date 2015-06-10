<?php

/**
 * @file
 * Default entity form.
 */

/**
 * Form wrapper for create, update and delete.
 *
 * @param array $contact
 *   Contact object or array of default values.
 * @param bool $delete
 *   Boolean indicator if create/edit or delete form should be displayed.
 *
 * @return array
 */
function crm_contact_ui_form_wrapper($contact = array(), $delete = FALSE) {
  if ($delete == TRUE && isset($contact->contact_id)) {
    return drupal_get_form('crm_contact_ui_delete_form', $contact);
  }
  if (empty($contact))  {
    $contact = entity_create('crm_contact', array());
  }
  return drupal_get_form('crm_contact_ui_form', $contact);
}

/**
 * Contact form.
 */
function crm_contact_ui_form($form, &$form_state, $contact) {

  $form['contact'] = array(
    '#type' => 'value',
    '#value' => $contact,
  );

  field_attach_form('crm_contact', $contact, $form, $form_state);

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
  $form['settings']['options']['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#required' => TRUE,
    '#default_value' => $contact->status,
    '#options' => crm_contact_status_options_list(),
  );

  $form['settings']['options']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#required' => TRUE,
    '#default_value' => $contact->language,
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
 * Contact form validation
 */
function crm_contact_ui_form_validate($form, &$form_state) {
  field_attach_form_validate('crm_contact', $form_state['values']['contact'], $form, $form_state);
}

/**
 * Contact form submit callback.
 */
function crm_contact_ui_form_submit($form, &$form_state) {
  $contact = $form_state['values']['contact'];

  $contact->status = $form_state['values']['status'];
  $contact->language = $form_state['values']['language'];
  $contact->log = $form_state['values']['log'];
  if (!isset($contact->revision)) {
    $contact->revision = TRUE;
  }
  try {
    field_attach_submit('crm_contact', $contact, $form, $form_state);
    entity_save('crm_contact', $contact);
    $uri = entity_uri('crm_contact', $contact);
    $form_state['redirect'] = $uri['path'];
    drupal_set_message(t('Contact has been saved'));
  }
  catch (Exception $e) {
    watchdog_exception('crm_channel', $e);
    $form_state['rebuild'] = TRUE;
    drupal_set_message(t('Unable to save contact'), 'error');
  }

}

/**
 * Delete confirmation form.
 */
function crm_contact_ui_delete_form($form, &$form_state, $contact) {
  $form['contact'] = array(
    '#type' => 'value',
    '#value' => $contact,
  );
  return confirm_form(
    $form,
    t('Do you want to delete contact?'),
    'admin/people/crm/contact'
  );
}

/**
 * Delete form submit handler.
 */
function crm_contact_ui_delete_form_submit($form, &$form_state) {
  $contact_id = entity_id('crm_contact', $form_state['values']['contact']);
  if (entity_delete('crm_contact', $contact_id) !== FALSE) {
    drupal_set_message(t('Contact @contact has been deleted', array('@contact' => $contact_id)));
  }
}
