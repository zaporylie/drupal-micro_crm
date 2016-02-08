(function ($) {
  Drupal.behaviors.crm_ui_knob = {
    attach: function (context, settings) {
      $('.crm-ui__knob-wrapper__knob', context).once('crm_ui_knob', function () {
        $(this).knob();
      });
    }
  };
}(jQuery));