-- instance
UPDATE `instance_template` SET `script`='instance_trial_of_the_crusader' WHERE `map`=649;

-- announcers
UPDATE `creature_template` SET `npcflag`=1, `scriptname`='npc_toc_announcer' WHERE `entry`=34816;

DELETE FROM `creature` WHERE `map` = 649 AND `id` IN 
(34797,34796,34799,35144,34780,34460,34463,34461,34471,34475,34472,34453,34455,34458,34454,34451,34456,34497,34496,34564,34467,35465,34468,35610,34473,34474,34441,34449,34448,34450);


-- grand crusaders
-- UPDATE `creature_template` SET `scriptname`='mob_toc_warrior' WHERE entry IN (34705, 35572);
-- UPDATE `creature_template` SET `scriptname`='mob_toc5_mage' WHERE entry IN (34702, 35569);
-- UPDATE `creature_template` SET `scriptname`='mob_toc5_shaman' WHERE entry IN (34701, 35571);
-- UPDATE `creature_template` SET `scriptname`='mob_toc5_hunter' WHERE entry IN (34657, 35570);
-- UPDATE `creature_template` SET `scriptname`='mob_toc5_rogue' WHERE entry IN (34703, 35617);

-- Nortrend beasts
-- UPDATE creature_template SET scriptname='boss_eadric' WHERE entry=35119;

-- Jaraxxus
-- UPDATE creature_template SET scriptname='boss_black_knight' WHERE entry=35451;

