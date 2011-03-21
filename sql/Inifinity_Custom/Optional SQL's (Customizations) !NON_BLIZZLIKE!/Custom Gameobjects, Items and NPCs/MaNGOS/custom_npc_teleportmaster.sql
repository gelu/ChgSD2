/* Template for TeleNet Employee <Teleport Master> */
DELETE FROM `creature_template` where `entry` = ('90000');
insert  into `creature_template`(`entry`,`difficulty_entry_1`,`difficulty_entry_2`,`difficulty_entry_3`,`KillCredit1`,`KillCredit2`,`modelid_1`,`modelid_2`,`modelid_3`,`modelid_4`,`name`,`subname`,`IconName`,`gossip_menu_id`,`minlevel`,`maxlevel`,`minhealth`,`maxhealth`,`minmana`,`maxmana`,`armor`,`faction_A`,`faction_H`,`npcflag`,`speed_walk`,`speed_run`,`scale`,`rank`,`mindmg`,`maxdmg`,`dmgschool`,`attackpower`,`dmg_multiplier`,`baseattacktime`,`rangeattacktime`,`unit_class`,`unit_flags`,`dynamicflags`,`family`,`trainer_type`,`trainer_spell`,`trainer_class`,`trainer_race`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`type`,`type_flags`,`lootid`,`pickpocketloot`,`skinloot`,`resistance1`,`resistance2`,`resistance3`,`resistance4`,`resistance5`,`resistance6`,`spell1`,`spell2`,`spell3`,`spell4`,`PetSpellDataId`,`mingold`,`maxgold`,`AIName`,`MovementType`,`InhabitType`,`unk16`,`unk17`,`RacialLeader`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`movementId`,`RegenHealth`,`equipment_id`,`mechanic_immune_mask`,`flags_extra`,`ScriptName`) values
(90000,0,0,0,0,0,7059,7061,7060,0,'Infinity Employee','Teleport Master',NULL,0,80,80,25000,25000,0,0,10000,35,35,1,1,1,1.5,1,2750,2750,0,250,1,1000,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,0,0,0,0,0,0,0,0,1,0,0,0,'custom_npc_teleportmaster');

DELETE FROM `creature_template` WHERE (`entry`=900007);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `vendor_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (900007, 0, 0, 0, 0, 0, 7061, 0, 0, 0, 'Infinity Employee', 'Teleport Master', '', 0, 80, 80, 25000, 25000, 25000, 25000, 50000, 35, 35, 1, 1, 1.14286, 1, 0, 25000, 43, 0, 11, 1, 1500, 1500, 1, 2, 0, 0, 0, 0, 0, 0, 23, 34, 9, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1.05, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'custom_npc_teleportmaster');

/* TeleNet Employee Spawns */
DELETE FROM `creature` where `id` = ('90000');

INSERT INTO `creature`(`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`)
values
(250000,90000,0,1,1,7061,0,-4902.14,-970.169,501.445,2.26234,25,0,0,25000,0,0,0),
(250001,90000,530,1,1,7061,0,-1861.74,5441.7,-10.4638,4.61735,25,0,0,25000,0,0,0),
(250002,90000,530,1,1,0,0,9356.65,-7280.5,14.2403,2.58004,25,0,0,25000,0,0,0),
(250003,90000,0,1,1,7061,0,1813.53,238.539,60.4944,0.102097,25,0,0,25000,0,0,0),
(250004,90000,1,1,1,7061,0,-1291.55,136.649,131.106,1.20406,25,0,0,25000,0,0,0),
(250005,90000,1,1,1,7061,0,1579.17,-4417.39,7.84218,3.48717,25,0,0,25000,0,0,0),
(250006,90000,0,1,1,0,0,-14340.5,599.961,107.723,1.55066,25,0,0,25000,0,0,0),
(250007,90000,0,1,1,0,0,-14406.8,571.105,146.991,5.74003,25,0,0,25000,0,0,0),
(250008,90000,0,1,1,0,0,-8956.44,511.693,96.5971,3.66388,25,0,0,25000,0,0,0),
(250009,90000,1,1,1,0,0,9944.91,2495.33,1317.56,4.4375,25,0,0,25000,0,0,0),
(250010,90000,530,1,1,7061,0,-3950.83,-11657.8,-138.657,2.43866,25,0,0,25000,0,0,0),
(250011,90000,571,1,1,0,0,5818.81,641.09,647.81,3.20,25,0,0,25000,0,0,0),
(250012,90000,0,1,1,0,0,-14324.1,600.97,101.6,6.18,25,0,0,25000,0,0,0),
(250013,90000,0,1,1,0,0,-14439.5,477.37,15.26,3.8,25,0,0,25000,0,0,0),
(250014,90000,1,1,1,0,0,6731.21,-4624.68,721.39,3.06,25,0,0,25000,0,0,0),
(250015,90000,1,1,1,0,0,-4575.25,-3178.3,34.29,1.46,25,0,0,25000,0,0,0),
(250016,90000,1,1,1,0,0,-7169.4,-3786.68,8.42,2.93,25,0,0,25000,0,0,0);