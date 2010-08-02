/* VIOLET HOLD */
UPDATE `instance_template` SET `ScriptName`='instance_violet_hold' WHERE `map`=608;
UPDATE `creature_template` SET `ScriptName`='npc_sinclari', `npcflag`=1,`minhealth`=50400, `maxhealth`=50400 WHERE `entry`='30658'; -- 64
UPDATE `creature_template` SET `ScriptName`='npc_azure_saboteur' WHERE `entry`='31079';
UPDATE `creature_template` SET `ScriptName`='boss_cyanigosa' WHERE `entry`='31134';
UPDATE `creature_template` SET `ScriptName`='boss_erekem' WHERE `entry`='29315';
UPDATE `creature_template` SET `ScriptName`='mob_erekem_guard' WHERE `entry`='29395';
UPDATE `creature_template` SET `ScriptName`='boss_ichoron' WHERE `entry`='29313';
UPDATE `creature_template` SET `ScriptName`='mob_ichor_globule',`modelid_1`=5492, `modelid_3`=5492 WHERE `entry`='29321';
UPDATE `creature_template` SET `modelid_1`=5492, `modelid_3`=5492 WHERE `entry`='31515'; -- heroic
UPDATE `creature_template` SET `ScriptName`='boss_lavanthor' WHERE `entry`='29312';
UPDATE `creature_template` SET `ScriptName`='boss_moragg' WHERE `entry`='29316';
UPDATE `creature_template` SET `ScriptName`='boss_xevozz' WHERE `entry`='29266';
UPDATE `creature_template` SET `ScriptName`='mob_ethereal_sphere' WHERE `entry`='29271';
UPDATE `creature_template` SET `ScriptName`='boss_zuramat' WHERE `entry`='29314';
UPDATE `creature_template` SET `ScriptName`='mob_zuramat_sentry' WHERE `entry`='29364';
UPDATE `creature_template` SET `ScriptName`='npc_violet_portal' WHERE `entry`='31011';

DELETE FROM `creature` WHERE map = 608 AND `id`='31011';
DELETE FROM `creature` WHERE map = 608 AND `id`='31134'; -- cyanigosa should not spawn
UPDATE `gameobject_template` SET `flags`=`flags`|4 WHERE `entry` IN (191723,191564,191563,191562,191606,191722,191556,191566,191565); -- door untargetable

UPDATE `creature_template` SET AIName='EventAI',`ScriptName`='' WHERE `entry` IN ('30660','30695','30666','30668','30667','32191');
DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN ('30660','30695','30666','30668','30667','32191');
INSERT INTO `creature_ai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES
(3066001, 30660, 0, 0, 100, 7, 5000, 7000, 6000, 9000, 11, 58504, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Guardian - Cast Agonizing Strike'),
(3066002, 30660, 0, 0, 100, 7, 7000, 12000, 9000, 11000, 11, 58508, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Guardian - Cast Side Swipe'),
(3066601, 30666, 1, 0, 100, 6, 1000, 1000, 0, 0, 11, 58040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Captain - Cast Destroy Door Seal on Spawn'),
(3066602, 30666, 9, 0, 100, 7, 0, 5, 5000, 9000, 11, 41056, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Captain - Cast Whirlwind'),
(3066603, 30666, 0, 0, 100, 7, 3000, 6000, 7000, 10000, 11, 32736, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Captain - Cast Mortal Strike'),
(3066701, 30667, 1, 0, 100, 6, 0, 0, 0, 0, 21, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Prevent Combat Movement and Set Phase to 0 on Spawn'),
(3066702, 30667, 1, 0, 100, 6, 1000, 1000, 0, 0, 11, 58040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Cast Destroy Door Seal on Spawn'),
(3066703, 30667, 4, 0, 100, 2, 0, 0, 0, 0, 11, 60181, 4, 0, 23, 1, 0, 0, 0, 0, 0, 0, 'Azure Sorceror (Normal) - Cast Arcane Stream and Set Phase 1 on Aggro'),
(3066704, 30667, 9, 5, 100, 3, 5, 35, 6000, 8000, 11, 60181, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror (Normal) - Cast Arcane Stream (Phase 1)'),
(3066705, 30667, 4, 0, 100, 4, 0, 0, 0, 0, 11, 60204, 4, 0, 23, 1, 0, 0, 0, 0, 0, 0, 'Azure Sorceror (Heroic) - Cast Arcane Stream and Set Phase 1 on Aggro'),
(3066706, 30667, 9, 5, 100, 5, 5, 35, 6000, 8000, 11, 60204, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror (Heroic) - Cast Arcane Stream (Phase 1)'),
(3066707, 30667, 3, 5, 100, 6, 7, 0, 0, 0, 21, 1, 0, 0, 23, 1, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Start Combat Movement and Set Phase 2 when Mana is at 7% (Phase 1)'),
(3066708, 30667, 9, 5, 100, 6, 30, 80, 0, 0, 21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Start Combat Movement at 30 Yards (Phase 1)'),
(3066709, 30667, 9, 5, 100, 6, 5, 15, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Prevent Combat Movement at 15 Yards (Phase 1)'),
(3066710, 30667, 9, 5, 100, 6, 0, 5, 0, 0, 21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Start Combat Movement Below 5 Yards'),
(3066711, 30667, 9, 0, 100, 3, 0, 8, 9000, 15000, 11, 60182, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror (Normal) - Cast Mana Detonation'),
(3066712, 30667, 9, 0, 100, 5, 0, 8, 9000, 15000, 11, 60205, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror (Heroic) - Cast Mana Detonation'),
(3066713, 30667, 3, 3, 100, 7, 100, 15, 100, 100, 23, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Set Phase 1 when Mana is above 15% (Phase 2)'),
(3066714, 30667, 7, 0, 100, 6, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Sorceror - Set Phase to 0 on Evade'),
(3066801, 30668, 1, 0, 100, 6, 1000, 1000, 0, 0, 11, 58040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Raider - Cast Destroy Door Seal on Spawn'),
(3066802, 30668, 9, 0, 100, 7, 0, 8, 9000, 14000, 11, 52719, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Raider - Cast Concussion Blow'),
(3066803, 30668, 0, 0, 100, 7, 7000, 14000, 12000, 18000, 11, 60158, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Raider - Cast Magic Reflection'),
(3069501, 30695, 1, 0, 100, 6, 0, 0, 0, 0, 21, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Prevent Combat Movement and Set Phase to 0 on Spawn'),
(3069502, 30695, 4, 0, 100, 2, 0, 0, 0, 0, 11, 58531, 1, 0, 23, 1, 0, 0, 0, 0, 0, 0, 'Portal Keeper (Normal) - Cast Arcane Missiles and Set Phase 1 on Aggro'),
(3069503, 30695, 9, 5, 100, 3, 0, 45, 6000, 8000, 11, 58531, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper (Normal) - Cast Arcane Missiles (Phase 1)'),
(3069504, 30695, 4, 0, 100, 4, 0, 0, 0, 0, 11, 61593, 1, 0, 23, 1, 0, 0, 0, 0, 0, 0, 'Portal Keeper (Heroic) - Cast Arcane Missiles and Set Phase 1 on Aggro'),
(3069505, 30695, 9, 5, 100, 5, 0, 45, 6000, 8000, 11, 61593, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper (Heroic) - Cast Arcane Missiles (Phase 1)'),
(3069506, 30695, 3, 5, 100, 6, 7, 0, 0, 0, 21, 1, 0, 0, 23, 1, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Start Combat Movement and Set Phase 2 when Mana is at 7% (Phase 1)'),
(3069507, 30695, 9, 5, 100, 6, 40, 80, 0, 0, 21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Start Combat Movement at 40 Yards (Phase 1)'),
(3069508, 30695, 9, 5, 100, 6, 5, 15, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Prevent Combat Movement at 15 Yards (Phase 1)'),
(3069509, 30695, 9, 5, 100, 6, 0, 5, 0, 0, 21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Start Combat Movement Below 5 Yards'),
(3069510, 30695, 9, 0, 100, 3, 0, 8, 13000, 19000, 11, 58532, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper (Normal) - Cast Frostbolt Volley'),
(3069511, 30695, 9, 0, 100, 5, 0, 8, 13000, 19000, 11, 61594, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper (Heroic) - Cast Frostbolt Volley'),
(3069512, 30695, 0, 0, 100, 7, 0, 8, 9000, 14000, 11, 58534, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Cast Deep Freeze'),
(3069513, 30695, 3, 3, 100, 7, 100, 15, 100, 100, 23, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Set Phase 1 when Mana is above 15% (Phase 2)'),
(3069514, 30695, 7, 0, 100, 6, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Portal Keeper - Set Phase to 0 on Evade'),
(3219101, 32191, 1, 0, 100, 6, 1000, 1000, 0, 0, 11, 58040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Stalker - Cast Destroy Door Seal on Spawn'),
(3219102, 32191, 9, 0, 100, 7, 0, 5, 5000, 7000, 11, 58471, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Stalker - Cast Backstab'),
(3219103, 32191, 0, 0, 100, 7, 11000, 16000, 13000, 21000, 11, 58470, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 'Azure Stalker - Cast Tactical Blink');

DELETE FROM `spell_script_target` WHERE `entry` IN (54160,59474);
INSERT INTO `spell_script_target` VALUES (54160, 1, 29266);
INSERT INTO `spell_script_target` VALUES (59474, 1, 29266);

UPDATE `creature_template` SET `ScriptName`='npc_door_seal_vh', `unit_flags`=33816580 WHERE `entry`=30896;
UPDATE `creature_template` SET `faction_A`=35, faction_H=35 WHERE `entry`=30658;
UPDATE `creature_template` SET `faction_A`=1720, faction_H=1720, `ScriptName`='mob_vh_dragons', `minlevel`=70,`maxlevel`=70,`minhealth`=7000,`maxhealth`=8000  WHERE `entry` IN (30660, 30661, 30662, 30663, 30664, 30666, 30667, 30668, 32191, 30695);

