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
