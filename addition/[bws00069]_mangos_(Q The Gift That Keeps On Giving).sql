-- Quest The Gift That Keeps On Giving
-- item spell script targets (Scarlet Miners)
DELETE FROM spell_script_target WHERE entry = 52479;
INSERT INTO spell_script_target VALUES
(52479, 1, 28819),
(52479, 1, 28822),
(52479, 1, 28841);

UPDATE `creature_template` SET `AIName` = "EventAI" WHERE `entry` = 28846;
UPDATE `creature_template` SET `ScriptName` = "mob_scarlet_ghoul" WHERE `entry` = 28845;
 
DELETE FROM `creature_ai_texts` WHERE `entry` IN (-286102, -286101, -286100);
INSERT INTO `creature_ai_texts` VALUES
(-286100, "Die, Scourge filth!", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Scarlet Ghost SAY1"),
(-286101, "It won't be that easy, friend!", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Scarlet Ghost SAY2"),
(-286102, "I'll take you with me!", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Scarlet Ghost SAY3");
 
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 28846;
INSERT INTO `creature_ai_scripts` VALUES
(2884601, 28846, 11, 0, 100, 0, 0, 0, 0, 0, 1, -286100, -286101, -286102, 0, 0, 0, 0, 0, 0, 0, 0, "Scarlet Ghost - Random say at spawn");