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

-- script name errors 
UPDATE creature_template SET ScriptName='npc_amanitar_mushroom' WHERE entry IN (30391, 30435);
UPDATE creature_template SET ScriptName='boss_amanitar' WHERE entry = 30258;
UPDATE creature_template SET ScriptName='mob_flame_orb' WHERE entry=30702;
UPDATE creature_template SET ScriptName='npc_twilight_volunteer' WHERE entry=30385;
UPDATE creature_template SET ScriptName='mob_twisted_visage' WHERE entry=30621;
UPDATE creature_template SET ScriptName='mob_ancient_void' WHERE entry=30622;
UPDATE creature_template SET ScriptName='mob_dark_rune_acolyte' WHERE entry=33110;
UPDATE creature_template SET ScriptName='mob_dark_rune_evoker' WHERE entry=32878;
UPDATE creature_template SET ScriptName='mob_ancient_void' WHERE entry =30622;
UPDATE creature_template SET ScriptName='mob_boombot' WHERE entry=33346;
UPDATE creature_template SET ScriptName='mob_orange_gas_stalker' WHERE entry=36659;
UPDATE creature_template SET Scriptname='boss_krick' WHERE entry=36477;
UPDATE creature_template SET ScriptName='boss_ick' WHERE entry=36476;
UPDATE creature_template SET ScriptName='mob_exploding_orb' WHERE entry=36610;
UPDATE creature_template SET ScriptName='boss_scourgelord_tyrannus' WHERE entry=36658;
UPDATE creature_template SET ScriptName = 'mob_assault_bot' WHERE entry = 34057;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_acolyte' WHERE entry = 33110;
UPDATE `creature_template` SET `ScriptName`='mob_rimefang', `AIName` ='' WHERE `entry`=36661;
UPDATE `creature_template` SET `ScriptName`='npc_jaina_or_sylvanas_POSintro', `AIName` ='' WHERE `entry` IN (36990,36993);
UPDATE `creature_template` SET `ScriptName`='npc_jaina_or_sylvanas_POSoutro', `AIName` ='' WHERE `entry` IN (38189,38188);
--UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_watchman' where `entry` IN (36478);
--UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_reaper' where `entry` IN (36499);
--UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_adept' where `entry` IN (36620);
--UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_bonecaster' where `entry` IN (36564);
--UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_animator' where `entry` IN (36516);
--UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soul_horror' where `entry` IN (36522);
--UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_spectral_warden' where `entry` IN (36666);
UPDATE creature_template SET ScriptName = 'mob_dark_rune_acolyte' WHERE entry = 33110;
UPDATE creature_template SET ScriptName='mob_dark_rune_evoker' WHERE entry=32878;
UPDATE `creature_template` SET `ScriptName` = 'boss_omor_the_unscarred' WHERE `entry` = 17308;
UPDATE `creature_template` SET `ScriptName` = 'boss_vazruden' WHERE `entry` = 17537;
UPDATE `creature_template` SET `ScriptName` = 'boss_watchkeeper_gargolmar' WHERE `entry` = 17306;
UPDATE `creature_template` SET `ScriptName` = 'boss_grand_warlock_nethekurse' WHERE `entry` = 16807;
UPDATE `creature_template` SET `ScriptName` = 'boss_warbringer_omrogg' WHERE `entry` = 16809;
UPDATE `creature_template` SET `ScriptName` = 'boss_warchief_kargath_bladefist' WHERE `entry` = 16808;
UPDATE `creature_template` SET `ScriptName` = 'boss_arugal' WHERE `entry` = 4275;
UPDATE `creature_template` SET `ScriptName` = 'mob_shadowmoon_channeler' WHERE `entry` = 17653;
UPDATE `creature_template` SET `ScriptName` = 'boss_rimefang' WHERE `entry` = 36661;

-- Areatrigger:
delete from scripted_areatrigger where entry = 5578;
insert into scripted_areatrigger values (5578, 'at_tyrannus');
