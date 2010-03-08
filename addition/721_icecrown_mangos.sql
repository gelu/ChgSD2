-- Entrance
UPDATE `areatrigger_teleport` SET `required_level` = '80' WHERE `areatrigger_teleport`.`id` =5670;

DELETE FROM `gameobject` WHERE `guid` = 913334;

INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(913334, 202244, 631, 3, 1, -209.5, 2211.91, 199.97, 3.07661, 0, 0, 0.999472, 0.0324833, 0, 0, 1);

UPDATE `gameobject_template` SET ScriptName = 'go_icecrown_teleporter' WHERE `entry`  IN (202242,202243,202244,202245,202235);
# 202223 202246
DELETE FROM `creature_template` WHERE `entry` = 99322;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(99322, 0, 0, 0, 0, 0, 22448, 0, 22448, 0, 'IceCrown teleporter', '', NULL, 0, 80, 80, 64200, 64200, 12300, 12300, 9730, 35, 35, 1, 1.48, 0.5, 0, 294, 441, 0, 110, 1.4, 1400, 1400, 0, 0, 0, 0, 0, 0, 0, 0, 235, 353, 88, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 1, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 'icecrown_teleporter');
REPLACE INTO `locales_creature` values ('99322','IceCrown teleporter','','','','','','','Телепортер цитадели Ледяной короны',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'');

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

DELETE FROM `locales_npc_text` WHERE `entry` = 99322;
INSERT INTO `locales_npc_text` (`entry`, `Text0_0_loc1`, `Text0_0_loc2`, `Text0_0_loc3`, `Text0_0_loc4`, `Text0_0_loc5`, `Text0_0_loc6`, `Text0_0_loc7`, `Text0_0_loc8`, `Text0_1_loc1`, `Text0_1_loc2`, `Text0_1_loc3`, `Text0_1_loc4`, `Text0_1_loc5`, `Text0_1_loc6`, `Text0_1_loc7`, `Text0_1_loc8`, `Text1_0_loc1`, `Text1_0_loc2`, `Text1_0_loc3`, `Text1_0_loc4`, `Text1_0_loc5`, `Text1_0_loc6`, `Text1_0_loc7`, `Text1_0_loc8`, `Text1_1_loc1`, `Text1_1_loc2`, `Text1_1_loc3`, `Text1_1_loc4`, `Text1_1_loc5`, `Text1_1_loc6`, `Text1_1_loc7`, `Text1_1_loc8`, `Text2_0_loc1`, `Text2_0_loc2`, `Text2_0_loc3`, `Text2_0_loc4`, `Text2_0_loc5`, `Text2_0_loc6`, `Text2_0_loc7`, `Text2_0_loc8`, `Text2_1_loc1`, `Text2_1_loc2`, `Text2_1_loc3`, `Text2_1_loc4`, `Text2_1_loc5`, `Text2_1_loc6`, `Text2_1_loc7`, `Text2_1_loc8`, `Text3_0_loc1`, `Text3_0_loc2`, `Text3_0_loc3`, `Text3_0_loc4`, `Text3_0_loc5`, `Text3_0_loc6`, `Text3_0_loc7`, `Text3_0_loc8`, `Text3_1_loc1`, `Text3_1_loc2`, `Text3_1_loc3`, `Text3_1_loc4`, `Text3_1_loc5`, `Text3_1_loc6`, `Text3_1_loc7`, `Text3_1_loc8`, `Text4_0_loc1`, `Text4_0_loc2`, `Text4_0_loc3`, `Text4_0_loc4`, `Text4_0_loc5`, `Text4_0_loc6`, `Text4_0_loc7`, `Text4_0_loc8`, `Text4_1_loc1`, `Text4_1_loc2`, `Text4_1_loc3`, `Text4_1_loc4`, `Text4_1_loc5`, `Text4_1_loc6`, `Text4_1_loc7`, `Text4_1_loc8`, `Text5_0_loc1`, `Text5_0_loc2`, `Text5_0_loc3`, `Text5_0_loc4`, `Text5_0_loc5`, `Text5_0_loc6`, `Text5_0_loc7`, `Text5_0_loc8`, `Text5_1_loc1`, `Text5_1_loc2`, `Text5_1_loc3`, `Text5_1_loc4`, `Text5_1_loc5`, `Text5_1_loc6`, `Text5_1_loc7`, `Text5_1_loc8`, `Text6_0_loc1`, `Text6_0_loc2`, `Text6_0_loc3`, `Text6_0_loc4`, `Text6_0_loc5`, `Text6_0_loc6`, `Text6_0_loc7`, `Text6_0_loc8`, `Text6_1_loc1`, `Text6_1_loc2`, `Text6_1_loc3`, `Text6_1_loc4`, `Text6_1_loc5`, `Text6_1_loc6`, `Text6_1_loc7`, `Text6_1_loc8`, `Text7_0_loc1`, `Text7_0_loc2`, `Text7_0_loc3`, `Text7_0_loc4`, `Text7_0_loc5`, `Text7_0_loc6`, `Text7_0_loc7`, `Text7_0_loc8`, `Text7_1_loc1`, `Text7_1_loc2`, `Text7_1_loc3`, `Text7_1_loc4`, `Text7_1_loc5`, `Text7_1_loc6`, `Text7_1_loc7`, `Text7_1_loc8`) VALUES
(99322, 'Teleporter is functional. Player $N can teleport to:', NULL, NULL, NULL, NULL, NULL, NULL, 'Приветствую $N! Телепортер функционирует нормально. Вы можете переместиться в следущие места:', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Приветствую $N', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '');

DELETE FROM `npc_text` WHERE `ID` = 99322;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`) VALUES
(99322, 'Teleporter is functional. Player $N can teleport to:', 'Teleporter is functional. Player $N can teleport to:', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `locales_npc_text` WHERE `entry` = 99323;
INSERT INTO `locales_npc_text` (`entry`, `Text0_0_loc1`, `Text0_0_loc2`, `Text0_0_loc3`, `Text0_0_loc4`, `Text0_0_loc5`, `Text0_0_loc6`, `Text0_0_loc7`, `Text0_0_loc8`, `Text0_1_loc1`, `Text0_1_loc2`, `Text0_1_loc3`, `Text0_1_loc4`, `Text0_1_loc5`, `Text0_1_loc6`, `Text0_1_loc7`, `Text0_1_loc8`, `Text1_0_loc1`, `Text1_0_loc2`, `Text1_0_loc3`, `Text1_0_loc4`, `Text1_0_loc5`, `Text1_0_loc6`, `Text1_0_loc7`, `Text1_0_loc8`, `Text1_1_loc1`, `Text1_1_loc2`, `Text1_1_loc3`, `Text1_1_loc4`, `Text1_1_loc5`, `Text1_1_loc6`, `Text1_1_loc7`, `Text1_1_loc8`, `Text2_0_loc1`, `Text2_0_loc2`, `Text2_0_loc3`, `Text2_0_loc4`, `Text2_0_loc5`, `Text2_0_loc6`, `Text2_0_loc7`, `Text2_0_loc8`, `Text2_1_loc1`, `Text2_1_loc2`, `Text2_1_loc3`, `Text2_1_loc4`, `Text2_1_loc5`, `Text2_1_loc6`, `Text2_1_loc7`, `Text2_1_loc8`, `Text3_0_loc1`, `Text3_0_loc2`, `Text3_0_loc3`, `Text3_0_loc4`, `Text3_0_loc5`, `Text3_0_loc6`, `Text3_0_loc7`, `Text3_0_loc8`, `Text3_1_loc1`, `Text3_1_loc2`, `Text3_1_loc3`, `Text3_1_loc4`, `Text3_1_loc5`, `Text3_1_loc6`, `Text3_1_loc7`, `Text3_1_loc8`, `Text4_0_loc1`, `Text4_0_loc2`, `Text4_0_loc3`, `Text4_0_loc4`, `Text4_0_loc5`, `Text4_0_loc6`, `Text4_0_loc7`, `Text4_0_loc8`, `Text4_1_loc1`, `Text4_1_loc2`, `Text4_1_loc3`, `Text4_1_loc4`, `Text4_1_loc5`, `Text4_1_loc6`, `Text4_1_loc7`, `Text4_1_loc8`, `Text5_0_loc1`, `Text5_0_loc2`, `Text5_0_loc3`, `Text5_0_loc4`, `Text5_0_loc5`, `Text5_0_loc6`, `Text5_0_loc7`, `Text5_0_loc8`, `Text5_1_loc1`, `Text5_1_loc2`, `Text5_1_loc3`, `Text5_1_loc4`, `Text5_1_loc5`, `Text5_1_loc6`, `Text5_1_loc7`, `Text5_1_loc8`, `Text6_0_loc1`, `Text6_0_loc2`, `Text6_0_loc3`, `Text6_0_loc4`, `Text6_0_loc5`, `Text6_0_loc6`, `Text6_0_loc7`, `Text6_0_loc8`, `Text6_1_loc1`, `Text6_1_loc2`, `Text6_1_loc3`, `Text6_1_loc4`, `Text6_1_loc5`, `Text6_1_loc6`, `Text6_1_loc7`, `Text6_1_loc8`, `Text7_0_loc1`, `Text7_0_loc2`, `Text7_0_loc3`, `Text7_0_loc4`, `Text7_0_loc5`, `Text7_0_loc6`, `Text7_0_loc7`, `Text7_0_loc8`, `Text7_1_loc1`, `Text7_1_loc2`, `Text7_1_loc3`, `Text7_1_loc4`, `Text7_1_loc5`, `Text7_1_loc6`, `Text7_1_loc7`, `Text7_1_loc8`) VALUES
(99323, 'Teleporter is functional. Select control panel to teleport.', NULL, NULL, NULL, NULL, NULL, NULL, 'Телепортер функционирует нормально. Выберите место назначения на контрольной панели.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Приветствую $N', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '');

DELETE FROM `npc_text` WHERE `ID` = 99323;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `em0_0`, `em0_1`, `em0_2`, `em0_3`, `em0_4`, `em0_5`, `text1_0`, `text1_1`, `lang1`, `prob1`, `em1_0`, `em1_1`, `em1_2`, `em1_3`, `em1_4`, `em1_5`, `text2_0`, `text2_1`, `lang2`, `prob2`, `em2_0`, `em2_1`, `em2_2`, `em2_3`, `em2_4`, `em2_5`, `text3_0`, `text3_1`, `lang3`, `prob3`, `em3_0`, `em3_1`, `em3_2`, `em3_3`, `em3_4`, `em3_5`, `text4_0`, `text4_1`, `lang4`, `prob4`, `em4_0`, `em4_1`, `em4_2`, `em4_3`, `em4_4`, `em4_5`, `text5_0`, `text5_1`, `lang5`, `prob5`, `em5_0`, `em5_1`, `em5_2`, `em5_3`, `em5_4`, `em5_5`, `text6_0`, `text6_1`, `lang6`, `prob6`, `em6_0`, `em6_1`, `em6_2`, `em6_3`, `em6_4`, `em6_5`, `text7_0`, `text7_1`, `lang7`, `prob7`, `em7_0`, `em7_1`, `em7_2`, `em7_3`, `em7_4`, `em7_5`) VALUES
(99323, 'Teleporter is functional. Select control panel to teleport.', 'Teleporter is functional. Select control panel to teleport.', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0, '', '', 0, 0, 0, 0, 0, 0, 0, 0);

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
-- UPDATE `creature_template` SET `ScriptName`='boss_rotface' WHERE `entry`= 36627;
-- Festergut
-- UPDATE `creature_template` SET `ScriptName`='boss_festergut' WHERE `entry`= 36626;
-- Professor putricide
-- UPDATE `creature_template` SET `ScriptName`='boss_professor_putricide' WHERE `entry`= 36678;
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201371,201370,201372,201614,201613);
UPDATE `gameobject` SET `state` = '1' WHERE `id` IN (201371,201370,201372,201614,201613);

-- Taldaram
-- UPDATE `creature_template` SET `ScriptName`='boss_taldaram' WHERE `entry`= 37973;
-- Valanar
-- UPDATE `creature_template` SET `ScriptName`='boss_valanar' WHERE `entry`= 37970;
-- Keleseth
-- UPDATE `creature_template` SET `ScriptName`='boss_keleseth' WHERE `entry`= 37972;
UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `gameobject_template`.`entry` IN (201376,201377,201378);
UPDATE `gameobject` SET `state` = '1' WHERE `id` IN (201376,201377,201378);

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
