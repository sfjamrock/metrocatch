INSTALLATION
------------------------------------------------------------------------------
1. Install quickcron module as described at http://drupal.org/node/70151.

2. Set up crontab to run quickcron. This is similar to what is described at
http://drupal.org/node/23714 but insetad of cron.php script use quickcron.php
which is in the module's directory. For example, if the module is installed in
/sites/all/modules, set up cron to request http://www.example.com/sites/all/modules/quickcron/quickcron.php.
Suggested interval for running quickcron is every ten minutes. Make sure that
max_execution_time php setting is greater than quickcron interval (see
http://www.php.net/manual/en/info.configuration.php), otherwise quickcron will
be interrupted in the middle of execution.

Note: when a new quickcron.php script is started, the previous instance of
quickcron.php finishes execution. You don't need more than one quickcron.php
script per site running at the same time.

CONFIGURATION
------------------------------------------------------------------------------
You can set up quickcron at Administer >> Site configuration >> Quickcron.
"Maximum execution time" setting specifies a timeout after which quickcron
finishes it's execution. It MUST be greater than quickcron interval (otherwise
one quickcron.php instance may finish execution before a new one is started by
cron daemon) and less than max_execution_time (otherwise there is a risk that
cron will be interrupted).

You can start, restart and stop quickckron as well as see quickcron status
at Administer >> Site configuration >> Quickcron.
Note that starting or restarting quickcron with this page is for development
or testing purposes, quickcron won't restart on it's own after it finishes
execution if it is started with this page.
