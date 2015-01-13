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
      1 => t('1'),
      3 => t('3'),
      6 => t('6'),
      12 => t('12'),
      24 => t('24'),
    ),
    '#default_value' => 3,
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

function weather_submission_form_func_submit($form, &$form_state) {
  $options = "q=" . $form_state['values']['weather_town_city'];

  $options .= "&format=json";

  $options .= "&key=" . variable_get('weather_api_key_field');

  $options .= "&num_of_days=" . $form_state['values']['weather_days_forecast'];

  $date_selection = $form_state['values']['weather_date'];

  if ($date_selection == 0) {
    // Today
    $options .= "&date=today";
  }
  elseif ($date_selection == 1) {
    $options .= "&date=tomorrow";
  }
  else {
    $options .= "&date=" . $form_state['values']['weather_date_custom']['year'] . "-" . $form_state['values']['weather_date_custom']['month'] . "-" . $form_state['values']['weather_date_custom']['day'];
  }

  $options .= "&tp=" . $form_state['values']['weather_hourly_reporting'];

  if ($form_state['values']['weather_current_conditions']) {
    $options .= "&cc=yes";
  }
  else {
    $options .= "&cc=no";
  }

  $cache_options = md5($options);

  $stored_cache = variable_get('weather_api_options_cache_id_' . $cache_options, 'null');

  if ($stored_cache !== 'null') {

    $response = $stored_cache;

    dpm("USING CACHED RESPONSE");

  }
  else {

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, variable_get('weather_api_url_field') . $options);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_PROXY, '10.23.12.110:8080');
    $response = curl_exec($ch);
    curl_close($ch);

    variable_set('weather_api_options_cache_id_' . $cache_options, $response);

    variable_set('weather_api_options_cache_id_num_of_days_' . $cache_options, $form_state['values']['weather_days_forecast']);

    dpm("USING NEW RESPONSE");

  }

  dpm($options);

  dpm($response);

  drupal_goto($GLOBALS['base_url'] . '/weather/results/' . $cache_options);

}