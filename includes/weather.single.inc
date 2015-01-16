<?php

function weather_forecast_single($node_id) {

  $node_data = node_load($node_id);

  // Work out total hours of daylight
  $sunrise = $node_data->field_sunrise['und'][0]['value'];
  $sunset = $node_data->field_sunset['und'][0]['value'];
  $hours_diff = strtotime($sunset) - strtotime($sunrise);
  $hours_of_daylight = gmdate("G", $hours_diff) . ' hours and ' . gmdate("i", $hours_diff) . ' minutes';

  $cc_table = array(
    'header' => array(t('Name'), t('Value from Resultset')),
    'rows' => array(
      // Simple row
      array(
        'Location',
        taxonomy_term_load($node_data->field_location['und'][0]['tid'])->name,
      ),
      array(
        'Date',
        $node_data->field_date['und'][0]['value']
      ),
      array(
        'Description',
        $node_data->body['und'][0]['value']
      ),
      array(
        'Icon',
        theme('image', array(
          'path' => $node_data->field_weather_icon['und'][0]['uri'],
          'attributes' => NULL
        )),
      ),
      array(
        'Sunrise',
        $sunrise
      ),
      array(
        'Sunset',
        $sunset
      ),
      array(
        'Total Hours of Daylight',
        $hours_of_daylight
      ),
      array(
        'Time Forecast Received',
        format_date($node_data->created, 'custom', 'h:m A'),
      )
    ),
    'attributes' => array(),
    'caption' => t('Forecast Details'),
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => '',
  );

  $content['forecast_single_table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', $cc_table),
  );

  return $content;

}