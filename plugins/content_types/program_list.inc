<?php

$plugin = array(
  'title' => t('Program Listing'),
  'category' => t('Custom Content & Features'),
  'single' => TRUE,
  'all contexts' => TRUE,
);

/**
 * Implements hook_PLUGIN_content_type_render().
 */
function gsb_program_finder_program_list_content_type_render($subtype, $conf, $panel_args, $context, $incoming) {
  $select = db_select('node','n');
  $select->fields('n', array('title', 'nid'));
  $select->condition('n.status', 1);
  $select->condition('n.type', 'program_instance');
  //Program Title
  $select->leftJoin('field_data_field_program', 'p', 'n.nid = p.entity_id AND p.deleted = 0');
  $select->leftJoin('node', 'n2', 'n2.nid = p.field_program_target_id');
  $select->addField('n2', 'title', 'program_title');
  //Program Teaser
  $select->leftJoin('field_data_field_overview_teaser', 'ot', 'n2.nid = ot.entity_id AND ot.deleted = 0');
  $select->addField('ot', 'field_overview_teaser_value', 'program_teaser');
  //Dates
  $select->leftJoin('field_data_field_instance_date', 'dt', 'n.nid = dt.entity_id AND dt.deleted = 0');
  $select->addField('dt', 'field_instance_date_value', 'start_date');
  $select->addField('dt', 'field_instance_date_value2', 'end_date');

  //Topics
  //field_data_field_program_topics
  $select->leftJoin('field_data_field_program_topics', 'pt', 'n2.nid = pt.entity_id AND pt.deleted = 0');
  $select->leftJoin('taxonomy_term_data', 'ttd_pt', 'pt.field_program_topics_tid = ttd_pt.tid');
  $select->addField('ttd_pt', 'tid', 'program_topics_tid');
  $select->addField('ttd_pt', 'name', 'program_topics');
  if (!empty($_GET['filter_program_topics'])) {
    $condition = db_or();
    foreach($_GET['filter_program_topics'] as $value) {

      $term_parts = explode('--', $value);
      $tid = (int) array_pop($term_parts);
      $condition->condition('ttd_pt.tid', $tid);
    }
    $select->condition($condition);
  }

  //Leadership Level
  $select->leftJoin('field_data_field_leadership_level', 'll', 'n2.nid = ll.entity_id AND ll.deleted = 0');
  $select->leftJoin('taxonomy_term_data', 'ttd_ll', 'll.field_leadership_level_tid = ttd_ll.tid');
  $select->addField('ttd_ll', 'tid', 'leadership_level_tid');
  $select->addField('ttd_ll', 'name', 'leadership_levels');
  if (!empty($_GET['filter_leadership_levels'])) {
    $condition = db_or();
    foreach($_GET['filter_leadership_levels'] as $value) {

      $term_parts = explode('--', $value);
      $tid = (int) array_pop($term_parts);
      $condition->condition('ttd_ll.tid', $tid);
    }
    $select->condition($condition);
  }

  //Location
  $select->leftJoin('field_data_field_program_location', 'pl', 'n.nid = pl.entity_id');
  $select->leftJoin('field_data_field_program_location_category', 'loc', 'pl.field_program_location_value = loc.entity_id AND loc.deleted = 0');
  $select->leftJoin('taxonomy_term_data', 'ttd_loc', 'loc.field_program_location_category_tid = ttd_loc.tid');
  $select->addField('ttd_loc', 'tid', 'program_location_category_tid');
  $select->addField('ttd_loc', 'name', 'program_locations');

  if (!empty($_GET['filter_locations'])) {
    $condition = db_or();
    foreach($_GET['filter_locations'] as $value) {
      $term_parts = explode('--', $value);
      $tid = (int) array_pop($term_parts);
      $condition->condition('ttd_loc.tid', $tid);
    }

    $select->condition($condition);
  }
  //Search
  if (!empty($_GET['search'])) {
    $select->leftJoin('field_data_field_search_field', 'sf', 'n.nid = sf.entity_id AND sf.deleted = 0');
    $select->condition('sf.field_search_field_value', '%' . $_GET['search'] . '%', 'LIKE');
  }
  $select->orderBy('title','ASC');
  $query = $select->execute();
  $items = '';
  $leadership_levels = array();
  $program_topics = array();
  $program_locations = array();
  $program = array();

  while ($result = $query->fetchObject()) {
    $nid = $result->nid;
    $program[$nid]['title'] = $result->title;
    $program[$nid]['program_title'] = $result->program_title;
    $program[$nid]['program_topic'] = $result->program_topics;
    $program[$nid]['leadership_level'] = $result->leadership_levels;
    $program[$nid]['program_location'] = $result->program_locations;
    $program[$nid]['program_teaser'] = $result->program_teaser;
    $program[$nid]['start_date'] = ($result->start_date);
    $program[$nid]['end_date'] = ($result->end_date);
  }

  foreach ($program as $nid => $data) {
    $program_topic = $data['program_topic'];
    if (!empty($data['program_topic'])) {
      $program_topic .= ', ' . $data['program_topic'];
    }
    $leadership_level = $data['leadership_level'];
    if (!empty($data['leadership_level'])) {
      $leadership_level .= ', ' . $data['leadership_level'];
    }
    $program_location = $data['program_location'];
    if (!empty($data['program_location'])) {
      $program_location .= ', ' . $data['program_location'];
    }
    $variables = array(
      'title' => $data['title'],
      'program_title' => $data['program_title'],
      'program_topic' => $data['program_topic'],
      'leadership_level' => $data['leadership_level'],
      'program_location' => $data['program_location'],
      'program_teaser' => $data['program_teaser'],
      'start_date' => $data['start_date'],
      'end_date' => $data['end_date'],
      //'link' => $data['link'],
      'nid' => $nid,
    );


    // Convert back to desired date format
    $date =  date('Ymd', strtotime($data['start_date']));
    $parameters = array(
      'data' => theme('program_isotope_item', $variables),
      'item-id' => $nid,
      'filters' => array(
        'daterange' => array($date),
      ),
    );

    if (!empty($data['program_topic'])) {
      $parameters['filters']['program_topic'][] = $data['program_topic'];
    }

    if (!empty($data['leadership_level'])) {
      $parameters['filters']['leadership_level'][] = $data['leadership_level'];
    }

    if (!empty($data['program_location'])) {
      $parameters['filters']['program_location'][] = $data['program_location'];
    }

    $items[] = $parameters;
  }

  // Get all Program Topics
  $program_topics_query = db_select('taxonomy_term_data', 'ttd');
  $program_topics_query->fields('ttd', array('tid', 'name'));
  $program_topics_query->leftJoin('taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');
  $program_topics_query->condition('tv.machine_name', 'program_topic');
  $program_topics_query->orderBy('ttd.name', 'ASC');
  $program_topics_results = $program_topics_query->execute();

  while ($program_topic = $program_topics_results->fetchAssoc()) {
    $key = drupal_html_class($program_topic['name']);
    $program_topics[$key] = $program_topic['name'];
  }
  asort($program_topics);
  // Get all leadership levels
  $leadership_level_query = db_select('taxonomy_term_data', 'ttd');
  $leadership_level_query->fields('ttd', array('tid', 'name'));
  $leadership_level_query->leftJoin('taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');
  $leadership_level_query->condition('tv.machine_name', 'leadership_level');
  $leadership_level_query->orderBy('ttd.name', 'ASC');
  $leadership_level_results = $leadership_level_query->execute();

  while ($leadership_level = $leadership_level_results->fetchAssoc()) {
    $key = drupal_html_class($leadership_level['name']);
    $leadership_levels[$key] = $leadership_level['name'];
  }
  asort($leadership_levels);
  // Get all Locations
  $location_query = db_select('taxonomy_term_data', 'ttd');
  $location_query->fields('ttd', array('tid', 'name'));
  $location_query->leftJoin('taxonomy_vocabulary', 'tv', 'tv.vid = ttd.vid');
  $location_query->condition('tv.machine_name', 'program_location');
  $location_query->orderBy('ttd.name', 'ASC');
  $location_results = $location_query->execute();

  while ($program_location = $location_results->fetchAssoc()) {
    $key = drupal_html_class($program_location['name']);
    $program_locations[$key] = $program_location['name'];
  }
  asort($program_locations);

  $options = array(
    'filters' => array(
      'search' => array(
        'type' => 'search',
        'placeholder' => t('search by name, research interests, or other keywords'),
      ),
      'submit' => array(
        'type' => 'submit',
        'title' => t('Search'),
      ),
      'date_range' => array(
        'type' => 'date range',
        'title' => t('Date Range'),
        'date_from_title' => t('Date From (Format: YYYYMMDD)'),
        'date_to_title' => t('Date To (Format: YYYYMMDD)'),
        'default' => '',
      ),
      'program_topic' => array(
        'type' => 'checkboxes',
        'title' => t('Program Topics'),
        'options' => $program_topics,
      ),
      'leadership_level' => array(
        'type' => 'checkboxes',
        'title' => t('Leadership Levels'),
        'options' => $leadership_levels,
      ),
      'program_location' => array(
        'type' => 'checkboxes',
        'title' => t('Locations'),
        'options' => $program_locations,
      ),
    ),
    'settings' => array(
      'callback' => '/program-search',
      'lazyLoad' => TRUE,
    ),
    'id' => 'program_list',
  );
  $output = isotopify_prepare($items, $options);
  return (object) array(
    'content' => array(
      '#markup' => $output,
    ),
  );
}