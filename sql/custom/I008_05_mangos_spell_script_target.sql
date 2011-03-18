-- Death comes from on high (Quest - 12641)

DELETE FROM `spell_script_target` WHERE `entry` = 51859;

INSERT INTO `spell_script_target` 
	(`entry`, `type`, `targetEntry`) 
VALUES
	(51859, 1, 28525),
	(51859, 1, 28542),
	(51859, 1, 28543),
	(51859, 1, 28544);
