<?php

/**
 * Page callback: Admin page to configure Main Program Finder Image CTAs
 */
function gsb_program_finder_config_main_image_ctas() {

  $main_image_ctas = variable_get('gsb_program_finder_config_main_image_ctas', array());

  $form['image_cta_one'] = array(
    '#type' => 'entityreference',
    '#title' => 'Image CTA 1',
    '#era_entity_type' => 'fieldable_panels_pane',
    '#era_bundles' => array('image_cta'),
    '#era_cardinality' => 1,
    '#default_value' => empty($main_image_ctas) ? '' : $main_image_ctas[0],
    '#process' => array('ajax_process_form', 'entityreference_autocomplete_process_entityreference'),
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#submit'][] = 'gsb_program_finder_config_main_image_ctas_submit';

  return $form;

}

/**
 * Custom submit callback for the admin config Main Program Finder Image CTAs
 */
function gsb_program_finder_config_main_image_ctas_submit(&$form, &$form_state) {

  $main_image_ctas = array();

  $main_image_ctas[0] = $form_state['values']['image_cta_one']['entity_id'];

  variable_set('gsb_program_finder_config_main_image_ctas', $main_image_ctas);

  drupal_set_message(t('Your configuration of main program finder image ctas has been successfully saved.'), 'status', FALSE);
}

/**
 * Page callback: Admin page to configure Exec Ed Program Finder Image CTAs
 */
function gsb_program_finder_config_ee_image_ctas() {

  $image_ctas = variable_get('gsb_program_finder_config_ee_image_ctas', array());

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#submit'][] = 'gsb_program_finder_config_ee_image_ctas_submit';

  return $form;

}

/**
 * Custom submit callback for the admin config Exec Ed Program Finder Image CTAs
 */
function gsb_program_finder_config_ee_image_ctas_submit(&$form, &$form_state) {

  drupal_set_message(t('Your configuration of exec ed program finder image ctas has been successfully saved.'), 'status', FALSE);
}
