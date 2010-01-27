DELETE FROM `gameobject` WHERE `guid` = 913334;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(913334, 202244, 631, 3, 1, -210.223, 2217.72, 199.97, 3.07661, 0, 0, 0.999472, 0.0324833, 0, 0, 1);
UPDATE `gameobject_template` SET ScriptName = 'icecrown_teleporter' WHERE `entry`  IN (202242,202243,202244,202245);
