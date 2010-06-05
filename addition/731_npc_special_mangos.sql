-- Snakes trap
UPDATE `creature_template` SET `ScriptName`='npc_snake_trap_serpents' WHERE `entry` IN (19921, 19833);
-- Mirror Immage
UPDATE `creature_template` SET `speed_walk` = 2.5, modelid_A = 11686,  `minlevel` = 80, `maxlevel` = 80, `AIName` = '', `ScriptName`='npc_mirror_image' WHERE `entry` = 31216;
-- Rune blade
UPDATE `creature_template` SET `modelid_A` = '11686', `modelid_H` = 11686, `AIName` = '', `ScriptName`='npc_runeblade' WHERE `entry` = 27893;
