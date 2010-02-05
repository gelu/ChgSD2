
-- instance
UPDATE `instance_template` SET `script`='instance_trial_of_the_crusader' WHERE `map`=649;
DELETE FROM `creature` WHERE `map` = 649 AND `id` IN 
(34797,34796,34799,35144,34780,34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34497,34496,34564,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);

-- announcers
UPDATE `creature_template` SET `npcflag`=1, `scriptname`='npc_toc_announcer' WHERE `entry`=34816;

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

UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(12266,12209,12212,12281,12190,12284,12269,12272,12229,12187,12091,12088,12169,12103,12106,12112,12166,12163,12175,12183,12303,12300);
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(12267,12210,12213,12282,12191,12285,12270,12273,12230,12188,12092,12089,12170,12104,12107,12113,12167,12164,12181,12184,12304,12301);
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(12268,12211,12214,12283,12192,12286,12271,12274,12231,12189,12093,12090,12171,12105,12108,12114,12168,12165,12182,12185,12305,12302);

-- N10
DELETE FROM `creature_loot_template` WHERE `entry` IN 
(34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);
-- H10
DELETE FROM `creature_loot_template` WHERE `entry` IN 
(12266,12209,12212,12281,12190,12284,12269,12272,12229,12187,12091,12088,12169,12103,12106,12112,12166,12163,12175,12183,12303,12300);
-- N25
DELETE FROM `creature_loot_template` WHERE `entry` IN 
(12267,12210,12213,12282,12191,12285,12270,12273,12230,12188,12092,12089,12170,12104,12107,12113,12167,12164,12181,12184,12304,12301);
-- H25
DELETE FROM `creature_loot_template` WHERE `entry` IN 
(12268,12211,12214,12283,12192,12286,12271,12274,12231,12189,12093,12090,12171,12105,12108,12114,12168,12165,12182,12185,12305,12302);

-- Nortrend beasts
-- UPDATE creature_template SET scriptname='boss_gormok' WHERE entry=34796;
-- UPDATE creature_template SET scriptname='boss_dreadscale' WHERE entry=34799;
-- UPDATE creature_template SET scriptname='boss_acidmaw' WHERE entry=35144;
-- UPDATE creature_template SET scriptname='boss_icehowl' WHERE entry=34797;
UPDATE `creature_template` SET `lootid`= 0 WHERE  `entry` IN
(34796,34799,35144);
DELETE FROM `creature_loot_template`  WHERE  `entry` IN
(34796,34799,35144);

INSERT IGNORE INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES
(34797, 47241, 100, 0, 3, 3, 0, 0, 0),
(34797, 47578, 0, 2, 1, 1, 6, 469, 0),
(34797, 47607, 0, 2, 1, 1, 6, 469, 0),
(34797, 47608, 0, 2, 1, 1, 6, 469, 0),
(34797, 47609, 0, 1, 1, 1, 6, 469, 0),
(34797, 47611, 0, 1, 1, 1, 6, 469, 0),
(34797, 47612, 0, 1, 1, 1, 6, 469, 0),
(34797, 47613, 0, 2, 1, 1, 6, 469, 0),
(34797, 47614, 0, 1, 1, 1, 6, 469, 0),
(34797, 47615, 0, 1, 1, 1, 6, 469, 0),
(34797, 47616, 0, 2, 1, 1, 6, 469, 0),
(34797, 47617, 0, 2, 1, 1, 6, 469, 0),
(34797, 47849, 0, 4, 1, 1, 6, 67, 0),
(34797, 47850, 0, 3, 1, 1, 6, 67, 0),
(34797, 47851, 0, 3, 1, 1, 6, 67, 0),
(34797, 47852, 0, 3, 1, 1, 6, 67, 0),
(34797, 47853, 0, 4, 1, 1, 6, 67, 0),
(34797, 47854, 0, 4, 1, 1, 6, 67, 0),
(34797, 47855, 0, 4, 1, 1, 6, 67, 0),
(34797, 47856, 0, 3, 1, 1, 6, 67, 0),
(34797, 47857, 0, 4, 1, 1, 6, 67, 0),
(34797, 47858, 0, 3, 1, 1, 6, 67, 0),
(34797, 47859, 0, 3, 1, 1, 6, 67, 0),
(34797, 47860, 0, 4, 1, 1, 6, 67, 0);
UPDATE `creature_template` SET `lootid` = 34797 WHERE `entry` = 34797;


-- Jaraxxus
-- UPDATE creature_template SET scriptname='boss_jaraxxus' WHERE entry=34780;
INSERT IGNORE INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES
(34780, 47241, 100, 0, 3, 3, 0, 0, 0),
(34780, 47618, 0, 2, 1, 1, 6, 469, 0),
(34780, 47619, 0, 2, 1, 1, 6, 469, 0),
(34780, 47620, 0, 2, 1, 1, 6, 469, 0),
(34780, 47621, 0, 1, 1, 1, 6, 469, 0),
(34780, 47663, 0, 2, 1, 1, 6, 469, 0),
(34780, 47669, 0, 2, 1, 1, 6, 469, 0),
(34780, 47676, 0, 1, 1, 1, 6, 469, 0),
(34780, 47679, 0, 2, 1, 1, 6, 469, 0),
(34780, 47680, 0, 1, 1, 1, 6, 469, 0),
(34780, 47683, 0, 1, 1, 1, 6, 469, 0),
(34780, 47703, 0, 2, 1, 1, 6, 469, 0),
(34780, 47711, 0, 1, 1, 1, 6, 469, 0),
(34780, 47861, 0, 4, 1, 1, 6, 67, 0),
(34780, 47862, 0, 4, 1, 1, 6, 67, 0),
(34780, 47863, 0, 4, 1, 1, 6, 67, 0),
(34780, 47864, 0, 4, 1, 1, 6, 67, 0),
(34780, 47865, 0, 4, 1, 1, 6, 67, 0),
(34780, 47866, 0, 3, 1, 1, 6, 67, 0),
(34780, 47867, 0, 3, 1, 1, 6, 67, 0),
(34780, 47868, 0, 4, 1, 1, 6, 67, 0),
(34780, 47869, 0, 3, 1, 1, 6, 67, 0),
(34780, 47870, 0, 3, 1, 1, 6, 67, 0),
(34780, 47871, 0, 3, 1, 1, 6, 67, 0),
(34780, 47872, 0, 4, 1, 1, 6, 67, 0),
(34780, 49235, 0, 1, 1, 1, 6, 469, 0),
(34780, 49236, 0, 3, 1, 1, 6, 67, 0);
UPDATE `creature_template` SET `lootid` = 34780 WHERE `entry` = 34780;


-- Froja's
-- UPDATE creature_template SET scriptname='boss_lightbane' WHERE entry=34497;
-- UPDATE creature_template SET scriptname='boss_darkbane' WHERE entry=34496;
INSERT IGNORE INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES
(34497, 47241, 100, 0, 3, 3, 0, 0, 0),
(34497, 47700, 0, 1, 1, 1, 6, 469, 0),
(34497, 47736, 0, 1, 1, 1, 6, 469, 0),
(34497, 47737, 0, 1, 1, 1, 6, 469, 0),
(34497, 47738, 0, 2, 1, 1, 6, 67, 0),
(34497, 47739, 0, 1, 1, 1, 6, 469, 0),
(34497, 47740, 0, 1, 1, 1, 6, 469, 0),
(34497, 47883, 0, 2, 1, 1, 6, 67, 0),
(34497, 47884, 0, 2, 1, 1, 6, 67, 0),
(34497, 47885, 0, 1, 1, 1, 6, 469, 0),
(34497, 47886, 0, 2, 1, 1, 6, 67, 0),
(34497, 47887, 0, 2, 1, 1, 6, 67, 0),
(34497, 47888, 0, 2, 1, 1, 6, 67, 0);
UPDATE `creature_template` SET `lootid` = 34497 WHERE `entry` = 34497;

INSERT IGNORE INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES
(34496, 47241, 100, 0, 3, 3, 0, 0, 0),
(34496, 47742, 0, 1, 1, 1, 6, 469, 0),
(34496, 47743, 0, 1, 1, 1, 6, 469, 0),
(34496, 47744, 0, 1, 1, 1, 6, 469, 0),
(34496, 47745, 0, 2, 1, 1, 6, 67, 0),
(34496, 47746, 0, 1, 1, 1, 6, 469, 0),
(34496, 47747, 0, 1, 1, 1, 6, 469, 0),
(34496, 47889, 0, 1, 1, 1, 6, 469, 0),
(34496, 47890, 0, 2, 1, 1, 6, 67, 0),
(34496, 47891, 0, 2, 1, 1, 6, 67, 0),
(34496, 47892, 0, 2, 1, 1, 6, 67, 0),
(34496, 47893, 0, 2, 1, 1, 6, 67, 0),
(34496, 49231, 0, 2, 1, 1, 6, 67, 0),
(34496, 49232, 0, 1, 1, 1, 6, 469, 0);
UPDATE `creature_template` SET `lootid` = 34496 WHERE `entry` = 34496;

-- Anub'arak
-- UPDATE creature_template SET scriptname='boss_anubarak' WHERE entry=34564;
INSERT IGNORE INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `groupid`, `mincountOrRef`, `maxcount`, `lootcondition`, `condition_value1`, `condition_value2`) VALUES
(34564, 47241, 100, 0, 3, 3, 0, 0, 0),
(34564, 47741, 0, 1, 1, 1, 6, 469, 0),
(34564, 47808, 0, 1, 1, 1, 6, 469, 0),
(34564, 47809, 0, 1, 1, 1, 6, 469, 0),
(34564, 47810, 0, 2, 1, 1, 6, 469, 0),
(34564, 47811, 0, 3, 1, 1, 6, 469, 0),
(34564, 47812, 0, 1, 1, 1, 6, 469, 0),
(34564, 47813, 0, 3, 1, 1, 6, 469, 0),
(34564, 47814, 0, 2, 1, 1, 6, 469, 0),
(34564, 47815, 0, 2, 1, 1, 6, 469, 0),
(34564, 47816, 0, 1, 1, 1, 6, 469, 0),
(34564, 47829, 0, 3, 1, 1, 6, 469, 0),
(34564, 47830, 0, 2, 1, 1, 6, 469, 0),
(34564, 47832, 0, 3, 1, 1, 6, 469, 0),
(34564, 47834, 0, 1, 1, 1, 6, 469, 0),
(34564, 47835, 0, 2, 1, 1, 6, 469, 0),
(34564, 47836, 0, 2, 1, 1, 6, 469, 0),
(34564, 47837, 0, 3, 1, 1, 6, 469, 0),
(34564, 47838, 0, 3, 1, 1, 6, 469, 0),
(34564, 47894, 0, 4, 1, 1, 6, 67, 0),
(34564, 47895, 0, 4, 1, 1, 6, 67, 0),
(34564, 47896, 0, 6, 1, 1, 6, 67, 0),
(34564, 47897, 0, 6, 1, 1, 6, 67, 0),
(34564, 47898, 0, 4, 1, 1, 6, 67, 0),
(34564, 47899, 0, 5, 1, 1, 6, 67, 0),
(34564, 47900, 0, 5, 1, 1, 6, 67, 0),
(34564, 47901, 0, 6, 1, 1, 6, 67, 0),
(34564, 47902, 0, 5, 1, 1, 6, 67, 0),
(34564, 47903, 0, 5, 1, 1, 6, 67, 0),
(34564, 47904, 0, 6, 1, 1, 6, 67, 0),
(34564, 47905, 0, 4, 1, 1, 6, 67, 0),
(34564, 47906, 0, 6, 1, 1, 6, 67, 0),
(34564, 47907, 0, 4, 1, 1, 6, 67, 0),
(34564, 47908, 0, 5, 1, 1, 6, 67, 0),
(34564, 47909, 0, 6, 1, 1, 6, 67, 0),
(34564, 47910, 0, 5, 1, 1, 6, 67, 0),
(34564, 47911, 0, 4, 1, 1, 6, 67, 0);
UPDATE `creature_template` SET `lootid` = 34564 WHERE `entry` = 34564;
