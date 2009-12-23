UPDATE `creature_template` SET `ScriptName`='boss_razorscale' WHERE `entry`=33186;
UPDATE `creature_template` SET `ScriptName`='boss_ignis' WHERE `entry`=33118;
UPDATE `creature_template` SET `ScriptName`='boss_xt002' WHERE `entry`=33293;
UPDATE `creature_template` SET `ScriptName`='boss_flame_leviathan' WHERE `entry`=33113;
UPDATE `creature_template` SET `ScriptName`='boss_auriaya' WHERE `entry`=33515;
UPDATE `creature_template` SET `ScriptName`='boss_kologarn' WHERE `entry`=32930;
UPDATE `creature_template` SET `ScriptName`='boss_kologarn_left_arm' WHERE `entry`=32933;
UPDATE `creature_template` SET `ScriptName`='boss_kologarn_right_arm' WHERE `entry`=32934;
UPDATE `creature_template` SET `ScriptName`='boss_steelbreaker' WHERE `entry`=32867;
UPDATE `creature_template` SET `ScriptName`='boss_molgeim' WHERE `entry`=32927;
UPDATE `creature_template` SET `ScriptName`='boss_brundir' WHERE `entry`=32857;
UPDATE `creature_template` SET `ScriptName`='boss_thorim' WHERE `entry`=32865;
UPDATE `creature_template` SET `ScriptName`='boss_hodir' WHERE `entry`=32845;
UPDATE `creature_template` SET `ScriptName`='boss_freya' WHERE `entry`=32906;
UPDATE `creature_template` SET `ScriptName`='boss_mimiron' WHERE `entry`=33350;
UPDATE `creature_template` SET `ScriptName`='boss_vezax' WHERE `entry`=33271;
UPDATE `creature_template` SET `ScriptName`='boss_yogg_saron' WHERE `entry`=33288;
UPDATE `creature_template` SET `ScriptName`='boss_algalon' WHERE `entry`=32871;
UPDATE `instance_template` SET `script`='instance_ulduar' WHERE `map`=603;

insert into `creature_template` values ('99005','0','0','0','0','0','22448','0','22448','0','Ulduar teleporter','',NULL,'0','80','80','64200','64200','12300','12300','31200','35','35','1','1.48','0.5','0','3170','6182','0','33235','1','1400','1900','0','0','0','0','0','0','0','0','0','0','100','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','1','3','1','1','0','0','0','0','0','0','0','0','1','99003','0','0','ulduar_teleporter');
replace into `creature_template_addon` values ('99005','0','0','0','0','0','48143 0');
replace into `locales_creature` values ('99005','','','','','','','','Ульдуарский телепорт',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'');
/*replace into `creature` values
('500000','99005','571','1','1','0','99003','9025.76','-1179.12','1060.18','4.59386','25','0','0','64200','12300','0','0'),
('500001','99005','603','2','1','0','99003','1497.84','-23.9801','421.367','5.0242','25','0','0','64200','12300','0','0'),
('500002','99005','603','2','1','0','99003','131.248','-35.3802','410.204','0','25','0','0','64200','12300','0','0'),
('500003','99005','603','2','1','0','99003','553.233','-12.3247','410.079','0','25','0','0','64200','12300','0','0'),
('500004','99005','603','2','1','0','99003','926.292','-11.4635','418.995','0','25','0','0','64200','12300','0','0'),
('500005','99005','603','2','1','0','99003','2086.17','-24.3111','421.639','3.11803','25','0','0','64200','12300','0','0');*/
