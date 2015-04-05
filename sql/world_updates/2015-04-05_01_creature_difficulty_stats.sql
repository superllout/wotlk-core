DROP TABLE IF EXISTS `creature_difficulty_stats`;
CREATE TABLE `creature_difficulty_stats` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `difficulty` smallint(5) unsigned NOT NULL DEFAULT '0',
  `minLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `maxLevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  `minHealth` int(10) unsigned NOT NULL DEFAULT '0',
  `maxHealth` int(10) unsigned NOT NULL DEFAULT '0',
  `mana` int(10) unsigned NOT NULL DEFAULT '0',
  `minDamage` int(10) unsigned NOT NULL DEFAULT '0',
  `maxDamage` int(10) unsigned NOT NULL DEFAULT '0',
  `minRangedDamage` int(10) unsigned NOT NULL DEFAULT '0',
  `maxRangedDamage` int(10) unsigned NOT NULL DEFAULT '0',
  `armor` int(10) unsigned NOT NULL DEFAULT '0',
  `holy_resistance` int(10) unsigned NOT NULL DEFAULT '0',
  `fire_resistance` int(10) unsigned NOT NULL DEFAULT '0',
  `nature_resistance` int(10) unsigned NOT NULL DEFAULT '0',
  `frost_resistance` int(10) unsigned NOT NULL DEFAULT '0',
  `shadow_resistance` int(10) unsigned NOT NULL DEFAULT '0',
  `arcane_resistance` int(10) unsigned NOT NULL DEFAULT '0',
  `immuneMask` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`,`difficulty`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
UPDATE `world_db_version` SET `LastUpdate` = '2015-04-05_01_creature_difficulty_stats' WHERE `LastUpdate` = '2015-03-30_01_creature_script_texts'; 