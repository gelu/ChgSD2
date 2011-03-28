-- World map scripts (selectors)
DELETE FROM `world_template` WHERE `map` = 0;
INSERT INTO `world_template` VALUES (0,'outdoor_pvp_eastern_kingdoms');
DELETE FROM `world_template` WHERE `map` = 1;
INSERT INTO `world_template` VALUES (1,'outdoor_pvp_kalimdor');
DELETE FROM `world_template` WHERE `map` = 530;
INSERT INTO `world_template` VALUES (530,'outdoor_pvp_outland');
DELETE FROM `world_template` WHERE `map` = 571;
INSERT INTO `world_template` VALUES (571,'outdoor_pvp_northrend');

-- from Xfurry
UPDATE gameobject_template SET ScriptName='go_silithyst' WHERE entry IN (181597,181598);
DELETE FROM scripted_areatrigger WHERE entry IN (4162, 4168);
INSERT INTO scripted_areatrigger VALUES (4162,'at_silithus'),(4168,'at_silithus');
