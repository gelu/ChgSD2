DELETE FROM creature_template_addon WHERE entry IN (30385, 31474);
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `emote`, `moveflags`, `auras`) VALUES
('30385', '0', '8', '1', '0', '0', NULL),
('31474', '0', '8', '1', '0', '0', NULL);

DELETE FROM creature_template WHERE entry IN (31474, 30385, 29310, 31465);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_A`, `modelid_A2`, `modelid_H`, `modelid_H2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(29310, 31465, 0, 0, 0, 0, 26777, 0, 26777, 0, 'Jedoga Shadowseeker', '', '', 0, 75, 75, 212700, 212700, 0, 0, 8204, 16, 16, 0, 1, 1, 339, 481, 0, 370, 7.5, 2000, 0, 2, 0, 0, 0, 0, 0, 0, 0, 293, 436, 53, 7, 0, 29310, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4176, 6960, '', 0, 3, 20, 20, 0, 0, 0, 0, 0, 0, 0, 172, 1, 1575, 0, 0, 'boss_jedoga'),
(30385, 31474, 0, 0, 0, 0, 27382, 27384, 27383, 27385, 'Twilight Volunteer', '', '', 0, 74, 74, 25705, 25705, 0, 0, 0, 16, 16, 0, 1, 1, 0, 0, 0, 0, 1, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 2.5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 93, 0, 0, 'npc_twilight_volunteer'),
(31465, 0, 0, 0, 0, 0, 26777, 0, 26777, 0, 'Jedoga Shadowseeker (1)', '', '', 0, 82, 82, 431392, 431392, 0, 0, 10253, 16, 16, 0, 1, 1, 463, 640, 0, 726, 13, 2000, 0, 2, 0, 0, 0, 0, 0, 0, 0, 360, 520, 91, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8352, 13920, '', 0, 3, 32, 20, 0, 0, 0, 0, 0, 0, 0, 172, 1, 0, 0, 1, ''),
(31474, 0, 0, 0, 0, 0, 27382, 27384, 27383, 27385, 'Twilight Volunteer (1)', '', '', 0, 81, 81, 58648, 58648, 0, 0, 0, 16, 16, 0,  1, 1, 0, 0, 0, 0, 1, 2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 4.5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '');

DELETE FROM creature_addon WHERE guid=131953;
DELETE FROM creature WHERE guid=131953;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`) VALUES
(131953, 29310, 619, 3, 1, 26777, 0, 357.353, -692.808, -10.7028, 5.56541, 14400, 5, 0, 212700, 0, 0, 1);


