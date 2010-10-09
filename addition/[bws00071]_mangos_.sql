-- Quest: Powering Our Defenses (8490)

UPDATE creature_template SET ScriptName='npc_infused_crystal' WHERE entry = 16364;

-- Enraged Wraith
DELETE FROM creature WHERE id = 17086;
UPDATE creature_template SET AIName='EventAI', flags_extra=flags_extra|64 WHERE entry=17086;

DELETE FROM creature_ai_scripts WHERE creature_id = 17086;
INSERT INTO creature_ai_scripts VALUES
(1708601,17086,2,0,100,0,25,0,0,0,11,8599,0,1,1,-106,0,0,0,0,0,0,'Enraged Wraith - Cast Enrage on 50% HP');