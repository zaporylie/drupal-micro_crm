<?php

/**
 * @file
 *
 */

class CRMMessageTemplateInlineFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Template'),
      'plural' => t('Templates'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::tableFields().
   *
   * Define how to show this entity inside inline entity form.
   */
  public function tableFields($bundles) {
    $fields = parent::tableFields($bundles);

    $fields['name'] = array(
      'type' => 'property',
      'label' => t('Name'),
      'weight' => 2,
    );

    return $fields;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   *
   * Prepare separate form for inline form version.
   */
  public function entityForm($entity_form, &$form_state) {
    $entity = $entity_form['#entity'];

    $entity_form['name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Template name'),
      '#required' => TRUE,
      '#default_value' => isset($entity->name) ? $entity->name : NULL,
      '#disabled' => $entity->locked ? TRUE : FALSE,
      '#machine_name' => array(
        'exists' => 'crm_message_template_menu_name_exists',
      ),
    );
    $entity_form['log'] = array(
      '#type' => 'value',
      '#value' => '',
    );
    return parent::entityForm($entity_form, $form_state);
  }
}