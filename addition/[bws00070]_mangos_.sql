-- Quest: Abduction (11590)
UPDATE creature_template SET
minhealth=6387,
maxhealth=7185,
minlevel=64,
maxlevel=70,
minmana=7031,
maxmana=7196
WHERE entry=25474;

UPDATE creature_template SET
ScriptName='npc_beryl_sorcerer' , AIName=''
WHERE entry=25316;