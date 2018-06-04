<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Datos de contacto'),
  'description' => t('Muestra los datos de contacto de un área.'),
  'category' => t('Drupar'),
  'render callback' => 'drupar_componentes_area_contacto_render',
  'admin info' => 'drupar_componentes_area_contacto_admin_info',
  'defaults' => array()
);

/**
* 'admin info' callback for panel pane.
*/
function drupar_componentes_area_contacto_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : t('Datos de contacto');
    $block->content = t('Datos de contacto');
    return $block;
  }
}
function drupar_componentes_area_contacto_render($subtype, $conf, $panel_args, $context = NULL){
  $block = new stdClass();
  $block->title = '';
  if(arg(0) == 'node' && is_numeric(arg(1)) && node_load(arg(1))->type == 'area'){
    $node = node_load(arg(1));
    $contacto = new stdClass;
    $contacto->direccion = field_get_items('node', $node, 'field_direccion', NULL);
    $contacto->direccion = isset($contacto->direccion[0]) && !empty($contacto->direccion[0]) ? $contacto->direccion[0]['safe_value'] : NULL;
    $contacto->codigo_postal = field_get_items('node', $node, 'field_codigo_postal', NULL);
    $contacto->codigo_postal = isset($contacto->codigo_postal[0]) && !empty($contacto->codigo_postal[0]) ? $contacto->codigo_postal[0]['safe_value'] : NULL;
    $contacto->telefono = field_get_items('node', $node, 'field_telefono', NULL);
    $contacto->telefono = isset($contacto->telefono[0]) && !empty($contacto->telefono[0]) ? $contacto->telefono[0]['safe_value'] : NULL;
    $contacto->correo_electronico = field_get_items('node', $node, 'field_correo_electronico', NULL);
    $contacto->correo_electronico = isset($contacto->correo_electronico[0]) && !empty($contacto->correo_electronico[0]) ? $contacto->correo_electronico[0]['email'] : NULL;

    $contacto->facebook = field_get_items('node', $node, 'field_facebook', NULL);
    $contacto->facebook = isset($contacto->facebook[0]) && !empty($contacto->facebook[0]) ? l('<i class="fa fa-facebook"></i>',
      'http://www.facebook.com/' . $contacto->facebook[0]['safe_value'],
      array(
        'html' => TRUE,
        'attributes' => array(
          'target' => '_blank'
        ),
      )
    ) : NULL;

    $contacto->twitter = field_get_items('node', $node, 'field_twitter', NULL);
    $contacto->twitter = isset($contacto->twitter[0]) && !empty($contacto->twitter[0]) ? 
    l('<i class="fa fa-twitter"></i>',
      'http://www.twitter.com/' . $contacto->twitter[0]['safe_value'],
      array(
        'html' => TRUE,
        'attributes' => array(
          'target' => '_blank'
        ),
      )
    ) : NULL;

    $contacto->instagram = field_get_items('node', $node, 'field_instagram', NULL);
    $contacto->instagram = isset($contacto->instagram[0]) && !empty($contacto->instagram[0]) ? 
    l('<i class="fa fa-instagram"></i>',
      'http://www.instagram.com/' . $contacto->instagram[0]['safe_value'],
      array(
        'html' => TRUE,
        'attributes' => array(
          'target' => '_blank'
        ),
      )
    ) : NULL;

    $contacto->youtube = field_get_items('node', $node, 'field_youtube', NULL);
    $contacto->youtube = isset($contacto->youtube[0]) && !empty($contacto->youtube[0]) ? 
    l('<i class="fa fa-youtube"></i>',
      $contacto->youtube[0]['safe_value'],
      array(
        'html' => TRUE,
        'attributes' => array(
          'target' => '_blank'
        ),
      )
    ) : NULL;

    $block->content = estaSeteadoContacto($contacto) ? theme('drupar_componentes_area_contacto', array('contacto' => $contacto)) : NULL;
  } else {
    $block->content = NULL;
  }

  return $block;
}

function estaSeteadoContacto($contacto){
  return(
    isset($contacto->direccion) || isset($contacto->codigo_postal)|| isset($contacto->telefono) || isset($contacto->correo_electronico) || isset($contacto->facebook) || isset($contacto->twitter)|| isset($contacto->youtube) || isset($contacto->instagram)
  );
}