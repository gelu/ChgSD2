DROP TABLE IF EXISTS `guildhalls`;
CREATE TABLE `guildhalls` (
  `id` int(8) unsigned NOT NULL auto_increment,
  `guildId` bigint(20) NOT NULL default '0',
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `map` int(11) NOT NULL,
  `comment` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC AUTO_INCREMENT=22 ;

INSERT INTO `guildhalls` VALUES
(1, 0, 16222, 16266, 14.2, 1, 'GM Island'),
(2, 0, -10711, 2483, 8, 1, 'Tauren village at Veiled Sea (Silithus)'),
(3, 0, -8323, -343, 146, 0, 'Fishing outside an Northshire Abbey (Elwynn Forest'),
(4, 0, 7368, -1560, 163, 1, 'Troll Village in mountains (Darkshore)'),
(5, 0, -4151, -1400, 198, 0, 'Dwarven village outside Ironforge (Wetlands)'),
(6, 0, -1840, -4233, 2.14, 0, 'Dwarven village (Arathi Highlands, Forbidding Sea)'),
(7, 0, -4151, -1400, 198, 0, 'Crashed zeppelin (Wetlands, Dun Modr)'),
(8, 0, -723, -1076, 179, 1, 'Tauren camp (Mulgore, Red Rock)'),
(9, 0, -206, 1666, 80, 0, 'Shadowfang Keep an outside instance (Silverpine Forest)'),
(10, 0, -6374, 1262, 7, 0, 'Harbor house outside Stormwind (Elwynn Forest)'),
(11, 0, -8640, 580, 96, 0, 'Empty jail between canals (Stormwind)'),
(12, 0, -4844, -1066, 502, 0, 'Old Ironforge'),
(13, 0, -4863, -1658, 503.5, 0, 'Ironforge Airport'),
(14, 0, 1146, -165, 313, 37, 'Azshara Crater instance (Alliance entrance)'),
(15, 0, -123, 858, 298, 37, 'Azshara Crater instance (Horde entrance)'),
(16, 0, 4303, -2760, 16.8, 0, 'Quel\Thalas Tower'),
(17, 0, -6161, -790, 423, 0, 'Crashed gnome airplane (between Dun Morogh and Searing Gorge)'),
(18, 0, -11790, -1640, 54.7, 0, 'Zul\'Gurub an outside instance (Stranglethorn Vale)'),
(19, 0, -11805, -4754, 6, 1, 'Goblin village (Tanaris, South Seas)'),
(20, 0, -9296, 670, 132, 0, 'Villains camp outside an Stormwind (Elwynn Forest)'),
(21, 0, 3414, -3380, 142.2, 0, 'Stratholm an outside instance');