<?php

/**
 * @file
 *
 */

class MicroCRMChannelInlineFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('Channel'),
      'plural' => t('Channels'),
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

    $fields['type'] = array(
      'type' => 'property',
      'label' => t('Type'),
      'weight' => 99,
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

    $entity_form['log'] = array(
      '#type' => 'value',
      '#value' => '',
    );
    $entity_form['status'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#weight' => 99,
      '#default_value' => $entity->status,
      '#required' => TRUE,
      '#options' => micro_crm_channel_status_option_list(),
    );
    return parent::entityForm($entity_form, $form_state);
  }
}