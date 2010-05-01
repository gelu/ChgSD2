DELETE FROM `script_texts` WHERE `entry` BETWEEN -1631600 AND -1631000;
INSERT INTO `script_texts` (`entry`,`content_loc8`, `content_default`, `sound`, `type`, `language`, `emote`, `comment`) VALUES

-- Lord Marrowgar
('-1631000','Это начало и конец, смертные. Никто не может войти в Храм!','This is the beginning AND the end, mortals. None may enter the master\'s sanctum!','16950','6','0','0','marrowgar SAY_INTRO'),
('-1631001','Проклятые несут миру смерть и разрушение!','The Scourge will wash over this world as a swarm of death and destruction!','16941','6','0','0','marrowgar SAY_AGGRO'),
('-1631002','Шторм костей!','BONE STORM!','16946','3','0','0','marrowgar SAY_BONESTORM'),
('-1631003','Проткнут костью!','Bound by bone!','16947','3','0','0','marrowgar SAY_BONESPIKE1'),
('-1631004','Кости вокруг!','Stick Around!','16948','3','0','0','marrowgar SAY_BONESPIKE2'),
('-1631005','Выход - только смерть!','The only escape is death!','16949','6','0','0','marrowgar SAY_BONESPIKE3'),
('-1631006','Больше костей!','More bones for the offering!','16942','6','0','0','marrowgar SAY_KILL1'),
('-1631007','Будьте прокляты!','Languish in damnation!','16943','6','0','0','marrowgar SAY_KILL2'),
('-1631008','ЯРОСТЬ МАСТЕРА ТЕЧЕТ ЧЕРЕЗ МЕНЯ!','THE MASTER\'S RAGE COURSES THROUGH ME!','16945','3','0','0','marrowgar SAY_ENRAGE'),
('-1631009','Я вижу... Только тьму...','I see... only darkness...','16944','6','0','0','marrowgar SAY_DEATH'),

-- Lady Deathwhisper
('-1631020','Взгляните на ваши мягкие руки! Сухожилия, мясо, кровь! Это слабость! Серьезная ошибка! Шутка создателя со своими творениями! Чем раньше вы поймете что жизнь - это дефект, тем раньше вы сможете преодолеть вашу слабость!','Fix your eyes upon your crude hands! The sinew, the soft meat, the dark blood coursing within! It is a weakness! A crippling flaw! A joke played by the creators upon their own creations! The sooner you come to accept your condition as a defect, the sooner you will find yourselves in a position to transcend it!','16878','6','0','0','deathwhisper SAY_INTRO1'),
('-1631021','Через нашего Мастера все возможно! Его сила не имеет предела, и его воля непреклонна! Те, кто против него будут истреблены! А те, кто служат, которые подчиняются полностью, беспрекословно, с беззаветной преданностью ума и души? Возвышены!','Through our master all things are possible! His power is without limit, and his will unbending! Those who oppose him will be destroyed utterly! And those who serve, who serve wholly, unquestioningly, with utter devotion of mind and soul? Elevated! To heights beyond your ken!','16879','6','0','0','deathwhisper SAY_INTRO2'),
('-1631022','Вы нашли свой путь здесь, потому что вы принадлежите к числу немногих одаренных истинным видением мира, проклятого слепотой! Вы можете видеть сквозь туман, что висит над этим миром, как саван, и понять, где истинная сила лжи!','You have found your way here, because you are among the few gifted with true vision in a world cursed with blindness! You can see through the fog that hangs over this world like a shroud and grasp where true power lies!','16880','6','0','0','deathwhisper SAY_INTRO3'),
('-1631023','Что это за беспорядок?! Вы смеете гадить на этой священной земле? Вот вам и место последнего упокоения!','What is this disturbance?! You dare trespass upon this hallowed ground? This shall be your final resting place.','16868','6','0','0','deathwhisper SAY_AGGRO'),
('-1631024','Однако! Я вижу что пора взять дело в свои руки.','Enough! I see I must take matters into my own hands!','16877','6','0','0','deathwhisper SAY_PHASE2'),
('-1631025','Вы слабы и бессильны против меня!','You are weak, powerless to resist my will!','16876','6','0','0','deathwhisper SAY_DOMINATEMIND'),
('-1631026','Возьмите это благословение и покажите этим злоумышленникам где раки зимуют!','Take this blessing and show these intruders a taste of our master\'s power.','16873','6','0','0','deathwhisper SAY_DARKEMPOWERMENT'),
('-1631027','Мои слуги! Я освобождаю вас от проклятия плоти!','Loyal adherent! I release you from the curse of flesh!','16874','6','0','0','deathwhisper SAY_DARKTRANSFORMATION'),
('-1631028','Встань и предстань в истинном виде!','Arise and exalt in your pure form!','16875','6','0','0','deathwhisper SAY_ANIMATEDEAD'),
('-1631029','Вы еще не осознали бесполезность своих действий?','Do you yet grasp of the futility of your actions?','16869','6','0','0','deathwhisper SAY_KILL1'),
('-1631030','Прими Тьму! Тьма вечна...','Embrace the darkness... Darkness eternal!','16870','6','0','0','deathwhisper SAY_KILL2'),
('-1631031','Это игра продолжается слишком долго!','This charade has gone on long enough.','16872','3','0','0','deathwhisper SAY_BERSERK'),
('-1631032','Все - части плана Мастера! Ваш конец неизбежен...','All part of the masters plan! Your end is... inevitable!','16871','6','0','0','deathwhisper SAY_DEATH'),

-- Saurfang
('-1631100','Во имя Короля-Лича!','BY THE MIGHT OF THE LICH KING!','16694','6','0','0','saurfang SAY_AGGRO'),
('-1631101','Земля станет красной от вашей крови!','The ground runs red with your blood!','16699','6','0','0','saurfang SAY_FALLENCHAMPION'),
('-1631102','Корм, мои собаки!','Feast, my minions!','16700','3','0','0','saurfang SAY_BLOODBEASTS'),
('-1631103','Ты никто!','You are nothing!','16695','6','0','0','saurfang SAY_KILL1'),
('-1631104','Ваши души не найдут здесь избавления!','Your soul will find no redemption here!','16696','6','0','0','saurfang SAY_KILL2'),
('-1631105','Я вижу приближение смерти!','I have become... death!','16698','3','0','0','saurfang SAY_BERSERK'),
('-1631106','Я... Использован...','I... Am... Released.','16697','6','0','0','saurfang SAY_DEATH'),

-- Festergut
('-1631201','','','17123','6','0','0','Putricide Valve01'),
('-1631202','','','17124','6','0','0','Putricide Festergut Dead'),
('-1631203','','','16901','6','0','0','Festergut Aggro'),
('-1631204','','','16902','6','0','0','Festergut Slay 01'),
('-1631205','','','16903','6','0','0','Festergut Slay 02'),
('-1631206','','','16904','6','0','0','Festergut Death'),
('-1631207','','','16905','6','0','0','Festergut Berserk'),
('-1631208','','','16906','6','0','0','Festergut Explunge Blight'),
('-1631209','','','16907','6','0','0','Festergut Stinky Death '),

-- Rotface
('-1631220','','','17126','6','0','0','Putricide Slime Flow'),
('-1631221','','','16986','6','0','0','Rotface Aggro'),
('-1631222','','','16987','6','0','0','Rotface Slay 01'),
('-1631223','','','16988','6','0','0','Rotface Slay 02'),
('-1631224','','','16989','6','0','0','Rotface Death'),
('-1631225','','','16990','6','0','0','Rotface Berserk'),
('-1631226','','','16991','6','0','0','Rotface Infection'),
('-1631227','','','16992','6','0','0','Rotface Unstable Ooze'),
('-1631228','','','16993','6','0','0','Rotface Precious played when precious dies'),
('-1631229','','','16994','6','0','0','Rotface'),

-- Professor Putricide
('-1631230','','','17114','6','0','0','Putricide Aggro'),
('-1631231','','','17115','6','0','0','Putricide Slay 01'),
('-1631232','','','17116','6','0','0','Putricide Slay 02'),
('-1631233','','','17117','6','0','0','Putricide Death'),
('-1631234','','','17118','6','0','0','Putricide Berserk'),
('-1631235','','','17119','6','0','0','Putricide Gas Explosion'),
('-1631236','','','17120','6','0','0','Putricide Transform 01'),
('-1631237','','','17121','6','0','0','Putricide Transform 02'),
('-1631238','','','17122','6','0','0','Putricide Summon Ooze'),
('-1631239','','','17125','6','0','0','Putricide Airlock01 Before fight'),

-- Blood Prince Council
('-1631301','','','16795','6','0','0','Lanathel Intro Princes'),
('-1631302','','','16681','6','0','0','Valanar Slay 01'),
('-1631303','','','16682','6','0','0','Valanar Slay 02'),
('-1631304','','','16683','6','0','0','Valanar Death'),
('-1631305','','','16684','6','0','0','Valanar Berserk'),
('-1631306','','','16685','6','0','0','Valanar Empower'),
('-1631307','','','16686','6','0','0','Valanar Special'),

-- Blood Queen Lana'thel
('-1631321','','','16782','6','0','0','Lanathel Aggro'),
('-1631322','','','16783','6','0','0','Lanathel Bite 01'),
('-1631323','','','16784','6','0','0','Lanathel Bite 02'),
('-1631324','','','16785','6','0','0','Lanathel Add'),
('-1631325','','','16786','6','0','0','Lanathel Special 01'),
('-1631326','','','16787','6','0','0','Lanathel Special 02'),
('-1631327','','','16788','6','0','0','Lanathel Special 03'),
('-1631328','','','16789','6','0','0','Lanathel Reset'),
('-1631329','','','16790','6','0','0','Lanathel Mind Control'),
('-1631330','','','16791','6','0','0','Lanathel Slay 01'),
('-1631331','','','16792','6','0','0','Lanathel Slay 02'),
('-1631332','','','16793','6','0','0','Lanathel Berserk'),
('-1631333','','','16794','6','0','0','Lanathel Death'),
('-1631334','','','16796','6','0','0','Lanathel Empower'),

-- Valithria Dreamwalker
('-1631401','','','17064','6','0','0','Valithria Aggro'),
('-1631402','','','17065','6','0','0','Valithria Slay Bad Hostile NPC'),
('-1631403','','','17066','6','0','0','Valithria Slay Good - Player'),
('-1631404','','','17067','6','0','0','Valithria Berserk'),
('-1631405','','','17068','6','0','0','Valithria Dream World Open'),
('-1631406','','','17069','6','0','0','Valithria Health Low'),
('-1631407','','','17070','6','0','0','Valithria Health High'),
('-1631408','','','17071','6','0','0','Valithria Win'),
('-1631409','','','17072','6','0','0','Valithria Lose'),

-- Sindragosa
('-1631420','','','17007','6','0','0','Sindragosa Aggro'),
('-1631421','','','17008','6','0','0','Sindragosa Slay 01'),
('-1631422','','','17009','6','0','0','Sindragosa Slay 02'),
('-1631423','','','17010','6','0','0','Sindragosa Death'),
('-1631424','','','17011','6','0','0','Sindragosa Berserk'),
('-1631425','','','17012','6','0','0','Sindragosa Take Off - fly'),
('-1631426','','','17013','6','0','0','Sindragosa Freeze'),
('-1631427','','','17014','6','0','0','Sindragosa Arcane'),
('-1631428','','','17015','6','0','0','Sindragosa Special'),
('-1631429','','','17016','6','0','0','Sindragosa Low HP'),

-- Lich king && Tirion
('-1631501','','','17349','6','0','0','Lich King SAY_INTRO1'),
('-1631502','','','17390','6','0','0','Tirion SAY_INTRO2'),
('-1631503','','','17350','6','0','0','Lich King SAY_INTRO3'),
('-1631504','','','17391','6','0','0','Tirion SAY_INTRO4'),
('-1631505','','','17351','6','0','0','Lich King SAY_AGGRO');
