<?php print $breadcrumb; ?>
<div class="col-md-8 col-md-offset-2">
  <h1><?php print $campos['titulo']; ?></h1>
  <div class="row">
    <div class="section-actions col-md-7 social-share">
      <p>Compartí <br>este servicio</p>
      <ul class="list-inline">
        <li><a href="#"><i class="fa fa-facebook"></i></a></li>
        <li><a href="#"><i class="fa fa-twitter "></i></a></li>
        <li><a href="#"><i class="fa fa-google-plus"></i></a></li>
        <li><a href="#"><i class="fa fa-print"></i></a></li>
        <li><a href="#"><i class="fa fa-envelope-o"></i></a></li>
      </ul>
    </div>
    <div class="col-md-5 additional_data">
      <h3 class="text-success"><?php print $campos['costo']; ?></h3>
    </div>
  </div>
  <hr>
  <p class="text-center">
    <a href="#" class="btn btn-primary"><i class="fa fa-play"></i>&nbsp; Iniciar</a> &nbsp; &nbsp;
    <a href="#" class="btn btn-primary"><i class="fa fa-info-circle"></i>&nbsp; Consultar</a> &nbsp; &nbsp;
    <a href="#" class="btn btn-primary"><i class="fa fa-calendar-check-o"></i>&nbsp; Solicitar turno</a>
  </p>
</div>