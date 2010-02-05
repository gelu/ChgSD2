-- instance
UPDATE `instance_template` SET `script`='instance_trial_of_the_crusader' WHERE `map`=649;

-- announcers
UPDATE `creature_template` SET `npcflag`=1, `scriptname`='npc_toc_announcer' WHERE `entry`=34816;

DELETE FROM `creature` WHERE `map` = 649 AND `id` IN 
(34797,34796,34799,35144,34780,34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34497,34496,34564,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);

UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(34797,34796,34799,35144,34780,34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34497,34496,34564,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);

DELETE FROM `creature_loot_template` WHERE `entry` IN 
(34797,34796,34799,35144,34780,34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34497,34496,34564,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);



-- grand crusaders
-- UPDATE `creature_template` SET `scriptname`='mob_toc_warrior' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_mage' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_shaman' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_hunter' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_rogue' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_priest' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_dk' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_paladin' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_druid' WHERE entry IN ();
-- UPDATE `creature_template` SET `scriptname`='mob_toc_warlock' WHERE entry IN ();

-- UPDATE `creature_template` SET `scriptname`='mob_toc_pet_cat' WHERE `entry` = 35465;
-- UPDATE `creature_template` SET `scriptname`='mob_toc_pet_dog' WHERE `entry` = 35610;

-- Nortrend beasts
-- UPDATE creature_template SET scriptname='boss_gormok' WHERE entry=34796;
-- UPDATE creature_template SET scriptname='boss_dreadscale' WHERE entry=34799;
-- UPDATE creature_template SET scriptname='boss_acidmaw' WHERE entry=35144;
-- UPDATE creature_template SET scriptname='boss_icehowl' WHERE entry=34797;

-- Jaraxxus
-- UPDATE creature_template SET scriptname='boss_jaraxxus' WHERE entry=34780;

-- Froja's
-- UPDATE creature_template SET scriptname='boss_lightbane' WHERE entry=34497;
-- UPDATE creature_template SET scriptname='boss_darkbane' WHERE entry=34496;

-- Anub'arak
-- UPDATE creature_template SET scriptname='boss_anubarak' WHERE entry=34564;


