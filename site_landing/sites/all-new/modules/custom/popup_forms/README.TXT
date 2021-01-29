How to use this module:

1. Implement hook_popup_forms_data().
  hook must return array of entries, where key is corresponding form id and value is array in following format:
  
    'test_form_id' => array(
      'params' => array('param1' => 'defalut_value1', 'param2' => 'defalut_value2'),
      'close_buttons' => array('delete', 'cancel'), // array with buttons names (#name FAPI key), that closes the popup
      'file' => $file, // file needs to be included
    ),
    
2. Call popup_forms_parent_js() on page, where you want to show poppup form
3. Load popup from JS by calling 
  popupFormsFormShow(
    form_id, // - your form_id, declared in hook_popup_forms_data()
    callback, // JS callback (see below)
    form_data, // keyed form parameters, declared in hook_popup_forms_data()
    dialog_options // JQuery UI dialog options
  );
  
4. Receive form execution result in your callback. First param of callback function is $form_state values, fetched after form submit