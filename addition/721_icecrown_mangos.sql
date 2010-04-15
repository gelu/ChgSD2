-- Entrance
UPDATE `areatrigger_teleport` SET `required_level` = '80' WHERE `areatrigger_teleport`.`id` =5670;

DELETE FROM `gameobject` WHERE `guid` = 913334;

INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(913334, 202244, 631, 3, 1, -209.5, 2211.91, 199.97, 3.07661, 0, 0, 0.999472, 0.0324833, 0, 0, 1);

UPDATE `gameobject_template` SET `flags` = 0, ScriptName = 'go_icecrown_teleporter' WHERE `entry`  IN (202242,202243,202244,202245,202235,202223,202246);
DELETE FROM `areatrigger_teleport` WHERE `id` = 5718 AND `target_map` = 631;

DELETE FROM `creature` WHERE `id` = 99322;
DELETE FROM `creature_template` WHERE `entry` = 99322;
DELETE FROM `locales_npc_text` WHERE `entry` = 99322;
DELETE FROM `npc_text` WHERE `ID` = 99322;
DELETE FROM `locales_npc_text` WHERE `entry` = 99323;
DELETE FROM `npc_text` WHERE `ID` = 99323;
DELETE FROM `gameobject` WHERE `guid` IN (913334);

DELETE FROM `creature` WHERE `guid` IN (599322,599323,599324,599325,599326,599327,599328,599329,599330);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(599322, 99322, 631, 15, 1, 0, 0, -209.5, 2211.91, 199.97, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599323, 99322, 631, 15, 1, 0, 0, -503.62, 2211.47, 62.8235, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599324, 99322, 631, 15, 1, 0, 0, -17.1928, 2211.44, 30.1158, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599325, 99322, 631, 15, 1, 0, 0, -549.131, 2211.29, 539.291, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599326, 99322, 631, 15, 1, 0, 0, -615.145, 2211.47, 199.972, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599327, 99322, 631, 15, 1, 0, 0, 4198.42, 2769.22, 351.065, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599328, 99322, 631, 15, 1, 0, 0, 4490.205566, 2769.275635, 403.983765, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599329, 99322, 631, 15, 1, 0, 0, 4356.236816, 2402.573242, 220.462540, 0, 3600, 0, 0, 64200, 12300, 0, 0),
(599330, 99322, 631, 15, 1, 0, 0, 4356.236816, 2402.573242, 220.462540, 0, 3600, 0, 0, 64200, 12300, 0, 0);

UPDATE `instance_template` SET `script`='instance_icecrown_spire' WHERE `map`=631;

-- Saurfang
UPDATE `creature_template` SET `ScriptName`='boss_deathbringer_saurfang' WHERE `entry`=37813;
DELETE FROM `gameobject` WHERE `guid` IN (913383, 913385, 913395, 913397);
DELETE FROM `gameobject_template` WHERE `entry` IN (902241,902242);

-- Deathwhisper
UPDATE `creature_template` SET `ScriptName`='boss_lady_deathwhisper' WHERE `entry`=36855;
UPDATE `creature_template` SET `faction_A`=14, `faction_H`=14,`ScriptName`='mob_vengeful_shade', `AIName`='' WHERE `entry`= 38222;
#UPDATE `gameobject` SET `spawntimesecs` = -25, `state` = 1 WHERE `id` = 202220;

-- Marrowgar
UPDATE `creature_template` SET `ScriptName`='boss_lord_marrowgar' WHERE `entry`= 36612;
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201910,201911);
UPDATE `gameobject` SET `state` = '1' WHERE `guid` IN (72526,72525);
UPDATE `creature_template` SET `ScriptName`='mob_coldflame', `minlevel` = 80, `maxlevel` = 80, `modelid_A` = 11686, `modelid_A2` = 11686, `modelid_H` = 11686, `modelid_H2` = 11686, `faction_A` = 35, `faction_H` = 35  WHERE `entry`= 36672;
UPDATE `creature_template` SET `ScriptName`='mob_bone_spike' WHERE `entry`= 38711;

-- Gunship battle
UPDATE `creature_template` SET `ScriptName`='mob_spire_frostwyrm', `AIName`='' WHERE `entry`= 37230;

DELETE FROM `creature` WHERE `guid` IN (81531, 83459, 78383, 78385, 81170, 81535, 81165, 81168, 124864);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(81531, 37230, 631, 15, 1, 0, 0, -541.262, 2287.1, 199.97, 2.73098, 300, 0, 0, 1078480, 41690, 0, 0),
(83459, 37230, 631, 15, 1, 0, 0, -536.326, 2125.85, 199.97, 2.86921, 300, 0, 0, 1078480, 41690, 0, 0),
(78383, 37230, 631, 15, 1, 0, 0, -436.712, 2078.51, 191.254, 3.82738, 300, 0, 0, 1078480, 41690, 0, 0),
(78385, 37230, 631, 15, 1, 0, 0, -436.977, 2345.78, 191.254, 3.53716, 300, 0, 0, 1078480, 41690, 0, 0),
(81170, 37230, 631, 15, 1, 0, 0, -302.465, 2229.49, 199.982, 2.04961, 300, 0, 0, 1078480, 41690, 0, 0),
(81535, 37230, 631, 15, 1, 0, 0, -223.486, 2200.9, 199.97, 3.07063, 300, 0, 0, 1078480, 41690, 0, 0),
(81165, 37230, 631, 15, 1, 0, 0, -505.65, 2320, 199.969, 0.369649, 300, 0, 0, 1078480, 41690, 0, 0),
(81168, 37230, 631, 15, 1, 0, 0, -298.341, 2203.06, 199.983, 4.17647, 300, 0, 0, 1078480, 41690, 0, 0),
(124864, 37230, 631, 15, 1, 0, 0, -222.214, 2220.68, 199.97, 3.1586, 300, 0, 0, 1078480, 41690, 0, 0);

-- Rotface
UPDATE `creature_template` SET `ScriptName`='boss_rotface', `AIName`=''  WHERE `entry`= 36627;
UPDATE `gameobject_template` SET `faction` = '114' WHERE `gameobject_template`.`entry` IN (201370);
UPDATE `gameobject` SET `state` = '0' WHERE `id` IN (201370);
-- Festergut
UPDATE `creature_template` SET `ScriptName`='boss_festergut', `AIName`=''  WHERE `entry`= 36626;
UPDATE `gameobject_template` SET `faction` = '114' WHERE `gameobject_template`.`entry` IN (201371);
UPDATE `gameobject` SET `state` = '0' WHERE `id` IN (201371);

-- Professor putricide
UPDATE `creature_template` SET `ScriptName`='boss_proffesor_putricide', `AIName`='' WHERE `entry`= 36678;
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201372,201614,201613, 201612);
UPDATE `gameobject` SET `state` = '1' WHERE `id` IN (201612,201614,201613);
UPDATE `gameobject` SET `state` = '0' WHERE `id` IN (201372);

-- Taldaram
-- UPDATE `creature_template` SET `ScriptName`='boss_taldaram' WHERE `entry`= 37973;
-- Valanar
-- UPDATE `creature_template` SET `ScriptName`='boss_valanar' WHERE `entry`= 37970;
-- Keleseth
-- UPDATE `creature_template` SET `ScriptName`='boss_keleseth' WHERE `entry`= 37972;
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201920,201377,201378);
UPDATE `gameobject` SET `state` = '1' WHERE `id` IN (201920,201377,201378);
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201376);
UPDATE `gameobject` SET `state` = '0' WHERE `id` IN (201376);

-- Qween Lana'thel
-- UPDATE `creature_template` SET `ScriptName`='boss_lanathel' WHERE `entry`= 37955;

-- Valithria dreamwalker
-- UPDATE `creature_template` SET `ScriptName`='boss_valithria' WHERE `entry`= 36789;
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201374,201375,201369);
UPDATE `gameobject` SET `state` = '1' WHERE `id` IN (201374,201375,201369);
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201380,201381,201382,201383);
UPDATE `gameobject` SET `state` = '1' WHERE `id` IN (201380,201381,201382,201383);

-- Sindragosa
-- UPDATE `creature_template` SET `ScriptName`='boss_sindragosa' WHERE `entry`= 37755;
-- Lich King
-- UPDATE `creature_template` SET `ScriptName`='boss_lich_king' WHERE `entry`= 29983;
