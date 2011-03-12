-- Death comes from on high (Quest - 12641)

DELETE FROM `creature_template_addon` WHERE `entry` IN (28511,28525,28542,28543,28544);

INSERT INTO `creature_template_addon` 
	(`entry`, `moveflags`, `auras`) 
VALUES
	(28511, 33562624, ''),
	(28525, 0, '51939 0'),
	(28542, 0, '51939 0'),
	(28543, 0, '51939 0'),
	(28544, 0, '51939 0');
	