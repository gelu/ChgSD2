-- quest 12801
UPDATE `creature_template` SET `AIName` = '', `ScriptName`='npc_highlord_darion_mograine' WHERE `entry`='29173';
UPDATE `creature_template` SET `AIName` = '', `ScriptName`='npc_the_lich_king_tirion_dawn' WHERE `entry` in (29183,29175);
UPDATE `creature_template` SET `AIName` = '', `ScriptName`='npc_minibosses_dawn_of_light' WHERE `entry` IN (29199,29204,29200);
UPDATE `creature_template` SET `AIName` = '', `ScriptName`='mob_acherus_ghoul' WHERE `entry`='29219';
UPDATE `creature_template` SET `AIName` = '', `ScriptName`='mob_warrior_of_the_frozen_wastes' WHERE `entry`='29206';

UPDATE `creature_template` SET `AIName`='EventAI', `unit_flags`=0,`type_flags`=8 WHERE entry IN (29174,29182,29186,29190,29219,29206,29176,29178,29179,29180,29177,29181);

DELETE FROM `spell_script_target` WHERE `entry` in (53658, 53679, 53701, 53705, 53706, 53677, 53685);
INSERT INTO `spell_script_target` VALUES (53679, 1, 29183);
INSERT INTO `spell_script_target` VALUES (53701, 1, 29175);
INSERT INTO `spell_script_target` VALUES (53705, 1, 29183);
INSERT INTO `spell_script_target` VALUES (53706, 1, 29183);
INSERT INTO `spell_script_target` VALUES (53677, 1, 29227);
INSERT INTO `spell_script_target` VALUES (53685, 1, 29175);

DELETE FROM `creature_ai_scripts` WHERE `id` BETWEEN 90051 AND 90084;
INSERT INTO `creature_ai_scripts` VALUES ('90063', '29174', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '29427', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HOLY_LIGHT1');
INSERT INTO `creature_ai_scripts` VALUES ('90064', '29174', '4', '0', '100', '0', '0', '0', '0', '0', '11', '53625', '1', '5', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light aggro');
INSERT INTO `creature_ai_scripts` VALUES ('90065', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53625', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HEROIC_LEAP');
INSERT INTO `creature_ai_scripts` VALUES ('90066', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53643', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HOLY_STRIKE');
INSERT INTO `creature_ai_scripts` VALUES ('90067', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53638', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_HOLY_WRATH');
INSERT INTO `creature_ai_scripts` VALUES ('90068', '29174', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '53629', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Defender of the Light - SPELL_UPPERCUT');
INSERT INTO `creature_ai_scripts` VALUES ('90069', '29182', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '33642', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Rimblat Earthshatter - SPELL_CHAIN_HEAL');
INSERT INTO `creature_ai_scripts` VALUES ('90070', '29182', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53630', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rimblat Earthshatter - SPELL_THUNDER');
INSERT INTO `creature_ai_scripts` VALUES ('90071', '29186', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53633', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rampaging Abomination - SPELL_CLEAVE1');
INSERT INTO `creature_ai_scripts` VALUES ('90072', '29186', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '50335', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rampaging Abomination - SPELL_SCOURGE_HOOK');
INSERT INTO `creature_ai_scripts` VALUES ('90073', '29190', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53634', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Flesh Behemoth - SPELL_SCOURGE_HOOK');
INSERT INTO `creature_ai_scripts` VALUES ('90074', '29190', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '36706', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Flesh Behemoth - SPELL_THUNDERCLAP');
INSERT INTO `creature_ai_scripts` VALUES ('90075', '29190', '0', '0', '100', '3', '5000', '10000', '5000', '10000', '11', '53627', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Flesh Behemoth - SPELL_THUNDERCLAP');
INSERT INTO `creature_ai_scripts` VALUES ('90076', '29219', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53632', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Volatile Ghoul - SPELL_GHOULPLOSION');
INSERT INTO `creature_ai_scripts` VALUES ('90077', '29206', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53631', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Warrior of the Frozen Wastes - SPELL_CLEAVE');
INSERT INTO `creature_ai_scripts` VALUES ('90078', '29176', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53631', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Korfax - SPELL_CLEAVE');
INSERT INTO `creature_ai_scripts` VALUES ('90079', '29176', '0', '0', '100', '3', '10000', '20000', '10000', '10000', '11', '53625', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Korfax - SPELL_CLEAVE');
INSERT INTO `creature_ai_scripts` VALUES ('90080', '29177', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '37979', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Commander Eligor Dawnbringer - SPELL_HOLY_LIGHT2');
INSERT INTO `creature_ai_scripts` VALUES ('90081', '29181', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '20664', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_REJUVENATION');
INSERT INTO `creature_ai_scripts` VALUES ('90082', '29181', '14', '0', '100', '3', '10000', '20', '5000', '10000', '11', '25817', '6', '1', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_TRANQUILITY');
INSERT INTO `creature_ai_scripts` VALUES ('90083', '29181', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '20678', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_STARFALL');
INSERT INTO `creature_ai_scripts` VALUES ('90084', '29181', '0', '0', '100', '3', '10000', '20000', '10000', '20000', '11', '21807', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'Rayne - SPELL_WRATH');

-- YTDB cleanup
DELETE FROM `creature` WHERE `map` = 609 AND `guid` IN (116863);
DELETE FROM `creature` WHERE `map` = 609 AND `id` IN (29219,29206,29190);

