<?php
/**
 * @file
 * dug_content_type_topic.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dug_content_type_topic_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_node_topic';
  $strongarm->value = '1';
  $export['auto_entitylabel_node_topic'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_node_topic';
  $strongarm->value = '[node:field-headline]';
  $export['auto_entitylabel_pattern_node_topic'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_php_node_topic';
  $strongarm->value = 0;
  $export['auto_entitylabel_php_node_topic'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_topic';
  $strongarm->value = array();
  $export['menu_options_topic'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_topic';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_topic'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_topic';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_topic'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_topic';
  $strongarm->value = 1;
  $export['node_submitted_topic'] = $strongarm;

  return $export;
}
