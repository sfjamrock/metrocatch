/**
 *  Drupal behavoir for the conversation page, handles ajax comments,
 *  "show/hide comments" links, "remove" links.
 */
Drupal.behaviors.metrocatch_conversation = function(context) {
  var conversation = Drupal.metrocatch_conversation;
  conversation.init(context);

  // handle "show/hide comments" link
  $('.conversation-entry-links a.conversation-entry-toggle-comments-link', context).click(function(event){
    // toggle visiblity of comments
    var nid = $(this).parents('.metrocatch-conversation-entry').find('.node').attr('id').split('node-')[1];
    conversation.toggle_comments(nid);
    event.stopPropagation();
    return false;
  });
  // show comment form, when "comment" is clicked
  $('.conversation-entry-links a.conversation-entry-comment-link', context).click(function(event){
    // show textarea and focus on it
    var form = $(this).parents('.metrocatch-conversation-entry').find('.add-comment-form-wrapper:first form');
    conversation.toggle_form(form);
    event.stopPropagation();
    return false;
  });
  // handle "remove" link
  $('a.conversation-entry-remove-link', context).click(function(event){
    conversation.remove_node($(this));
    event.stopPropagation();
    return false;
  });
  // handle "view more" link
  $('a.metrocatch-conversation-more-link', context).click(function(event){
    conversation.show_more($(this));
    event.stopPropagation();
    return false;
  });
  // Handle 'Add/Remove' feed links:
  $('.metrocatch-conversation-feeds a', context).click(function(event) {
    var args = $(this).attr('id').split('-');
    if (args[0] == 'feed') {
      conversation.openDialog($(this), args[1]);
      event.stopPropagation();
      return false;
    }
  });
  /**
   * Submit comment form on pressing Enter.
   */
  $('form.node-comment-form textarea[name="comment"]', context).keypress(function(event) {
    if (event.keyCode == 13 && !event.shiftKey) {
      var button = $(this).parents('form').find('input[type="submit"]');
      if (!button.attr('disabled')) {
        button.click();
        $(this).attr('disabled', 'disabled');
      }
      event.stopPropagation();
      return false;
    }
  });
  /**
   * Submit new record form on pressing Enter.
   */
  $('#metrocatch-converstion-new-entry-form textarea[name="new_record"]', context).keypress(function(event) {
    if (event.keyCode == 13 && !event.shiftKey) {
      var button = $(this).parents('form').find('input[type="submit"]');arguments
      if (!button.attr('disabled')) {
        button.click();
        $(this).attr('disabled', 'disabled');
      }
      event.stopPropagation();
      return false;
    }
  });
  // Hide submit button on new record form.
  $('#metrocatch-converstion-new-entry-form input[type="submit"]').hide();
  // Hide submit button on comment forms.
  $('form.node-comment-form input[type="submit"]', context).hide();
}

/**
 * Behaviour for "cancel" link in "remove conversation entry" popup
 */
Drupal.behaviors.metrocatch_conversation_cancel_link = function(context) {
  if (typeof(Drupal.popup_forms_child) != 'undefined') {
    $('#metrocatch-conversation-delete-entry-form .container-inline a', context).click(function(){$('a.ui-dialog-titlebar-close').trigger('click');});
  }
}

Drupal.metrocatch_conversation = {
  // An associative array of conversation arguments with followong structure:
  // {more: [], show_forms: [], show_comments: []}.
  args: {},
  // An associative array with node ids in keys and last comment ids in values.
  comments: {},
  // Comments ordering: 1 means new first, 2 means old first.
  comments_order: 0,
  // Internal pointer on current AJAX call.
  _current_ajax: null,
  // boolean indicating whether conversation is inited.
  _inited: false,
  /**
   * Get and parse document hash, loads more entries, shows necessary comments and forms.
   *
   * All AJAX calls are queued in the following sequence:
   * 1. If 'more' key is present in args, load additional entries.
   * 2. If 'comments_showed' key is present in args, load comments for all necessary entries.
   */
  init: function (context) {
    if (!this._inited) {
      this._inited = true;
      this._get_hash(); // Parse url hash
      if (!! Drupal.settings.metrocatch_conversation) {
        this.comments_order = Drupal.settings.metrocatch_conversation.comments_order;
      }
      else {
        this.comments_order = 1;
      }
      var _this = this;
      // First of all we load additional entries:
      if (!!this.args.more) {
        $.each(this.args.more, function (key, skip) {
          var uid = _this._arg(1);
          _this._get_more(uid, skip);
        });
      }
      // Then expand comments:
      if (!!this.args.comments_showed) {
        $.each(this.args.comments_showed, function (key, nid) {
          _this.get_comments(nid);
        });
      }
    }
  },
  /**
   * Open popup with FB/Twitter login dialog.
   */
  openDialog: function (link, provider) {
    var winTitles = {
      'facebook': Drupal.t('Add Facebook Feed'),
      'twitter': Drupal.t('Add Twitter Feed')
    };
    var winProps = "menubar=0,location=1,resizable=1,scrollbars=1,status=1,toolbar=0,width=650,height=500";
    window.open(link.attr('href'), winTitles[provider], winProps);
  },
  /**
   * Get comments for a node.
   */
  get_comments: function (nid) {
    var _this = this;
    var comments_wrapper = this._get_comments_wrapper(nid);
    if (!this.comments[nid]) {
      // Check if comments are already loaded.
      var selector = (this.comments_order == 1) ? '.comment:first' : '.comment:last';
      // Get last comment ID:
      if ($(selector, comments_wrapper).length) {
        this.comments[nid] = $(selector, comments_wrapper).prev('a').attr('id').split('comment-')[1];
      }
    }
    last_comment = this.comments[nid] || 0;
    this.ajax_queue({
      url: Drupal.settings.basePath + '?q=ajax-metrocatch-conversation-get-comments',
      data: {
        nid: nid,
        last_comment: last_comment
      },
      type: 'POST',
      success: function(response, status) {
        if (typeof(response) == 'string') {
          response = Drupal.parseJson(response);
        }
        var wrapper = _this._get_comments_wrapper(nid);
        if (response.comments.length) {
          var comments = $('<div></div>').html(response.comments);
          var func = (this.comments_order == 1 ? 'prepend' : 'append');
          var children = comments.children();
          Drupal.attachBehaviors(comments);
          comments.empty().append(children);
          wrapper[func](comments).show();
          _this.comments[nid] = response.last_comment;
          _this.set_counter(nid);
          // We must rebuild zebra and first/last.
          var count = 1;
          $('.comment', comments_wrapper).each(function() {
            $(this).removeClass('odd').removeClass('even').removeClass('first').removeClass('last');
            $(this).addClass(count % 2 ? 'odd' : 'even');
            count++;
          });
          // Set first/last classes.
          $('.comment:first', comments_wrapper).addClass('first');
          $('.comment:last', comments_wrapper).addClass('last');
        }
      }
    });
  },
  /**
   * Shows or hides comments, changes document hash.
   */
  toggle_comments: function(nid) {
    var wrapper = this._get_comments_wrapper(nid);
    if (wrapper.css('display') == 'none') {
      wrapper.show();
      this._add_hash('comments_showed', nid);
      this.get_comments(nid);
    }
    else {
      wrapper.hide();
      this._remove_hash('comments_showed', nid);
    }
  },
  /**
   * Is called when user submits comment form.
   */
  form_submit_callback: function (nid) {
    var wrapper = this._get_comments_wrapper(nid);
    // show comments if not already:
    if (wrapper.css('display') == 'none') {
      wrapper.show();
      this._add_hash('comments_showed', nid);
    }
    this.get_comments(nid);
    $('#node-comment-form-' + nid).ajaxStop(function(params) {
      $('textarea:first', params.target).focus();
    });
  },
  /**
   * Shows or hides form, change document hash.
   */
  toggle_form: function (form) {
    if (form.size()) {
      var wrapper = form.parent();
      wrapper.toggle();
      var nid = form.attr('id').split('comment-form-')[1];
      if (wrapper.css('display') != 'none') {
        $('textarea:first', form).focus();
      }
    }
  },

  /**
   * Set counter in 'Show or hide comments (@count)' link.
   */
  set_counter: function (nid) {
    var entry = this._get_entry(nid);
    var count = $('.comment_wrapper', entry).find('.comment').length;
    $('.conversation-entry-toggle-comments-link', entry).html(Drupal.t('Show or Hide Comments (@comment_count)', {'@comment_count': count}));
  },

  /**
   * Handles 'view more' button.
   */
  show_more: function (link) {
    link.append('<div class="ahah-progress ahah-progress-throbber"><div class="throbber">&nbsp;</div></div>');
    var skip = this._arg(3, link.attr('href'));
    var uid = this._arg(1, link.attr('href'));
    this._get_more(uid, skip);
    this._add_hash('more', skip);
  },

  /**
   * Shows node deleting popup form.
   */
  remove_node: function (element) {
    var href = element.attr('href');
    var i = href.indexOf('?', 0);
    if (i != -1) {
      href = href.substr(0, i);
    }
    var args = href.split('/');
    popupFormsFormShow('metrocatch_conversation_delete_entry_form', this._remove_node_callback, {nid : args.pop()},
    {
      fail_callback: function(options){window.location.href = options.backurl;},
      backurl: element.attr('href')
    });
  },

  /**
   * Get more entries.
   */
  _get_more: function (uid, skip) {
    this.ajax_queue({
      url: Drupal.settings.basePath + '?q=user/' + uid + '/conversation/' + skip,
      data: {'ajax' : 1},
      dataType: 'json',
      success : function(result,  textStatus, jqXHR) {
        var link = $('.metrocatch-conversation-more-link', '.metrocatch-conversation-page');
        link.wrap('<div></div>');
        var wrapper = link.parent('div');
        wrapper.html(result.data);
        Drupal.attachBehaviors(wrapper.get(0));
        wrapper.replaceWith(wrapper.children());
      }
    });
  },

  /**
   * Queues AJAX calls.
   */
  ajax_queue: function (options) {
    var success = options.success;
    var error = options.error;
    var complete = options.complete;
    var _this = this;

    options.success = function()    {
      if (success) {
         success.apply(this, arguments);
      }
    };
    options.error = function() {
      if (error) {
        error.apply(this, arguments)
      }
    };
    options.complete = function() {
      if (complete) {
        complete.apply(this, arguments);
      }
      _this._current_ajax = null;
      $(window).dequeue("ajax");
    };
    $(window).queue("ajax", function() {
      _this._current_ajax = jQuery.ajax(options);
    });
    if (_this._current_ajax === null) {
      $(window).dequeue("ajax");
    }
  },

  /**
   * Callback for popupFormsFormShow().
   */
  _remove_node_callback: function (form_state) {
    $('.metrocatch-conversation-entry.entry-'+ form_state.values.nid).slideUp('slow', function(){
      $(this).remove();
    });
  },

  /**
   * Add hash.
   */
  _add_hash: function (name, value) {
    if (!this._get_hash(name)) {
      this.args[name] = [];
    }
    if ($.inArray(value, this.args[name])) {
      // add arg if not present.
      this.args[name].push(value);
    }
    this._set_hash();
  },

  /**
   * Remove hash.
   */
  _remove_hash: function (name, value) {
    value = value || null;
    if (this.args[name]) {
      if (value) {
        // delete single value:
        var index = $.inArray(value, this.args[name]);
        this.args[name].splice(index, 1);
      }
      if (!value || !this.args[name].length) {
        delete this.args[name];
      }
    }
    this._set_hash();
  },

  /**
   * Set document hash.
   */
  _set_hash: function() {
    var hashes = [];
    $.each(this.args, function (name, vals) {
      if (vals.length) {
        hashes.push(name + '=' + vals.join(','));
      }
    });
    if (hashes.length) {
      document.location.hash = hashes.join('|');
    }
    else {
      document.location.hash = '_';
    }
  },

  /**
   * Get hash or build args if not yet.
   */
  _get_hash: function(name) {
    if (!this.args.length) {
      // init hashes.
      var _this = this;
      _this.args = {};
      $.each(document.location.hash.slice(document.location.hash.indexOf('#') + 1).split('|'), function(key, val){
        args = val.split('=');
        if (!!args[1]) {
          _this.args[args[0]] = args[1].split(',');
        }
      });
    }
    return this.args[name] || false;
  },

  /**
   * Get url's arg.
   *
   * @param index
   *  Numeric index of arg.
   * @param url
   *  (optional) Url to parse. Default is current window url.
   */
  _arg: function (index, url) {
    url = url || document.location.href;
    if (url.indexOf('#') > 0) {
      // Remove hash from url:
      url = url.split('#')[0];
    }
    if (url.indexOf('?') > 0) {
      // Remove params:
      url = url.split('?')[0];
    }
    var base = document.location.hostname + Drupal.settings.basePath;
    if (url.indexOf(base) < 0) {
      // this is not absolute url.
      base = Drupal.settings.basePath;
    }
    var args = url.slice(url.indexOf(base) + base.length).split('/');
    return args[index] || false;
  },

  /**
   * Scrolls document to a specified element.
   */
  _scroll_to: function (element) {
    var offset = element.offset().top;
    $(document).scrollTop(offset);
  },

  /**
   * Get an entry for a nid.
   */
  _get_entry: function (nid) {
    return $('.entry-' + nid);
  },

  /**
   * Get form by Nid.
   */
  _get_form: function (nid) {
    return $('form', '#node-comment-form-' + nid);
  },

  /**
   * Get comments wrapper by Nid.
   */
  _get_comments_wrapper: function (nid) {
    return $('.comment_wrapper', this._get_entry(nid));
  }
}
