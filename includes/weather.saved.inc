<?php

function weather_saved_forecasts() {

  $content['Hello'] = array(
    '#type' => 'markup',
    '#markup' => t('Simply click "View More" on your desired location and date to view the forecast!'),
  );

  $rows = array();

  foreach (taxonomy_get_tree(taxonomy_vocabulary_machine_name_load('locations')->vid) as $term) {

    foreach (array_reverse(taxonomy_select_nodes($term->tid)) as $node) {
      $node_data = node_load($node);
      $tid = $node_data->field_location[LANGUAGE_NONE][0]['tid'];

      $term_data = taxonomy_term_load($tid);

      if (isset($term_data->name)) {
        $term_name = $term_data->name;
        array_push($rows, array(
          $term_name,
          $node_data->field_date[LANGUAGE_NONE][0]['value'],
          l(t("View More"), "node/" . $node)
        ));
      }
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