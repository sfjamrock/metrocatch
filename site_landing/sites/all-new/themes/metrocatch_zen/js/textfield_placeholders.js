/**
 * Behavior for text-hints on textfields and texareas
 */
Drupal.behaviors.textfield_placeholders = function(context) {
  function textfielPlaceholdersRefreshElement(element, text) {
    var wrapper = element.parent('.textfield-placeholder-wrapper');
    if (wrapper.size() == 0) {
      // create new div over the textfield
      element.wrap('<div class="textfield-placeholder-wrapper"></div>');
      wrapper = element.parent('.textfield-placeholder-wrapper').css('position', 'relative');
      wrapper.append('<div class="textfield-placeholder"></div>');
      // calculate padding-top for our div
    }
    padding_top = parseInt(element.css('padding-top')) + parseInt(element.css('border-top-width')) + 'px';
    // calculate line-height for our div
    if (wrapper.children('textarea').size()) {
      // textarea
      line_height = element.css('line-height');
    }
    else {
      // text or password field
      line_height = element.height() + 'px';
    }
    wrapper.children('.textfield-placeholder')
      .css({height: element.height(), width: element.width(), left: '0px', top: '0px', position: 'absolute', 'z-index' : 8, 'line-height': line_height , 'padding-left' : element.css('padding-left'), 'padding-top' : padding_top, 'margin-top': element.css('margin-top'), 'margin-left': element.css('margin-left'), 'font-size': element.css('font-size'), 'font-weight': element.css('font-weight'), 'font-family': element.css('font-family')})
      .click(function(){
        $(this).hide().parent('.textfield-placeholder-wrapper').children('input, textarea').trigger('focus').blur(function(){
          if ($(this).val() == '') {
            $(this).parent('.textfield-placeholder-wrapper').children('.textfield-placeholder').show();
          }
        });
      }
    );
    if (typeof(text) != 'undefined') {
      wrapper.children('.textfield-placeholder').text(Drupal.settings.textfield_placeholders[i]['text']);
    }
    // correct div position
    if (element.css('position') == 'absolute') {
      wrapper.children('.textfield-placeholder').css({left: element.css('left'), top: element.css('top')});
    }
    if (element.val() != '') {
      wrapper.children('.textfield-placeholder').hide();
    }
  }
  // process all selectors from Drupal.settings
  for (i in Drupal.settings.textfield_placeholders) {
    selector = Drupal.settings.textfield_placeholders[i]['selector'];
    if (selector == '') {
      continue;
    }
    element = $(selector, context);
    if (!element.size()) {
      continue;
    }
    textfielPlaceholdersRefreshElement(element, Drupal.settings.textfield_placeholders[i]['text']);
    element.focus(function(){
      textfielPlaceholdersRefreshElement($(this));
      $(this).parent('.textfield-placeholder-wrapper').children('.textfield-placeholder').hide();
    }).blur(function(){
      if ($(this).val() == '') {
        $(this).parent('.textfield-placeholder-wrapper').children('.textfield-placeholder').show();
      }
    });
  }
};
