<?php

function weather_results_show($cache_id) {

  if (!empty(cache_get('weather_api_options_cache_id_' . $cache_id)->data)) {
    // Show results for legitimate cache

    // Get response
    $response = cache_get('weather_api_options_cache_id_' . $cache_id)->data;

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
          theme('image', array(
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
    for ($i = 0; $i < count($response['data']['weather']); $i++) {
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
      'caption' => t('Forecast from (' . date("d/m/y", strtotime($response['data']['weather'][0]['date'])) . ') to (' . date("d/m/y", strtotime($response['data']['weather'][count($response['data']['weather']) - 1]['date'])) . ')'),
      'colgroups' => array(),
      'sticky' => FALSE,
      'empty' => '',
    );

    $content['weather_current_conditions_table'] = array(
      '#type' => 'markup',
      '#markup' => theme('table', $cc_table),
    );

    $content['weather_forecast_table'] = array(
      '#type' => 'markup',
      '#markup' => theme('table', $forecast_table),
    );

    for ($i = 0; $i < count($response['data']['weather']); $i++) {

      $response_day = $response['data']['weather'][$i];

      $variables = $response_day;

      $variables['date_correct'] = format_date(strtotime($variables['date']), 'custom', 'd/m/Y');

      // Set sunrise
      $variables['sunrise'] = $response_day['astronomy'][0]['sunrise'];
      // Set sunset
      $variables['sunset'] = $response_day['astronomy'][0]['sunset'];
      $variables['before_sunrise_hours'] = array();
      $variables['before_sunset_hours'] = array();
      $variables['after_sunset_hours'] = array();
      // Get hours before sunset into array
      foreach ($variables['hourly'] as $hour) {
        $hour['weather_image'] = theme('image', array(
          'path' => $hour['weatherIconUrl'][0]['value'],
          'attributes' => NULL
        ));
        if (strlen($hour['time']) < 4 && strlen($hour['time']) > 1) {
          $hour['time'] = '0' . $hour['time'];
        }
        elseif (strlen($hour['time']) == 1) {
          $hour['time'] = '000' . $hour['time'];
        }
        $hour['time_formatted'] = format_date(strtotime($hour['time']), 'custom', 'h:i A');
        if (strtotime($hour['time']) < strtotime($variables['sunrise'])) {
          array_push($variables['before_sunrise_hours'], $hour);
        }
        elseif (strtotime($hour['time']) < strtotime($variables['sunset'])) {
          array_push($variables['before_sunset_hours'], $hour);
        }
        else {
          array_push($variables['after_sunset_hours'], $hour);
        }
      }

      $content['weather_forecast_day_' . $i] = array(
        '#type' => 'markup',
        '#markup' => theme('weather_forecast_day', $variables),
      );

    }

    return $content;

  }
  else {
    // Redirect 404
    drupal_not_found();
  }
}