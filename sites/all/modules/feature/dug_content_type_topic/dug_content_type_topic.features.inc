<?php
/**
 * @file
 * dug_content_type_topic.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function dug_content_type_topic_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function dug_content_type_topic_node_info() {
  $items = array(
    'topic' => array(
      'name' => t('Thema'),
      'base' => 'node_content',
      'description' => t('Ein Themenvorschlag, Sessionankündigung, Sessionwunsch oder Idee'),
      'has_title' => '1',
      'title_label' => t('Titel'),
      'help' => '',
    ),
  );
  return $items;
}
