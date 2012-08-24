<?php
/**
 * @file vc_onlywire.admin.inc
 * Functions for vc_onlywire settings.
 */

/**
 * Helper function to build node type options.
 */
function _vc_onlywire_node_type_options() {
  $options = array();
  foreach (node_type_get_types() as $type => $node_type) {
    $options[$type] = $node_type->name;
  }
  return $options;
}

/**
 * Settings form for VC onlywire.com
 */
function vc_onlywire_settings_form($form, $form_state) {
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#required' => TRUE,
    '#default_value' => variable_get('vc_onlywire_username', ''),
    '#description' => t('Your onlywire.com username.'),
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Your onlywire.com password. Leave blank to keep old value.'),
  );

  $form['node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#options' => _vc_onlywire_node_type_options(),
    '#default_value' => variable_get('vc_onlywire_node_types', array()),
    '#description' => t('Select node type which will be bookmarked on created.'),
  );

  $form['template']['#tree'] = TRUE;

  foreach (node_type_get_types() as $type => $info) {
    $form['template'][$type] = array(
      '#type' => 'fieldset',
      '#title' => t('Bookmark Template - !label', array('!label' => $info->name)),
      '#collapsible' => TRUE,
    );

    $form['template'][$type]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => variable_get('vc_onlywire_'. $type .'_template_title', '[node:title]'),
    );

    $form['template'][$type]['notes'] = array(
      '#type' => 'textfield',
      '#title' => t('Notes'),
      '#default_value' => variable_get('vc_onlywire_'. $type .'_template_notes', '[node:body]'),
    );

    $form['template'][$type]['category'] = array(
      '#type' => 'textfield',
      '#title' => t('Category'),
      '#default_value' => variable_get('vc_onlywire_'. $type .'_template_category', ''),
    );

    $form['template'][$type]['tags'] = array(
      '#type' => 'textfield',
      '#title' => t('Tags'),
      '#default_value' => variable_get('vc_onlywire_'. $type .'_template_tags', ''),
    );

    $form['template'][$type]['scheduledtime'] = array(
      '#type' => 'textfield',
      '#title' => t('Scheduled Time'),
      '#default_value' => variable_get('vc_onlywire_'. $type .'_template_scheduledtime', ''),
      '#description' => t('Optional value, related to node created time. Example of valid values: + 1 hour, + 5 minutes, … Leave blank to use Onlywire server time.'),
    );

    $form['template'][$type]['private'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private'),
      '#default_value' => variable_get('vc_onlywire_'. $type .'_template_private', FALSE),
      '#description' => t('Keep the bookmark private'),
    );

    $form['template'][$type]['shortener'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use OnlyWire\'s Shortener'),
      '#default_value' => variable_get('vc_onlywire_'. $type .'_template_shortener', FALSE),
    );

    $form['template'][$type]['tokens'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('node'),
      '#global_types' => TRUE,
      '#click_insert' => TRUE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for vc_onlywire settings form.
 */
function vc_onlywire_settings_form_validate($form, $form_state) {
  $v = &$form_state['values'];

  if (empty($v['template'])) return;
  foreach (node_type_get_types() as $type => $info) {
    if (empty($v['template'][$type]['scheduledtime'])) continue;

    $error = TRUE;
    $scheduled_time = strtotime($v['template'][$type]['scheduledtime']);
    if (FALSE !== $scheduled_time) {
      if ($scheduled_time >= REQUEST_TIME) {
        $error = FALSE;
      }
    }

    if (!$error) continue;
    $msg = t('Invalid value for Scheduled Time (!name)', array('!name' => $info->name));
    form_set_error('template]['. $type .'][scheduledtime', $msg);
  }
}

/**
 * Submit handler for vc_onlywire settings form.
 */
function vc_onlywire_settings_form_submit($form, $form_state) {
  $v = &$form_state['values'];

  variable_set('vc_onlywire_username', $v['username']);
  if (!empty($v['password'])) {
    variable_set('vc_onlywire_password', $v['password']);
  }
  variable_set('vc_onlywire_node_types', $v['node_types']);

  foreach (node_type_get_types() as $type => $info) {
    variable_set('vc_onlywire_'. $type .'_template_title',         $v['template'][$type]['title']);
    variable_set('vc_onlywire_'. $type .'_template_notes',         $v['template'][$type]['notes']);
    variable_set('vc_onlywire_'. $type .'_template_category',      $v['template'][$type]['tags']);
    variable_set('vc_onlywire_'. $type .'_template_tags',          $v['template'][$type]['tags']);
    variable_set('vc_onlywire_'. $type .'_template_private',       $v['template'][$type]['private']);
    variable_set('vc_onlywire_'. $type .'_template_shortener',     $v['template'][$type]['shortener']);
    variable_set('vc_onlywire_'. $type .'_template_scheduledtime', $v['template'][$type]['scheduledtime']);
  }
}
