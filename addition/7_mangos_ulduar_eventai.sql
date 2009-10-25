UPDATE creature_template SET AIName='EventAI', ScriptName='' WHERE entry IN (32918, 36561);
DELETE FROM creature_ai_scripts WHERE creature_id IN (32918, 36561);
INSERT INTO creature_ai_scripts VALUES 
(3291800, 32918, 6, 0, 100, 2, 0, 0, 0, 0, 11, 62598, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Detonating Lasher - OnDeath (N)'),
(3291801, 32918, 6, 0, 100, 4, 0, 0, 0, 0, 11, 62937, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 'Detonating Lasher - OnDeath (H)');
