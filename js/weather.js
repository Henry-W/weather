/**
 * Created by HWHITE on 12/01/2015.
 */
$(document).ready(function () {
    $('input[type="radio"]').click(function () {
        if ($(this).attr('id') == 'edit-weather-date-2') {
            $('.form-item-weather-date-custom').show();
        }
        else {
            $('.form-item-weather-date-custom').hide();
        }
    });
    if ($('input[name=weather_date]:checked').attr('id') == 'edit-weather-date-2') {
        $('.form-item-weather-date-custom').show();
    }
    else {
        $('.form-item-weather-date-custom').hide();
    }
});