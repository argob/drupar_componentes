<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Campo'),
  'description' => t('Renderiza un campo del nodo'),
  'category' => t('Drupar'),
  'render callback' => 'drupar_componentes_campo_render',
  'edit form' => 'drupar_componentes_campo_edit_form',
  'admin info' => 'drupar_componentes_campo_admin_info',
  'defaults' => array(
    'campo' => '',
  ),
);

/**
* 'admin info' callback for panel pane.
*/
function drupar_componentes_campo_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : $conf['campo'];
    $block->content = $conf['campo'];
    return $block;


  }

}

/**
* 'Edit form' callback for the content type.
*/
function drupar_componentes_campo_edit_form($form, &$form_state) {

  $form['campo'] = array(
    '#type' => 'select',
    '#title' => t('Campo'),
    '#required' => TRUE,
    '#options' => array(
      'costo' => t('Costo de trámite'),
      'requisitos' => t('Requisitos de trámite'),
      'tipo_tramite' => t('Tipo de trámite'),
      'categoria_noticia' => t('Categoría de noticia'),
      'siglas_area' => t('Siglas de Área'),
      'tags' => t('Tags'),
      'links_relacionados' => t('Links relacionados'),
      'paginas_relacionadas' => t('Páginas relacionadas'),
      'archivos' => t('Archivos'),
      'creditos' => t('Créditos'),
      'download' => t('Descarga'),
      'store' => t('Descarga de aplicaciones'),
      'audio' => t('Audios'),
      'vigencia' => t('Vigencia'),
      'acciones' => t('Acciones'),
      'servicios_relacionados' => t('Servicios Relacionados')
    ),
    '#default_value' => $form_state['conf']['campo'],
    '#description' => t('Campo a mostrar'),
  );
  
  return $form;
}

/**
* The submit form stores the data in $conf.
*/
function drupar_componentes_campo_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

function drupar_componentes_campo_render($subtype, $conf, $panel_args, $context = NULL){
  $block = new stdClass();
  $block->title = '';
  $titulo = $conf['override_title'] ? $conf['override_title_text'] : 'Descargas';
  $campo = obtenerCampo(node_load(arg(1)), $conf['campo']);
  $block->content = !empty($campo) ? theme('drupar_componentes_campo_' . $conf['campo'], array('campo' => $campo, 'titulo' => $titulo)) : NULL;
  return $block;

}


function obtenerCampo($nodo, $tipo_campo)
{
    switch ($tipo_campo) {
    case 'costo':
      $campo = field_get_items('node', $nodo, 'field_tramite_costo', null);
      $campo = isset($campo) && !empty($campo) ? $campo[0]['safe_value'] : null;
      break;
    case 'audio':
      $campo = field_get_items('node', $nodo, 'field_audio', null);
      $campo = isset($campo) && !empty($campo) ? file_create_url($campo[0]['uri']) : null;
      break;
    case 'requisitos':
      $campo = field_get_items('node', $nodo, 'field_requisitos', null);
      $campo = isset($campo) && !empty($campo) ? $campo[0]['safe_value'] : null;
      break;
      case 'creditos':
      $campo = field_get_items('node', $nodo, 'field_creditos', null);
      $campo = isset($campo) && !empty($campo) ? $campo[0]['safe_value'] : null;
      break;
    case 'siglas_area':
      $campo = field_get_items('node', $nodo, 'field_siglas', null);
      $campo = isset($campo) && !empty($campo) ? $campo[0]['safe_value'] : null;
      break;
    case 'links_relacionados':
      $campo = field_get_items('node', $nodo, 'field_links_relacionados', null);
      $campo = isset($campo) && !empty($campo) ? obtener($tipo_campo, $campo) : '';
      break;
    case 'paginas_relacionadas':
      $campo = field_get_items('node', $nodo, 'field_paginas_relacionadas', null);
      $campo = isset($campo) && !empty($campo) ? obtener($tipo_campo, $campo) : '';
      break;
    case 'tipo_tramite':
      $campo = field_get_items('node', $nodo, 'field_tramite_tipo', null);
      $campo = isset($campo) ? taxonomy_term_load($campo[0]["target_id"]) : null;
      $campo = isset($campo) ? $campo->name : null;
      break;
    case 'categoria_noticia':
      $campo = field_get_items('node', $nodo, 'field_categoria', null);
      $campo = isset($campo) ? taxonomy_term_load($campo[0]["target_id"]) : null;
      $campo = isset($campo) ? $campo->name : null;
      break;
    case 'tags':
      $campo = field_get_items('node', $nodo, 'field_tags', null);
      $campo = isset($campo) && !empty($campo) ? obtener($tipo_campo, $campo) : null;
      break;
      case 'download':
      $campo = field_get_items('node', $nodo, 'field_download', null);
      $campo = isset($campo) && !empty($campo) ? obtener($tipo_campo, $campo) : null;
      break;
      case 'store':
      $campo['android'] = field_get_items('node', $nodo, 'field_link_android', null);
      $campo['iphone'] = field_get_items('node', $nodo, 'field_link_iphone', null);
      $campo['windows'] = field_get_items('node', $nodo, 'field_link_windows', null);
      $campo['blackberry'] = field_get_items('node', $nodo, 'field_link_blackberry', null);
      $campo = isset($campo) && !empty($campo) ? obtener($tipo_campo, $campo) : null;
      break;
    default:
      # code...
      break;
  }
    return $campo;
}

function obtener($nombreCampo, $campo)
{
    $items = array();
    switch ($nombreCampo) {
    case 'tags':
      foreach ($campo as $item) {
          $items[] = $item['taxonomy_term']->name;
      }
      break;
    case 'paginas_relacionadas':
      foreach ($campo as $item) {
          $items[] = $item;
      }
      break;
    case 'links_relacionados':
      foreach ($campo as $item) {
          $items[] = $item;
      }
      break;
      case 'download':
      foreach ($campo as $item) {
          $items[] = $item;
      }
      break;
      case 'store':
      foreach ($campo as $key => $item) {
          if ($item <> null) {
              $items[$key] = $item;
          }
      }
      break;
    default:
      # code...
      break;
  }
    return $items;
}
