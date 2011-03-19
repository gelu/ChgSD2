-- TitleMaster
-- OLD SQL --
/*
REPLACE into `creature_template` (`entry`,`difficulty_entry_1`,`difficulty_entry_2`,`difficulty_entry_3`,`KillCredit1`,`KillCredit2`,`modelid_1`,`modelid_2`,`modelid_3`,`modelid_4`,`name`,`subname`,`IconName`,`gossip_menu_id`,`minlevel`,`maxlevel`,`minhealth`,`maxhealth`,`minmana`,`maxmana`,`armor`,`faction_A`,`faction_H`,`npcflag`,`speed_walk`,`speed_run`,`scale`,`rank`,`mindmg`,`maxdmg`,`dmgschool`,`attackpower`,`dmg_multiplier`,`baseattacktime`,`rangeattacktime`,`unit_class`,`unit_flags`,`dynamicflags`,`family`,`trainer_type`,`trainer_spell`,`trainer_class`,`trainer_race`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`type`,`type_flags`,`lootid`,`pickpocketloot`,`skinloot`,`resistance1`,`resistance2`,`resistance3`,`resistance4`,`resistance5`,`resistance6`,`spell1`,`spell2`,`spell3`,`spell4`,`PetSpellDataId`,`mingold`,`maxgold`,`AIName`,`MovementType`,`InhabitType`,`unk16`,`unk17`,`RacialLeader`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`movementId`,`RegenHealth`,`equipment_id`,`mechanic_immune_mask`,`flags_extra`,`ScriptName`) VALUES ('90006','0','0','0','0','0','7061','0','0','0','Title Master','','','0','80','80','25000','25000','25000','25000','500000','35','35','1','2.5','8','1','1','25000','25001','0','656','1','2000','1760','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','90','90','90','90','90','90','0','0','0','0','0','0','0','0','0','EventAI','0','3','1','1','0','0','0','0','0','0','0','0','1','0','0','0','custom_npc_titlemaster');
*/

-- NEW SQL --
DELETE FROM `creature_template` WHERE (`entry`=900008);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid_1`, `modelid_2`, `modelid_3`, `modelid_4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `unk16`, `unk17`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `vendor_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (900008, 0, 0, 0, 0, 0, 7061, 0, 0, 0, 'Title Master', '', '', 0, 80, 80, 25000, 25000, 25000, 25000, 50000, 35, 35, 1, 1, 1.14286, 1, 0, 25000, 43, 0, 11, 1, 1500, 1500, 1, 2, 0, 0, 0, 0, 0, 0, 23, 34, 9, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1.05, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 'custom_npc_titlemaster');