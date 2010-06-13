-- sound / text
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1666404 AND -1666000;

-- xerestrasza
INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
('-1666000','Help! I am trapped within this tree! I require aid!','Спасите! Я под этим деревом. Мне нужна помощь!','17490','1','0','0','SAY_XERESTRASZA_YELL_1'),
('-1666001','Thank you! I could have not held out for much longer. A terrible thing has happened here.','','17491','1','0','0','SAY_XERESTRASZA_YELL_2'),
('-1666002','We believed that the Sanctum was well fortified, but we were not prepareted for the nature of this assault.','','17492','0','0','0','SAY_XERESTRASZA_SAY_1'),
('-1666003','The Black Dragonkin materialized from thin air, and set upon us before we could react.','','17493','0','0','0','SAY_XERESTRASZA_SAY_2'),
('-1666004','We did not stand a chance. As my brethren perished around me, I managed to retreat hear and bar the entrance.','','17494','0','0','0','SAY_XERESTRASZA_SAY_3'),
('-1666005','They slaughtered us with cold efficiency, but the true focus of their interest seemed to be the eggs kept here in the sanctum.','','17495','0','0','0','SAY_XERESTRASZA_SAY_4'),
('-1666006','The commander of the forces on the ground here is a cruel brute named Zarithrian. But I fear there are greater powers at work.','','17496','0','0','0','SAY_XERESTRASZA_SAY_5'),
('-1666007','In their initial assault I caught a glimpse of their true leader, a fearsome full-grown Twilight Dragon.','','17497','0','0','0','SAY_XERESTRASZA_SAY_6'),
('-1666008','I know not the extent of their plans heroes, but I know this: they cannot be allowed to succeed!','','17498','0','0','0','SAY_XERESTRASZA_SAY_7'),

-- Halion
('-1666100','Meddlesome insects, you\'re too late! The Ruby Sanctum is lost.','','17499','1','0','0','SAY_HALION_SPAWN'),
('-1666101','Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!','','17500','1','0','0','SAY_HALION_AGGRO'),
('-1666102','Another hero falls.','','17501','1','0','0','SAY_HALION_SLAY_1'),
('-1666103','Ha Ha Ha!','','17502','1','0','0','SAY_HALION_SLAY_2'),
('-1666104','Relish this victory mortals, for it will be your last. This world will burn with the Master\'s return!','','17503','1','0','0','SAY_HALION_DEATH'),
('-1666105','Not good enough!','','17504','1','0','0','SAY_HALION_BERSERK'),
('-1666106','The heavens burn!','Небеса в огне!','17505','1','0','0','SAY_HALION_SPECIAL_1'),
('-1666107','Beware the shadow!','','17506','1','0','0','SAY_HALION_SPECIAL_2'),
('-1666108','You will find only suffering within the realm of Twilight. Enter if you dare.','','17507','1','0','0','SAY_HALION_PHASE_2'),
('-1666109','I am the light AND the darkness! Cower mortals before the Herald of Deathwing!','','17508','1','0','0','SAY_HALION_PHASE_3'),

-- Zarthrian
('-1666200','Alexstrasza has chosen capable allies. A pity that I must end you!','','17512','1','0','0','SAY_ZARITHRIAN_AGGRO'),
('-1666201','You thought you stood a chance?','','17513','1','0','0','SAY_ZARITHRIAN_SLAY_1'),
('-1666202','It\'s for the best.','','17514','1','0','0','SAY_ZARITHRIAN_SLAY_2'),
('-1666203','Halion! I\'m...aah!','','17515','1','0','0','SAY_ZARITHRIAN_DEATH'),
('-1666204','Turn them to ash, minions!','','17516','1','0','0','SAY_ZARITHRIAN_SPECIAL_1'),

-- baltharus
('-1666300','Ah, the entertainment has arrived...','','17520','1','0','0','SAY_BALTHARUS_AGGRO'),
('-1666301','Baltharus leaves no survivors!','','17521','1','0','0','SAY_BALTHARUS_SLAY_1'),
('-1666302','This world has enough heroes!','','17522','1','0','0','SAY_BALTHARUS_SLAY_2'),
('-1666303','I...didn\'t see that coming...','','17523','1','0','0','SAY_BALTHARUS_DEATH'),
('-1666304','Twice the pain and half the fun!','','17524','1','0','0','SAY_BALTHARUS_SPECIAL_1'),
('-1666305','Your power wanes, ancient one! Soon, you will join your friends!','','17525','1','0','0','SAY_BALTHARUS_YELL'),

-- saviana
('-1666400','You will suffer for this intrusion...','','17528','1','0','0','SAY_SAVIANA_AGGRO'),
('-1666401','As it should be!','','17529','1','0','0','SAY_SAVIANA_SLAY_1'),
('-1666402','Halion will be pleased...','','17530','1','0','0','SAY_SAVIANA_SLAY_2'),
('-1666403','<screaming>','','17531','1','0','0','SAY_SAVIANA_DEATH'),
('-1666404','Burn in the master\'s flame!','','17532','1','0','0','SAY_SAVIANA_SPECIAL_1');
