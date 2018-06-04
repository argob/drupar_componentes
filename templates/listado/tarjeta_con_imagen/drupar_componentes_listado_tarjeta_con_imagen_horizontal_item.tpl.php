<?php if($imagen_path){ ?><div style="background-image:url(<?php print $imagen_path; ?>);" class="panel-heading"></div><?php } ?>
<div class="panel-body">
  <?php if(isset($fecha)){ ?><time><?php print $fecha; ?></time><?php } ?>
  <h3><?php echo $titulo; ?></h3>
</div>