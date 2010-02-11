-- iron council
#UPDATE creature_template SET scriptname='mob_ulduar_lightning_elemental' WHERE entry=32958;
#UPDATE creature_template SET scriptname='mob_rune_of_power' WHERE entry=33705;
#UPDATE creature_template SET scriptname='mob_rune_of_summoning' WHERE entry=33051;
#UPDATE creature_template SET mechanic_immune_mask=619395071, scriptname='boss_brundir' WHERE entry=32857;
#UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_molgeim' WHERE entry=32927;
#UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_steelbreaker' WHERE entry=32867;

-- kologarn
DELETE FROM creature WHERE id IN (32933, 32934, 232933);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType) 
VALUES (9327991, 32933, 603, 3, 65535, 0, 0, 1799.68, -24.3599, 452.227, 3.14747, 604800, 0, 0, 543855, 0, 0, 0);
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType) 
VALUES (9327992, 32934, 603, 3, 65535, 0, 0, 1799.68, -24.3599, 452.227, 3.14747, 604800, 0, 0, 543855, 0, 0, 0);
UPDATE creature_model_info SET bounding_radius=15, combat_reach=15 WHERE modelid IN (28638, 28822, 28821);
UPDATE creature_template SET mechanic_immune_mask=652951551, scriptname='boss_kologarn_right_arm' WHERE entry=32934;
UPDATE creature_template SET mechanic_immune_mask=652951551, scriptname='boss_kologarn_left_arm' WHERE entry=32933;
UPDATE creature_template SET scriptname='mob_ulduar_rubble' WHERE entry=33768;
UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_kologarn' WHERE entry=32930;

-- razorscale
#UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_razorscale' WHERE entry=33186;
#UPDATE creature_template SET scriptname='mob_devouring_flame_target' WHERE entry=34188;
#UPDATE creature_template SET scriptname='mob_dark_rune_watcher' WHERE entry=33453;
#UPDATE creature_template SET scriptname='mob_dark_rune_sentinel' WHERE entry=33846;
#UPDATE creature_template SET scriptname='mob_dark_rune_guardian' WHERE entry=33388;
#UPDATE creature_template SET scriptname='npc_expedition_commander' WHERE entry=33210;

-- ignis
#UPDATE creature_template SET mechanic_immune_mask=617299803, scriptname='boss_ignis' WHERE entry=33118;
#UPDATE creature_template SET minlevel=80, maxlevel=80, faction_h=1925, faction_a=1925, scale=0.5, scriptname='mob_scorch_target' WHERE entry=33221;
#UPDATE creature_template SET scriptname='mob_iron_construct' WHERE entry=33121;
