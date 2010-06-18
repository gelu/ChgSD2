-- Forge of souls
UPDATE `instance_template` SET `script`='instance_forge_of_souls' WHERE `map`=632;
UPDATE `creature_template` SET `ScriptName`='boss_bronjahm', `AIName` ='' WHERE `entry`=36497;
UPDATE `creature_template` SET `ScriptName`='mob_soul_fragment', `modelid_A`= 30233, `modelid_H`= 30233, `AIName` ='' WHERE `entry`=36535;
-- UPDATE `creature_template` SET `ScriptName`='mob_soul_storm', `AIName` ='' WHERE `entry`=;

-- UPDATE `creature_template` SET `ScriptName`='boss_devourer', `AIName` ='' WHERE `entry`=33113;
UPDATE `creature_template` SET `AIName`='', `Scriptname`='boss_devourer_of_souls' where `entry` IN (36502);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_well_of_soul' where `entry` IN (36536);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_unleashed_soul' where `entry` IN (36595);

UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_jaina_and_sylvana_FSintro' where `entry` IN (37597, 37596);
UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_jaina_and_sylvana_FSextro' where `entry` IN (38160, 38161);
UPDATE `creature_template` SET `scale`='0.8', `equipment_id`='1221' where `entry` IN (37597, 38160, 36993, 38188, 37221, 36955);
UPDATE `creature_template` SET `scale`='0.8' where `entry` IN (36658, 37225, 37223, 37226, 37554);
UPDATE `creature_template` SET `npcflag`='0' where `entry` IN (38160, 38161);
UPDATE `creature_template` SET `npcflag`='3' where `entry` IN (37597, 37596, 36993, 36990);
UPDATE `creature_template` SET `scale`='1' where `entry` IN (38161, 37596, 36990);
UPDATE `creature_template` SET `scale`='1' where `entry` IN (37755);
UPDATE `creature_template` SET `equipment_id`='1290' where `entry` IN (36990, 37596, 38161, 38189, 37223, 37554);
-- UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_watchman' where `entry` IN (36478);
-- UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_reaper' where `entry` IN (36499);
-- UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_adept' where `entry` IN (36620);
-- UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_bonecaster' where `entry` IN (36564);
-- UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soulguard_animator' where `entry` IN (36516);
-- UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_soul_horror' where `entry` IN (36522);
-- UPDATE `creature_template` SET `AIName`='', `Scriptname`='npc_npc_spectral_warden' where `entry` IN (36666);

-- Pit of saron
-- UPDATE `instance_template` SET `script`='instance_pit_of_saron' WHERE `map`=658;
-- UPDATE `creature_template` SET `ScriptName`='boss_ick', `AIName` ='' WHERE `entry`=36476;
-- UPDATE `creature_template` SET `ScriptName`='boss_krick', `AIName` ='' WHERE `entry`=36477;
-- UPDATE `creature_template` SET `ScriptName`='mob_exploding_orb', `AIName` ='' WHERE `entry`=36610;
-- UPDATE `creature_template` SET `ScriptName`='boss_forgemaster_garfrost', `AIName` ='' WHERE `entry`=36494;
-- UPDATE `creature_template` SET `ScriptName`='boss_scourgelord_tyrannus', `AIName` ='' WHERE `entry`=36658;

-- Halls of reflection
UPDATE `instance_template` SET `script`='instance_halls_of_reflection' WHERE `map`=668;
UPDATE `creature_template` SET `ScriptName`='boss_falryn' WHERE `entry`=38112;
UPDATE `creature_template` SET `ScriptName`='boss_marwyn' WHERE `entry`=38113;
-- UPDATE `creature_template` SET `ScriptName`='boss_lich_king_fh' WHERE `entry`=37226;
UPDATE `gameobject_template` SET ScriptName = 'go_frostmourne_altar' WHERE `entry` = 202236;
UPDATE `gameobject_template` SET ScriptName = 'go_frostmourne' WHERE `entry` = 202302;
DELETE FROM `creature` WHERE `map` = 668 AND `id` IN (38177,38176,38173,38172,38567,38175);
UPDATE `creature_template` SET `ScriptName`='generic_creature' WHERE `entry` IN (38177,38176,38173,38172,38567,38175);
UPDATE `gameobject_template` SET `faction` = '114' WHERE `entry` IN (197341, 201976);
