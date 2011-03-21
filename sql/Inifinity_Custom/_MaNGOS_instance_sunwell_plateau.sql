# instance sunwell plateau fixes

UPDATE creature_template SET ScriptName='mob_sunblade_scout' WHERE entry=25372;

# fixes for boss_felmyst fight
UPDATE `creature_template` SET `ScriptName`="boss_felmyst", `minhealth`=7300000, `maxhealth`=7300000 WHERE `entry` = 25038;
UPDATE `creature_template` SET `ScriptName`="mob_felmyst_vapor"WHERE `entry`= 25265;
UPDATE `creature_template` SET `ScriptName`="mob_deathcloud" WHERE `entry`=25703;
UPDATE `creature_template` SET `unit_flags` = 0x2020002, `faction_A` = '16', `faction_H` = '16', `minlevel` = '73', `maxlevel` = '73', `ScriptName` = 'mob_felmyst_vapor_trail' WHERE `entry` = '25267';

# fixes for boss_eredar_twins fight
# update npc 25165 (Sacrolash) 
UPDATE `creature_template` SET `ScriptName` = "boss_sacrolash", `minhealth`=3500000, `maxhealth`=3500000 WHERE `entry` = 25165;
# update npc 25166 (Alythess)
UPDATE `creature_template` SET `ScriptName` = "boss_alythess", `minhealth`=3500000, `maxhealth`=3500000  WHERE `entry` = 25166;
# update npc 25214 (Shadow Image)
UPDATE `creature_template` SET `ScriptName` = "npc_shadow_image" WHERE `entry` = 25214;
# update go 50439 (The Second Gate)

# fixes for boss_muru_entropius fight

UPDATE `creature_template` SET `ScriptName` = "boss_muru", `maxhealth`=3500000, `minhealth`=3500000 WHERE `entry` = 25741;
# update npc 25744 (dark fiend)
UPDATE `creature_template` SET `ScriptName` = "dark_fiend" WHERE `entry` = 25744;
# update npc 25772 (void sentinel)
UPDATE `creature_template` SET `ScriptName` = "mob_voidsentinel" WHERE `entry` = 25772;
# update npc 25855 (singularity)
UPDATE `creature_template` SET `ScriptName` = "mob_singularity" WHERE `entry` = 25855;
# update npc 25770 (m'uru teleport target)
UPDATE `creature_template` SET `flags_extra`= 0, `modelid_1`=25206, `modelid_3`=25206, `InhabitType`=4 WHERE `entry`= 25770;

# fixes for boss_kiljaeden fight
# update npc 25608 (kil'jaeden controller)
UPDATE `creature_template` SET `ScriptName` = "mob_kiljaeden_controller" WHERE `entry`= 25608;
# update npc 25588 (hand of the deceiver)
UPDATE `creature_template` SET `ScriptName` = "mob_deceiver" WHERE `entry`= 25588;
# update npc 25315 (boss_kiljaeden)
UPDATE `creature_template` SET `ScriptName` = "boss_kiljaeden", `minhealth`=13000000, `maxhealth`=13000000 WHERE `entry`= 25315;
# update npc 25598 (volatile felfire fiend)
UPDATE `creature_template` SET `ScriptName` = "mob_volatile_felfire_fiend" WHERE `entry` = 25598;
# update npc 25735 (armageddon target)
UPDATE `creature_template` SET `ScriptName` = "mob_armagedon" WHERE `entry` = 25735;
# update npc 25502 (shield orb)
UPDATE `creature_template` SET `ScriptName` = "mob_orb" WHERE `entry` = 25502;
# update npc 25653 (power of the blue flight)
UPDATE `creature_template` SET `ScriptName` = "dragon" WHERE `entry` = 25653;
# update npc 30598 (shadow spike target)
UPDATE `creature_template` SET `ScriptName` = "mob_shadowspike" WHERE `entry` = 30598;
