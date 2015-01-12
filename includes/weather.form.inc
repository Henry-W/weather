<?php

/*
 * Implements hook_form().
 */
function weather_submission_form() {
  return drupal_get_form('weather_submission_form_func');
}

function weather_submission_form_func($form, &$form_state) {
  $form = array();

  $form['weather_town_city'] = array(
    '#type' => 'textfield',
    '#title' => t('Town/City'),
    '#required' => TRUE,
    '#default_value' => 'Vauxhall',
  );

  $weather_date_options = array(
    0 => t('Today'),
    1 => t('Tomorrow'),
    2 => t('Enter Date')
  );

  $form['weather_date'] = array(
    '#type' => 'radios',
    '#title' => t('Date'),
    '#options' => $weather_date_options,
    '#default_value' => 0,
    '#required' => TRUE,
  );

  $form['weather_date_custom'] = array(
    '#type' => 'date',
    '#title' => t('Choose Date'),
  );

  $form['weather_days_forecast'] = array(
    '#type' => 'textfield',
    '#title' => t('Days to forecast'),
    '#required' => TRUE,
    '#default_value' => '5',
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['weather_hourly_reporting'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => t('1'),
      1 => t('3'),
      2 => t('6'),
      3 => t('12'),
      4 => t('24'),
    ),
    '#default_value' => 1,
    '#title' => t('Hourly Interval of Reporting'),
    '#required' => TRUE,
  );

  $form['weather_current_conditions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show current conditions'),
    '#default_value' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'weather') . '/css/weather.css',
  );

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'weather') . '/js/jquery-2.1.3.min.js',
    drupal_get_path('module', 'weather') . '/js/weather.js',
  );

  return $form;
}

function weather_submission_form_func_submit() {
//  $data = ''; // Parameters to pass to URL
//  $options = array(
//    'method' => 'POST',
//    'data' => $data, // Parameters
//    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
//  );
//  $weather_data = drupal_http_request('?q=London&format=json&key=ade5f8b9d77a3eb016b7515df6464&num_of_days=5');

  $data = array(
    'q' => 'London',
    'format' => 'json',
    'key' => 'ade5f8b9d77a3eb016b7515df6464',
    'num_of_days' => '5',
  );

  $options = array(
    'method' => 'POST',
    'data' => drupal_http_build_query($data),
    'timeout' => 15000,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );

  $weather_data = drupal_http_request('http://api.worldweatheronline.com/free/v2/weather.ashx', $options);

  dpm($weather_data);
}