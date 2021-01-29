<?php
/**
 * @file
 * Theme implementation for log types.
 *
 * Variables:
 * - $record: log record, as fetched from DB
 */
?>
<span class="<?php print $record->is_warning ? 'cyd-warning' : 'cyd-error'; ?>">
</span>
