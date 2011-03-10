-- DK Ghoul (Raise Dead & Army of the Dead)

UPDATE `creature_template` SET `ScriptName` = "pet_dk_ghoul" WHERE `entry` IN (26125, 24207);

-- Greater Earth Elemental

UPDATE `creature_template` SET `ScriptName` = "pet_greater_earth_elemental" WHERE `entry` = '15352';

-- Greater Fire Elemental

UPDATE `creature_template` SET `ScriptName` = "pet_greater_fire_elemental" WHERE `entry` = '15438';

-- Training Dummy

UPDATE `creature_template` SET `AIName` = 'EventAI', `flags_extra` = '2', `unit_flags` = '393220', `mechanic_immune_mask` = '4399133',  `ScriptName` = 'npc_training_dummy' WHERE `entry` IN (32666, 33229, 33243, 33272, 31146, 32667, 31144, 25225, 32666, 5652, 17578, 19139, 17059, 17060, 2673, 18504, 32542, 32546, 32545, 32543, 1921, 18504, 16211, 5202, 27222, 27223, 32541);

-- ScriptName's for Quest Massacre at Light's Point

UPDATE `creature_template` SET `ScriptName` = 'npc_mine_car' WHERE `entry` = '28817';
UPDATE `creature_template` SET `ScriptName` = 'npc_scarlet_miner' WHERE `entry` = '28841';

-- Instance gundrak

UPDATE `creature_template` SET `ScriptName` = 'mob_colossus_elemental' WHERE `entry` = '29573'; -- boss colossus mob colossus elemental

-- Thargold Ironwing

UPDATE `creature_template` SET `ScriptName`='npc_thargold_ironwing' WHERE `entry`='29154';

-- Thirst Unending

UPDATE `creature_template` SET `ScriptName`='npc_mana_wyrm' WHERE `entry`='15274';

-- Feast of Winter Veil Fixes

UPDATE `creature_template` SET `ScriptName` = 'npc_winter_reveler' WHERE `entry`='15760';
UPDATE `creature_template` SET `ScriptName` = 'npc_metzen', `npcflag` = '1' WHERE entry = '15664';

-- ScriptName's and Walk Speed for Escort Gryphon (Massacre at Light's Point)

UPDATE `creature_template` SET `speed_walk`='2.7', `speed_run`='2.7', `ScriptName`='npc_scourge_gryphon' WHERE `entry`='28864';

-- NPC Slag in instance Halls of Lightning

UPDATE `creature_template` SET `ScriptName`='npc_slag' WHERE `entry`='28585';

-- Instance Ahn'kahet

UPDATE `creature_template` SET `ScriptName` = 'mob_flame_orb' WHERE `entry` = '30702';
UPDATE `creature_template` SET `ScriptName` = 'npc_twilight_volunteer' WHERE `entry` = '30385';
UPDATE `creature_template` SET `ScriptName` = 'mob_volazj_clone' WHERE `entry` IN ('30621', '30622', '30623', '30624', '30625');
UPDATE `creature_template` SET `ScriptName` = 'boss_volazj' WHERE `entry` = '29311';
