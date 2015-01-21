<?php

function weather_forecast_single($node_id) {

  $node_data = node_load($node_id);
  $node_term = taxonomy_term_load($node_data->field_location[LANGUAGE_NONE][0]['tid']);
  $node_term_name = NULL;
  if (isset($node_term->name)) {
    $node_term_name = $node_term->name;
  }
  // Work out total hours of daylight
  $sunrise = $node_data->field_sunrise[LANGUAGE_NONE][0]['value'];
  $sunset = $node_data->field_sunset[LANGUAGE_NONE][0]['value'];
  $hours_diff = calculate_hours_daylight($node_id);
  $hours_of_daylight = t("@hours hours and @minutes minutes",
    array(
      '@hours' => gmdate("G", $hours_diff),
      '@minutes' => gmdate("i", $hours_diff)
    ));



  $cc_table = array(
    'header' => array(t('Name'), t('Value from Resultset')),
    'rows' => array(
      // Simple row
      array(
        t('Location'),
        $node_term_name,
      ),
      array(
        t('Date'),
        $node_data->field_date[LANGUAGE_NONE][0]['value']
      ),
      array(
        t('Description'),
        $node_data->body[LANGUAGE_NONE][0]['value']
      ),
      array(
        t('Icon'),
        theme('image', array(
          'path' => $node_data->field_weather_icon[LANGUAGE_NONE][0]['uri'],
          'attributes' => NULL
        )),
      ),
      array(
        t('Sunrise'),
        $sunrise
      ),
      array(
        t('Sunset'),
        $sunset
      ),
      array(
        t('Total Hours of Daylight'),
        $hours_of_daylight
      ),
      array(
        t('Time Forecast Received'),
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