UPDATE `creature_template` SET `AIName` = 'EventAI', `ScriptName` = '' WHERE `entry` = 16286;
DELETE FROM `creature_ai_scripts` WHERE `creature_id` = 16286;
INSERT INTO `creature_ai_scripts` VALUES
('1628601','16286','6','0','100','6','0','0','0','0','11','29232','0','7','0','0','0','0','0','0','0','0','Spore - Cast Fungal Creep on Death'),
('1628602','16286','1','0','100','6','1000','1000','0','0','41','0','0','0','0','0','0','0','0','0','0','0','Spore - Despawn when OOC');
