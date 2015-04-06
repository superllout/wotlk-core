ALTER TABLE `character_achievement` CHANGE `date` `date` INT(11) UNSIGNED NULL; 
UPDATE `character_db_version` SET `LastUpdate`='2015-04-06_01_character_achievement' WHERE`LastUpdate`='2011-11-16_22-00_saved_mail';
