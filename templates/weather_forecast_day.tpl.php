<hr>
<h1><b>Day:</b> <?php print render($date_correct); ?></h1>
<?php if (!empty($before_sunrise_hours)) { ?>
  <h2><b>Before Sunrise:</b></h2>
  <?php foreach ($before_sunrise_hours as $hour) { ?>
    <h3><?php print render($hour['time_formatted']); ?></h3>
    <table>
      <thead>
      <tr>
        <th>Icon</th>
        <th>Forecast</th>
        <th>Temp (C)</th>
      </tr>
      </thead>
      <tbody>
      <tr class="odd">
        <td><?php print render($hour['weather_image']); ?></td>
        <td><?php print render($hour['weatherDesc'][0]['value']); ?></td>
        <td><?php print render($hour['tempC']); ?></td>
      </tr>
      </tbody>
    </table>
  <?php } ?>
<?php } ?>
<?php if (!empty($before_sunset_hours)) { ?>
  <h2><b>After Sunrise:</b> (<?php print render($sunrise); ?>)</h2>
  <?php foreach ($before_sunset_hours as $hour) { ?>
    <h3><?php print render($hour['time_formatted']); ?></h3>
    <table>
      <thead>
      <tr>
        <th>Icon</th>
        <th>Forecast</th>
        <th>Temp (C)</th>
      </tr>
      </thead>
      <tbody>
      <tr class="odd">
        <td><?php print render($hour['weather_image']); ?></td>
        <td><?php print render($hour['weatherDesc'][0]['value']); ?></td>
        <td><?php print render($hour['tempC']); ?></td>
      </tr>
      </tbody>
    </table>
  <?php } ?>
<?php } ?>
<?php if (!empty($after_sunset_hours)) { ?>
  <h2><b>After Sunset:</b> (<?php print render($sunset); ?>)</h2>
  <?php foreach ($after_sunset_hours as $hour) { ?>
    <h3><?php print render($hour['time_formatted']); ?></h3>
    <table>
      <thead>
      <tr>
        <th>Icon</th>
        <th>Forecast</th>
        <th>Temp (C)</th>
      </tr>
      </thead>
      <tbody>
      <tr class="odd">
        <td><?php print render($hour['weather_image']); ?></td>
        <td><?php print render($hour['weatherDesc'][0]['value']); ?></td>
        <td><?php print render($hour['tempC']); ?></td>
      </tr>
      </tbody>
    </table>
  <?php } ?>
<?php } ?>