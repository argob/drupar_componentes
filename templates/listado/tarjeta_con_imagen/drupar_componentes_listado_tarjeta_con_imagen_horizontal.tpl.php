<h2 class="h3 m-b-2"><?php echo $titulo; ?></h2>

<?php if($destacar_primer_fila) { ?>
	<div class="row panels-row">
		<?php for ($i=0; $i < 3; $i++) { ?>
		<div class="col-xs-12 col-sm-12 col-md-4">
			<?php print array_shift($links); ?>
		</div>
		<?php } ?>
	</div>
<?php } ?>

<?php foreach (array_chunk($links, 4) as $key => $row) { ?>
  <div class="row panels-row">
  <?php foreach ($row as $item) { ?>
    <div class="col-xs-12 col-sm-3">
      <?php echo $item; ?>
    </div>
  <?php } ?>
  </div>
  <?php } ?>
<?php if(isset($boton)) { print $boton; } ?>
<?php if(isset($paginador)) { print $paginador; } ?>