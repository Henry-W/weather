<?php

/*
 * Implements hook_admin().
 */
function weather_admin_settings_form($form) {
  if (empty(variable_get('weather_api_url_field')) || empty(variable_get('weather_api_key_field'))) {
    drupal_set_message(t('This module will not function correctly if you have not completed the fields below:'), 'warning');
  }

  $form['weather_api_url_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Weather API URL'),
    '#default_value' => t(variable_get('weather_api_url_field')),
    '#description' => t('Please enter the API URL to use.'),
    '#required' => TRUE,
  );

  $form['weather_api_past_url_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Past Weather API URL'),
    '#default_value' => t(variable_get('weather_api_past_url_field')),
    '#description' => t('Please enter the API URL to use.'),
    '#required' => TRUE,
  );

  $form['weather_api_key_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Weather API Key'),
    '#default_value' => t(variable_get('weather_api_key_field')),
    '#description' => t('Please enter the API Key to use.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}