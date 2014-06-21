<?php

/**
 * @file
 * FeedsUserProcessor class.
 */

/**
 * Feeds processor plugin. Create users from feed items.
 */
class FeedsUserProcessor extends FeedsProcessor {
  /**
   * Unencrypted password.
   */
  const PASS_UNENCRYPTED = 'none';

  /**
   * MD5 encrypted password.
   */
  const PASS_MD5 = 'md5';

  /**
   * SHA512 encrypted password.
   */
  const PASS_SHA512 = 'sha512';

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'user';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Users');
    return $info;
  }

  /**
   * Creates a new user account in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $account = new stdClass();
    $account->uid = 0;
    $account->roles = array_filter($this->config['roles']);
    $account->status = $this->config['status'];
    return $account;
  }

  /**
   * Loads an existing user.
   */
  protected function entityLoad(FeedsSource $source, $uid) {
    $user = parent::entityLoad($source, $uid);

    // Copy the password so that we can compare it again at save.
    $user->feeds_original_pass = $user->pass;
    return $user;
  }

  /**
   * Validates a user account.
   */
  protected function entityValidate($account) {
    if (empty($account->name) || empty($account->mail) || !valid_email_address($account->mail)) {
      throw new FeedsValidationException(t('User name missing or email not valid.'));
    }
  }

  /**
   * Save a user account.
   */
  protected function entitySave($account) {
    if ($this->config['defuse_mail']) {
      $account->mail = $account->mail . '_test';
    }

    $edit = (array) $account;

    // Remove pass from $edit if the password is unchanged.
    if (isset($account->feeds_original_pass) && $account->pass == $account->feeds_original_pass) {
      unset($edit['pass']);
    }

    user_save($account, $edit);
    if ($account->uid && !empty($account->pass_crypted)) {
      db_update('users')
        ->fields(array('pass' => $account->pass_crypted))
        ->condition('uid', $account->uid)
        ->execute();
    }
    if ($account->uid && !empty($account->openid)) {
      $authmap = array(
        'uid' => $account->uid,
        'module' => 'openid',
        'authname' => $account->openid,
      );
      if (SAVED_UPDATED != drupal_write_record('authmap', $authmap, array('uid', 'module'))) {
        drupal_write_record('authmap', $authmap);
      }
    }
  }

  /**
   * Delete multiple user accounts.
   */
  protected function entityDeleteMultiple($uids) {
    user_delete_multiple($uids);
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'roles' => array(),
      'status' => 1,
      'defuse_mail' => FALSE,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#description' => t('Select whether users should be imported active or blocked.'),
      '#options' => array(0 => t('Blocked'), 1 => t('Active')),
      '#default_value' => $this->config['status'],
    );

    $roles = user_roles(TRUE);
    unset($roles[2]);
    if (count($roles)) {
      $form['roles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Additional roles'),
        '#description' => t('Every user is assigned the "authenticated user" role. Select additional roles here.'),
        '#default_value' => $this->config['roles'],
        '#options' => $roles,
      );
    }
    $form['defuse_mail'] = array(
      '#type' => 'checkbox',
      '#title' => t('Defuse e-mail addresses'),
      '#description' => t('This appends _test to all imported e-mail addresses to ensure they cannot be used as recipients.'),
      '#default_value' => $this->config['defuse_mail'],
    );
    return $form;
  }

  /**
   * Override setTargetElement() to operate on a target item that is an user.
   */
  public function setTargetElement(FeedsSource $source, $target_user, $target_element, $value, $mapping = array()) {
    switch ($target_element) {
      case 'pass':
        $this->setPassTarget($source, $target_user, $target_element, $value, $mapping);
        break;
      case 'created':
        $target_user->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;
      case 'language':
        $target_user->language = strtolower($value);
        break;
      default:
        parent::setTargetElement($source, $target_user, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'name' => array(
        'name' => t('User name'),
        'description' => t('Name of the user.'),
        'optional_unique' => TRUE,
       ),
      'mail' => array(
        'name' => t('Email address'),
        'description' => t('Email address of the user.'),
        'optional_unique' => TRUE,
       ),
      'created' => array(
        'name' => t('Created date'),
        'description' => t('The created (e. g. joined) data of the user.'),
       ),
      'pass' => array(
        'name' => t('Password'),
        'description' => t('The user password.'),
        'summary_callback' => array($this, 'passSummaryCallback'),
        'form_callback' => array($this, 'passFormCallback'),
      ),
      'status' => array(
        'name' => t('Account status'),
        'description' => t('Whether a user is active or not. 1 stands for active, 0 for blocked.'),
      ),
      'language' => array(
        'name' => t('User language'),
        'description' => t('Default language for the user.'),
      ),
    );
    if (module_exists('openid')) {
      $targets['openid'] = array(
        'name' => t('OpenID identifier'),
        'description' => t('The OpenID identifier of the user. <strong>CAUTION:</strong> Use only for migration purposes, misconfiguration of the OpenID identifier can lead to severe security breaches like users gaining access to accounts other than their own.'),
        'optional_unique' => TRUE,
       );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->bundle();
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }

  /**
   * Get id of an existing feed item term if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($uid = parent::existingEntityId($source, $result)) {
      return $uid;
    }

    // Iterate through all unique targets and try to find a user for the
    // target's value.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'name':
          $uid = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $value))->fetchField();
          break;
        case 'mail':
          $uid = db_query("SELECT uid FROM {users} WHERE mail = :mail", array(':mail' => $value))->fetchField();
          break;
        case 'openid':
          $uid = db_query("SELECT uid FROM {authmap} WHERE authname = :authname AND module = 'openid'", array(':authname' => $value))->fetchField();
          break;
      }
      if ($uid) {
        // Return with the first nid found.
        return $uid;
      }
    }
    return 0;
  }

  /**
   * Summary callback for pass mapper.
   *
   * @param array $mapping
   *   Associative array of the mapping settings.
   * @param array $target
   *   Array of target settings, as defined by the processor.
   * @param array $form
   *   The whole mapping form.
   * @param array $form_state
   *   The form state of the mapping form.
   *
   * @return string
   *   Returns, as a string that may contain HTML, the summary to display while
   *   the full form isn't visible.
   *   If the return value is empty, no summary and no option to view the form
   *   will be displayed.
   */
  public function passSummaryCallback($mapping, $target, $form, $form_state) {
    $options = $this->passSummaryCallbackOptions();
    if (!isset($mapping['pass_encryption'])) {
      $mapping['pass_encryption'] = self::PASS_UNENCRYPTED;
    }
    return t('Password encryption: <strong>@encryption</strong>', array('@encryption' => $options[$mapping['pass_encryption']]));
  }

  /**
   * Returns the list of available password encryption methods.
   *
   * Used by ::passSummaryCallback().
   *
   * @return array
   *    An array of password encryption option titles.
   * @see passSummaryCallback()
   */
  protected function passSummaryCallbackOptions() {
    return array(
      self::PASS_UNENCRYPTED => t('None'),
      self::PASS_MD5 => t('MD5'),
      self::PASS_SHA512 => t('SHA512'),
    );
  }

  /**
   * Settings form callback for pass mapper.
   *
   * @param array $mapping
   *   Associative array of the mapping settings.
   * @param array $target
   *   Array of target settings, as defined by the processor.
   * @param array $form
   *   The whole mapping form.
   * @param array $form_state
   *   The form state of the mapping form.
   *
   * @return array
   *   Mapping configuration form for pass mapper.
   */
  public function passFormCallback($mapping, $target, $form, $form_state) {
    return array(
      'pass_encryption' => array(
        '#type' => 'select',
        '#title' => t('Password encryption'),
        '#options' => $this->passFormCallbackOptions(),
        '#default_value' => !empty($mapping['pass_encryption']) ? $mapping['pass_encryption'] : self::PASS_UNENCRYPTED,
      ),
    );
  }

  /**
   * Returns the list of available password encryption methods.
   *
   * Used by ::passFormCallback().
   *
   * @return array
   *    An array of password encryption option titles.
   * @see passFormCallback()
   */
  protected function passFormCallbackOptions() {
    return array(
      self::PASS_UNENCRYPTED => t('Unencrypted'),
      self::PASS_MD5 => t('MD5 (used in older versions of Drupal)'),
      self::PASS_SHA512 => t('SHA512 (default in Drupal 7)'),
    );
  }

  /**
   * Sets the password on the user target.
   *
   * @see setTargetElement()
   */
  protected function setPassTarget($source, $target_user, $target_element, $value, $mapping) {
    if (empty($value)) {
      return;
    }
    if (!isset($mapping['pass_encryption'])) {
      $mapping['pass_encryption'] = self::PASS_UNENCRYPTED;
    }

    switch ($mapping['pass_encryption']) {
      case self::PASS_MD5:
        require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
        $new_hash = user_hash_password($value);
        if ($new_hash) {
          // Indicate an updated password.
          $new_hash  = 'U' . $new_hash;
          $target_user->pass_crypted = $new_hash;
        }
        break;
      case self::PASS_SHA512:
        $target_user->pass_crypted = $value;
        break;
      default:
        $target_user->pass = $value;
        break;

    }
  }
}
