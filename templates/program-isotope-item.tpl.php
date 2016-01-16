<!--<div class="program-image">
  <img data-original="<?php //print image_style_url('170x170', $photo); ?>" height="170" width="170" />
</div>-->
<!--<div class="program-title" style="padding-right:300px">
  <?php //print $title; ?>
</div>-->
<div class="program-title" style="padding-right:300px;font-weight: bold;margin-top:20px">
  <?php print $program_title; ?>
</div>
<?php if($start_date != '' && $end_date != '' ) { ?>
<div class="program-date" style="padding-right:300px">
  <?php print date_format(date_create($start_date),'d M Y').' - '.date_format(date_create($end_date),'d M Y'); ?>
</div>
<?php } ?>
<?php if ($program_location != '') { ?>
  <div class="program-location" style="padding-right:300px;font-weight: bold;">
    <?php print $program_location; ?>
  </div>
<?php } ?>
<!--<div class="leadership-level" style="padding-right:300px">
  <?php //print $leadership_level; ?>
</div>-->
<div class="program-teaser" style="padding-right:300px">
  <?php print $program_teaser; ?>
</div>
<?php if ($program_topic != '') { ?>
  <div class="program-topic" style="padding-right:300px;font-weight: bold;">
    <?php print $program_topic; ?>
  </div>
<?php } ?>
