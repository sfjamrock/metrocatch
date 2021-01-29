<?php

/**
 * @file
 * Hook provided by the quickcron module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Perform frequent actions.
 *
 * This hook is similar to hook_cron() but is meant to be called very
 * frequently. Interval is set by administrator and is 1 second by default.
 * Most of time this hook is called within the same script, so use static
 * caches wisely.
 *
 * @see hook_cron()
 */
function hook_quickcron() {
  // Clear form cache in real time.
  cache_clear_all(NULL, 'cache_form');
}

/**
 * @} End of "addtogroup hooks".
 */