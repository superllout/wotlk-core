DROP TABLE IF EXISTS `achievement_reward`;
CREATE TABLE `achievement_reward` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `title` smallint(5) unsigned NOT NULL DEFAULT '0',
  `item` int(10) unsigned NOT NULL DEFAULT '0',
  `sender` int(10) unsigned NOT NULL DEFAULT '0',
  `subject` varchar(255) NOT NULL DEFAULT '0',
  `text` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
