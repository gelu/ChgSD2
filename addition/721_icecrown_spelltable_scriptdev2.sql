-- Icecrown citadel spelltable

-- Lord Marrowgar
DELETE FROM `boss_spell_table` WHERE `entry` = 36612;

-- Lady Deathwhisper
DELETE FROM `boss_spell_table` WHERE `entry` = 36855;

-- Deathbringer Saurfang
DELETE FROM `boss_spell_table` WHERE `entry` = 37813;

-- Festergut
DELETE FROM `boss_spell_table` WHERE `entry` = 36626;
INSERT INTO `boss_spell_table` (`entry`, `spellID_N10`, `spellID_N25`, `spellID_H10`, `spellID_H25`, `timerMin_N10`, `timerMin_N25`, `timerMin_H10`, `timerMin_H25`, `timerMax_N10`, `timerMax_N25`, `timerMax_H10`, `timerMax_H25`, `CastType`, `isVisualEffect`, `isBugged`) VALUES
( 36626, 70138, 70140, 70140, 70137, 10000, 0, 0, 0, 30000, 0, 0, 0, 4, 0, 0),
( 36626, 69161, 70139, 70139, 70140, 10000, 0, 0, 0, 30000, 0, 0, 0, 4, 0, 0),
( 36626, 70468, 69161, 69161, 70139, 10000, 0, 0, 0, 30000, 0, 0, 0, 4, 0, 0),
( 36626, 69165, 69165, 69165, 69165, 10000, 0, 0, 0, 30000, 0, 0, 0, 1, 0, 0),
( 36626, 71912, 71912, 71912, 71912, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 4, 1, 0),
( 36626, 69195, 73031, 71219, 73032, 20000, 0, 0, 0, 30000, 0, 0, 0, 1, 0, 0),
( 36626, 69279, 69279, 69279, 69279, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 4, 1, 0),
( 36626, 72103, 72103, 72103, 72103, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 4, 0, 0),
( 36626, 72219, 72551, 72551, 72553, 10000, 0, 0, 0, 20000, 0, 0, 0, 3, 0, 0),
( 36626, 72227, 72227, 72229, 72230, 20000, 20000, 20000, 20000, 30000, 30000, 30000, 30000, 4, 0, 0),
( 36626, 72272, 72273, 72273, 73020, 20000, 0, 0, 0, 30000, 0, 0, 0, 1, 0, 0),
( 36626, 47008, 47008, 47008, 47008, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 300000, 1, 0, 0);

-- Rotface
DELETE FROM `boss_spell_table` WHERE `entry` = 36627;
INSERT INTO `boss_spell_table` (`entry`, `spellID_N10`, `spellID_N25`, `spellID_H10`, `spellID_H25`, `timerMin_N10`, `timerMin_N25`, `timerMin_H10`, `timerMin_H25`, `timerMax_N10`, `timerMax_N25`, `timerMax_H10`, `timerMax_H25`, `CastType`, `isVisualEffect`, `isBugged`) VALUES
(36627, 47008, 0, 0, 0, 300000, 0, 0, 0, 300000, 0, 0, 0, 1, 0, 0),
(36627, 69508, 0, 0, 0, 15000, 0, 0, 0, 20000, 0, 0, 0, 3, 0, 0),
(36627, 69674, 0, 0, 0, 15000, 0, 0, 0, 30000, 0, 0, 0, 4, 0, 0),
(36627, 69788, 0, 0, 0, 20000, 0, 0, 0, 40000, 0, 0, 0, 1, 0, 0),
(36627, 69783, 69797, 69799, 69802, 20000, 0, 0, 0, 40000, 0, 0, 0, 3, 0, 0),
(36627, 69789, 0, 0, 0, 20000, 0, 0, 0, 40000, 0, 0, 0, 1, 0, 0);

-- Professor Putricide
DELETE FROM `boss_spell_table` WHERE `entry` = 36678;
INSERT INTO `boss_spell_table` (`entry`, `spellID_N10`, `spellID_N25`, `spellID_H10`, `spellID_H25`, `timerMin_N10`, `timerMin_N25`, `timerMin_H10`, `timerMin_H25`, `timerMax_N10`, `timerMax_N25`, `timerMax_H10`, `timerMax_H25`, `CastType`, `isVisualEffect`, `isBugged`) VALUES
(36678, 47008, 0, 0, 0, 300000, 0, 0, 0, 300000, 0, 0, 0, 1, 0, 0);

-- Taldaram
DELETE FROM `boss_spell_table` WHERE `entry` = 37973;

-- Valanar
DELETE FROM `boss_spell_table` WHERE `entry` = 37970;

-- Keleseth
DELETE FROM `boss_spell_table` WHERE `entry` = 37972;

-- Lanathel
DELETE FROM `boss_spell_table` WHERE `entry` = 37955;

-- Valithria
DELETE FROM `boss_spell_table` WHERE `entry` = 36789;

-- Sindragosa
DELETE FROM `boss_spell_table` WHERE `entry` = 36853;

-- Lich king
DELETE FROM `boss_spell_table` WHERE `entry` = 36597;
