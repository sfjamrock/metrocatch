<?php

/**
 * @file
 * Dump of profiling related functions / classes.
 */

class CYDUtilsTimeIntervalEvent {
  const StartEvent = 0;
  const EndEvent = 1;
  const InfoEvent = 2;

  public $time;
  public $name;
  public $type;
  public $description;

  public function __construct($type, $name, $description = NULL) {
    $this->time = microtime(TRUE);
    $this->name = $name;
    $this->type = $type;
    $this->description = $description;
  }
}

class CYDUtilsEventSet {
  private $events = array();

  public function start($eventName, $description = NULL) {
    if (isset($description) && !is_string($description)) {
      unset($description);
    }
    $this->events[] = new CYDUtilsTimeIntervalEvent(CYDUtilsTimeIntervalEvent::StartEvent, $eventName, $description);
  }

  public function stop($eventName) {
    $this->events[] = new CYDUtilsTimeIntervalEvent(CYDUtilsTimeIntervalEvent::EndEvent, $eventName);
  }

  public function outputEvents() {
    $resultTree = array();
    $eventStack = array(&$resultTree);
    foreach ($this->events as $event) {
      switch ($event->type) {
        case CYDUtilsTimeIntervalEvent::StartEvent:
          $stackLength = sizeof($eventStack);
          $eventInfo = array('event' => &$event, 'children' => array());
          $eventStack[$stackLength - 1]['children'][] = &$eventInfo;
          $eventStack[] = &$eventInfo;
          unset($eventInfo);
          unset($event);
          break;
        case CYDUtilsTimeIntervalEvent::EndEvent:
          $stackLength = sizeof($eventStack);
          if ($event->name == $eventStack[$stackLength - 1]['event']->name) {
            $eventStack[$stackLength - 1]['interval'] = $event->time - $eventStack[$stackLength - 1]['event']->time;
            array_pop($eventStack);
          }
          break;
      }
    }
    $current_time = microtime(TRUE);
    for ($i = sizeof($eventStack) - 1; $i > 1; --$i) {
      $eventStack[$i]['interval'] = $current_time - $eventStack[$i]['event']->time;
    }
    if (!empty($resultTree)) {
      $this->prepareEventTree($resultTree);
    }
    unset($resultTree['info']);

    // TODO: Remove devel dependency.
    dpm('Time:');
    dpm($resultTree);
  }

  public function resetEvents() {
    $this->events = array();
  }

  public function flushEvents() {
    $this->outputEvents();
    $this->resetEvents();
  }

  private function prepareEventTree(&$tree) {
    foreach ($tree['children'] as &$child) {
      $this->prepareEventTree($child);
      if (isset($tree['interval'])) {
        $child['info'] .= ' | '. ($child['interval'] / $tree['interval'] * 100.0) .'%';
      }
      unset($child['interval']);
    }
    $tree['info'] = $tree['event']->name .'('. $tree['event']->description .') '. $tree['interval'];
    $tree = array_merge($tree, $tree['children']);
    unset($tree['event']);
    unset($tree['children']);
  }
}

/**
 * Get default event set instance.
 * @return CYDUtilsEventSet
 */
function cyd_utils_event_set() {
  static $default_set = NULL;
  if (!isset($default_set)) {
    $default_set = new CYDUtilsEventSet();
  }
  return $default_set;
}

/*
// In hook_init():
  if (variable_get('cyd_utils_store_time_information', FALSE)) {
    cyd_utils_event_set()->start('drupal');
  }
*/

/*
// In shutdown_handler():
  if (variable_get('cyd_utils_store_time_information', FALSE)) {
    cyd_utils_event_set()->stop('drupal');
    cyd_utils_event_set()->flushEvents();
  }
*/