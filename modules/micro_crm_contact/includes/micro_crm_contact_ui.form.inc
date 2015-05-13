<?php

/**
 * @file
 * Default entity form.
 */

function micro_crm_contact_ui_form_wrapper($contact = array()) {
  if (empty($contact))  {
    $contact = entity_create('micro_crm_contact', array());
  }
  return drupal_get_form('micro_crm_contact_ui_form', $contact);
}

/**
 * Contact form.
 */
function micro_crm_contact_ui_form($form, &$form_state, $contact) {

  $form['contact'] = array(
    '#type' => 'value',
    '#value' => $contact,
  );

  field_attach_form('micro_crm_contact', $contact, $form, $form_state);

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
    '#options' => micro_crm_contact_status_options_list(),
  );

  $form['settings']['options']['language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#required' => TRUE,
    '#default_value' => $contact->language,
    '#options' => array(LANGUAGE_NONE => t('Undefined')) + micro_crm_language_list(),
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
function micro_crm_contact_ui_form_validate($form, &$form_state) {
  field_attach_form_validate('micro_crm_contact', $form_state['values']['contact'], $form, $form_state);
}

function micro_crm_contact_ui_form_submit($form, $form_state) {
  $contact = $form_state['values']['contact'];

  $contact->status = $form_state['values']['status'];
  $contact->language = $form_state['values']['language'];
  $contact->log = $form_state['values']['log'];
  if (!isset($contact->revision)) {
    $contact->revision = TRUE;
  }
  field_attach_submit('micro_crm_contact', $contact, $form, $form_state);
  entity_save('micro_crm_contact', $contact);

  $uri = entity_uri('micro_crm_contact', $contact);
  $form_state['redirect'] = $uri['path'];
  drupal_set_message(t('Contact has been saved'));
}
