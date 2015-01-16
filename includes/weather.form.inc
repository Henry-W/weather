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

  $form['save_weather'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save weather forecast'),
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

  $decoded_response = weather_make_request($form_state);

  if ($form_state['values']['save_weather'] == TRUE) {

    for ($i = 0; $i < count($decoded_response['data']['weather']); $i++) {
      $node = new stdClass();
      $node->type = 'weather_log_entry';
      $node->title = "Weather for " . $decoded_response['data']['request'][0]['query'];
      $node->language = LANGUAGE_NONE;
      // Weather Summary
      $node->body[$node->language][0]['value'] = "On " . $decoded_response['data']['weather'][$i]['date'] . " the temperature for " . $decoded_response['data']['request'][0]['query'] . "; As low as " . $decoded_response['data']['weather'][$i]['mintempC'] . " (C) and as high as " . $decoded_response['data']['weather'][$i]['maxtempC'] . " (C)";
      // Weather Icon
      $external_image_path = $decoded_response['data']['weather'][$i]['hourly'][0]['weatherIconUrl'][0]['value'];
      $image = file_get_contents_curl($external_image_path); // string
      $file = file_save_data($image, NULL, FILE_EXISTS_REPLACE);
      $node->field_weather_icon[$node->language][0]['fid'] = $file->fid;
      // Sunrise
      $node->field_sunrise[$node->language][0]['value'] = $decoded_response['data']['weather'][$i]['astronomy'][0]['sunrise'];
      // Sunset
      $node->field_sunset[$node->language][0]['value'] = $decoded_response['data']['weather'][$i]['astronomy'][0]['sunset'];
      // Date
      $node->field_date[$node->language][0]['value'] = format_date(strtotime($decoded_response['data']['weather'][$i]['date']), 'custom', 'd/m/Y');
      // Location
      $term_boolean = FALSE;
      do {
        $search_terms = taxonomy_get_term_by_name($form_state['values']['weather_town_city']);
        if ($search_terms) {
          // Term already exists, but from which vocabulary??
          foreach ($search_terms as $term) {
            if ($term->vid == 2) {
              $node->field_location[$node->language][]['tid'] = $term->tid;
              $term_boolean = TRUE;
            }
            else {
              // Term exists for wrong vocabulary, make a new one for this one:
              $existing_term_new_vocab = array(
                'vid' => 2,
                'name' => $form_state['values']['weather_town_city'],
                // You can optionally also set id of parent term, e.g. 'parent' => 25
              );
              $existing_term_new_vocab = (object) $existing_term_new_vocab;
              taxonomy_term_save($existing_term_new_vocab);
              $term_boolean = FALSE;
            }
          }
        }
        else {
          // Term does not already exist, make a new one:
          $new_term = array(
            'vid' => 2,
            'name' => $form_state['values']['weather_town_city'],
            // You can optionally also set id of parent term, e.g. 'parent' => 25
          );
          $new_term = (object) $new_term;
          taxonomy_term_save($new_term);
          $term_boolean = FALSE;
        }
      }
      while ($term_boolean == FALSE);
      // Node meta
      $node->uid = 1;
      $path = 'node_created_on' . date('YmdHis');
      $node->path = array('alias' => $path);
      // Verify not already saved
      $already_exists = FALSE;

      foreach (taxonomy_get_tree(2) as $term) {

        foreach (array_reverse(taxonomy_select_nodes($term->tid)) as $eachnode) {
          $node_data = node_load($eachnode);
          $term_retrieve_by_name = taxonomy_get_term_by_name($form_state['values']['weather_town_city']);
          $term_retrieve = array_shift($term_retrieve_by_name);
          if ($node_data->field_date['und'][0]['value'] == $node->field_date[$node->language][0]['value'] && $node_data->field_location['und'][0]['tid'] == $term_retrieve->tid) {
            $already_exists = TRUE;
          }
        }

      }

      echo $already_exists;

      // Save node
      if ($already_exists == FALSE) {
        if ($node = node_submit($node)) { // Prepare node for saving
          node_save($node);
          drupal_set_message(t('The forecast for ' . $term_retrieve->name . ' on ' . $node->field_date[$node->language][0]['value'] . ' has successfully been saved.'), 'status');
        }
      }
      else {
        drupal_set_message(t('The forecast for ' . $term_retrieve->name . ' on ' . $node->field_date[$node->language][0]['value'] . ' could not be saved since it has already been saved before.'), 'error');
      }
    }

  }

  drupal_goto($GLOBALS['base_url'] . '/weather/results/' . $decoded_response['options_cache_id']);

}

function file_get_contents_curl($url) {
  $ch = curl_init();

  curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
  curl_setopt($ch, CURLOPT_PROXY, '10.23.12.110:8080');

  $data = curl_exec($ch);
  curl_close($ch);

  return $data;
}