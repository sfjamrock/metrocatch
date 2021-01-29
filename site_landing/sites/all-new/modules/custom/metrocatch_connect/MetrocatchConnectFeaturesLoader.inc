<?php
/**
 * @file
 * Contains MetrocatchConnectFeaturesLoader interface and implementation.
 *
 * This interface allows to easily load features of a given type,
 * for a given user and in a given period and/or segment.
 *
 * Usage example:
 * @code
 *   // Feature duration is 30 days, Subfeature durations is 15 days.
 *   $loader = new MetrocatchConnectFeaturesLoader(30, 15);
 *   // Load 12 active subfeatures in Man-interested-in-women segment
 *   // and shuffle result
 *   $features = $loader
 *    ->type('subfeature')
 *    ->active()
 *    ->segment('man', 'women')
 *    ->limit(12)
 *    ->shuffle()
 *    ->fetchAll();
 * @endcode
 */

/**
 * Defines the common interface for loading Features from database.
 */
interface MetrocatchConnectFeaturesLoaderInterface {

  /**
   * Constructor for this class must take feature durations as arguments.
   *
   * @param integer $feature_duration
   *   Number of days during Feature is active.
   * @param integer $subfeature_duration
   *   Number of days during Subfeature is active.
   */
  public function __construct($feature_duration = NULL, $subfeature_duration = NULL);

  /**
   * Get the features with a given types.
   *
   * @param string $type1
   * @param string $type2[optional]
   * @param _ string[optional]
   */
  public function types($type1, $type2 = NULL, $_ = NULL);

  /**
   * Get features with a given type.
   *
   * @param string $type
   */
  public function type($type);

  /**
   * Get features in a given segment.
   *
   * @param string $sex
   *   Sex indentifier of a given segment.
   * @param string $preference
   *   Preference indentifier of a given segment.
   */
  public function segment($sex, $preference);

  /**
   * Get only today's features.
   */
  public function todays();

  /**
   * Get only active features.
   *
   * 'Active' means that only those features will be loaded,
   * which date is between current time and duration of a feature
   * (depends on feature and subfeature duration params of constructor).
   */
  public function active();

  /**
   * Get features in a given period of time.
   *
   * @param string $type
   *   The type of a feature related to a given period.
   * @param integer $period
   *   UNIX timestamp of a given period of time.
   */
  public function period($type, $period);

  /**
   * Get the features with a given UID.
   *
   * @param integer $uid
   *   The UID of the user whose features to get.
   */
  public function uid($uid);

  /**
   * Get the features with a given UIDs.
   *
   * @param array $uids
   *   An array of user UIDs whose features to get.
   */
  public function uids($uids);

  /**
   * Exclude features with a given user IDs.
   *
   * @param integer|array $uids
   *   An array (or a single) user IDs to exclude.
   */
  public function exclude($uids);

  /**
   * Shuffle result array.
   */
  public function shuffle();

  /**
   * Orders the result set by a given field.
   *
   * @param string $field
   *   The field on which to order.
   * @param string $direction
   *   The direction to sort. Legal values are "ASC" and "DESC".
   */
  public function order($field, $direction = 'ASC');

  /**
   * Set the first record from the result set to return.
   *
   * @param integer $offset
   */
  public function offset($offset);

  /**
   * Limit the result number of loaded features.
   *
   * @param integer $limit
   *   The limit of features to load.
   */
  public function limit($limit);

  /**
   * Fetch a single loaded feature.
   *
   * @return
   *   stdClass object of the loaded feature on success, FALSE otherwise.
   */
  public function fetchSingle();

  /**
   * Fetch all the loaded features.
   *
   * @return
   *   An array containing user IDs in keys and stdClass object of an appropriate features on success
   *   FALSE otherwise.
   */
  public function fetchAll();

  /**
   * Fetch the number of loaded features.
   *
   * @return
   *   An integer containing the number of features.
   */
  public function fetchCount();

}

/**
 * This class can be used for loading necessary Features.
 */
class MetrocatchConnectFeaturesLoader implements MetrocatchConnectFeaturesLoaderInterface {

    /**
     * An array containing feature types in keys and an appropriate durations in values.
     */
    private $durations;

    /**
     * An array containing query params.
     */
    private $params;

    /**
     * Stores result of a database query.
     */
    private $result;

    public function __construct($feature_duration = NULL, $subfeature_duration = NULL) {
      $this->params = array(
        'types' => array(),
        'periods' => array(),
        'uids' => array(),
        'exclude' => array(),
        'shuffle' => FALSE,
        'order' => array(),
        'offset' => 0,
        'limit' => -1,  // -1 means no limiting
      );
      $this->durations = array(
        'feature' => $feature_duration,
        'subfeature' => $subfeature_duration,
      );
      $this->result = NULL;
      // Set the default ordering:
      $this->order('date', 'DESC');
    }

    public function types($type1, $type2 = NULL, $_ = NULL) {
      $types = func_get_args();
      $this->params['types'] = array_merge($this->params['types'], $types);
      return $this;
    }

    public function type($type) {
      $this->types($type);
      return $this;
    }

    public function segment($sex, $preference) {
      $this->params['sex'] = $sex;
      $this->params['preference'] = $preference;
      return $this;
    }

    public function todays() {
      foreach ($this->params['types'] as $type) {
        $this->params['periods'][$type] = 0;
      }
      return $this;
    }

    public function active() {
      foreach ($this->params['types'] as $type) {
        $this->params['periods'][$type] = $this->durations[$type];
      }
      return $this;
    }

    public function period($type, $period) {
      $this->params['periods'][$type] = $period;
      return $this;
    }

    public function uid($uid) {
      $this->params['uids'][] = $uid;
      return $this;
    }

    public function uids($uids) {
      $this->params['uids'] = array_merge($this->params['uids'], $uids);
      return $this;
    }

    public function exclude($uids) {
      if (!is_array($uids)) {
        $this->params['exclude'][] = $uids;
      }
      else {
        $this->params['exclude'] = array_merge($this->params['exclude'], $uids);
      }
      return $this;
    }

    public function shuffle() {
      $this->params['shuffle'] = TRUE;
      return $this;
    }

    public function order($field, $direction = 'ASC') {
      $this->params['order'][] = array(
        'field' => $field,
        'direction' => $direction,
      );
      return $this;
    }

    public function offset($offset) {
      $this->params['offset'] = $offset;
      return $this;
    }

    public function limit($limit) {
      $this->params['limit'] = $limit;
      return $this;
    }

    public function fetchSingle() {
      $this->query();
      $feature = db_fetch_object($this->result);
      return ($feature ? metrocatch_connect_prepare_catch($feature) : FALSE);
    }

    public function fetchAll() {
      $this->query();
      $features = array();
      while ($feature = db_fetch_object($this->result)) {
        $features[$feature->uid] = metrocatch_connect_prepare_catch($feature);
      }
      if (!empty($this->params['shuffle'])) {
        shuffle($features);
      }
      return $features;
    }

    public function fetchCount() {
      $this->query(TRUE);
      return db_result($this->result);
    }

    /**
     * Prepares and runs a query to get necessary features from database.
     *
     * @param bool $count_query
     *   (optional) Boolean indicating whether to build count query. Default is FALSE.
     *
     * @return
     *   A database query result resource, or FALSE if the query was not executed correctly.
     */
    private function query($count_query = FALSE) {
      // The query has already run, fall back.
      if (!empty($this->result)) {
        return;
      }
      if ($count_query) {
        $query = 'SELECT COUNT(DISTINCT(mcc.uid)) AS c';
      }
      else {
        $query = 'SELECT mcc.*';
      }
      $query .= " FROM {metrocatch_connect_catches} mcc
      INNER JOIN {users} u ON u.uid = mcc.uid";
      $wheres = $args = array();
      $wheres[] = 'u.status <> 0';
      if (!empty($this->params['types'])) {
        $or = array();
        foreach ($this->params['types'] as $type) {
          $or[] = "(mcc.type = '%s' AND (DATEDIFF(CURDATE(), mcc.date) BETWEEN 0 AND %d))";
          $args[] = $type;
          $args[] = (isset($this->params['periods'][$type]) ? $this->params['periods'][$type] : 0);
        }
        $wheres[] = '('. implode(' OR ', $or) .')';
      }
      if (!empty($this->params['sex'])) {
        $wheres[] = "mcc.sex = '%s'";
        $args[] = $this->params['sex'];
      }
      if (!empty($this->params['preference'])) {
        $wheres[] = "mcc.sex_preference = '%s'";
        $args[] = $this->params['preference'];
      }
      if (!empty($this->params['uids'])) {
        $wheres[] = 'mcc.uid IN ('. db_placeholders($this->params['uids']) .')';
        $args = array_merge($args, $this->params['uids']);
      }
      if (!empty($this->params['exclude'])) {
        $wheres[] = 'mcc.uid NOT IN ('. db_placeholders($this->params['exclude']) .')';
        $args = array_merge($args, $this->params['exclude']);
      }
      $query .= ' WHERE '. implode(' AND ', $wheres);
      if (!$count_query) {
        if (!empty($this->params['order'])) {
          $orderings = array();
          foreach ($this->params['order'] as $ordering) {
            $orderings[] = $ordering['field'] .' '. $ordering['direction'];
          }
          $query .= ' ORDER BY '. implode(', ', $orderings);
        }
        $subquery = $query;
        // Select only the latest feature for each user.
        $query = 'SELECT * FROM ('. $subquery .') AS t GROUP BY t.uid';
        if ($this->params['limit'] >= 0) {
          $this->result = db_query_range($query, $args, $this->params['offset'], $this->params['limit']);
        }
        else {
          $this->result = db_query($query, $args);
        }
      }
      else {
        $this->result = db_query($query, $args);
      }
    }
}
