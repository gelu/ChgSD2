update `creature_template` set `minhealth`='8600', `maxhealth`='8600' where `entry` in (31127, 31126, 28167, 28169);
update `creature_template` set `minhealth`='8600', `maxhealth`='8600' where `entry` in (10002, 10003, 10004, 10005);
update `creature_template` set `faction_A`='35', `faction_H`='35' where `entry` in (31127, 31126, 28167, 28169);
update `creature_template` set `faction_A`='35', `faction_H`='35' where `entry` in (10002, 10003, 10004, 10005);
update `creature_template` set `minhealth`='15000', `maxhealth`='15000' where `entry` in (27737);
update `creature_template` set `minhealth`='25000', `maxhealth`='25000' where `entry` in (31208);

DELETE FROM `creature` WHERE (`id`=27744);
DELETE FROM `creature` WHERE (`id`=26530);
DELETE FROM `creature` WHERE (`id`=26529);
DELETE FROM `creature` WHERE (`id`=26532);
DELETE FROM `creature` WHERE (`id`=26533);

update `creature_template` set `AIName`='', `Scriptname`='npc_arthas' where `entry` in (26499);
update `creature_template` set `AIName`='', `Scriptname`='dark_conversion' where `entry` in (31127, 31126, 28167, 28169);
update `creature_template` set `AIName`='', `Scriptname`='npc_patricia' where `entry` in (31028);
update `creature_template` set `AIName`='', `Scriptname`='npc_time_riftCS' where `entry` in (28409);
update `creature_template` set `AIName`='', `Scriptname`='boss_salramm' where `entry` in (26530);
update `creature_template` set `AIName`='', `Scriptname`='boss_meathook' where `entry` in (26529);
update `creature_template` set `AIName`='', `Scriptname`='boss_lord_epoch' where `entry` in (26532);
update `creature_template` set `AIName`='', `Scriptname`='boss_malganis' where `entry` in (26533);
update `instance_template` set `script` = 'instance_culling_of_stratholme' WHERE map=595;
-- Spawning Salramm in the instance --
INSERT INTO creature VALUES (4458724,26530,595,1,1,0,0,2174.32,1307.32,131.866,4.32264,25,0,0,337025,62535,0,0);
-- heroic version --
INSERT INTO creature VALUES (4458725,26530,595,2,1,0,0,2178.93,1307.89,131.526,4.12069,25,0,0,421281,62535,0,0);
-- Spawning Meathook in the instance --
INSERT INTO creature VALUES (4458738,26529,595,1,1,0,0,2351.43,1218.03,130.078,4.64621,25,0,0,337025,0,0,0);
-- heroic version --
INSERT INTO creature VALUES (4458739,26529,595,2,1,0,0,2350.42,1215.31,130.23,4.76314,25,0,0,421281,0,0,0);
-- Spawning Lord Epoch in the instance --
INSERT INTO creature VALUES (4458740,26532,595,1,1,0,1820,2446.25,1111.97,148.077,3.37779,25,0,0,337025,41690,0,0);
-- heroic version --
INSERT INTO creature VALUES (4458741,26532,595,2,1,0,1820,2446.17,1111.64,148.076,3.25517,25,0,0,421281,41690,0,0);
-- Spawning Malganis in the instance
INSERT INTO creature VALUES (4459981,26533,595,1,1,0,0,2298.33,1501.03,128.362,5.11213,25,0,0,404430,41690,0,0);
-- heroic version --
INSERT INTO creature VALUES (4459615,26533,595,2,1,0,0,2298.9,1502.32,128.361,5.21301,25,0,0,505538,41690,0,0);
update `creature` set `spawntimesecs`='3600' where `id` in (31127, 31126, 28167, 28169);

DELETE FROM `creature` WHERE (`guid`=4456649);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (4456649, 26499, 595, 1, 0, 0, 1920.87, 1287.12, 142.935, 6.25562, 25, 0, 0, 44100, 7988, 0, 0);

DELETE FROM `creature` WHERE (`guid`=4456653);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES (4456653, 26497, 595, 1, 0, 0, 1895.48, 1292.66, 143.706, 0.023475, 25, 0, 0, 100800, 88140, 0, 0);

DELETE FROM `gameobject_template` WHERE (`entry`=188686);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES (188686, 0, 7831, 'Doodad_LD_hidden_door_room01', '', 1375, 6553632, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

DELETE FROM `gameobject_template` WHERE (`entry`=187711);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES (187711, 0, 3631, 'Crusaders\' Square Gate', '', 1375, 6553632, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

DELETE FROM `gameobject_template` WHERE (`entry`=187723);
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `castBarCaption`, `faction`, `flags`, `size`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES (187723, 0, 3631, 'Crusaders\' Square Gate', '', 1375, 6553632, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

update `gameobject` set `state` = '1' where `id` in (187711);
update `gameobject` set `spawntimesecs` = '-604800', `state` = '0'  where `id` in (190663);

UPDATE `creature` set `curhealth` = '8600' where `id`=28167;
UPDATE `creature` set `curhealth` = '8600' where `id`=28169;
UPDATE `creature` set `curhealth` = '15000' where `id`=27737;
UPDATE `quest_template` set `SpecialFlags` = '1' where `entry`=13151;

