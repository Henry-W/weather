<?php

function weather_saved_forecasts() {

  $content['Hello'] = array(
    '#type' => 'markup',
    '#markup' => 'Simply click "View More" on your desired location and date to view the forecast!',
  );

  $rows = array();

  foreach (taxonomy_get_tree(2) as $term) {

    foreach (array_reverse(taxonomy_select_nodes($term->tid)) as $node) {
      $node_data = node_load($node);
      $term = taxonomy_term_load($node_data->field_location[LANGUAGE_NONE][0]['tid']);
      $term_name = $term->name;

      array_push($rows, array(
        $term_name,
        $node_data->field_date['und'][0]['value'],
        "<a href='" . $GLOBALS['base_url'] . "/weather/single/" . $node . "'>View More</a>",
      ));
    }

  }

  $saved_forecasts_table = array(
    'header' => array(t('Location'), t('Date'), t('Full View')),
    'rows' => $rows,
    'attributes' => array(),
  );

  $content['table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', $saved_forecasts_table),
  );

  return $content;

}