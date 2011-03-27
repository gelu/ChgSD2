UPDATE `creature_template_addon` SET `auras`='51859' WHERE `entry`='28525';
UPDATE `creature_template_addon` SET `auras`='51859' WHERE `entry`='28542';
UPDATE `creature_template_addon` SET `auras`='51859' WHERE `entry`='28543';
UPDATE `creature_template_addon` SET `auras`='51859' WHERE `entry`='28544';
UPDATE `creature_template_addon` SET `auras`='70022' WHERE `entry`='37186';

INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(115064, 29310, 619, 3, 1, 0, 0, 372.331, -705.278, -0.524178, 5.42797, 43200, 0, 0, 212700, 71220, 0, 0);

DELETE FROM `creature` WHERE `id`=38589;
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(47709, 38589, 631, 15, 64, 0, 0, 4203.47, 2484.5, 364.872, 6.27767, 7200, 0, 0, 10000000, 0, 0, 0),
(47738, 38589, 631, 15, 128, 0, 0, 4203.47, 2484.5, 364.872, 6.27767, 7200, 0, 0, 10000000, 0, 0, 0);

DELETE FROM `creature_ai_scripts` WHERE (`creature_id`=30176);
INSERT INTO `creature_ai_scripts` VALUES 
( 3017611, 30176, 8, 0, 100, 6, 56153, 0, 0, 0, 28, 0, 56153, 0, 0, 0, 0, 0, 0, 0, 0, 0, ''),
(3017610, 30176, 11, 0, 100, 6, 0, 0, 0, 0, 11, 56151, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

UPDATE `creature_template` SET `npcflag` = 2 WHERE `entry` = 38160;
UPDATE `creature_template` SET `npcflag` = 2 WHERE `entry` = 38161;

UPDATE `creature_template` SET `lootid` = 31360 WHERE `entry` = 31360;

DELETE FROM `reference_loot_template` WHERE (`entry`='40630' AND `item`='40630') OR (`entry`='40630' AND `item`='40629') OR (`entry`='40630' AND `item`='40628');

UPDATE `creature_template` SET `AIName` = 'EventAI', `ScriptName` = '' WHERE `entry` = '30176';
DELETE FROM `creature_ai_scripts` WHERE (`id`='3017611');
INSERT INTO `creature_ai_scripts` VALUES ('3017611', '30176', '8', '0', '100', '6', '56153', '-1', '0', '0', '28', '0', '56153', '0', '0', '0', '0', '0', '0', '0', '0', '0', '');

