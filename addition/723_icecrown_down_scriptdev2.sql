-- Pit of saron
DELETE FROM `script_texts` WHERE `entry` <= -1658000 and `entry` >= -1658999;
INSERT INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
-- Garfrost
(36494,-1658001,'Tiny creatures under feet, you bring Garfrost something good to eat!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16912,1,0,0,'garfrost SAY_AGGRO'),
(36494,-1658002,'Will save for snack. For later.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16913,1,0,0,'garfrost SAY_SLAY_1'),
(36494,-1658003,'That one maybe not so good to eat now. Stupid Garfrost! BAD! BAD!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16914,1,0,0,'garfrost SAY_SLAY_2'),
(36494,-1658004,'Garfrost hope giant underpants clean. Save boss great shame. For later.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16915,1,0,0,'garfrost SAY_DEATH'),
(36494,-1658005,'Axe too weak. Garfrost make better and CRUSH YOU!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16916,1,0,0,'garfrost SAY_PHASE2'),
(36494,-1658006,'Garfrost tired of puny mortals. Now your bones will freeze!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16917,1,0,0,'garfrost SAY_PHASE3'),
(36658,-1658007,'Another shall take his place. You waste your time.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16752,1,0,0,'Tyrannus SAY_TYRANNUS_DEATH'),

-- Krick
(36477,-1658010,'Our work must not be interrupted! Ick! Take care of them!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16926,1,0,0,'Krick SAY_AGGRO'),
(36477,-1658011,'Ooh...We could probably use these parts!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16927,1,0,0,'Krick SAY_SLAY_1'),
(36477,-1658012,'Arms and legs are in short supply...Thanks for your contribution!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16928,1,0,0,'Krick SAY_SLAY_2'),
(36477,-1658013,'Enough moving around! Hold still while I blow them all up!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16929,1,0,0,'Krick SAY_BARRAGE_1'),
(36477,-1658014,'Krick begins rapidly conjuring explosive mines!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Krick SAY_BARRAGE_2'),
(36477,-1658015,'Quickly! Poison them all while they''re still close!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16930,1,0,0,'Krick SAY_POISON_NOVA'),
(36477,-1658016,'No! That one! That one! Get that one!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16931,1,0,0,'Krick SAY_CHASE_1'),
(36477,-1658017,'I''ve changed my mind...go get that one instead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16932,1,0,0,'Krick SAY_CHASE_2'),
(36477,-1658018,'What are you attacking him for? The dangerous one is over there,fool!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16933,1,0,0,'Krick SAY_CHASE_3'),

-- Ick
(36476,-1658020,'Ick begins to unleash a toxic poison cloud!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Ick SAY_ICK_POISON_NOVA'),
(36476,-1658021,'Ick is chasing you!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Ick SAY_ICK_CHASE_1'),

-- Krick OUTRO
(36477,-1658030,'Wait! Stop! Don''t kill me, please! I''ll tell you everything!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16934,1,0,0,'Krick SAY_KRICK_OUTRO_1'),
(36993,-1658031,'I''m not so naive as to believe your appeal for clemency, but I will listen.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16611,1,0,0,'Jaina SAY_JAINA_OUTRO_2'),
(36990,-1658032,'Why should the Banshee Queen spare your miserable life?',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17033,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_2'),
(36477,-1658033,'What you seek is in the master''s lair, but you must destroy Tyrannus to gain entry. Within the Halls of Reflection you will find Frostmourne. It... it holds the truth.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16935,1,0,0,'Krick SAY_KRICK_OUTRO_3'),
(36993,-1658034,'Frostmourne lies unguarded? Impossible!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16612,1,0,0,'Jaina SAY_JAINA_OUTRO_4'),
(36990,-1658035,'Frostmourne? The Lich King is never without his blade! If you are lying to me...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17034,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_4'),
(36477,-1658036,'I swear it is true! Please, don''t kill me!!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16936,1,0,0,'Krick SAY_KRICK_OUTRO_5'),
(36658,-1658037,'Worthless gnat! Death is all that awaits you!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16753,1,0,0,'Tyrannus SAY_TYRANNUS_OUTRO_7'),
(36477,-1658038,'Urg... no!!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16937,1,0,0,'Krick SAY_KRICK_OUTRO_8'),
(36658,-1658039,'Do not think that I shall permit you entry into my master''s sanctum so easily. Pursue me if you dare.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16754,1,0,0,'Tyrannus SAY_TYRANNUS_OUTRO_9'),
(36993,-1658040,'What a cruel end. Come, heroes. We must see if the gnome''s story is true. If we can separate Arthas from Frostmourne, we might have a chance at stopping him.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16613,1,0,0,'Jaina SAY_JAINA_OUTRO_10'),
(36990,-1658041,'A fitting end for a traitor. Come, we must free the slaves and see what is within the Lich King''s chamber for ourselves.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17035,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_10'),

-- Tyrannus
(36658,-1658050,'Your pursuit shall be in vain, adventurers, for the Lich King has placed an army of undead at my command! Behold!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16755,1,0,0,'Tyrannus SAY_AMBUSH_1'),
(36658,-1658051,'Persistent whelps! You will not reach the entrance of my lord''s lair! Soldiers, destroy them!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16756,1,0,0,'Tyrannus SAY_AMBUSH_2'),
(36658,-1658052,'Rimefang! Trap them within the tunnel! Bury them alive!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16757,1,0,0,'Tyrannus SAY_GAUNTLET_START'),
(36658,-1658053,'Alas, brave, brave adventurers, your meddling has reached its end. Do you hear the clatter of bone and steel coming up the tunnel behind you? That is the sound of your impending demise.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16758,1,0,0,'Tyrannus SAY_INTRO_1'),
(36658,-1658054,'Ha, such an amusing gesture from the rabble. When I have finished with you, my master''s blade will feast upon your souls. Die!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16759,1,0,0,'Tyrannus SAY_INTRO_2'),

(36658,-1658055,'I shall not fail The Lich King! Come and meet your end!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16760,1,0,0,'Tyrannus SAY_AGGRO'),
(36658,-1658056,'Such a shameful display... You are better off dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16761,1,0,0,'Tyrannus SAY_SLAY_1'),
(36658,-1658057,'Perhaps you should have stayed in the mountains!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16762,1,0,0,'Tyrannus SAY_SLAY_2'),
(36658,-1658058,'Impossible! Rimefang... Warn...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16763,1,0,0,'Tyrannus SAY_DEATH'),
(36658,-1658059,'Rimefang, destroy this fool!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16764,1,0,0,'Tyrannus SAY_MARK_RIMEFANG_1'),
(36658,-1658060,'The frostwyrm Rimefang gazes at $N and readies an icy attack!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Tyrannus SAY_MARK_RIMEFANG_2'),
(36658,-1658061,'Power... overwhelming!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16765,1,0,0,'Tyrannus SAY_DARK_MIGHT_1'),
(36658,-1658062,'Scourgelord Tyrannus roars and swells with dark might!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Tyrannus SAY_DARK_MIGHT_2'),

(37592,-1658063,'Brave champions, we owe you our lives, our freedom... Though it be a tiny gesture in the face of this enormous debt, I pledge that from this day forth, all will know of your deeds, and the blazing path of light you cut through the shadow of this dark citadel.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0,'Gorkun  SAY_GORKUN_OUTRO_1'),
(37592,-1658064,'This day will stand as a testament not only to your valor, but to the fact that no foe, not even the Lich King himself, can stand when Alliance and Horde set aside their differences and ---',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0,'Gorkun  SAY_GORKUN_OUTRO_2'),
(36993,-1658065,'Heroes, to me!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16614,1,0,0,'Jaina SAY_JAYNA_OUTRO_3'),
(36990,-1658066,'Take cover behind me! Quickly!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17037,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_3'),
(36993,-1658067,'The Frost Queen is gone. We must keep moving - our objective is near.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16615,0,0,0,'Jaina SAY_JAYNA_OUTRO_4'),
(36990,-1658068,'I thought he''d never shut up. At last, Sindragosa silenced that long-winded fool. To the Halls of Reflection, champions! Our objective  is near... I can sense it.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17036,0,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_4'),
(36993,-1658069,'I... I could not save them... Damn you, Arthas! DAMN YOU!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16616,0,0,0,'Jaina SAY_JAYNA_OUTRO_5');

-- Forge of souls
DELETE FROM `script_texts` WHERE `entry` <= -1632000 and `entry` >= -1632999;
INSERT INTO `script_texts` (`npc_entry`,`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
-- Bronjham
(36497,-1632001,'Finally...a captive audience!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16595,1,0,0,'Bronjham SAY_AGGRO'),
(36497,-1632002,'Fodder for the engine!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16596,1,0,0,'Bronjham SAY_SLAY_1'),
(36497,-1632003,'Another soul to strengthen the host!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16597,1,0,0,'Bronjham SAY_SLAY_2'),
(36497,-1632004,'',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16598,1,0,0,'Bronjham SAY_DEATH'),
(36497,-1632005,'The vortex of the harvested calls to you!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16599,1,0,0,'Bronjham SAY_SOUL_STORM'),
(36497,-1632006,'I will sever the soul from your body!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16600,1,0,0,'Bronjham SAY_CORRUPT_SOUL'),

-- Devourer of Souls
(36502,-1632010,'You dare look upon the host of souls? I SHALL DEVOUR YOU WHOLE!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16884,1,0,0,'Devoureur SAY_FACE_ANGER_AGGRO'),
(36502,-1632011,'You dare look upon the host of souls? I SHALL DEVOUR YOU WHOLE!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16890,1,0,0,'Devoureur SAY_FACE_DESIRE_AGGRO'),
(36502,-1632012,'Damnation!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16885,1,0,0,'Devoureur SAY_FACE_ANGER_SLAY_1'),
(36502,-1632013,'Damnation!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16896,1,0,0,'Devoureur SAY_FACE_SORROW_SLAY_1'),
(36502,-1632014,'Damnation!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16891,1,0,0,'Devoureur SAY_FACE_DESIRE_SLAY_1'),
(36502,-1632015,'Doomed for eternity!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16886,1,0,0,'Devoureur SAY_FACE_ANGER_SLAY_2'),
(36502,-1632016,'Doomed for eternity!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16897,1,0,0,'Devoureur SAY_FACE_SORROW_SLAY_2'),
(36502,-1632017,'Doomed for eternity!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16892,1,0,0,'Devoureur SAY_FACE_DESIRE_SLAY_2'),
(36502,-1632018,'The swell of souls will not be abated! You only delay the inevitable!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16887,1,0,0,'Devoureur SAY_FACE_ANGER_DEATH'),
(36502,-1632019,'The swell of souls will not be abated! You only delay the inevitable!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16898,1,0,0,'Devoureur SAY_FACE_SORROW_DEATH'),
(36502,-1632020,'The swell of souls will not be abated! You only delay the inevitable!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16893,1,0,0,'Devoureur SAY_FACE_DESIRE_DEATH'),
(36502,-1632021,'Devourer of Souls begins to cast Mirrored Soul!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Devoureur EMOTE_MIRRORED_SOUL'),
(36502,-1632022,'Devourer of Souls begins to Unleash Souls!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Devoureur EMOTE_UNLEASH_SOUL'),
(36502,-1632023,'SUFFERING! ANGUISH! CHAOS! RISE AND FEED!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16888,1,0,0,'Devoureur SAY_FACE_ANGER_UNLEASH_SOUL'),
(36502,-1632024,'SUFFERING! ANGUISH! CHAOS! RISE AND FEED!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16899,1,0,0,'Devoureur SAY_FACE_SORROW_UNLEASH_SOUL'),
(36502,-1632025,'SUFFERING! ANGUISH! CHAOS! RISE AND FEED!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16894,1,0,0,'Devoureur SAY_FACE_DESIRE_UNLEASH_SOUL'),
(36502,-1632026,'Devourer of Souls begins to cast Wailing Souls!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Devoureur EMOTE_WAILING_SOUL'),
(36502,-1632027,'Stare into the abyss, and see your end!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16889,1,0,0,'Devoureur SAY_FACE_ANGER_WAILING_SOUL'),
(36502,-1632028,'Stare into the abyss, and see your end!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16895,1,0,0,'Devoureur SAY_FACE_DESIRE_WAILING_SOUL'),
(38160,-1632029,'Excellent work, champions! We shall set up our base camp in these chambers. My mages will get the Scourge transport device working shortly. Step inside it when you''re ready for your next mission. I will meet you on the other side.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16625,1,0,0,'Jaina SAY_JAINA_OUTRO'),
(38161,-1632030,'Excellent work, champions! We shall set up our base camp in these chambers. My mages will get the Scourge transport device working shortly. Step inside when you are ready for your next mission. I will meet you on the other side.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17044,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO'),

-- Jaina
(37597,-1632040,'Thank the light for seeing you here safely. We have much work to do if we are to defeat the Lich King and put an end to the Scourge.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16617,0,0,0,'Jaina SAY_INTRO_1'),
(37597,-1632041,'Our allies within the Argent Crusade and the Knights of the Ebon Blade have broken through the front gate of Icecrown and are attempting to establish a foothold within the Citadel.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16618,0,0,0,'Jaina SAY_INTRO_2'),
(37597,-1632042,'Their success hinges upon what we discover in these cursed halls. Although our mission is a wrought with peril, we must persevere!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16619,0,0,0,'Jaina SAY_INTRO_3'),
(37597,-1632043,'With the attention of the Lich King drawn toward the front gate, we will be working our way through the side in search of information that will enable us to defeat the Scourge - once and for all.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16620,0,0,0,'Jaina SAY_INTRO_4'),
(37597,-1632044,'King Varian''s SI7 agents have gathered information about a private sanctum of the Lich King''s deep within a place called the Halls of Reflection.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16621,0,0,0,'Jaina SAY_INTRO_5'),
(37597,-1632045,'We will carve a path through this wretched place and find a way to enter the Halls of Reflection. I sense powerful magic hidden away within those walls... Magic that could be the key to destroy the Scourge.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16622,0,0,0,'Jaina SAY_INTRO_6'),
(37597,-1632046,'Your first mission is to destroy the machines of death within this malevolent engine of souls, and clear a path for our soldiers.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16623,0,0,0,'Jaina SAY_INTRO_7'),
(37597,-1632047,'Make haste, champions! I will prepare the troops to fall in behind you.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16624,0,0,0,'Jaina SAY_INTRO_8'),

-- Sylvanas
(37596,-1632050,'The Argent Crusade and the Knights of the Ebon Blade have assaulted the gates of Icecrown Citadel and are preparing for a massive attack upon the Scourge. Our missition is a bit more subtle, but equally as important.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17038,0,0,0,'Sylvanas SAY_INTRO_1'),
(37596,-1632051,'With the attention of the Lich King turned towards the front gate, we''ll be working our way through the side in search of information that will enable us to defeat the Lich King - once and for all.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17039,0,0,0,'Sylvanas SAY_INTRO_2'),
(37596,-1632052,'Our scouts have reported that the Lich King has a private chamber, outside of the Frozen Throne, deep within a place called the Halls of Reflection. That is our target, champions.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17040,0,0,0,'Sylvanas SAY_INTRO_3'),
(37596,-1632053,'We will cut a swath of destruction through this cursed place and find a way to enter the Halls of Reflection. If there is anything of value to be found here, it will be found in the Halls.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17041,0,0,0,'Sylvanas SAY_INTRO_4'),
(37596,-1632054,'Your first mission is to destroy the machines of death within this wretched engine of souls, and clear a path for our soldiers.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17042,0,0,0,'Sylvanas SAY_INTRO_5'),
(37596,-1632055,'The Dark Lady watches over you. Make haste!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17043,0,0,0,'Sylvanas SAY_INTRO_6');
