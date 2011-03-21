-- The Nexus - The Nexus

UPDATE `creature_template` SET scriptname = 'boss_commander_stoutbeard' where entry = '26796';
UPDATE `creature_template` SET scriptname = 'boss_commander_kolurg' where entry = '26798';

---------------------------------------------------------------------------------------------------------------------
-- instance sunwell plateau fixes

UPDATE creature_template SET ScriptName='mob_sunblade_scout' WHERE entry=25372;

-- fixes for boss_felmyst fight
UPDATE `creature_template` SET `ScriptName`="boss_felmyst", `minhealth`=7300000, `maxhealth`=7300000 WHERE `entry` = 25038;
UPDATE `creature_template` SET `ScriptName`="mob_felmyst_vapor"WHERE `entry`= 25265;
UPDATE `creature_template` SET `ScriptName`="mob_deathcloud" WHERE `entry`=25703;
UPDATE `creature_template` SET `unit_flags` = 0x2020002, `faction_A` = '16', `faction_H` = '16', `minlevel` = '73', `maxlevel` = '73', `ScriptName` = 'mob_felmyst_vapor_trail' WHERE `entry` = '25267';

-- fixes for boss_eredar_twins fight
-- update npc 25165 (Sacrolash) 
UPDATE `creature_template` SET `ScriptName` = "boss_sacrolash", `minhealth`=3500000, `maxhealth`=3500000 WHERE `entry` = 25165;
-- update npc 25166 (Alythess)
UPDATE `creature_template` SET `ScriptName` = "boss_alythess", `minhealth`=3500000, `maxhealth`=3500000  WHERE `entry` = 25166;
-- update npc 25214 (Shadow Image)
UPDATE `creature_template` SET `ScriptName` = "npc_shadow_image" WHERE `entry` = 25214;
-- update go 50439 (The Second Gate)

-- fixes for boss_muru_entropius fight

UPDATE `creature_template` SET `ScriptName` = "boss_muru", `maxhealth`=3500000, `minhealth`=3500000 WHERE `entry` = 25741;
-- update npc 25744 (dark fiend)
UPDATE `creature_template` SET `ScriptName` = "dark_fiend" WHERE `entry` = 25744;
-- update npc 25772 (void sentinel)
UPDATE `creature_template` SET `ScriptName` = "mob_voidsentinel" WHERE `entry` = 25772;
-- update npc 25855 (singularity)
UPDATE `creature_template` SET `ScriptName` = "mob_singularity" WHERE `entry` = 25855;
-- update npc 25770 (m'uru teleport target)
UPDATE `creature_template` SET `flags_extra`= 0, `modelid_1`=25206, `modelid_3`=25206, `InhabitType`=4 WHERE `entry`= 25770;

-- fixes for boss_kiljaeden fight
-- update npc 25608 (kil'jaeden controller)
UPDATE `creature_template` SET `ScriptName` = "mob_kiljaeden_controller" WHERE `entry`= 25608;
-- update npc 25588 (hand of the deceiver)
UPDATE `creature_template` SET `ScriptName` = "mob_deceiver" WHERE `entry`= 25588;
-- update npc 25315 (boss_kiljaeden)
UPDATE `creature_template` SET `ScriptName` = "boss_kiljaeden", `minhealth`=13000000, `maxhealth`=13000000 WHERE `entry`= 25315;
-- update npc 25598 (volatile felfire fiend)
UPDATE `creature_template` SET `ScriptName` = "mob_volatile_felfire_fiend" WHERE `entry` = 25598;
-- update npc 25735 (armageddon target)
UPDATE `creature_template` SET `ScriptName` = "mob_armagedon" WHERE `entry` = 25735;
-- update npc 25502 (shield orb)
UPDATE `creature_template` SET `ScriptName` = "mob_orb" WHERE `entry` = 25502;
-- update npc 25653 (power of the blue flight)
UPDATE `creature_template` SET `ScriptName` = "dragon" WHERE `entry` = 25653;
-- update npc 30598 (shadow spike target)
UPDATE `creature_template` SET `ScriptName` = "mob_shadowspike" WHERE `entry` = 30598;

---------------------------------------------------------------------------------------------------------------------

-- Karazhan instance
UPDATE `creature_template` SET `ScriptName` = 'boss_nightbane' WHERE `entry` = 17225;

-------------------------------------------------------------------------------------------------------------------------

-- Oculus instance
UPDATE `instance_template` SET `ScriptName`='instance_oculus' WHERE `map`=578;

-- Nexus Portal
UPDATE `gameobject_template` SET `ScriptName` = 'go_oculus_portal' WHERE `entry` = 189985;

-- Orb of the Nexus
UPDATE `gameobject_template` SET `ScriptName` = 'go_oculus_portal' WHERE `entry` = 188715;

-- Drakos the Interrogator
UPDATE `creature_template` SET `maxhealth` = 431392 WHERE `entry` = 31558;  -- Hard Instance Version  data from wow.com
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299931 WHERE `entry` = 27654;  -- added immune to pacify
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299931 WHERE `entry` = 31558;  -- added immune to pacify to hard version

-- Varos
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299931 WHERE `entry` = 27447; -- added immune to pacify
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299931 WHERE `entry` = 31559; -- added immune to pacify to hard version 
UPDATE `creature_template` SET `unit_flags` = 33280 WHERE `entry` = 28183;

--  Centrifuge Construct
UPDATE `creature_template` SET `KillCredit1` = 27641 WHERE `entry` = 27641;

-- Eternos
UPDATE `creature_template` SET `npcflag` = 1 WHERE `entry` = 27659; -- YTDB FIX Make eternos capable of gossip - to give out drake

-- herbalism flower   a ytdb bugs flowers cant wander around lol
UPDATE `creature_template` SET `unit_flags` = 33555204, `dynamicflags` = 8 WHERE `entry` = 29888;
UPDATE `creature_template` SET `speed_walk` = 0, `speed_run` = 0, `movementId` = 0 WHERE `entry` = 29888;

--------------------------------------------------------------------------------------------------------------------------

/* ALTERAC VALLEY */
-- Scriptnames
UPDATE `creature_template` SET ScriptName = "boss_galvangar" WHERE entry = 11947;
UPDATE `creature_template` SET ScriptName = "boss_balinda" WHERE entry = 11949;
UPDATE `creature_template` SET ScriptName = "boss_vanndar" WHERE entry = 11948;
UPDATE `creature_template` SET ScriptName = "boss_drekthar" WHERE entry = 11946;

----------------------------------------------------------------------------------------------------------------------------
-- WORLD BOSS, GUARDS AND WORLD LEADERS

UPDATE `creature_template` SET `ScriptName` = 'g_marcus_jonathan' WHERE `entry` = 466;
UPDATE `creature_template` SET `ScriptName` = 'archbishop_benedictus' WHERE `entry` = 1284;
UPDATE `creature_template` SET `ScriptName` = 'highlord_bolvar_fordragon' WHERE `entry` = 1748;
UPDATE `creature_template` SET `ScriptName` = 'overlord_saurfang' WHERE `entry` = 14720;
UPDATE `creature_template` SET `ScriptName` = 'high_fire_mage' WHERE `entry` IN (17098);
UPDATE `creature_template` SET `ScriptName` = 'ancient_of_war' WHERE `entry` IN (3468,3469);
UPDATE `creature_template` SET `ScriptName` = 'high_sorcerer_andromath' WHERE `entry` = 5694;
UPDATE `creature_template` SET `ScriptName` = 'cathedral_of_light' WHERE `entry` = 6171;
UPDATE `creature_template` SET `ScriptName` = 'city_guard' WHERE `entry` IN (3083,1756,4423,12480,12481,12786,12787,12788,12789,12790,12791,13839,14304,20672,20674);
UPDATE `creature_template` SET `ScriptName` = 'city_officer' WHERE `entry` IN (2041,2439,14363,14365,14367,14375,14376,14377,14378,14379,14380,14402,14403,14404,14423,14438,14439,14440,14441,14442,18103,21970,21971);

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

