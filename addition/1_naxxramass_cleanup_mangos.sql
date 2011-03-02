-- cleanup instance Naxxramas (use once!)

UPDATE `creature_template` SET `ScriptName`='', `AIName`='EventAI' WHERE `entry` IN 
(16124,16127, 16286, 16126, 16150, 16125, 16148, 16573, 15929, 15930);

UPDATE `creature_template` SET `ScriptName`='' WHERE `entry` IN 
(16506, 16363, 16360, 15384, 29912 );

DELETE FROM scripted_areatrigger WHERE entry=4112;
INSERT INTO scripted_areatrigger VALUES (4112,'at_naxxramas');

UPDATE instance_template SET ScriptName='instance_naxxramas' WHERE map=533;
UPDATE creature_template SET ScriptName='boss_anubrekhan' WHERE entry=15956;
UPDATE creature_template SET ScriptName='boss_faerlina' WHERE entry=15953;
UPDATE creature_template SET ScriptName='boss_maexxna' WHERE entry=15952;
UPDATE creature_template SET ScriptName='npc_web_wrap' WHERE entry=16486;
UPDATE creature_template SET ScriptName='boss_noth' WHERE entry=15954;
UPDATE creature_template SET ScriptName='boss_heigan' WHERE entry=15936;
UPDATE creature_template SET ScriptName='boss_loatheb' WHERE entry=16011;
UPDATE creature_template SET ScriptName='boss_razuvious' WHERE entry=16061;
UPDATE creature_template SET ScriptName='boss_gothik' WHERE entry=16060;
UPDATE creature_template SET ScriptName='spell_anchor' WHERE entry=16137;
UPDATE creature_template SET ScriptName='boss_thane_korthazz' WHERE entry=16064;
UPDATE creature_template SET ScriptName='boss_sir_zeliek' WHERE entry=16063;
UPDATE creature_template SET ScriptName='boss_lady_blaumeux' WHERE entry=16065;
UPDATE creature_template SET ScriptName='boss_rivendare_naxx' WHERE entry=30549;
UPDATE creature_template SET ScriptName='boss_patchwerk' WHERE entry=16028;
UPDATE creature_template SET ScriptName='boss_grobbulus' WHERE entry=15931;
UPDATE creature_template SET ScriptName='boss_gluth' WHERE entry=15932;
-- UPDATE creature_template SET ScriptName='boss_thaddius' WHERE entry=15928;
-- UPDATE creature_template SET ScriptName='boss_stalagg' WHERE entry=15929;
-- UPDATE creature_template SET ScriptName='boss_feugen' WHERE entry=15930;
UPDATE creature_template SET ScriptName='boss_sapphiron' WHERE entry=15989;
UPDATE creature_template SET ScriptName='boss_kelthuzad' WHERE entry=15990;
