ALTER TABLE `achievement_reward` 
    CHANGE `title` `title_A` SMALLINT(5) UNSIGNED DEFAULT 0 NOT NULL, 
    ADD COLUMN `title_H` SMALLINT(5) UNSIGNED DEFAULT 0 NOT NULL AFTER `title_A`, 
    CHANGE `subject` `subject` VARCHAR(255) CHARSET utf8 COLLATE utf8_general_ci DEFAULT '' NULL, 
    ADD COLUMN `mailTemplate` SMALLINT(5) UNSIGNED DEFAULT 0 NOT NULL AFTER `text`; 
TRUNCATE TABLE `achievement_reward`; -- removing data because it needs full reimport (db update with data will come in db repo)
UPDATE `world_db_version` SET `LastUpdate` = '2015-03-20_17_24_achievement_rewards' WHERE `LastUpdate` = '2015-03-06_05_11_achievement_rewards'; 
