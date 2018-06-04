<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Listado'),
  'description' => t('Crea un listado de contenido configurable'),
  'category' => t('Drupar'),
  'render callback' => 'drupar_componentes_listado_render',
  'edit form' => 'drupar_componentes_listado_edit_form',
  'admin info' => 'drupar_componentes_listado_admin_info',
  'defaults' => array(
    'destacar_primer_fila' => 0,
    'cantidad_links' => 8,
    'tipo_de_listado' => 'recientes',
    'contenido_custom' => '',
    'entity_types' => 'nodes',
    'taxonomy_vocabularies' => '',
    'tipos_de_contenido' => '',
    'formato_de_listado' => 'solo_texto',
    'titulo' => t('Listado'),
    'icono' => 'iconarg-mod-lupa',
    'tipo_noticia' => '',
    'tipo_tramite' => '',
    'orientacion_de_listado' => 'vertical',
    'todos_los_items' => FALSE,
    'paginador' => 0,
  )
);

/**
* 'admin info' callback for panel pane.
*/
function drupar_componentes_listado_admin_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass;
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : t('Listado de %tipoDeContenido', array('%tipoDeContenido' => $conf['tipos_de_contenido']));
    $block->content = $conf['titulo'];
    return $block;
  }
}

/**
* 'Edit form' callback for the content type.
*/
function drupar_componentes_listado_edit_form($form, &$form_state) {

  foreach (node_type_get_types() as $item) {
    $tiposDeContenido[$item->type] = $item->name;
  }

  $form['titulo'] = array(
    '#title' => t('Título'),
    '#type' => 'textfield',
    '#default_value' => $form_state['conf']['titulo'],
    '#required' => FALSE,
  );
  $form['tipo_de_listado'] = array(
    '#type' => 'select',
    '#title' => t('Tipo de listado'),
    '#options' => array(
      'recientes' => t('Últimos cargados'),
      'relacionados' => t('Relacionados'),
      'custom' => t('Personalizado'),
    ),
    '#default_value' => $form_state['conf']['tipo_de_listado'],
    '#description' => t('Cada tipo de listado tiene un comportamiento diferente'),
  );
  $form['contenido_custom'] = array(
    '#type' => 'entityreference',
    '#title' => t('Contenido personalizado'),
    '#description' => t('Separar cada item a listar con comas'),
    '#default_value' => array_keys($form_state['conf']['contenido_custom']),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="tipo_de_listado"]' => array('value' => 'custom'),
      ),
    ),
    '#era_entity_type' => 'node',
    '#era_bundles' => array(),
    '#era_cardinality' => ERA_CARDINALITY_UNLIMITED,
  );
  $form['entity_types'] = array(
    '#title' => t('Entidad'),
    '#type' => 'radios',
    '#default_value' => $form_state['conf']['entity_types'],
    '#required' => TRUE,
    '#options' => array(
      'nodes' => t('Contenido'),
      'taxonomy_vocabularies' => t('Vocabulario') 
    ),
    '#description' => t('Tipo de contenido a listar'),
    '#states' => array(
      'disabled' => array(
        ':input[name="tipo_de_listado"]' => array('value' => 'custom'),
      ),
    ),
  );
  $form['tipos_de_contenido'] = array(
    '#title' => t('Tipo de contenido'),
    '#type' => 'select',
    '#default_value' => $form_state['conf']['tipos_de_contenido'],
    '#required' => FALSE,
    '#options' => $tiposDeContenido,
    '#description' => t('Tipo de contenido a listar'),
    '#states' => array(
      'disabled' => array(
        ':input[name="tipo_de_listado"]' => array('value' => 'custom'),
      ),
      'visible' => array(
        ':input[name="entity_types"]' => array('value' => 'nodes'),
      ),
    ),
  );
  $form['taxonomy_vocabularies'] = array(
    '#title' => t('Taxonomías'),
    '#type' => 'select',
    '#default_value' => $form_state['conf']['taxonomy_vocabularies'],
    '#required' => FALSE,
    '#options' => obtenerTaxonomyVocabularies(),
    '#description' => t('Vocabulario a listar'),
    '#states' => array(
      'disabled' => array(
        ':input[name="tipo_de_listado"]' => array('value' => 'custom'),
      ),
      'visible' => array(
        ':input[name="entity_types"]' => array('value' => 'taxonomy_vocabularies'),
      ),
    ),
  );
  $form['tipo_tramite'] = array(
    '#type' => 'entityreference',
    '#title' => t('Tipo de Trámites'),
    '#description' => t('Filtrar por tipo de trámite. Dejar vacío para listar todos los trámites'),
    '#default_value' => $form_state['conf']['tipo_tramite'],
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="tipos_de_contenido"]' => array('value' => 'tramite'),
      ),
      'disabled' => array(
        ':input[name="tipo_de_listado"]' => array('value' => 'custom'),
      ),
    ),
    '#era_entity_type' => 'taxonomy_term',  // Mandatory.
    '#era_bundles' => array('tramites'), // Optional (Any bundle by default).
    '#era_cardinality' => 2,       // Optional (1 By default).
  );
  $form['tipo_noticia'] = array(
    '#type' => 'entityreference',
    '#title' => t('Categoría de Noticias'),
    '#default_value' => $form_state['conf']['tipo_noticia'],
    '#description' => t('Filtrar por categoría de noticias. Dejar vacío para listar todos los noticias'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="tipos_de_contenido"]' => array('value' => 'noticia'),
      ),
      'disabled' => array(
        ':input[name="tipo_de_listado"]' => array('value' => 'custom'),
      ),
    ),
    '#era_entity_type' => 'taxonomy_term',  // Mandatory.
    '#era_bundles' => array('noticias'), // Optional (Any bundle by default).
    '#era_cardinality' => 1,       // Optional (1 By default).
  );
  $form['formato_de_listado'] = array(
    '#type' => 'select',
    '#title' => t('Formato de listado'),
    '#options' => array(
      'drupar_componentes_listado_tarjeta_simple' => t('Tarjeta simple'),
      'drupar_componentes_listado_tarjeta_con_imagen' => t('Tarjeta con imagen'),
      'drupar_componentes_listado_lista_simple' => t('Lista simple'),
    ),
    '#default_value' => $form_state['conf']['formato_de_listado'],
    '#description' => t('Cada tipo de listado tiene un comportamiento diferente'),
  );
  $form['opciones_de_visualizacion'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Opciones de visualización'), 
    '#weight' => 5, 
    '#collapsible' => TRUE, 
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        array(
          array(':input[name="formato_de_listado"]' => array('value' => 'drupar_componentes_listado_tarjeta_simple')),
          'or',
          array(':input[name="formato_de_listado"]' => array('value' => 'drupar_componentes_listado_tarjeta_con_imagen')),
        ),
      ),
    ),
  );
  $form['opciones_de_visualizacion']['destacar_primer_fila'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Destacar primer fila'),  
    '#default_value' => $form_state['conf']['destacar_primer_fila'],
    '#description' => t("Si se tilda, los items de la primer fila se destacarán sobre los demás"),
  );
  $form['orientacion_de_listado'] = array(
    '#title' => t('Orientación'),
    '#type' => 'radios',
    '#default_value' => $form_state['conf']['orientacion_de_listado'],
    '#required' => TRUE,
    '#options' => array(
      'vertical' => t("Vertical"),
      'horizontal' => t("Horizontal")
    ),
  );
  $form['cantidad_links'] = array(
    '#type' => 'textfield',
    '#title' => t('Cantidad'),
    '#default_value' => $form_state['conf']['cantidad_links'],
    '#element_validate' => array('element_validate_integer_positive'), 
    '#description' => t('Cantidad máxima de items del listado'),
  );
  $form['todos_los_items'] = array(
    '#type' => 'checkbox',
    '#title' => t('Incluír opción "Todos los items"'),
    '#default_value' => $form_state['conf']['todos_los_items'],
    '#description' => t('Si está tildado se mostrará un link a todos los items del tipo de contenido seleccionado'),
  );
  $form['paginador'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mostrar paginador'),
    '#default_value' => $form_state['conf']['paginador'],
    '#description' => t('Si está tildado y el número de resultados es mayor a la cantidad de items a mostrar por página, se mostrará un paginador'),
  );

  return $form;
}

/**
* The submit form stores the data in $conf.
*/
function drupar_componentes_listado_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
function drupar_componentes_listado_render($subtype, $conf, $panel_args, $context = NULL){
  $tipoNoticia = !empty($conf['tipo_noticia']) ? reset($conf['tipo_noticia']) : array();
  $tipoTramite = !empty($conf['tipo_tramite']) ? reset($conf['tipo_tramite']) : array();
  $block = new stdClass();
  $block->title = '';

  $block->content = !empty(obtenerLinks($conf)) && sizeof(obtenerLinks($conf)) > 0 ? theme(
    $conf["formato_de_listado"] . '_' . $conf["orientacion_de_listado"],
    array(
      "titulo" => isset($conf["titulo"]) && !empty($conf["titulo"]) ? $conf["titulo"] : NULL,
      'links' => obtenerLinks($conf),
      'icono' => $conf['icono'],
      'boton' => crearBoton($conf),
      'paginador' => $conf['paginador'] ? theme("pager") : NULL,
      'destacar_primer_fila' => $conf['destacar_primer_fila']
    )
  ) : NULL;
  return $block;
}

function obtenerLinks($conf){
  if($conf['tipo_de_listado'] == 'custom'){
    foreach ($conf['contenido_custom'] as $key => $item) {
      $links[] = crearLink(node_load($key), $conf["formato_de_listado"] . '_' . $conf["orientacion_de_listado"], $conf);
    }
  } else {
    if($conf['entity_types'] == 'taxonomy_vocabularies'){
      $terms = taxonomy_get_tree($conf['taxonomy_vocabularies'], $parent = 0, $max_depth = NULL, $load_entities = FALSE);
      for ($i=0; $i < $conf['cantidad_links']; $i++) {
        $links[] = crearTaxonomyLink($terms[$i], $conf["formato_de_listado"] . '_' . $conf["orientacion_de_listado"], $conf);
      }
    } else {
      $view = views_get_view('listado_de_contenido');
      $view->set_display('listado_de_contenido_default');
      $view->set_arguments(
        array(
          $conf['tipos_de_contenido'],
          !(empty($conf['tipo_noticia'])) ? $tipoNoticia['entity_id'] : 'all',
          !(empty($conf['tipo_tramite'])) ? $tipoTramite['entity_id'] : 'all',
          $conf['tipo_de_listado'] == 'relacionados' && (esNoticia() || esTramite()) ? arg(1) : 'all',
          $conf['tipo_de_listado'] == 'relacionados' && (esArea() || esPaginaRelacionadaDeNodo()) ? arg(1) : 'all'
        )
      );
      $view->set_items_per_page($conf["cantidad_links"]);
      $view->execute();
      foreach ($view->result as $key => $item) {
        $links[] = crearLink($item->_field_data["nid"]["entity"], $conf["formato_de_listado"] . '_' . $conf["orientacion_de_listado"], $conf);
      }
    }
  }
  return $links;
}

function crearBoton($conf){
  return $conf['todos_los_items'] ? l('Ver todos', obtenerLinkVerTodos($conf), array('attributes' => array('class' => array('btn', 'btn-primary')))) : NULL;
}

function obtenerLinkVerTodos($conf){
  return esRelacionadosDeArea($conf) ? obtenerLinkListadoRelacionado($conf) : obtenerLinkListadoGeneral($conf);
}

function obtenerLinkListadoRelacionado($conf){
  return "node/" . arg(1) . "/" . plural($conf["tipos_de_contenido"]);
}

function obtenerLinkListadoGeneral($conf){
  return "/" . plural($conf["tipos_de_contenido"]);
}

function plural($texto){
  switch ($texto) {
    case 'noticia':
      $val = 'noticias';
      break;
    case 'servicio':
      $val = 'servicios';
      break;
    case 'area':
      $val = 'areas';
      break;
    default:
      $val = $texto . 's';
      break;
  }
  return $val;
}

function esRelacionadosDeArea($conf){
  return (esArea() && $conf['tipo_de_listado'] == 'relacionados');
}

function crearTaxonomyLink($item, $theme, $conf = NULL){
  return l(
    theme(
      $theme . '_item',
      array(
        'titulo' => $item->name,
        'texto' => views_trim_text(
          array(
            'max_length' => 90,
            'ellipsis' => TRUE,
            'word_boundary' => TRUE,
            'html' => TRUE,
          ),
          $item->description
        ),
      )
    ),
    'taxonomy/term/' . $item->tid,
    array(
      'html' => TRUE,
      'attributes' => array(
        'class' => $conf["formato_de_listado"] == "drupar_componentes_listado_lista_simple" ? array() :  array('panel panel-default'),
      )
    )
  );
}

function crearLink($item, $theme, $conf = NULL){
  $link = new stdClass();
  $link->id = $item->nid;
  $link->texto = field_get_items('node', $item, 'body', NULL);
  $link->texto = $link->texto[0]["summary"];
  $link->imagen = field_get_items('node', $item, 'field_featured_image', NULL);
  $link->imagen_path = (isset($link->imagen)) ? image_style_url('listado', $link->imagen[0]["uri"]) : NULL;
  $link->imagen = (isset($link->imagen)) ? theme_image_style(array('style_name' => 'list', 'path' => $link->imagen[0]["uri"], 'width' => '', 'height' => '', 'attributes' => array('class' => array('media-object', 'img-rounded')))) : NULL;
  $link->type = $item->type;
  $link->category = ($link->type == 'tramite' || $link->type == 'noticia') && isset($item->_field_data["nid"]["entity"]) ? obtenerTaxonomia($item, $link->type) : NULL;
  $link->category = isset($link->category) ? taxonomy_term_load($link->category[0]["target_id"]) : NULL;
  $link->category = isset($link->category) && !empty($link->category) ? l($link->category->name, 'taxonomy/term/' . $link->category->tid) : NULL;

  return l(
    theme(
      $theme . '_item',
      array(
        'titulo' => $item->title,
        'texto' => views_trim_text(
          array(
            'max_length' => 90,
            'ellipsis' => TRUE,
            'word_boundary' => TRUE,
            'html' => TRUE,
          ),
          $link->texto
        ),
        'imagen' => $link->imagen,
        'imagen_path' => $link->imagen_path,
        'icono' => $conf['icono'],
        'fecha' => ($conf["tipos_de_contenido"] == 'noticia') ? format_date($item->created, 'custom', 'd \d\e F \d\e o') : NULL
      )
    ),
    'node/' . $item->nid,
    array(
      'html' => TRUE,
      'attributes' => array(
        'class' => $conf["formato_de_listado"] == "drupar_componentes_listado_lista_simple" ? array() :  array('panel panel-default'),
      )
    )
  );
}

function obtenerTaxonomia($item, $tipo){
  return $tipo == 'tramite' ? field_get_items('node', $item->_field_data["nid"]["entity"], 'field_tramite_tipo', NULL) : field_get_items('node', $item->_field_data["nid"]["entity"], 'field_categoria', NULL);
}

function obtenerTaxonomyVocabularies(){
  foreach (taxonomy_get_vocabularies() as $key => $value) {
    $vocabularies[$value->vid] = $value->name;
  }
  return $vocabularies;
}