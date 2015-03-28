DROP TABLE IF EXISTS `spell_area`;
CREATE TABLE `spell_area` (
 `spell` mediumint(8) unsigned NOT NULL DEFAULT '0',
 `area` mediumint(8) unsigned NOT NULL DEFAULT '0',
 `quest_start` mediumint(8) unsigned NOT NULL DEFAULT '0',
 `quest_start_active` tinyint(1) unsigned NOT NULL DEFAULT '0',
 `quest_end` mediumint(8) unsigned NOT NULL DEFAULT '0',
 `aura_spell` mediumint(8) NOT NULL DEFAULT '0',
 `racemask` mediumint(8) unsigned NOT NULL DEFAULT '0',
 `gender` tinyint(1) unsigned NOT NULL DEFAULT '2',
 `autocast` tinyint(1) unsigned NOT NULL DEFAULT '0',
 PRIMARY KEY (`spell`,`area`,`quest_start`,`quest_start_active`,`aura_spell`,`racemask`,`gender`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

UPDATE `world_db_version` SET `LastUpdate` = '2015-03-28_01_spell_area' WHERE `LastUpdate` = '2015-03-20_17_24_achievement_rewards'; 