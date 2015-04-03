DROP TABLE IF EXISTS `achievement_reward`;

CREATE TABLE `achievement_reward` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `title_A` smallint(5) unsigned NOT NULL DEFAULT '0',
  `title_H` smallint(5) unsigned NOT NULL DEFAULT '0',
  `item` int(10) unsigned NOT NULL DEFAULT '0',
  `sender` int(10) unsigned NOT NULL DEFAULT '0',
  `subject` varchar(255) DEFAULT '',
  `text` text,
  `mailTemplate` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

UPDATE `world_db_version` SET `LastUpdate` = '2015-03-20_17_24_achievement_rewards' WHERE `LastUpdate` = '2015-03-06_05_11_achievement_rewards'; 
