-- Ruby sanctum
UPDATE `instance_template` SET `ScriptName`='instance_ruby_sanctum' WHERE `map`=724;
-- Halion
UPDATE `creature_template` SET `ScriptName`='boss_halion_real', `AIName` ='' WHERE `entry`=39863;
UPDATE `creature_template` SET `ScriptName`='boss_halion_twilight', `AIName` ='' WHERE `entry`=40142;
UPDATE `creature_template` SET `ScriptName`='mob_halion_meteor', `AIName` ='' WHERE `entry` = 40029;
UPDATE `creature_template` SET `ScriptName`='mob_halion_flame', `AIName` ='' WHERE `entry` IN (40041);
UPDATE `creature_template` SET `ScriptName`='mob_halion_control', `AIName` ='' WHERE `entry` IN (40146);
UPDATE `creature_template` SET `ScriptName`='mob_halion_orb', `AIName` ='' WHERE `entry` IN (40083,40100);
UPDATE `creature_template` SET `ScriptName`='mob_orb_rotation_focus', `AIName` ='' WHERE `entry` = 40091;
UPDATE `creature_template` SET `ScriptName`='mob_orb_carrier', `AIName` ='' WHERE `entry` = 40081;
UPDATE `creature_template` SET `ScriptName`='mob_fiery_combustion', `AIName` ='' WHERE `entry` = 40001;
UPDATE `creature_template` SET `ScriptName`='mob_soul_consumption', `AIName` ='' WHERE `entry` = 40135;
UPDATE `creature_template` SET `ScriptName`='', `AIName` ='' WHERE `entry` IN (40143, 40144, 40145);
#
UPDATE `gameobject_template` SET `data10` = 74807, `faction` = '0', `ScriptName` = 'go_halion_portal_twilight' WHERE `gameobject_template`.`entry` IN (202794,202795);
UPDATE `gameobject_template` SET `faction` = '0', `ScriptName` = 'go_halion_portal_real' WHERE `gameobject_template`.`entry` IN (202796);

-- Baltharus
UPDATE `creature_template` SET `ScriptName`='boss_baltharus', `AIName` ='', `dmg_multiplier` = 80  WHERE `entry`=39751;
UPDATE `creature_template` SET `ScriptName`='mob_baltharus_clone', `AIName` ='', `dmg_multiplier` = 80  WHERE `entry`=39899;
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(39900, 0, 0, 0, 0, 0, 11686, 0, 11686, 0, 'Baltharus Target Dummy', '', NULL, 0, 1, 1, 25, 27, 0, 0, 17, 35, 35, 0, 1, 1.14286, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 258, '');
UPDATE `creature_template` SET `ScriptName`='', `AIName` ='' WHERE `entry`=39900;

-- override for YTDB 558_FIX_10271
INSERT IGNORE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(39921, 0, 0, 0, 0, 0, 31761, 0, 0, 0, 'Baltharus the Warborn', '', '', 0, 83, 83, 3486250, 3486250, 0, 0, 10328, 103, 103, 0, 2.8, 1.71429, 1, 3, 452, 678, 0, 169, 80, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 362, 542, 135, 2, 76, 39751, 0, 39751, 0, 0, 0, 0, 0, 0, 75125, 40504, 74502, 74509, 0, 0, 0, '', 0, 3, 250, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 1568, 803160063, 1, '');
UPDATE `creature_template` SET `difficulty_entry_2` = '39921' WHERE `entry` = 39751;
UPDATE `mangos`.`creature_template` SET `lootid` = '0',`skinloot` = '0' WHERE `creature_template`.`entry` = 39899;

-- Zarithian
UPDATE `creature_template` SET `ScriptName`='boss_zarithian', `AIName` ='' WHERE `entry`=39746;
UPDATE `creature` SET `position_x` = '3008.552734',`position_y` = '530.471680',`position_z` = '89.195290',`orientation` = '6.16' WHERE `id` = 39746;
UPDATE `creature_template` SET `ScriptName`='mob_flamecaller_ruby', `AIName` ='' WHERE `entry`=39814;

-- Saviana Ragefire
UPDATE `creature_template` SET `ScriptName`='boss_ragefire', `AIName` ='' WHERE `entry`=39747;
DELETE FROM `spell_script_target` WHERE `entry` IN (74455);
INSERT INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES 
('74455', '1', '39747');

-- Xerestrasza
UPDATE `creature_template` SET `ScriptName`='mob_xerestrasza', `AIName` ='' WHERE `entry`=40429;

/* Original YTDB Baltharus records 558_FIX_10271
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `PowerType`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(39751, 39920, 39899, 39922, 0, 0, 31761, 0, 0, 0, 'Baltharus the Warborn', '', '', 0, 83, 83, 3486250, 3486250, 0, 0, 0, 10328, 103, 103, 0, 2.8, 1.71429, 1, 3, 452, 678, 0, 169, 80, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 362, 542, 135, 2, 76, 39751, 0, 39751, 0, 0, 0, 0, 0, 0, 75125, 40504, 74502, 74509, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 250, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 1568, 803160063, 1, 'boss_baltharus'),
(39899, 0, 0, 0, 0, 0, 31761, 0, 0, 0, 'Baltharus the Warborn', '', '', 0, 83, 83, 3486250, 3486250, 0, 0, 0, 10328, 103, 103, 0, 2.8, 1.71429, 1, 3, 452, 678, 0, 169, 80, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 362, 542, 135, 2, 76, 39751, 0, 39751, 0, 0, 0, 0, 0, 0, 75125, 40504, 74502, 74509, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 250, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 1568, 803160063, 1, 'mob_baltharus_clone'),
(39920, 0, 0, 0, 0, 0, 31761, 0, 0, 0, 'Baltharus the Warborn (1)', '', '', 0, 83, 83, 11156000, 11156000, 0, 0, 0, 10328, 103, 103, 0, 2.8, 1.71429, 1, 3, 452, 678, 0, 169, 150, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 362, 542, 135, 2, 76, 39751, 0, 39751, 0, 0, 0, 0, 0, 0, 75125, 40504, 74502, 74509, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 800, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 1568, 803160063, 1, ''),
(39922, 0, 0, 0, 0, 0, 31761, 0, 0, 0, 'Baltharus the Warborn (1)', '', '', 0, 83, 83, 11156000, 11156000, 0, 0, 0, 10328, 103, 103, 0, 2.8, 1.71429, 1, 3, 452, 678, 0, 169, 150, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 0, 362, 542, 135, 2, 76, 39751, 0, 39751, 0, 0, 0, 0, 0, 0, 75125, 40504, 74502, 74509, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 800, 1, 0, 0, 0, 0, 0, 0, 0, 171, 1, 1568, 803160063, 1, '');
*/
