UPDATE `creature_template` SET `ScriptName`='boss_razorscale' WHERE `entry`=33186;
UPDATE `creature_template` SET `ScriptName`='boss_ignis' WHERE `entry`=33118;
UPDATE `creature_template` SET `ScriptName`='boss_xt002' WHERE `entry`=33293;
UPDATE `creature_template` SET `ScriptName`='boss_flame_leviathan' WHERE `entry`=33113;
UPDATE `creature_template` SET `ScriptName`='boss_auriaya' WHERE `entry`=33515;
UPDATE `creature_template` SET `ScriptName`='boss_kologarn' WHERE `entry`=32930;
UPDATE `creature_template` SET `ScriptName`='boss_kologarn_left_arm' WHERE `entry`=32933;
UPDATE `creature_template` SET `ScriptName`='boss_kologarn_right_arm' WHERE `entry`=32934;
UPDATE `creature_template` SET `ScriptName`='boss_steelbreaker' WHERE `entry`=32867;
UPDATE `creature_template` SET `ScriptName`='boss_molgeim' WHERE `entry`=32927;
UPDATE `creature_template` SET `ScriptName`='boss_brundir' WHERE `entry`=32857;
UPDATE `creature_template` SET `ScriptName`='boss_thorim' WHERE `entry`=32865;
UPDATE `creature_template` SET `ScriptName`='boss_hodir' WHERE `entry`=32845;
UPDATE `creature_template` SET `ScriptName`='boss_freya' WHERE `entry`=32906;
UPDATE `creature_template` SET `ScriptName`='boss_mimiron' WHERE `entry`=33350;
UPDATE `creature_template` SET `ScriptName`='boss_vezax' WHERE `entry`=33271;
UPDATE `creature_template` SET `ScriptName`='boss_yogg_saron' WHERE `entry`=33288;
UPDATE `creature_template` SET `ScriptName`='boss_algalon' WHERE `entry`=32871;
UPDATE `instance_template` SET `script`='instance_ulduar' WHERE `map`=603;

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) 
VALUES (99005, 0, 0, 0, 0, 0, 22448, 0, 22448, 0, 'Ulduar teleporter', '', NULL, 0, 80, 80, 64200, 64200, 12300, 12300, 9730, 35, 35, 1, 0.5, 0, 294, 441, 0, 110, 1.4, 1400, 1400, 0, 0, 0, 0, 0, 0, 0, 0, 235, 353, 88, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'ulduar_teleporter');
REPLACE INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `moveflags`, `auras`) VALUES
(99005, 0, 0, 0, 0, 0, '48143 0');
REPLACE INTO `locales_creature` values ('99005','Ulduar teleporter','','','','','','','Телепортер Ульдуара',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'');
DELETE FROM `creature` WHERE `guid` IN (500000,500001,500002,500003,500004,500005,500006,500007,500008);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(500000, 99005, 571, 3, 1, 0, 0, 9025.76, -1179.12, 1060.18, 4.59386, 3600, 0, 0, 64200, 12300, 0, 0),
(500001, 99005, 603, 3, 1, 0, 0, 1497.84, -23.9801, 421.367, 5.0242, 3600, 0, 0, 64200, 12300, 0, 0),
(500002, 99005, 603, 3, 1, 0, 0, 131.248, -35.3802, 410.204, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(500003, 99005, 603, 3, 1, 0, 0, 553.233, -12.3247, 410.079, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(500004, 99005, 603, 3, 1, 0, 0, 926.292, -11.4635, 418.995, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(500005, 99005, 603, 3, 1, 0, 0, 2086.17, -24.3111, 421.639, 3.11803, 3600, 0, 0, 64200, 12300, 0, 0),
(500006, 99005, 603, 3, 1, 0, 0, -705.304, -92.5391, 429.879, 4.87217, 3600, 0, 0, 64200, 12300, 0, 0),
(500007, 99005, 603, 3, 1, 0, 0, 2516.96, 2568.03, 412.4, 0.672303, 3600, 0, 0, 64200, 12300, 0, 0),
(500008, 99005, 603, 3, 1, 0, 0, 1854.2975, -11.0173, 334.3998, 0, 3600, 0, 0, 64200, 12300, 0, 0);
