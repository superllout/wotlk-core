ALTER TABLE `account` 
    CHANGE `username` `username` VARCHAR(32) CHARSET utf8 COLLATE utf8_unicode_ci DEFAULT '' NOT NULL COMMENT 'Login username', 
    CHANGE `password` `password` VARCHAR(32) CHARSET utf8 COLLATE utf8_unicode_ci DEFAULT '' NOT NULL COMMENT 'Login password', 
    CHANGE `banned` `banned` INT(10) UNSIGNED DEFAULT 0 NOT NULL, 
    CHANGE `banreason` `banreason` VARCHAR(255) CHARSET utf8 COLLATE utf8_unicode_ci DEFAULT '' NULL; 