DROP TABLE IF EXISTS `creature_script_texts`;
CREATE TABLE `creature_script_texts` (
  `entry` int(10) unsigned NOT NULL,
  `textIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `text` varchar(255) NOT NULL DEFAULT '""',
  `textType` smallint(5) unsigned NOT NULL DEFAULT '0',
  `langauge` smallint(5) unsigned NOT NULL DEFAULT '0',
  `emoteId` int(10) unsigned NOT NULL DEFAULT '0',
  `emoteDelay` int(10) unsigned NOT NULL DEFAULT '0',
  `soundId` int(10) unsigned NOT NULL DEFAULT '0',
  `comment` varchar(255) NOT NULL DEFAULT '""',
  PRIMARY KEY (`entry`,`textIndex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

UPDATE `world_db_version` SET `LastUpdate` = '2015-03-30_01_creature_script_texts' WHERE `LastUpdate` = '2015-03-28_01_spell_area'; 