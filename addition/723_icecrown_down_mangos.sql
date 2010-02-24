-- UPDATE `instance_template` SET `script`='instance_forge_of_souls' WHERE `map`=632;

-- UPDATE `creature_template` SET `ScriptName`='boss_bronjahm' WHERE `entry`=36497;
-- UPDATE `creature_template` SET `ScriptName`='boss_devourer' WHERE `entry`=33113;

-- UPDATE `instance_template` SET `script`='instance_pit_of_saron' WHERE `map`=658;

UPDATE `instance_template` SET `script`='instance_halls_of_reflection' WHERE `map`=668;
UPDATE `creature_template` SET `ScriptName`='boss_falryn' WHERE `entry`=38112;
UPDATE `creature_template` SET `ScriptName`='boss_marwyn' WHERE `entry`=38113;
-- UPDATE `creature_template` SET `ScriptName`='boss_lich_king_fh' WHERE `entry`=37226;
UPDATE `gameobject_template` SET ScriptName = 'go_frostmourne_altar' WHERE `entry` = 202236;
UPDATE `gameobject_template` SET ScriptName = 'go_frostmourne' WHERE `entry` = 202302;
DELETE FROM `creature` WHERE `map` = 668 AND `id` IN (38177,38176,38173,38172,38567,38175);
UPDATE `creature_template` SET `ScriptName`='generic_creature' WHERE `entry` IN (38177,38176,38173,38172,38567,38175);
