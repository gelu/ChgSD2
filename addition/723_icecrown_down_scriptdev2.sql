-- Pit of saron
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1658100 AND -1658000;
INSERT INTO `script_texts` (`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
-- Garfrost
(-1658001,'Tiny creatures under feet, you bring Garfrost something good to eat!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16912,1,0,0,'garfrost SAY_AGGRO'),
(-1658002,'Will save for snack. For later.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16913,1,0,0,'garfrost SAY_SLAY_1'),
(-1658003,'That one maybe not so good to eat now. Stupid Garfrost! BAD! BAD!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16914,1,0,0,'garfrost SAY_SLAY_2'),
(-1658004,'Garfrost hope giant underpants clean. Save boss great shame. For later.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16915,1,0,0,'garfrost SAY_DEATH'),
(-1658005,'Axe too weak. Garfrost make better and CRUSH YOU!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16916,1,0,0,'garfrost SAY_PHASE2'),
(-1658006,'Garfrost tired of puny mortals. Now your bones will freeze!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16917,1,0,0,'garfrost SAY_PHASE3'),
(-1658007,'Another shall take his place. You waste your time.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16752,1,0,0,'Tyrannus SAY_TYRANNUS_DEATH'),

-- Krick
(-1658010,'Our work must not be interrupted! Ick! Take care of them!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16926,1,0,0,'Krick SAY_AGGRO'),
(-1658011,'Ooh...We could probably use these parts!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16927,1,0,0,'Krick SAY_SLAY_1'),
(-1658012,'Arms and legs are in short supply...Thanks for your contribution!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16928,1,0,0,'Krick SAY_SLAY_2'),
(-1658013,'Enough moving around! Hold still while I blow them all up!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16929,1,0,0,'Krick SAY_BARRAGE_1'),
(-1658014,'Krick begins rapidly conjuring explosive mines!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Krick SAY_BARRAGE_2'),
(-1658015,'Quickly! Poison them all while they''re still close!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16930,1,0,0,'Krick SAY_POISON_NOVA'),
(-1658016,'No! That one! That one! Get that one!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16931,1,0,0,'Krick SAY_CHASE_1'),
(-1658017,'I''ve changed my mind...go get that one instead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16932,1,0,0,'Krick SAY_CHASE_2'),
(-1658018,'What are you attacking him for? The dangerous one is over there,fool!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16933,1,0,0,'Krick SAY_CHASE_3'),

-- Ick
(-1658020,'Ick begins to unleash a toxic poison cloud!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Ick SAY_ICK_POISON_NOVA'),
(-1658021,'Ick is chasing you!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Ick SAY_ICK_CHASE_1'),

-- Krick OUTRO
(-1658030,'Wait! Stop! Don''t kill me, please! I''ll tell you everything!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16934,1,0,0,'Krick SAY_KRICK_OUTRO_1'),
(-1658031,'I''m not so naive as to believe your appeal for clemency, but I will listen.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16611,1,0,0,'Jaina SAY_JAINA_OUTRO_2'),
(-1658032,'Why should the Banshee Queen spare your miserable life?',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17033,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_2'),
(-1658033,'What you seek is in the master''s lair, but you must destroy Tyrannus to gain entry. Within the Halls of Reflection you will find Frostmourne. It... it holds the truth.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16935,1,0,0,'Krick SAY_KRICK_OUTRO_3'),
(-1658034,'Frostmourne lies unguarded? Impossible!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16612,1,0,0,'Jaina SAY_JAINA_OUTRO_4'),
(-1658035,'Frostmourne? The Lich King is never without his blade! If you are lying to me...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17034,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_4'),
(-1658036,'I swear it is true! Please, don''t kill me!!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16936,1,0,0,'Krick SAY_KRICK_OUTRO_5'),
(-1658037,'Worthless gnat! Death is all that awaits you!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16753,1,0,0,'Tyrannus SAY_TYRANNUS_OUTRO_7'),
(-1658038,'Urg... no!!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16937,1,0,0,'Krick SAY_KRICK_OUTRO_8'),
(-1658039,'Do not think that I shall permit you entry into my master''s sanctum so easily. Pursue me if you dare.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16754,1,0,0,'Tyrannus SAY_TYRANNUS_OUTRO_9'),
(-1658040,'What a cruel end. Come, heroes. We must see if the gnome''s story is true. If we can separate Arthas from Frostmourne, we might have a chance at stopping him.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16613,1,0,0,'Jaina SAY_JAINA_OUTRO_10'),
(-1658041,'A fitting end for a traitor. Come, we must free the slaves and see what is within the Lich King''s chamber for ourselves.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17035,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_10'),

-- Tyrannus
(-1658050,'Your pursuit shall be in vain, adventurers, for the Lich King has placed an army of undead at my command! Behold!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16755,1,0,0,'Tyrannus SAY_AMBUSH_1'),
(-1658051,'Persistent whelps! You will not reach the entrance of my lord''s lair! Soldiers, destroy them!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16756,1,0,0,'Tyrannus SAY_AMBUSH_2'),
(-1658052,'Rimefang! Trap them within the tunnel! Bury them alive!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16757,1,0,0,'Tyrannus SAY_GAUNTLET_START'),
(-1658053,'Alas, brave, brave adventurers, your meddling has reached its end. Do you hear the clatter of bone and steel coming up the tunnel behind you? That is the sound of your impending demise.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16758,1,0,0,'Tyrannus SAY_INTRO_1'),
(-1658054,'Ha, such an amusing gesture from the rabble. When I have finished with you, my master''s blade will feast upon your souls. Die!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16759,1,0,0,'Tyrannus SAY_INTRO_2'),

(-1658055,'I shall not fail The Lich King! Come and meet your end!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16760,1,0,0,'Tyrannus SAY_AGGRO'),
(-1658056,'Such a shameful display... You are better off dead!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16761,1,0,0,'Tyrannus SAY_SLAY_1'),
(-1658057,'Perhaps you should have stayed in the mountains!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16762,1,0,0,'Tyrannus SAY_SLAY_2'),
(-1658058,'Impossible! Rimefang... Warn...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16763,1,0,0,'Tyrannus SAY_DEATH'),
(-1658059,'Rimefang, destroy this fool!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16764,1,0,0,'Tyrannus SAY_MARK_RIMEFANG_1'),
(-1658060,'The frostwyrm Rimefang gazes at $N and readies an icy attack!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Tyrannus SAY_MARK_RIMEFANG_2'),
(-1658061,'Power... overwhelming!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16765,1,0,0,'Tyrannus SAY_DARK_MIGHT_1'),
(-1658062,'Scourgelord Tyrannus roars and swells with dark might!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Tyrannus SAY_DARK_MIGHT_2'),

(-1658063,'Brave champions, we owe you our lives, our freedom... Though it be a tiny gesture in the face of this enormous debt, I pledge that from this day forth, all will know of your deeds, and the blazing path of light you cut through the shadow of this dark citadel.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0,'Gorkun  SAY_GORKUN_OUTRO_1'),
(-1658064,'This day will stand as a testament not only to your valor, but to the fact that no foe, not even the Lich King himself, can stand when Alliance and Horde set aside their differences and ---',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,1,0,0,'Gorkun  SAY_GORKUN_OUTRO_2'),
(-1658065,'Heroes, to me!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16614,1,0,0,'Jaina SAY_JAYNA_OUTRO_3'),
(-1658066,'Take cover behind me! Quickly!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17037,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_3'),
(-1658067,'The Frost Queen is gone. We must keep moving - our objective is near.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16615,0,0,0,'Jaina SAY_JAYNA_OUTRO_4'),
(-1658068,'I thought he''d never shut up. At last, Sindragosa silenced that long-winded fool. To the Halls of Reflection, champions! Our objective  is near... I can sense it.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,17036,0,0,0,'Sylvanas SAY_SYLVANAS_OUTRO_4'),
(-1658069,'I... I could not save them... Damn you, Arthas! DAMN YOU!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16616,0,0,0,'Jaina SAY_JAYNA_OUTRO_5');

-- Forge of souls
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1632099 AND -1632000;
INSERT INTO `script_texts` (`entry`,`content_default`,`content_loc1`,`content_loc2`,`content_loc3`,`content_loc4`,`content_loc5`,`content_loc6`,`content_loc7`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
-- Bronjham
(-1632001,'Finally...a captive audience!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Наконец то! Гости пожаловали!',16595,6,0,0,'Bronjham SAY_AGGRO'),
(-1632002,'Fodder for the engine!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Скормлю тебя машине!',16596,6,0,0,'Bronjham SAY_SLAY_1'),
(-1632003,'Another soul to strengthen the host!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Еще одна душа обогатит вместилище!',16597,6,0,0,'Bronjham SAY_SLAY_2'),
(-1632004,'Oooooo...',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Аааааааааааа...',16598,6,0,0,'Bronjham SAY_DEATH'),
(-1632005,'The vortex of the harvested calls to you!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Вихрь погубленных душ, взываю к вам!',16599,3,0,0,'Bronjham SAY_SOUL_STORM'),
(-1632006,'I will sever the soul from your body!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Я вырву душу из твоего тела!',16600,6,0,0,'Bronjham SAY_CORRUPT_SOUL'),

-- Devourer of Souls
(-1632010,'You dare look upon the host of souls? I SHALL DEVOUR YOU WHOLE!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Вы осмелились взглянуть на вместилище душ! Я сожру вас заживо!',16884,1,0,0,'Devoureur SAY_FACE_ANGER_AGGRO'),
(-1632011,'You dare look upon the host of souls? I SHALL DEVOUR YOU WHOLE!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16890,1,0,0,'Devoureur SAY_FACE_DESIRE_AGGRO'),
(-1632012,'Damnation!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Проклинаю тебя!',16885,1,0,0,'Devoureur SAY_FACE_ANGER_SLAY_1'),
(-1632013,'Damnation!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16896,1,0,0,'Devoureur SAY_FACE_SORROW_SLAY_1'),
(-1632014,'Damnation!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16891,1,0,0,'Devoureur SAY_FACE_DESIRE_SLAY_1'),
(-1632015,'Doomed for eternity!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Обрекаю тебя на вечные муки!',16886,1,0,0,'Devoureur SAY_FACE_ANGER_SLAY_2'),
(-1632016,'Doomed for eternity!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16897,1,0,0,'Devoureur SAY_FACE_SORROW_SLAY_2'),
(-1632017,'Doomed for eternity!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16892,1,0,0,'Devoureur SAY_FACE_DESIRE_SLAY_2'),
(-1632018,'The swell of souls will not be abated! You only delay the inevitable!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Вместилише душ не ослабнет! Вы лишь пытаетесь отсрочить неизбежное.',16887,1,0,0,'Devoureur SAY_FACE_ANGER_DEATH'),
(-1632019,'The swell of souls will not be abated! You only delay the inevitable!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16898,1,0,0,'Devoureur SAY_FACE_SORROW_DEATH'),
(-1632020,'The swell of souls will not be abated! You only delay the inevitable!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16893,1,0,0,'Devoureur SAY_FACE_DESIRE_DEATH'),
(-1632021,'Devourer of Souls begins to cast Mirrored Soul!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Devoureur EMOTE_MIRRORED_SOUL'),
(-1632022,'Devourer of Souls begins to Unleash Souls!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Devoureur EMOTE_UNLEASH_SOUL'),
(-1632023,'SUFFERING! ANGUISH! CHAOS! RISE AND FEED!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Страдание, мучение, хаос! Восстаньте и пируйте!',16888,1,0,0,'Devoureur SAY_FACE_ANGER_UNLEASH_SOUL'),
(-1632024,'SUFFERING! ANGUISH! CHAOS! RISE AND FEED!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16899,1,0,0,'Devoureur SAY_FACE_SORROW_UNLEASH_SOUL'),
(-1632025,'SUFFERING! ANGUISH! CHAOS! RISE AND FEED!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16894,1,0,0,'Devoureur SAY_FACE_DESIRE_UNLEASH_SOUL'),
(-1632026,'Devourer of Souls begins to cast Wailing Souls!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,3,0,0,'Devoureur EMOTE_WAILING_SOUL'),
(-1632027,'Stare into the abyss, and see your end!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Вглядитесь в бездну, и узрите свою смерть!',16889,1,0,0,'Devoureur SAY_FACE_ANGER_WAILING_SOUL'),
(-1632028,'Stare into the abyss, and see your end!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,16895,1,0,0,'Devoureur SAY_FACE_DESIRE_WAILING_SOUL'),
(-1632029,'Excellent work, champions! We shall set up our base camp in these chambers. My mages will get the Scourge transport device working shortly. Step inside it when you''re ready for your next mission. I will meet you on the other side.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Вы справились. Мы разобьем лагерь в этих покоях. Вскоре мои маги заставят портал плети работать! Войдите в него, когда будете готовы к следующему заданию. Я присоеденусь к вам чуть позже.',16625,1,0,0,'Jaina SAY_JAINA_OUTRO'),
(-1632030,'Excellent work, champions! We shall set up our base camp in these chambers. My mages will get the Scourge transport device working shortly. Step inside when you are ready for your next mission. I will meet you on the other side.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Превосходно! Мы разобьем лагерь в этих покоях! Вскоре мои маги заставят портал плети работать, войдите в него когда будете готовы к следующему заданию! Я присоединюсь к вам позже.',17044,1,0,0,'Sylvanas SAY_SYLVANAS_OUTRO'),

-- Jaina
(-1632040,'Thank the light for seeing you here safely. We have much work to do if we are to defeat the Lich King and put an end to the Scourge.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Слава свету! Вы целы и невредимы нам предстоит многое сделать, если мы хотим покончить с королем личем и плетью.',16617,0,0,0,'Jaina SAY_INTRO_1'),
(-1632041,'Our allies within the Argent Crusade and the Knights of the Ebon Blade have broken through the front gate of Icecrown and are attempting to establish a foothold within the Citadel.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Серебряный Авангард и рыцари черного клинка прорвались через главные ворота и пытаются укрепить свои позиции в цитадели!',16618,0,0,0,'Jaina SAY_INTRO_2'),
(-1632042,'Their success hinges upon what we discover in these cursed halls. Although our mission is a wrought with peril, we must persevere!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Их успех зависит от того что мы найдем этих ужасных залах. Пусть наша миссия опасна, но мы должны выстоять.',16619,0,0,0,'Jaina SAY_INTRO_3'),
(-1632043,'With the attention of the Lich King drawn toward the front gate, we will be working our way through the side in search of information that will enable us to defeat the Scourge - once and for all.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Пока король лич отвлекся на главные ворота, мы проникнем внутрь другим путем и постараемся узнать как можно покончить с плетью раз и навсегда.',16620,0,0,0,'Jaina SAY_INTRO_4'),
(-1632044,'King Varian''s SI7 agents have gathered information about a private sanctum of the Lich King''s deep within a place called the Halls of Reflection.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Разведчики SI7 отправленные Варианом сообщают что покои короля находятся в глубине дворца! Это место называется Залами отражений.',16621,0,0,0,'Jaina SAY_INTRO_5'),
(-1632045,'We will carve a path through this wretched place and find a way to enter the Halls of Reflection. I sense powerful magic hidden away within those walls... Magic that could be the key to destroy the Scourge.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Мы проложим себе путь сквозь это проклятое место и найдем вход в залы отражений. Я чувствую что в них сокрыта могушественная магия, которая поможет нам сокрушить плеть!',16622,0,0,0,'Jaina SAY_INTRO_6'),
(-1632046,'Your first mission is to destroy the machines of death within this malevolent engine of souls, and clear a path for our soldiers.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Ваша первая задача разрушить машины смерти в этом механизме душ, это откроет путь нашим солдатам.',16623,0,0,0,'Jaina SAY_INTRO_7'),
(-1632047,'Make haste, champions! I will prepare the troops to fall in behind you.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Спешите герои, я прикажу солдатам следовать за вами!',16624,0,0,0,'Jaina SAY_INTRO_8'),

-- Sylvanas
(-1632050,'The Argent Crusade and the Knights of the Ebon Blade have assaulted the gates of Icecrown Citadel and are preparing for a massive attack upon the Scourge. Our missition is a bit more subtle, but equally as important.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Серебряный Авангард и рыцари черного клинка штурмуют ворота цитадели ледяной короны! И готовятся нанести решаюший удар! Мы будем действовать незаметно, но не менее эффективно.',17038,0,0,0,'Sylvanas SAY_INTRO_1'),
(-1632051,'With the attention of the Lich King turned towards the front gate, we''ll be working our way through the side in search of information that will enable us to defeat the Lich King - once and for all.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Пока Король-Лич отвлекся мы проникнем внутрь другим путем и постараемся понять как можно покончить с ним раз и навсегда.',17039,0,0,0,'Sylvanas SAY_INTRO_2'),
(-1632052,'Our scouts have reported that the Lich King has a private chamber, outside of the Frozen Throne, deep within a place called the Halls of Reflection. That is our target, champions.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Наши разведчики сообщили что покои короля лича находятся в глубине дворца, недалеко от ледяного трона. Это место называется залами отражений, туда и лежит наш путь.',17040,0,0,0,'Sylvanas SAY_INTRO_3'),
(-1632053,'We will cut a swath of destruction through this cursed place and find a way to enter the Halls of Reflection. If there is anything of value to be found here, it will be found in the Halls.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Мы проложим себе путь сквозь это проклетое место и найдем и найдем вход в залы отражений! Если в цитадели и есть что то достойное внимания оно ждет нас именно там.',17041,0,0,0,'Sylvanas SAY_INTRO_4'),
(-1632054,'Your first mission is to destroy the machines of death within this wretched engine of souls, and clear a path for our soldiers.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Вашей первой задачей будет  разрушение машин смерти в этом гнусном механизме душ, это откроет путь к нашим солдатам',17042,0,0,0,'Sylvanas SAY_INTRO_5'),
(-1632055,'The Dark Lady watches over you. Make haste!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Темная госпожа будет наблюдать за вами, спешите!',17043,0,0,0,'Sylvanas SAY_INTRO_6');

-- Halls of reflection (from MaxXx2021 aka Mioka)
DELETE FROM `script_texts` WHERE `entry` BETWEEN -1594540 AND -1594430;
INSERT INTO `script_texts` (`entry`,`content_default`,`content_loc8`,`sound`,`type`,`language`,`emote`,`comment`) VALUES
# SCENE - Hall Of Reflection (Intro) - PreUther
(-1594433, '<need translate>', 'Как же тут холодно... Кровь стынет в жилах.', 16631,0,0,1, '67234'),
(-1594434, '<need translate>', 'Я... Я не верю своим глазам. Ледяная скорбь перед нами без всякой охраны! Как и говорил гном. Вперед герои!', 17049,0,0,1, '67234'),
(-1594435, '<need translate>', 'Что это там впереди? Неужели? Скорее герои!', 16632,1,0,1, '67234'),
(-1594436, '<need translate>', 'Ледяная Скорбь, клинок, разрушивший наше королевство...', 16633,1,0,1, '67234'),
(-1594437, '<need translate>', 'Боль снова захлестывает меня, когда я так близко вижу меч, отнявший у меня жизнь.', 17050,0,0,1, '67234'),
(-1594438, '<need translate>', 'Отойдите. Тот кто коснется этого клинка обречет себя на вечные муки. Я попытаюсь заговорить с душами заключенными в Ледяной скорби. Расступитесь... Чуть назад! Прошу.', 16634,1,0,1, '67234'),
(-1594439, '<need translate>', 'Я не смею его коснуться. Назад! Отступите! Я попробую установить связь с мечом. Возможно, спасение находится внутри!', 17051,1,0,1, '67234'),
# SCENE - Hall Of Reflection (Intro) - UtherDialog
(-1594440, '<need translate>', 'Джайна? Неужели это ты?', 16666,0,0,1, '67234'),
(-1594441, '<need translate>', 'Осторожней девочка! Однажды мне уже говорили, что этот проклятый меч может нас спасти. Посмотри вокруг, и ты увидишь, что из этого вышло.', 16659,0,0,1, '67234'),
(-1594442, '<need translate>', 'Утер? Милый Утер! Мне... Мне так жаль.', 16635,0,0,1, '67234'),
(-1594443, '<need translate>', 'Утер? Утер Светоносный? Как...', 17052,0,0,1, '67234'),
(-1594444, '<need translate>', 'Джайна, у вас мало времени. Король - Лич видит все что видит Ледяная Скорбь. Вскоре он будет здесь.', 16667,0,0,1, '67234'),
(-1594445, '<need translate>', 'У вас мало времени. Король - Лич видит все что видит Ледяная Скорбь. Вскоре он будет здесь.', 16660,0,0,1, '67234'),
(-1594446, '<need translate>', 'Артас здесь? Может я...', 16636,0,0,1, '67234'),
(-1594447, '<need translate>', 'Король - Лич здесь? Значит моя судьба решится сегодня!', 17053,1,0,1, '67234'),
(-1594448, '<need translate>', 'Нет девочка. Артаса здесь нет. Артас лишь тень, мелькающая в сознании Короля - Лича. Смутная тень.', 16668,0,0,1, '67234'),
(-1594449, '<need translate>', 'Вам не победить Короля - Лича. Покрайней мере не здесь. Глупо и пытаться. Он убьет твоих соратников и воскресит их как воинов плети. Но что до тебя Сильвана, он готовит тебе участь еще страшнее, чем в прошлый раз.', 16661,0,0,1, '67234'),
(-1594450, '<need translate>', 'Но если есть малейшая надежда вернуть Артаса... Я должна попытаться!', 16637,0,0,1, '67234'),
(-1594451, '<need translate>', 'Должен быть способ!', 17054,0,0,1, '67234'),
(-1594452, '<need translate>', 'Джайна послушай меня. Вам нужно уничтожить Короля - Лича. С ним нельзя договориться. Он убьет вас всех и превратит в могущественных воинов Плети.', 16669,0,0,1, '67234'),
(-1594453, '<need translate>', 'Возможно... Но знай! Король - Лич должен быть всегда. Даже если вы убьете Артаса кто то обязан будет занять его место. Лишившись правителя Плеть налетит на мир как стая саранчи и уничтожит все на своем пути.', 16662,0,0,1, '67234'),
(-1594454, '<need translate>', 'Но как Утер? Как мне убить моего принца, моего...', 16638,0,0,1, '67234'),
(-1594455, '<need translate>', 'Кому по силам такое бремя?', 17055,0,0,1, '67234'),
(-1594456, '<need translate>', 'Забудь об этом девочка. Короля - Лича нужно уничтожить на том месте, где он слился с Нерзулом. На самой вершине, у Ледяного Трона!', 16670,0,0,1, '67234'),
(-1594457, '<need translate>', 'Не знаю, Королева Баньши... Если бы не Артас, который все еще является частью Короля - Лича, Плеть давно бы уже уничтожила Азерот.', 16663,0,0,1, '67234'),
(-1594458, '<need translate>', 'Ты прав Утер, прости меня... Я не знаю что на меня нашло. Мы передадим твои слова Королю и рыцарям, которые сражаются с Плетью в Цитадели Ледяной Короны.', 16639,0,0,1, '67234'),
(-1594459, '<need translate>', 'Тебе нужно знать еще кое что о Короле - Личе. Плеть не должна выйти из под контроля. Даже если вы убьете Короля - Лича, кто-то должен будет занять его место. Без Короля Плеть налетит на мир как стая саранчи и уничтожит все живое.', 16671,0,0,1, '67234'),
(-1594460, '<need translate>', 'Увы единственый способ одолеть Короля - Лича - это убить его там где он был порожден.', 16664,0,0,1, '67234'),
(-1594461, '<need translate>', 'Кому по силам такое бремя?', 16640,0,0,1, '67234'),
(-1594462, '<need translate>', 'Ледяной Трон!', 17056,0,0,1, '67234'),
(-1594463, '<need translate>', 'Великая жертва, благородной души.', 16672,0,0,1, '67234'),
(-1594464, '<need translate>', 'Не знаю Джайна... мне кажется если бы не Артас, который все еще является частью Короля - Лича, Плеть давно бы уже уничтожила Азерот.', 16673,0,0,1, '67234'),
(-1594465, '<need translate>', 'Но может еще есть надежда?', 16641,0,0,1, '67234'),
(-1594466, '<need translate>', 'Нет Джайна... Эээээ... Он... Он приближается... Вы... Вы должны...', 16674,1,0,1, '67234'),
(-1594467, '<need translate>', 'Да... Эээээ... Он... Он приближается... Вы... Вы должны...', 16665,1,0,1, '67234'),
(-1594468, '<need translate>', 'Замолчи, паладин.', 17225,1,0,0, '67234'),
(-1594469, '<need translate>', 'Так ты хочешь поговорить с мертвыми? Нет ничего проще!', 17226,1,0,0, '67234'),
(-1594470, '<need translate>', 'Фалрик, Марвин, когда закончите, принесите их тела в мои покои.', 17227,0,0,0, '67234'),
(-1594471, '<need translate>', 'Ты от меня не отмахнешься Артас. Я должна понять, я должна знать.', 16642,1,0,1, '67234'),
(-1594472, '<need translate>', 'Ты так просто от меня не уйдешь Артас. Я отомщу тебе!', 17057,1,0,1, '67234'),
(-1594473, '<need translate>', 'Глупая девчонка! Тот кого ты ищещь давно убит! Теперь он лишь призрак, слабый отзвук в моем сознании!', 17229,1,0,0, '67234'),
(-1594474, '<need translate>', 'Я не повторю прежней ошибки, Сильвана. На этот раз тебе не спастись. Ты не оправдала моего доверия и теперь тебя ждет только забвение!', 17228,1,0,0, '67234'),
(-1594475, '<need translate>', 'Как пожелаете, мой господин!', 16717,1,0,0, '67234'),
(-1594476, '<need translate>', 'Как пожелаете, мой господин!', 16741,1,0,0, '67234'),
# SCENE - Hall Of Reflection (Extro) - PreEscape
(-1594477, '<need translate>', 'Твои союзники прибыли, Джайна! Как ты и обещала... Ха-ха-ха-ха... Все вы станете могучими солдатами Плети...', 17212,1,0,0, '67234'),
(-1594478, '<need translate>', 'Я не повторю прежней ошибки, Сильвана! На этот раз тебе не спастись. Вы все будите служить мне после смерти...', 17213,1,0,0, '67234'),
(-1594479, '<need translate>', 'Он слишком силен. Мы должны выбраться от сюда как можно скорее. Моя магия задержит его ненадолго, быстрее герои...', 16644,0,0,1, '67234'),
(-1594480, '<need translate>', 'Он слишком силен. Герои скорее, за мной. Мы должны выбраться отсюда немедленно. Я постараюсь его задержать, пока мы будем убегать.', 17058,0,0,1, '67234'),
# SCENE - Hall Of Reflection (Extro) - Escape
(-1594481, '<need translate>', 'Смерть распростерла ледяные обьятия!', 17221,1,0,0, '67234'),
(-1594482, '<need translate>', 'Восстаньте прислужники, не дайте им сбежать!', 17216,1,0,0, '67234'),
(-1594483, '<need translate>', 'Схватите их! Принесите мне их тела!', 17222,1,0,0, '67234'),
(-1594484, '<need translate>', 'Надежды нет!', 17214,1,0,0, '67234'),
(-1594485, '<need translate>', 'Смирись с судьбой.', 17215,1,0,0, '67234'),
(-1594486, '<need translate>', 'Бежать некуда!', 17217,1,0,0, '67234'),
(-1594487, '<need translate>', 'Я разрушу эту преграду, а вы удерживайте нежить на расстоянии!', 16607,1,0,0, '67234'),
(-1594488, '<need translate>', 'Никакие стены не удержат Королеву Баньши. Держите нежить на расстоянии, я сокрушу эту преграду.', 17029,1,0,0, '67234'),
(-1594489, '<need translate>', 'Опять ледяная стена... Я разобью ее, только не дайте нежити прервать мои заклинания...', 16608,1,0,0, '67234'),
(-1594490, '<need translate>', 'Еще одна преграда. Держитесь герои! Я разрушу эту стену!', 17030,1,0,0, '67234'),
(-1594491, '<need translate>', 'Покоритесь Леденящей смерти!', 17218,1,0,0, '67234'),
(-1594492, '<need translate>', 'Вы в ловушке!', 17219,1,0,0, '67234'),
(-1594493, '<need translate>', 'Как долго вы сможете сопротивляться?', 17220,1,0,0, '67234'),
(-1594494, '<need translate>', 'Он с нами играет. Я  покажу ему что бывает когда лед встречается со огнем!', 16609,0,0,0, '67234'),
(-1594495, '<need translate>', 'Твои преграды больше не задержат нас, чудовище. Я смету их с пути!', 16610,1,0,0, '67234'),
(-1594496, '<need translate>', 'Я устала от этих игр Артас. Твои стены не остановят меня!', 17031,1,0,0, '67234'),
(-1594497, '<need translate>', 'Ты не помешаешь нам уйти, монстр. Сдерживайте нежить, а я уничтожу эту преграду.', 17032,1,0,0, '67234'),
(-1594498, '<need translate>', 'Я вижу выход, скорее!', 16645,1,0,0, '67234'),
(-1594499, '<need translate>', 'Мы почти выбрались, не сдавайтесь!', 16646,1,0,0, '67234'),
(-1594500, '<need translate>', 'Я вижу выход, скорее!', 17059,1,0,0, '67234'),
(-1594501, '<need translate>', 'Мы почти выбрались, не сдавайтесь!', 17060,1,0,0, '67234'),
(-1594502, '<need translate>', 'Больше некуда бежать. Теперь нам придется сражаться. Это наша последняя битва!', 16647,1,0,0, '67234'),
(-1594503, '<need translate>', 'Проклятый тупик, значит все закончится здесь. Готовьтесь герои, это наша последняя битва.', 17061,1,0,0, '67234'),
(-1594504, '<need translate>', 'Ха-ха-ха... Бежать некуда. Теперь вы мои!', 17223,1,0,0, '67234'),
(-1594505, '<need translate>', 'Солдаты Лордерона, восстаньте по зову Господина!', 16714,1,0,0, '67234'),
(-1594506, '<need translate>', 'Господин осмотрел свое королевство и признал его негодным! Его суд был быстрым и суровым - предать всех смерти!', 16738,1,0,0, '67234'),

#Falric
(-1594507, '<need translate>', 'Мужчины, Женщины и дети... Никто не избежал гнева господина. Вы разделите их участь!', 16710,1,0,0, '67234'),
(-1594508, '<need translate>', 'Марвин... Добей их...', 16713,1,0,0, '67234'),
(-1594509, '<need translate>', 'Сопливый червяк!', 16711,1,0,0, '67234'),
(-1594510, '<need translate>', 'Стратхольмские детишки - и те сражались отчаяннее!', 16712,1,0,0, '67234'),
(-1594511, '<need translate>', 'Как сладостно отчаянье!', 16715,1,0,0, '67234'),
(-1594512, '<need translate>', 'Как приятен страх!', 16716,1,0,0, '67234'),

#Marwyn
(-1594513, '<need translate>', 'Вы найдете здесь лишь смерть!', 16734,1,0,0, '67234'),
(-1594514, '<need translate>', 'Эээээ... Да... Бегите навстречу судьбе. Ее жестокие и холодные обьятия ждут вас...', 16737,1,0,0, '67234'),
(-1594515, '<need translate>', 'У Теренаса был такой же взгляд в миг смерти, он никак не мог поверить... Ха-ха-ха-ха-ха...', 16735,1,0,0, '67234'),
(-1594516, '<need translate>', 'Захлебнись страданием!', 16736,1,0,0, '67234'),
(-1594517, '<need translate>', 'Вы увидите как разлагается ваша плоть!', 16739,1,0,0, '67234'),
(-1594518, '<need translate>', 'Сгиньте без следа!', 16740,1,0,0, '67234'),

#FrostWorn General
(-1594519, '<need translate>', 'Вы недостойны предстать перед Королем - Личом!', 16921,1,0,0, '67234'),
(-1594520, '<need translate>', 'Господин... Я подвел вас...', 16922,1,0,0, '67234'),

#add
(-1594531, '<need translate>', 'Ну теперь-то точно пора сваливать.', 0,0,0,0, '67234'),
(-1594532, '<need translate>', 'Вот вам сундук за работу.', 0,0,0,0, '67234'),
(-1594533, '<need translate>', 'И, поскольку корабля с оффа не будет, вот вам портал в Даларан.', 0,0,0,0, '67234');

-- Waipoints to escort event on Halls of reflection

DELETE FROM script_waypoint WHERE entry=36955;
DELETE FROM script_waypoint WHERE entry=37226;
DELETE FROM script_waypoint WHERE entry=37554;

INSERT INTO script_waypoint VALUES
-- Jaina

   (36955, 0, 5587.682,2228.586,733.011, 0, 'WP1'),
   (36955, 1, 5600.715,2209.058,731.618, 0, 'WP2'),
   (36955, 2, 5606.417,2193.029,731.129, 0, 'WP3'),
   (36955, 3, 5598.562,2167.806,730.918, 0, 'WP4 - Summon IceWall 01'), 
   (36955, 4, 5556.436,2099.827,731.827, 0, 'WP5 - Spell Channel'),
   (36955, 5, 5543.498,2071.234,731.702, 0, 'WP6'),
   (36955, 6, 5528.969,2036.121,731.407, 0, 'WP7'),
   (36955, 7, 5512.045,1996.702,735.122, 0, 'WP8'),
   (36955, 8, 5504.490,1988.789,735.886, 0, 'WP9 - Spell Channel'),
   (36955, 9, 5489.645,1966.389,737.653, 0, 'WP10'),
   (36955, 10, 5475.517,1943.176,741.146, 0, 'WP11'),
   (36955, 11, 5466.930,1926.049,743.536, 0, 'WP12'),
   (36955, 12, 5445.157,1894.955,748.757, 0, 'WP13 - Spell Channel'),
   (36955, 13, 5425.907,1869.708,753.237, 0, 'WP14'),
   (36955, 14, 5405.118,1833.937,757.486, 0, 'WP15'),
   (36955, 15, 5370.324,1799.375,761.007, 0, 'WP16'),
   (36955, 16, 5335.422,1766.951,767.635, 0, 'WP17 - Spell Channel'),
   (36955, 17, 5311.438,1739.390,774.165, 0, 'WP18'),
   (36955, 18, 5283.589,1703.755,784.176, 0, 'WP19'),
   (36955, 19, 5260.400,1677.775,784.301, 3000, 'WP20'),
   (36955, 20, 5262.439,1680.410,784.294, 0, 'WP21'),
   (36955, 21, 5260.400,1677.775,784.301, 0, 'WP22'),

-- Sylvana

   (37554, 0, 5587.682,2228.586,733.011, 0, 'WP1'),
   (37554, 1, 5600.715,2209.058,731.618, 0, 'WP2'),
   (37554, 2, 5606.417,2193.029,731.129, 0, 'WP3'),
   (37554, 3, 5598.562,2167.806,730.918, 0, 'WP4 - Summon IceWall 01'), 
   (37554, 4, 5556.436,2099.827,731.827, 0, 'WP5 - Spell Channel'),
   (37554, 5, 5543.498,2071.234,731.702, 0, 'WP6'),
   (37554, 6, 5528.969,2036.121,731.407, 0, 'WP7'),
   (37554, 7, 5512.045,1996.702,735.122, 0, 'WP8'),
   (37554, 8, 5504.490,1988.789,735.886, 0, 'WP9 - Spell Channel'),
   (37554, 9, 5489.645,1966.389,737.653, 0, 'WP10'),
   (37554, 10, 5475.517,1943.176,741.146, 0, 'WP11'),
   (37554, 11, 5466.930,1926.049,743.536, 0, 'WP12'),
   (37554, 12, 5445.157,1894.955,748.757, 0, 'WP13 - Spell Channel'),
   (37554, 13, 5425.907,1869.708,753.237, 0, 'WP14'),
   (37554, 14, 5405.118,1833.937,757.486, 0, 'WP15'),
   (37554, 15, 5370.324,1799.375,761.007, 0, 'WP16'),
   (37554, 16, 5335.422,1766.951,767.635, 0, 'WP17 - Spell Channel'),
   (37554, 17, 5311.438,1739.390,774.165, 0, 'WP18'),
   (37554, 18, 5283.589,1703.755,784.176, 0, 'WP19'),
   (37554, 19, 5260.400,1677.775,784.301, 3000, 'WP20'),
   (37554, 20, 5262.439,1680.410,784.294, 0, 'WP21'),
   (37554, 21, 5260.400,1677.775,784.301, 0, 'WP22'),

-- Lich King

   (37226, 0, 5577.187,2236.003,733.012, 0, 'WP1'),
   (37226, 1, 5587.682,2228.586,733.011, 0, 'WP2'),
   (37226, 2, 5600.715,2209.058,731.618, 0, 'WP3'),
   (37226, 3, 5606.417,2193.029,731.129, 0, 'WP4'),
   (37226, 4, 5598.562,2167.806,730.918, 0, 'WP5'), 
   (37226, 5, 5559.218,2106.802,731.229, 0, 'WP6'),
   (37226, 6, 5543.498,2071.234,731.702, 0, 'WP7'),
   (37226, 7, 5528.969,2036.121,731.407, 0, 'WP8'),
   (37226, 8, 5512.045,1996.702,735.122, 0, 'WP9'),
   (37226, 9, 5504.490,1988.789,735.886, 0, 'WP10'),

   (37226, 10, 5489.645,1966.389,737.653, 0, 'WP10'),
   (37226, 11, 5475.517,1943.176,741.146, 0, 'WP11'),
   (37226, 12, 5466.930,1926.049,743.536, 0, 'WP12'),
   (37226, 13, 5445.157,1894.955,748.757, 0, 'WP13'),
   (37226, 14, 5425.907,1869.708,753.237, 0, 'WP14'),
   (37226, 15, 5405.118,1833.937,757.486, 0, 'WP15'),
   (37226, 16, 5370.324,1799.375,761.007, 0, 'WP16'),
   (37226, 17, 5335.422,1766.951,767.635, 0, 'WP17'),
   (37226, 18, 5311.438,1739.390,774.165, 0, 'WP18'),
   (37226, 19, 5283.589,1703.755,784.176, 0, 'WP19'),
   (37226, 20, 5278.694,1697.912,785.692, 0, 'WP20'),
   (37226, 21, 5283.589,1703.755,784.176, 0, 'WP19');
