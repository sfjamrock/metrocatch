DESCRIPTION
--------------------------
Позволяет пользователям запускать тесты для проверки консистентности базы
данных. Если тест завершился неудачей, то пользователю показывается сообщение со
списком идентификаторов неправильных строк в проверяемой таблице. Также, если
тест предоставляет решения для восстановления консистентности, пользователю
будет предложено использовать одно из них.
Модуль DB Consistency предоставляет API для запуска тестов. Также разработчики
могут добавить новые тесты с помощью реализации хука hook_db_test().

REQUIREMENTS
----------------
PHP Version 5.2
Drupal 6.20 or higher

INSTALLATION
---------------
1. Unpack the db_consistency folder and contents in the appropriate modules
   directory of your Drupal installation.  This is probably sites/all/modules/.
2. Enable the DB Consistency module in the administration tools.
3. If you're not using Drupal's default administrative account, make sure
   "test db consistency" is enabled through access control administration.
4. Visit the DB Consistency tests page to view existing tests at
   Administer > Reports > Database consistency tests.

CREATING TESTS
----------------
View examples and description of hook_db_test() implementations in
db_consistency.tests.inc and db_consistency.api.inc, and write your own
hook_db_test() implementation.
