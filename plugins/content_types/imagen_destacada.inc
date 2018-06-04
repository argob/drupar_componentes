<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Encabezado'),
  'description' => t(''),
  'category' => t('Drupar'),
  'render callback' => 'drupar_componentes_imagen_destacada_render',
  'admin info' => 'drupar_componentes_imagen_destacada_admin_info',
  'defaults' => array(
    'imagen_destacada' =>   NULL,
  )
);

/**
* 'admin info' callback for panel pane.
*/
function drupar_componentes_imagen_destacada_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : t('Imagen destacada');
    $block->content = t('Imagen destacada.');
    return $block;
  }
}
function drupar_componentes_imagen_destacada_render($subtype, $conf, $panel_args, $context = NULL){
  $block = new stdClass();
  $block->title = '';
  if(arg(0) == 'node' && is_numeric(arg(1))){
    $node = node_load(arg(1));
      $imagen = field_get_items('node', $node, 'field_featured_image', NULL);
      $texto = field_get_items('node', $node, 'body', NULL);
      $theme = esNoticia() ? 'drupar_componentes_imagen_destacada_noticia' : 'drupar_componentes_imagen_destacada';
      $block->content = theme(
        'drupar_componentes_imagen_destacada_' . $node->type, 
        array(
          'campos' => obtenerData($node),
          'breadcrumb' => theme('breadcrumb', array('breadcrumb' => drupal_get_breadcrumb()))
        )
    );
  } else {
    $block->content = '';
  }
  return $block;
}
function obtenerData($node){
  $campos = array();
  $campos['titulo'] = $node->title;
  switch ($node->type) {
    case 'tramite':
      $costo = field_get_items('node', $node, 'field_tramite_costo', NULL);
      $campos['costo'] = $costo[0]['safe_value'];
      break;
    case 'aplicacion':
      $icono = field_get_items('node', $node, 'field_icono', NULL);
      $campos['body'] = field_get_items('node', $node, 'body', NULL);
      $campos['icono'] = isset($icono[0]) ? image_style_url('icono_aplicacion', $icono[0]['uri']) : '';
    case 'noticia':
      $campos['fecha'] = format_date($node->created, 'custom', 'l d \d\e F \d\e o');
    case 'area':
    case 'noticia':
    case 'aplicacion':
      $imagen = field_get_items('node', $node, 'field_featured_image', NULL);
      $contenido = field_get_items('node', $node, 'body', NULL);
      $campos['imagen_destacada'] = isset($imagen[0]) ? image_style_url('jumbotron', $imagen[0]['uri']) : '';
      $campos['resumen'] = isset($contenido[0]) ? $contenido[0]['summary'] : '';
      break;
    default:
      # code...
      break;
  }
  return $campos;
}