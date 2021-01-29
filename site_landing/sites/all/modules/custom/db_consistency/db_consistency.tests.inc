<?php
/**
 * @file
 * Create all tests in this modules
 */

/**
 *  Implementation hook_db_test().
 */
function node_db_test() {
  return array(
    'node.authors_exist' => array(
      'title' => t('Missing node authors'),
      'description' => t('Check if nodes belong to nonexistent users.'),
      'table' => 'node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node} n LEFT JOIN {users} u ON n.uid = u.uid WHERE u.uid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT n.nid FROM {node} n LEFT JOIN {users} u ON n.uid = u.uid WHERE u.uid IS NULL",
      'error_list_description' => 'Nids of nodes which belong to missing users: @list',
      'fixes' => array(
        'node_set_admin_author' => array(
          'description' => t('Set superuser as author of wrong nodes.'),
          'query' => "UPDATE {node} n LEFT JOIN {users} u ON n.uid = u.uid SET n.uid = 1 WHERE u.uid IS NULL",
        ),
        'node_set_anonymous_author' => array(
          'description' => t('Set anonymous user as author of wrong nodes.'),
          'query' => "UPDATE {node} n LEFT JOIN {users} u ON n.uid = u.uid SET n.uid = 0 WHERE u.uid IS NULL",
        ),
      ),
    ),
    'node.node_types_exist' => array(
      'title' => t('Missing node types'),
      'description' => t('Check if nodes have nonexistent types'),
      'table' => 'node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node} n LEFT JOIN {node_type} nt ON n.type = nt.type WHERE nt.type IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT n.nid FROM {node} n LEFT JOIN {node_type} nt ON n.type = nt.type WHERE nt.type IS NULL",
      'error_list_description' => 'Nids of nodes with missing types: @list',
      'fixes' => array(
        'node_add_missing_types' => array(
          'description' => t('Create missing node types.'),
          'query' =>
            "INSERT INTO {node_type} (type, name, module, description, has_title, title_label, has_body, body_label, custom, modified)
            SELECT DISTINCT n.type, n.type, 'node', 'Node type created by DB consistency module', 1, 'Title', 1, 'Body', 1, 1 FROM {node} n
            LEFT JOIN {node_type} nt ON n.type = nt.type WHERE nt.type IS NULL",
        ),
      ),
    ),
    'node.node_revisions_exist' => array(
      'title' => t('Missing node revisions'),
      'description' => t('Check if nodes have nonexistent revisions.'),
      'table' => 'node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node} n LEFT JOIN {node_revisions} nr ON (n.vid = nr.vid) WHERE nr.vid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT n.nid FROM {node} n LEFT JOIN {node_revisions} nr ON (n.vid = nr.vid) WHERE nr.vid IS NULL",
      'error_list_description' => 'Nids of nodes with missing revisions: @list',
      'fixes' => array(
        'node_set_last_revision_where_no_revisions' => array(
          'description' => t('Link node to the last revision.'),
          'query' =>
            "UPDATE {node} n
            LEFT JOIN {node_revisions} nr ON n.vid = nr.vid
            LEFT JOIN {node_revisions} nr_any ON nr_any.nid = n.nid
            SET n.vid = (SELECT MAX(nr_i.vid) FROM {node_revisions} nr_i WHERE nr_i.nid = n.nid)
            WHERE ISNULL(nr.vid) AND NOT ISNULL(nr_any.vid)",
        ),
      ),
    ),
    'node.node_vid_consistent' => array(
      'title' => t('Node-node revision consistency'),
      'description' => t('Check if current node revisions actually belong to other nodes.'),
      'table' => 'node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node} n LEFT JOIN {node_revisions} nv ON n.vid=nv.vid WHERE n.nid<>nv.nid",
      'test_type' => 'negative',
      'error_list_query' => "SELECT n.nid FROM {node} n LEFT JOIN {node_revisions} nv ON n.vid=nv.vid WHERE n.nid<>nv.nid",
      'error_list_description' => 'Wrong nid in current revisions of nodes: @list',
      'fixes' => array(
        'node_take_revision_nids_from_referring_nodes' => array(
          'description' => t('Change nids of records in node_revisions table to nids of nodes that refer to them.'),
          'query' =>
            "UPDATE {node_revisions} nv
            LEFT JOIN {node} n ON n.vid = nv.vid
            SET nv.nid=n.nid
            WHERE n.nid<>nv.nid",
        ),
        'node_set_last_revision' => array(
          'description' => t('Change vid of records in node table to vids of their last revisions.'),
          'query' =>
            "UPDATE {node} n
            LEFT JOIN {node_revisions} nr ON n.vid = nr.vid
            LEFT JOIN {node_revisions} nr_any ON nr_any.nid = n.nid
            SET n.vid = (SELECT MAX(nr_i.vid) FROM {node_revisions} nr_i WHERE nr_i.nid = n.nid)
            WHERE n.nid<>nr.nid AND NOT ISNULL(nr_any.vid)",
        ),
      ),
    ),
    'node.revision_nodes_exist' => array(
      'title' => t('Missing nodes in node revisions'),
      'description' => t('Check if node revisions belong to nonexistent nodes.'),
      'table' => 'node_revisions',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node} n LEFT JOIN {node_revisions} nr ON (n.nid = nr.nid) WHERE nr.nid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT nr.vid FROM {node_revisions} nr LEFT JOIN {node} n ON (nr.nid = n.nid) WHERE n.nid IS NULL",
      'error_list_description' => 'Vids of node revisions which belong to missing nodes: @list',
    ),
    'node.node_translation_sources_exist' => array(
      'title' => t('Missing node translation source'),
      'description' => t('Check if nodes have nonexistent translation source nodes.'),
      'table' => 'node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node} n1 LEFT JOIN {node} n2 ON n1.tnid = n2.nid WHERE n2.nid IS NULL AND n1.tnid <> 0",
      'test_type' => 'negative',
      'error_list_query' => "SELECT n1.nid FROM {node} n1 LEFT JOIN {node} n2 ON n1.tnid = n2.nid WHERE n2.nid IS NULL AND n1.tnid <> 0",
      'error_list_description' => 'Nids of nodes with missing translation source: @list',
      'fixes' => array(
        'node_set_translation_source_zero' => array(
          'description' => t('Set translation source nid to zero for wrong nodes.'),
          'query' =>
            "UPDATE {node} n1
            LEFT JOIN {node} n2 ON n1.tnid = n2.nid
            SET n1.tnid = 0
            WHERE n2.nid IS NULL AND n1.tnid <> 0",
        ),
      ),
    ),
    'node.node_format_exist' => array(
      'title' => t('Missing body input formats'),
      'description' => t('Check if node bodies have nonexistent input formats.'),
      'table' => 'node_revisions',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node_revisions} n LEFT JOIN {filter_formats} f ON n.format = f.format WHERE ISNULL(f.format) AND n.format<>0",
      'test_type' => 'negative',
      'error_list_query' => "SELECT n.vid FROM {node_revisions} n LEFT JOIN {filter_formats} f ON n.format = f.format WHERE ISNULL(f.format) AND n.format<>0",
      'error_list_description' => 'Nids of nodes with missing input format: @list',
    ),
    'node.counter_nodes_exist' => array(
      'title' => t('Missing nodes in counter table'),
      'description' => t('Check if node_counter records refer to nonexistent nodes.'),
      'table' => 'node_counter',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node_counter} nc LEFT JOIN {node} n ON nc.nid = n.nid WHERE n.nid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT nc.nid FROM {node_counter} nc LEFT JOIN {node} n ON nc.nid = n.nid WHERE n.nid IS NULL",
      'error_list_description' => 'Nids of records which refer to missing nodes: @list',
      'fixes' => array(
        'node_delete_node_counter_wrong_nid_records' => array(
          'description' => t('Delete wrong records from node_counter table.'),
          'query' =>
            "DELETE nc
            FROM {node_counter} nc
            LEFT JOIN {node} n ON nc.nid = n.nid
            WHERE n.nid IS NULL",
        ),
      ),
    ),
    'node.access_nodes_exists' => array(
      'title' => t('Missing nodes in node_access table'),
      'description' => t('Check if node_access records refers to nonexistent nodes.'),
      'table' => 'node_access',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {node_access} na LEFT JOIN {node} n ON n.nid = na.nid WHERE ISNULL(n.nid) AND na.nid<>0",
      'test_type' => 'negative',
      'error_list_query' => "SELECT DISTINCT(na.nid) FROM {node_access} na LEFT JOIN {node} n ON n.nid = na.nid WHERE ISNULL(n.nid) AND na.nid<>0",
      'error_list_description' => 'Nids of records which refer to missing nodes: @list',
      'fixes' => array(
        'node_delete_node_access_wrong_nid_records' => array(
          'description' => t('Delete records from node_access table where nid is incorrect.'),
          'query' =>
            "DELETE na
            FROM {node_access} na
            LEFT JOIN {node} n ON na.nid = n.nid
            WHERE n.nid IS NULL",
        ),
      ),
    ),
  );
}

/**
 *  Implementation hook_db_test().
 */
function user_db_test() {
  $site_mail = variable_get('site_mail', ini_get('sendmail_from'));
  $site_mail = $site_mail ? $site_mail : 'admin@' . $_SERVER['SERVER_NAME'];
  $time = time();
  $empty_array = serialize(array());
  return array(
    'user.anonymous_exists' => array(
      'title' => t('Anonymous user exists'),
      'description' => t('Check if anonymous user exists in "users" table.'),
      'table' => 'users',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {users} WHERE uid = 0",
      'test_type' => 'positive',
      'fixes' => array(
        'user_add_anonymous' => array(
          'description' => t('Create anonymous user.'),
          'query' => array(
            "INSERT INTO {users} (name, mail) VALUES('', '')",
            "UPDATE {users} u SET u.uid = u.uid - u.uid WHERE u.name = ''",
          ),
        ),
      ),
    ),
    'user.administrator_exists' => array(
      'title' => t('Administrator exists'),
      'description' => t('Check if superuser exists in "users" table.'),
      'table' => 'users',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {users} WHERE uid = 1",
      'test_type' => 'positive',
      'fixes' => array(
        'user_add_administrator' => array(
          'description' => t('Create superuser with login "admin" and password "password".'),
          'query' =>
            "INSERT INTO {users} (uid, name, pass, mail, created, status, data)
              VALUES('1', 'admin', MD5('password'), '$site_mail', '$time', '1', '$empty_array')",
        ),
      ),
    ),
    'user.no_duplicate_names' => array(
      'title' => t('Duplicate usernames'),
      'description' => t('Check if users have duplicate usernames.'),
      'table' => 'users',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {users} u1 LEFT JOIN {users} u2 ON (u1.name = u2.name) WHERE u1.uid<>u2.uid",
      'test_type' => 'negative',
      'error_list_query' => "SELECT u1.uid FROM {users} u1 LEFT JOIN {users} u2 ON (u1.name = u2.name) WHERE u1.uid<>u2.uid",
      'error_list_description' => 'Uids of users with duplicate usernames: @list.',
      'fixes' => array(
        'user_add_name_suffix' => array(
          'description' => t('Add "_{uid}" suffix to duplicate usernames.'),
          'query' =>
            "UPDATE {users} u1
            INNER JOIN {users} u2 ON u1.name = u2.name
            SET u1.name = CONCAT(u1.name, '_', u1.uid)
            WHERE u1.uid <> u2.uid",
        ),
      ),
    ),
    'user.no_duplicate_mails' => array(
      'title' => t('Duplicate mail'),
      'description' => t('Check if users have duplicate email adresses.'),
      'table' => 'users',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {users} u1 LEFT JOIN {users} u2 ON u1.mail = u2.mail WHERE u1.uid<>u2.uid",
      'test_type' => 'negative',
      'error_list_query' => "SELECT DISTINCT u1.uid FROM {users} u1 LEFT JOIN {users} u2 ON u1.mail = u2.mail WHERE u1.uid<>u2.uid",
      'error_list_description' => 'Uids of users with duplicate emails: @list',
    ),
    'user.user_roles_roles_exist' => array(
      'title' => t('Missing roles in user-role relation'),
      'description' => t('Check if users have nonexistent roles.'),
      'table' => 'users_roles',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {users_roles} ur LEFT JOIN {role} r ON ur.rid = r.rid WHERE r.rid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT CONCAT(ur.uid, '-', ur.rid) FROM {users_roles} ur LEFT JOIN {role} r ON ur.rid = r.rid WHERE r.rid IS NULL",
      'error_list_description' => 'Uid-rid pairs of records with missing roles: @list',
      'fixes' => array(
        'user_delete_user_roles_wrong_role_records' => array(
          'description' => t('Delete wrong records from users_roles table.'),
          'query' =>
            "DELETE ur
            FROM {users_roles} ur
            LEFT JOIN {role} r ON ur.rid = r.rid
            WHERE r.rid IS NULL",
        ),
      ),
    ),
    'user.user_roles_users_exist' => array(
      'title' => t('Missing users in user-role relation'),
      'description' => t('Check if roles are attached to nonexistent users.'),
      'table' => 'users_roles',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {users_roles} ur LEFT JOIN {users} u ON ur.uid = u.uid WHERE u.uid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT CONCAT(ur.uid, '-', ur.rid) FROM {users_roles} ur LEFT JOIN {role} r ON ur.rid = r.rid WHERE r.rid IS NULL",
      'error_list_description' => 'Uid-rid pairs of records with missing users: @list',
      'fixes' => array(
        'user_delete_user_roles_wrong_users_records' => array(
          'description' => t('Delete wrong records from users_roles table.'),
          'query' =>
            "DELETE ur
            FROM {users_roles} ur
            LEFT JOIN {users} u ON ur.uid = u.uid
            WHERE u.uid IS NULL",
        ),
      ),
    ),
    'user.permission_roles_exist' => array(
      'title' => t('Missing roles in permissions'),
      'description' => t('Check if permissions are attached to nonexistent roles.'),
      'table' => 'permission',
      'severity' => 'warning',
      'test_query' => "SELECT COUNT(*) FROM {permission} p LEFT JOIN {role} r ON p.rid = r.rid WHERE r.rid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT DISTINCT p.pid FROM {permission} p LEFT JOIN {role} r ON p.rid = r.rid WHERE r.rid IS NULL",
      'error_list_description' => 'Pids of records with missing roles: @list',
      'fixes' => array(
        'user_delete_permission_wrong_roles_records' => array(
          'description' => t('Delete wrong records from permission table.'),
          'query' =>
            "DELETE p
            FROM {permission} p
            LEFT JOIN {role} r ON p.rid = r.rid
            WHERE r.rid IS NULL",
        ),
      ),
    ),
  );
}

/**
 *  Implementation hook_db_test().
 */
function taxonomy_db_test() {
  return array(
    'taxonomy.term_vocabularies_exist' => array(
      'title' => t('Missing vocabularies'),
      'description' => t('Check if terms belong to nonexistent vocabularies.'),
      'table' => 'term_data',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {term_data} td LEFT JOIN {vocabulary} v ON td.vid = v.vid WHERE v.vid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT td.tid FROM {term_data} td LEFT JOIN {vocabulary} v ON td.vid = v.vid WHERE v.vid IS NULL",
      'error_list_description' => 'Tids of records with missing vocabularies: @list',
      'fixes' => array(
        'taxonomy_add_missing_vocabularies' => array(
          'description' => t('Add missing vocabularies'),
          'query' =>
            "INSERT INTO {vocabulary} (vid, name, description, help, relations, hierarchy, multiple, required, tags, module, weight) (
            SELECT DISTINCT(td.vid), 'Vocabulary created by DB consistency module', '', '', 1, 1, 1, 0, 0, 'taxonomy', 0
            FROM {term_data} td LEFT JOIN {vocabulary} v ON td.vid = v.vid WHERE v.vid IS NULL
          )",
        ),
      ),
    ),
    'taxonomy.term_parents_exist' => array(
      'title' => t('Missing parents'),
      'description' => t('Check if terms have nonexistent parent terms.'),
      'table' => 'term_hierarchy',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {term_hierarchy} th LEFT JOIN {term_data} t ON th.parent = t.tid WHERE t.tid IS NULL AND th.parent <> 0",
      'test_type' => 'negative',
      'error_list_query' => "SELECT CONCAT(th.tid, '-', th.parent) FROM {term_hierarchy} th LEFT JOIN {term_data} t ON th.parent = t.tid WHERE t.tid IS NULL AND th.parent <> 0",
      'error_list_description' => 'Tid-parent pairs of records with missing parents: @list',
      'fixes' => array(
        'taxonomy_set_root_parent' => array(
          'description' => t('Set root as parent for wrong terms.'),
          'query' =>
            "UPDATE {term_hierarchy} th
            LEFT JOIN {term_data} td ON th.parent = td.tid
            SET th.parent = 0
            WHERE td.tid IS NULL AND th.parent <> 0",
        ),
      ),
    ),
    'taxonomy.terms_in_hierarchy_exist' => array(
      'title' => t('Missing terms in term hierarchy'),
      'description' => t('Check if terms used in hierarchy exist.'),
      'table' => 'term_hierarchy',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {term_hierarchy} th LEFT JOIN {term_data} t ON th.tid = t.tid WHERE t.tid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT CONCAT(th.tid, '-', th.parent) FROM {term_hierarchy} th LEFT JOIN {term_data} t ON th.tid = t.tid WHERE t.tid IS NULL",
      'error_list_description' => 'Tid-parent pairs of records which refer to missing terms: @list',
      'fixes' => array(
        'taxonomy_delete_term_hierarchy_wrong_term_records' => array(
          'description' => t('Delete wrong records from term_hierarchy table.'),
          'query' =>
            "DELETE th
            FROM {term_hierarchy} th
            LEFT JOIN {term_data} t ON th.tid = t.tid
            WHERE t.tid IS NULL",
        ),
      ),
    ),
    'taxonomy.term_nodes_exist' => array(
      'title' => t('Missing nodes in term-node relation'),
      'description' => t('Check if term belong to nonexistent nodes.'),
      'table' => 'term_node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {term_node} tn LEFT JOIN {node} n ON tn.nid = n.nid WHERE n.nid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT CONCAT(tn.tid, '-', tn.vid) FROM {term_node} tn LEFT JOIN {node} n ON tn.nid = n.nid WHERE n.nid IS NULL",
      'error_list_description' => 'Tid-vid pairs of records which refer to missing nodes: @list',
      'fixes' => array(
        'taxonomy_delete_term_node_wrong_node_records' => array(
          'description' => t('Delete wrong records from term_node table.'),
          'query' =>
            "DELETE tn
            FROM {term_node} tn
            LEFT JOIN {node} n ON tn.nid = n.nid
            WHERE n.nid IS NULL",
        ),
      ),
    ),
    'taxonomy.term_node_revisions_exist' => array(
      'title' => t('Missing node revisions in term-node relation'),
      'description' => t('Check if term belong to nonexistent node revisions.'),
      'table' => 'term_node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {term_node} tn LEFT JOIN {node_revisions} nr ON tn.vid = nr.vid WHERE nr.vid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT CONCAT(tn.tid, '-', tn.vid) FROM {term_node} tn LEFT JOIN {node_revisions} nr ON tn.vid = nr.vid WHERE nr.vid IS NULL",
      'error_list_description' => 'Tid-vid pairs of records which refer to missing node revisions: @list',
      'fixes' => array(
        'taxonomy_delete_term_node_wrong_revision_record' => array(
          'description' => t('Delete wrong records from term_node table.'),
          'query' =>
            "DELETE tn
            FROM {term_node} tn
            LEFT JOIN {node_revisions} nr ON tn.vid = nr.vid
            WHERE nr.vid IS NULL",
        ),
      ),
    ),
    'taxonomy.node_vid_consistent' => array(
      'title' => t('Node-node revision consistency'),
      'description' => t('Check if node revisions referenced in term_node table actually belong to other nodes (i.e. nid is incorrect).'),
      'table' => 'term_node',
      'severity' => 'error',
      'test_query' =>
        "SELECT COUNT(*)
        FROM {term_node} tn
        LEFT JOIN {node_revisions} nr ON nr.nid = tn.nid AND nr.vid = tn.vid
        WHERE ISNULL(nr.nid)",
      'test_type' => 'negative',
      'error_list_query' =>
        "SELECT CONCAT(tn.tid, '-', tn.vid)
        FROM {term_node} tn
        LEFT JOIN {node_revisions} nr ON nr.nid = tn.nid AND nr.vid = tn.vid
        WHERE ISNULL(nr.nid)",
      'error_list_description' => 'Tid-vid pairs of records with wrong nids in term_node table: @list',
      'fixes' => array(
        'taxonomy_take_term_node_nid_from_revision' => array(
          'description' => t('Take nid from corresponding node revision.'),
          'query' =>
            "UPDATE {term_node} tn
            JOIN {node_revisions} nr ON nr.vid = tn.vid
            SET tn.nid = nr.nid
            WHERE nr.nid <> tn.nid",
        ),
        'taxonomy_delete_term_node_wrong_nid_records' => array(
          'description' => t('Delete records from term_node table where nid is incorrect.'),
          'query' =>
            "DELETE tn
            FROM {term_node} tn
            JOIN {node_revisions} nr ON nr.vid = tn.vid
            WHERE nr.nid <> tn.nid",
        ),
      ),
    ),
    'taxonomy.node_terms_exist' => array(
      'title' => t('Missing terms in term-node relation'),
      'description' => t('Check if nodes belong to nonexistent terms.'),
      'table' => 'term_node',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {term_node} tn LEFT JOIN {term_data} t ON tn.tid = t.tid WHERE t.tid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT CONCAT(tn.tid, '-', tn.vid) FROM {term_node} tn LEFT JOIN {term_data} t ON tn.tid = t.tid WHERE t.tid IS NULL",
      'error_list_description' => 'Tid-vid pairs of records which refer to missing terms: @list',
      'fixes' => array(
        'taxonomy_delete_term_node_wrong_term_records' => array(
          'description' => t('Delete wrong records from term_node table.'),
          'query' =>
            "DELETE tn
            FROM {term_node} tn
            LEFT JOIN {term_data} t ON tn.tid = t.tid
            WHERE t.tid IS NULL",
        ),
      ),
    ),
    'taxonomy.related_terms_exist' => array(
      'title' => t('Missing related terms'),
      'description' => t('Check if terms relate to nonexistent terms.'),
      'table' => 'term_relation',
      'severity' => 'error',
      'test_query' =>
        "SELECT COUNT(*)
        FROM {term_relation} tr
        LEFT JOIN {term_data} t1 ON tr.tid1 = t1.tid
        LEFT JOIN {term_data} t2 ON tr.tid2 = t2.tid
        WHERE t1.tid IS NULL OR t2.tid IS NULL",
      'test_type' => 'negative',
      'error_list_query' =>
        "SELECT CONCAT(tr.tid1, '-', tr.tid2)
        FROM {term_relation} tr
        LEFT JOIN {term_data} t1 ON tr.tid1 = t1.tid
        LEFT JOIN {term_data} t2 ON tr.tid2 = t2.tid
        WHERE t1.tid IS NULL OR t2.tid IS NULL",
      'error_list_description' => 'Tid1-tid2 pairs of terms one of which doesn\'t exist: @list',
      'fixes' => array(
        'taxonomy_delete_term_relation_wrong_relation_records' => array(
          'description' => t('Delete wrong records from term_relation table.'),
          'query' =>
            "DELETE tr
            FROM {term_relation} tr
            LEFT JOIN {term_data} t1 ON tr.tid1 = t1.tid
            LEFT JOIN {term_data} t2 ON tr.tid2 = t2.tid
            WHERE t1.tid IS NULL OR t2.tid IS NULL",
        ),
      ),
    ),
    'taxonomy.synonym_terms_exist' => array(
      'title' => t('Missing synonym terms'),
      'description' => t('Check if there are synonyms of nonexistent terms.'),
      'table' => 'term_synonym',
      'severity' => 'error',
      'test_query' => "SELECT COUNT(*) FROM {term_synonym} ts LEFT JOIN {term_data} t ON ts.tid = t.tid WHERE t.tid IS NULL",
      'test_type' => 'negative',
      'error_list_query' => "SELECT ts.tsid FROM {term_synonym} ts LEFT JOIN {term_data} t ON ts.tid = t.tid WHERE t.tid IS NULL",
      'error_list_description' => 'Tsids of records which refer to missing terms: @list',
      'fixes' => array(
        'taxonomy_delete_term_synonym_wrong_term_records' => array(
          'description' => t('Delete wrong records from term_synonym table.'),
          'query' =>
            "DELETE ts
            FROM {term_synonym} ts
            LEFT JOIN {term_data} td ON ts.tid = td.tid
            WHERE td.tid IS NULL",
        ),
      ),
    ),
    'taxonomy.vocabulary_for_node_types_exist' => array(
      'title' => t('Missing vocabularies in vocabulary-node type relation'),
      'description' => t('Check if nonexistent vocabularies are assigned to node types.'),
      'table' => 'vocabulary_node_types',
      'severity' => 'error',
      'test_query' =>
        "SELECT COUNT(*)
        FROM {vocabulary_node_types} vnt
        LEFT JOIN {vocabulary} v ON vnt.vid = v.vid
        WHERE v.vid IS NULL",
      'test_type' => 'negative',
      'error_list_query' =>
        "SELECT CONCAT(vnt.vid, '-', vnt.type)
        FROM {vocabulary_node_types} vnt
        LEFT JOIN {vocabulary} v ON vnt.vid = v.vid
        WHERE v.vid IS NULL",
      'error_list_description' => 'Vid-type pairs of records which refer to missing vocabularies: @list',
      'fixes' => array(
        'taxonomy_delete_vocabulary_node_types_wrong_vocabulary_records' => array(
          'description' => t('Delete wrong records from vocabulary_node_types table.'),
          'query' =>
            "DELETE vnt
            FROM {vocabulary_node_types} vnt
            LEFT JOIN {vocabulary} v ON vnt.vid = v.vid
            WHERE v.vid IS NULL",
        ),
      ),
    ),
    'taxonomy.vocabulaty_node_types_exist' => array(
      'title' => t('Missing node types in vocabulary-node type relation'),
      'description' => t('Check if vocabularies are assigned to nonexistent node types.'),
      'table' => 'vocabulary_node_types',
      'severity' => 'error',
      'test_query' =>
        "SELECT COUNT(*)
        FROM {vocabulary_node_types} vnt
        LEFT JOIN {node_type} nt ON vnt.type = nt.type
        WHERE nt.type IS NULL",
      'test_type' => 'negative',
      'error_list_query' =>
        "SELECT CONCAT(vnt.vid, '-', vnt.type)
        FROM {vocabulary_node_types} vnt
        LEFT JOIN {node_type} nt ON vnt.type = nt.type
        WHERE nt.type IS NULL",
      'error_list_description' => 'Vid-type pairs of records which refer to missing node types: @list',
      'fixes' => array(
        'taxonomy_delete_vocabulary_node_types_wrong_type_records' => array(
          'description' => t('Delete wrong records from vocabulary_node_types table.'),
          'query' =>
            "DELETE vnt
            FROM {vocabulary_node_types} vnt
            LEFT JOIN {node_type} nt ON vnt.type = nt.type
            WHERE nt.type IS NULL",
        ),
      ),
    ),
    'taxonomy.vocabulary_multiple_check' => array(
      'title' => t('Multiple term check'),
      'description' => t('Check for multiple term select (for single-term vocabularies).'),
      'table' => 'term_node',
      'severity' => 'warning',
      'test_query' =>
        "SELECT COUNT(*) FROM (
          SELECT COUNT(tn.nid) as cnt, tn.nid AS nid
          FROM {term_node} tn
          INNER JOIN {term_data} td ON td.tid=tn.tid
          INNER JOIN {vocabulary} v ON v.vid=td.vid
          WHERE v.multiple=1
          GROUP BY v.vid, tn.nid
        ) tmp
        WHERE tmp.cnt > 1",
      'test_type' => 'negative',
      'error_list_query' =>
        "SELECT CONCAT(tmp.vid, '-', tmp.nid) FROM (
          SELECT COUNT(tn.nid) AS cnt, tn.nid AS nid, v.vid AS vid
          FROM {term_node} tn
          INNER JOIN {term_data} td ON td.tid=tn.tid
          INNER JOIN {vocabulary} v ON v.vid=td.vid
          WHERE v.multiple=1
          GROUP BY v.vid, tn.nid
         ) tmp
         WHERE tmp.cnt > 1",
      'error_list_description' => 'Vid-nid pairs of nodes with mutiple terms: @list',
    ),
    'taxonomy.required_terms_check' => array(
      'title' => t('Required term check'),
      'description' => t('Check for nodes, which not have any term from "required" vocabulary.'),
      'table' => 'term_node',
      'severity' => 'warning',
      'test_query' =>
        "SELECT COUNT(*) FROM (
          SELECT COUNT(td.tid) AS term_count, v.vid as vid, n.nid AS nid
          FROM {node} n
          INNER JOIN {vocabulary_node_types} vt ON vt.type = n.type
          INNER JOIN {vocabulary} v ON v.vid = vt.vid
          LEFT JOIN {term_node} tn ON tn.nid = n.nid
          LEFT JOIN {term_data} td ON td.tid = tn.tid AND td.vid = v.vid
          WHERE v.required = 1
          GROUP BY v.vid, n.nid
        ) tmp
        WHERE tmp.term_count = 0",
      'test_type' => 'negative',
      'error_list_query' =>
        "SELECT CONCAT(tmp.vid, '-', tmp.nid) FROM (
          SELECT COUNT(td.tid) AS term_count, v.vid as vid, n.nid AS nid
          FROM {node} n
          INNER JOIN {vocabulary_node_types} vt ON vt.type = n.type
          INNER JOIN {vocabulary} v ON v.vid = vt.vid
          LEFT JOIN {term_node} tn ON tn.nid = n.nid
          LEFT JOIN {term_data} td ON td.tid = tn.tid AND td.vid = v.vid
          WHERE v.required = 1
          GROUP BY v.vid, n.nid
        ) tmp
        WHERE tmp.term_count = 0",
      'error_list_description' => 'Vid-nid pairs of nodes without required terms: @list',
    ),
  );
}
