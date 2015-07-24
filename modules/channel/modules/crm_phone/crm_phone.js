(function ($) {
  Drupal.behaviors.crmPhone = {
    attach: function (context, settings) {
      $('.crm-phone-sms-message', context).once('crmPhoneSmsMessage', function () {
        var counter = SmsCounter.count($(this).val());
        $(this).before('<div class="crm-phone-sms-message-counter">' + Drupal.theme('displaySMSCounter', counter) + '</div>');
        $(this).bind('keyup', function() {
          var counter = SmsCounter.count($(this).val());
          $(this).parent().find('.crm-phone-sms-message-counter').html(Drupal.theme('displaySMSCounter', counter));
        });
      });
    }
  };
})(jQuery);

Drupal.theme.prototype.displaySMSCounter = function(counter) {
  return '<div class="messages-' + counter['messages'] + '">'
    + Drupal.t('Length:')
    + ' <span class="length">'
      + counter['length']
    + '</span> / '
    + Drupal.t('Remaining:')
    + ' <span class="remaining">'
      + counter['remaining']
    + '</span> / '
    + Drupal.t('Number of messages:')
    + '<span class="messages">'
      + counter['messages']
    + '</span>'
  + '</div>';
};
