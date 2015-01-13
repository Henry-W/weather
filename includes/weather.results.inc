<?php

function weather_results_show($cache_id) {

  if (variable_get('weather_api_options_cache_id_' . $cache_id, 'null') !== 'null') {
    // Show results for legitimate cache

    // Get response
    $response = drupal_json_decode(variable_get('weather_api_options_cache_id_' . $cache_id));

    krumo($response);

    $cc_table = array(
      'header' => array(t('Name'), t('Value from Resultset')),
      'rows' => array(
        // Simple row
        array(
          'Description',
          $response['data']['current_condition'][0]['weatherDesc'][0]['value']
        ),
        array(
          'Icon',
          theme_image(array(
            'path' => $response['data']['current_condition'][0]['weatherIconUrl'][0]['value'],
            'attributes' => NULL
          )),
        ),
        array(
          'Temperature (C)',
          $response['data']['current_condition'][0]['FeelsLikeC']
        ),
        array(
          'Observation Time',
          $response['data']['current_condition'][0]['observation_time']
        )
      ),
      'attributes' => array(),
      'caption' => t('Current Conditions'),
      'colgroups' => array(),
      'sticky' => FALSE,
      'empty' => '',
    );

    $rows = array();

    // Simple row
    for ($i = 0; $i < variable_get('weather_api_options_cache_id_num_of_days_' . $cache_id); $i++) {
      array_push($rows,
        array(
          date("d/m/y", strtotime($response['data']['weather'][$i]['date'])),
          $response['data']['weather'][$i]['mintempC'],
          $response['data']['weather'][$i]['maxtempC']
        ));
    }

    $forecast_table = array(
      'header' => array(t('Date'), t('Min temp (C)'), t('Max temp (C)')),
      'rows' => $rows,
      'attributes' => array(),
      'caption' => t('Forecast from (' . date("d/m/y", strtotime($response['data']['weather'][0]['date'])) . ') to (' . date("d/m/y", strtotime($response['data']['weather'][variable_get('weather_api_options_cache_id_num_of_days_' . $cache_id) - 1]['date'])) . ')'),
      'colgroups' => array(),
      'sticky' => FALSE,
      'empty' => '',
    );

    $content['weather_current_conditions_table'] = array(
      '#type' => 'markup',
      '#markup' => theme_table($cc_table),
    );

    $content['weather_forecast_table'] = array(
      '#type' => 'markup',
      '#markup' => theme_table($forecast_table),
    );

    return $content;

  }
  else {
    // Redirect 404
    drupal_not_found();
  }
}