<!--<div class="program-image">
  <img data-original="<?php //print image_style_url('170x170', $photo); ?>" height="170" width="170" />
</div>-->
<!--<div class="program-header">
  <?php //print $title; ?>
</div>-->
<div class="program-title">
  <?php print $program_title; ?>
</div>
<?php if($start_date != '' && $end_date != '' ) { ?>
<div class="program-date">
  <?php print date_format(date_create($start_date),'d M Y').' – '.date_format(date_create($end_date),'d M Y'); ?>
</div>
<?php } ?>
<?php if ($program_location != '') { ?>
  <div class="program-location">
    <?php print $program_location; ?>
  </div>
<?php } ?>
<!--<div class="program-leadership-level">
  <?php //print $leadership_level; ?>
</div>-->
<div class="program-teaser">
  <?php print $program_teaser; ?>
</div>
<?php if ($program_topic != '') { ?>
  <div class="program-topic">
    <?php print $program_topic; ?>
  </div>
<?php } ?>
