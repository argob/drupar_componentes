<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Separador'),
  'description' => t('Crea un separador'),
  'category' => t('Drupar'),
  'edit form' => 'drupar_componentes_separador_edit_form',
  'render callback' => 'drupar_componentes_separador_render',
  'admin info' => 'drupar_componentes_separador_admin_info',
  'defaults' => array(
    'linea' => FALSE,
  )
);


/**
* 'admin info' callback for panel pane.
*/
function drupar_componentes_separador_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : t('Separador');
    $block->content = t('Separador');
    return $block;
  }
}
function drupar_componentes_separador_edit_form($form, &$form_state){
  $form['linea'] = array(
    '#type' => 'checkbox',
    '#title' => t('Línea de separación'),
    '#default_value' => $form_state['conf']['linea'],
    '#description' => t('Si está tildado se mostrará una linea de separación. Sino sólo un márgen'),
  );

  return $form;
}
/**
* The submit form stores the data in $conf.
*/
function drupar_componentes_separador_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
function drupar_componentes_separador_render($subtype, $conf, $panel_args, $context = NULL){
  $block = new stdClass();
  $block->title = '';
  $block->content = ($conf['linea']) ? theme('drupar_componentes_separador_linea') : theme('drupar_componentes_separador');
  return $block;
}