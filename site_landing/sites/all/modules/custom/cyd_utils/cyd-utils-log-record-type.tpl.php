<?php
/**
 * @file
 * template for "record type" field (used in log pages)
 *
 * Variables:
 * - $record: log record, as fetched from DB
 */
?>
<span class="<?php print $record->is_warning ? 'cyd-warning' : 'cyd-error'; ?>">
</span>


