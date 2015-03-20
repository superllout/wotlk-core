/*
SQLyog Community v12.09 (64 bit)
MySQL - 5.6.22-log : Database - logon
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `account` */

DROP TABLE IF EXISTS `account`;

CREATE TABLE `account` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique ID',
  `username` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Login username',
  `password` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Login password',
  `sha_pass_hash` varchar(42) COLLATE utf8_unicode_ci NOT NULL DEFAULT '',
  `gm` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Game permissions',
  `banned` int(10) unsigned NOT NULL DEFAULT '0',
  `lastlogin` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'Last login timestamp',
  `lastip` varchar(16) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Last remote address',
  `email` varchar(64) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT 'Contact e-mail address',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Client flags',
  `forceLanguage` varchar(5) COLLATE utf8_unicode_ci NOT NULL DEFAULT 'enUS',
  `muted` int(30) NOT NULL DEFAULT '0',
  `banreason` varchar(255) COLLATE utf8_unicode_ci DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Account Information';

/*Table structure for table `ipbans` */

DROP TABLE IF EXISTS `ipbans`;

CREATE TABLE `ipbans` (
  `ip` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
  `expire` int(10) NOT NULL COMMENT 'Expiry time (s)',
  `banreason` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  PRIMARY KEY (`ip`),
  UNIQUE KEY `a` (`ip`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='IPBanner';

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
