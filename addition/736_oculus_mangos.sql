-- Oculus instance

-- from traponinet
/* Belgaristrasz and his companions give Drake, after completed quest (13124) */
UPDATE `creature_template` SET `npcflag` = npcflag|1 WHERE `entry` IN (27657, 27658, 27659);
UPDATE `creature_template` SET `gossip_menu_id` = 27657 WHERE `entry` = 27657;
UPDATE `creature_template` SET `gossip_menu_id` = 27658 WHERE `entry` = 27658;
UPDATE `creature_template` SET `gossip_menu_id` = 27659 WHERE `entry` = 27659;
DELETE FROM `gossip_scripts` WHERE id IN (27657, 27658, 27659);
INSERT INTO `gossip_scripts` VALUES (27657,0,17,37815,1,0,0,0,0,0,0,0,0,0,0,0,''),(27658,0,17,37860,1,0,0,0,0,0,0,0,0,0,0,0,''),(27659,0,17,37859,1,0,0,0,0,0,0,0,0,0,0,0,'');
DELETE FROM `gossip_menu_option` WHERE `menu_id` IN (27657, 27658, 27659);
INSERT INTO `gossip_menu_option` VALUES 
(27657,0,0,'Take the Emerald Essence if you want to fly on the wings of the Green Flight.',1,1,0,0,27657,0,0,NULL,9,13126,0,16,37815,1,0,0,0),
(27657,1,0,'Take the Emerald Essence if you want to fly on the wings of the Green Flight.',1,1,0,0,27657,0,0,NULL,9,13127,0,16,37815,1,0,0,0),
(27657,2,0,'Take the Emerald Essence if you want to fly on the wings of the Green Flight.',1,1,0,0,27657,0,0,NULL,9,13128,0,16,37815,1,0,0,0),
(27659,0,0,'Take the Amber Essence if you want to fly on the wings of the Bronze Flight.',1,1,0,0,27659,0,0,NULL,9,13126,0,16,37859,1,0,0,0),
(27659,1,0,'Take the Amber Essence if you want to fly on the wings of the Bronze Flight.',1,1,0,0,27659,0,0,NULL,9,13127,0,16,37859,1,0,0,0),
(27659,2,0,'Take the Amber Essence if you want to fly on the wings of the Bronze Flight.',1,1,0,0,27659,0,0,NULL,9,13128,0,16,37859,1,0,0,0),
(27658,1,0,'Take the Ruby Essence if you want to fly on the wings of the Red Flight.',1,1,0,0,27658,0,0,NULL,9,13126,0,16,37860,1,0,0,0),
(27658,2,0,'Take the Ruby Essence if you want to fly on the wings of the Red Flight.',1,1,0,0,27658,0,0,NULL,9,13127,0,16,37860,1,0,0,0),
(27658,3,0,'Take the Ruby Essence if you want to fly on the wings of the Red Flight.',1,1,0,0,27658,0,0,NULL,9,13128,0,16,37860,1,0,0,0),
(27658,0,0,'GOSSIP_OPTION_QUESTGIVER',2,2,0,0,0,0,0,NULL,0,0,0,0,0,0,0,0,0);
/* hack for broken Nexus Portal */
UPDATE `gameobject_template` SET `data0` = 49665 WHERE `entry` = 189985;
UPDATE `spell_target_position` SET `id` = 49665 WHERE `id` = 49305;
/* summon Image of Belgaristrasz */
REPLACE INTO `creature_ai_scripts` VALUES (2744751,27447,6,0,100,30,0,0,0,0,12,28012,0,180000,0,0,0,0,0,0,0,0,'q13126');
REPLACE INTO `creature_ai_scripts` VALUES (2765551,27655,6,0,100,30,0,0,0,0,12,28012,0,180000,0,0,0,0,0,0,0,0,'q13127');
UPDATE `creature_template` SET `AIName` = 'EventAI', `ScriptName` = '' WHERE `entry` IN (27447,27655);

UPDATE `creature_template` SET `spell6` = 57403, `InhabitType` = 3 WHERE `entry` IN (27692,27755,27756,28670,32535);

-- from me
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = 'mob_oculus_dragon' WHERE `entry` IN (27692,27756,27755);
DELETE FROM `spell_script_target` WHERE `entry` IN (49460, 49346, 49464);
INSERT INTO `spell_script_target` VALUES (49460, 1, 27755);
INSERT INTO `spell_script_target` VALUES (49346, 1, 27692);
INSERT INTO `spell_script_target` VALUES (49464, 1, 27756);
