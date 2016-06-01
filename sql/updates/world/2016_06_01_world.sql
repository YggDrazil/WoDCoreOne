-- Hack fix The Eye of the Storm (28805, 28826)
-- Waters of Farseeing SAI
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI', `ScriptName`='' WHERE `entry` IN (207414, 207416);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (207414, 207416) AND `source_type`=1;
INSERT INTO `smart_scripts` 
(`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(207414, 1, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 33, 50054, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Waters of Farseeing - On gossip hello - Killcredit'),
(207414, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 28, 94687, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Waters of Farseeing - On link - Remove Waters of Farseeing'),
(207416, 1, 0, 1, 64, 0, 100, 0, 0, 0, 0, 0, 33, 50054, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Waters of Farseeing - On gossip hello - Killcredit'),
(207416, 1, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 28, 94687, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Waters of Farseeing - On link - Remove Waters of Farseeing');


-- Fixed Quest The Maelstrom (27203)
-- Spawn Portals to the Maelstrom
DELETE FROM `gameobject` WHERE `id`=205268;
INSERT INTO `gameobject` 
(`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `state`) VALUES 
(@, 205268, 1, 1, 1, 2048.06, -4377.16, 98.9305, 0, 300, 1), -- Orgrimmar
(@, 205268, 0, 1, 1, -8209.03, 428.774, 118.164, 2.84488, 300, 1); -- Stormwind City

DELETE FROM `spell_target_position` WHERE `id`=84464;
INSERT INTO `spell_target_position` 
(`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(84464, 0, 730, 851.307, 1067.76, -10.0183, 4.50656); -- Teleport to the Maelstrom

DELETE FROM `conditions` WHERE `SourceEntry`=84464 AND `SourceTypeOrReferenceId`=17;
INSERT INTO `conditions` 
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(17, 0, 84464, 0, 0, 28, 0, 27203, 0, 0, 0, 0, '', 'Show gossip if complete quest 27203 taken');
-- Trall
UPDATE `creature` SET `modelid`=0 WHERE `id`=45042; -- Remove model id

-- Hack fix Deepholm, Realm of Earth (27123)
-- Wyvern SAI
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=43713;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=43713;
INSERT INTO `smart_scripts` VALUES
(43713, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wyvern - On just summoned - Remove Ride Vehicle Hardcoded'),
(43713, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 11, 84700, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wyvern - On link - Cast Teleport to Temple of Earth'),
(43713, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wyvern - On link - Force despawn');

DELETE FROM `spell_target_position` WHERE `id`=84700;
INSERT INTO `spell_target_position` 
(`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(84700, 0, 646, 1076.78, 537.665, -44.5984, 3.38237); -- Teleport to Temple of Earth

DELETE FROM `conditions` WHERE `SourceGroup`=45005 AND `SourceTypeOrReferenceId`=18;
INSERT INTO `conditions` 
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(18, 45005, 81780, 0, 0, 28, 0, 27123, 0, 0, 0, 0, '', 'Show gossip if complete quest 27123 taken');

-- Hack fix Deepholm, Realm of Earth (27123)
-- Wyvern SAI
UPDATE `creature_template` SET `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=43713;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=43713;
INSERT INTO `smart_scripts` VALUES
(43713, 0, 0, 1, 54, 0, 100, 0, 0, 0, 0, 0, 28, 46598, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wyvern - On just summoned - Remove Ride Vehicle Hardcoded'),
(43713, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 11, 84700, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Wyvern - On link - Cast Teleport to Temple of Earth'),
(43713, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 1000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Wyvern - On link - Force despawn');

DELETE FROM `spell_target_position` WHERE `id`=84700;
INSERT INTO `spell_target_position` 
(`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(84700, 0, 646, 1076.78, 537.665, -44.5984, 3.38237); -- Teleport to Temple of Earth

DELETE FROM `conditions` WHERE `SourceGroup`=45005 AND `SourceTypeOrReferenceId`=18;
INSERT INTO `conditions` 
(`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(18, 45005, 81780, 0, 0, 28, 0, 27123, 0, 0, 0, 0, '', 'Show gossip if complete quest 27123 taken');

-- Added Quest A Personal Summons to Hero's Call Board
DELETE FROM `gameobject_queststarter` WHERE `id` IN (206111, 206294) AND `quest`=28825;
INSERT INTO `gameobject_queststarter` 
(`id`, `quest`) VALUES 
(206111, 28825),
(206294, 28825);

-- Fixed Quest The Maelstrom (Alliance)
UPDATE `quest_template` SET `NextQuestId`=27203 WHERE `Id` IN (27722, 27727);
UPDATE `quest_template` SET `PrevQuestId`=0 WHERE `Id`=27203;

-- Portal to the Maelstrom
UPDATE `gameobject_template` SET `AIName`='', `ScriptName`='', `data0`=84464 WHERE `entry`=205268;

-- Correct position
DELETE FROM `spell_target_position` WHERE `id`=84464;
INSERT INTO `spell_target_position` 
(`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(84464, 0, 730, 851.307, 1067.76, -10.0183, 4.50656); -- Portal to the Maelstrom

-- Wyvern
UPDATE `creature_template` SET `AIName`='', `ScriptName`='', `npcflag`=16777216, `IconName`='vehichleCursor' WHERE `entry`=45005;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=45005;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(45005, 81780, 1, 0); -- Force Cast Summon Wyvern

-- Extinguishing Hope
DELETE FROM`conditions` WHERE`SourceTypeOrReferenceId`= 13 AND `SourceEntry`=80208;
INSERT INTO `conditions`(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(13, 1, 80208, 0, 0, 31, 0, 3, 42940, 0, 0, 0, '', 'Spray Water requires target Northshire Vineyards Fire Trigger');

-- Northshire Vineyards Fire Trigger
SET @ENTRY:= 42940;
SET @SOURCETYPE:= 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`= @ENTRY AND `source_type`= @SOURCETYPE;
UPDATE `creature_template` SET `AIName`= 'SmartAI' WHERE `entry`= @ENTRY;
INSERT INTO`smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,25,0,100,0,0,0,0,0,22,1,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Northshire Vineyards Fire Trigger - SetEventPhase_1 on Reset"),
(@ENTRY,@SOURCETYPE,1,0,23,1,100,0,80175,0,0,0,11,80175,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Northshire Vineyards Fire Trigger - Cast_Vineyard Fire"),
(@ENTRY,@SOURCETYPE,2,3,8,1,100,0,80208,0,0,0,33,42940,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Northshire Vineyards Fire Trigger - Give Kill Credit on SpellHit_Spray Water"),
(@ENTRY,@SOURCETYPE,3,0,61,1,100,0,0,0,0,0,22,2,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Northshire Vineyards Fire Trigger - SetEventPhase_2"),
(@ENTRY,@SOURCETYPE,4,0,8,2,100,0,80208,0,1000,1000,37,80223,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Northshire Vineyards Fire Trigger - Die on SpellHit_Spray Water");

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (51348, 0, 0, 0, 0, 0, 37310, 37311, 0, 0, 'Stormwind City Guard', '', 'Directions', 435, 85, 85, 3, 11, 11, 1, 3, 1.14286, 1, 0, 530, 713, 0, 827, 15, 2000, 0, 1, 32768, 0, 2048, 0, 0, 0, 0, 0, 530, 713, 827, 7, 2048, 0, 0, 0, 0, 0, 0, 0, 0, 0, 95826, 71507, 96161, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 1.5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 197, 1, 0, 0, 'npc_chevaucheur_de_griffon_de_hurlevent', 13623);

DELETE FROM `creature` WHERE `id`=51348;
INSERT INTO `creature` (`guid`,`id`,`map`,`zone`,`area`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(9545, 51348, 0, 0, 0, 1, 1, 37310, 0, -8910.77, 719.401, 189.385, 5.13402, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(9546, 51348, 0, 0, 0, 1, 1, 37311, 0, -8742.07, 779.179, 142.107, 6.20641, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(9547, 51348, 0, 0, 0, 1, 1, 37310, 0, -8676.09, 726.647, 138.437, 5.41546, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(29491, 51348, 0, 0, 0, 1, 1, 37311, 0, -8900.46, 805.858, 186.161, 4.86605, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(29575, 51348, 0, 0, 0, 1, 1, 37310, 0, -8443.39, 474.887, 203.329, 2.59658, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(29621, 51348, 0, 0, 0, 1, 1, 37311, 0, -8452.52, 1044.45, 115.854, 1.70701, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(29685, 51348, 0, 0, 0, 1, 1, 37310, 0, -8478.62, 831.696, 169.986, 2.05418, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(29808, 51348, 0, 0, 0, 1, 1, 37310, 0, -8213.27, 302.234, 152.003, 0.97455, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30037, 51348, 0, 0, 0, 1, 1, 37310, 0, -8891.52, 864.431, 152.746, 0.301183, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30083, 51348, 0, 0, 0, 1, 1, 37311, 0, -8549.09, 402.625, 159.751, 2.3669, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30176, 51348, 0, 0, 0, 1, 1, 37310, 0, -8885.64, 567.706, 173.358, 2.14195, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30189, 51348, 0, 0, 0, 1, 1, 37311, 0, -8741.49, 604.227, 138.578, 2.22179, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30274, 51348, 0, 0, 0, 1, 1, 37311, 0, -8535.35, 417.141, 159.753, 2.36493, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30293, 51348, 0, 0, 0, 1, 1, 37311, 0, -8740.93, 789.161, 142.111, 6.20993, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30321, 51348, 0, 0, 0, 1, 1, 37311, 0, -8876.44, 808.396, 186.22, 4.83142, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30377, 51348, 0, 0, 0, 1, 1, 37311, 0, -8541.11, 408.939, 159.302, 2.38722, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30435, 51348, 0, 0, 0, 1, 1, 37310, 0, -8864.6, 810.298, 186.22, 4.83141, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30458, 51348, 0, 0, 0, 1, 1, 37310, 0, -3772.62, -4533.36, 11.5873, 5.13411, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30512, 51348, 0, 0, 0, 1, 1, 37311, 0, -8888.79, 729.03, 189.385, 5.13407, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30516, 51348, 0, 0, 0, 1, 1, 37310, 0, -8695.25, 480.635, 136.671, 3.82219, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30521, 51348, 0, 0, 0, 1, 1, 37311, 0, -8888.28, 806.493, 186.22, 4.83146, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30597, 51348, 0, 0, 0, 1, 1, 37310, 0, -8740.58, 799.123, 142.107, 6.20638, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(30604, 51348, 0, 0, 0, 1, 1, 37311, 0, -8390.78, 1183.16, 51.5864, 1.40384, 90, 20, 0, 116235, 0, 1, 0, 0, 0),
(54935, 51348, 0, 0, 0, 1, 1, 0, 0, -8576.25, 423.107, 190.191, 3.10823, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55085, 51348, 0, 0, 0, 1, 1, 0, 0, -8257.02, 431.226, 158.593, 3.96593, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55086, 51348, 0, 0, 0, 1, 1, 0, 0, -8250.15, 423.581, 157.674, 3.99881, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55088, 51348, 0, 0, 0, 1, 1, 0, 0, -8237.75, 373.199, 168.072, 0.76873, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55090, 51348, 0, 0, 0, 1, 1, 0, 0, -8173.84, 371.332, 140.227, 1.11493, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55133, 51348, 0, 0, 0, 1, 1, 0, 0, -8401.41, 586.174, 138.092, 3.63335, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55135, 51348, 0, 0, 0, 1, 1, 0, 0, -8438.06, 629.128, 192.479, 3.8739, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55145, 51348, 0, 0, 0, 1, 1, 0, 0, -8520.15, 623.63, 135.672, 1.7238, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55147, 51348, 0, 0, 0, 1, 1, 0, 0, -8346.49, 674.399, 198.225, 1.12166, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55148, 51348, 0, 0, 0, 1, 1, 0, 0, -8357.31, 679.583, 198.225, 1.12167, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55149, 51348, 0, 0, 0, 1, 1, 0, 0, -8368.13, 684.767, 198.225, 1.12168, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55150, 51348, 0, 0, 0, 1, 1, 0, 0, -8379, 689.8, 198.208, 1.12439, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55151, 51348, 0, 0, 0, 1, 1, 0, 0, -8389.78, 695.135, 198.225, 1.12172, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55152, 51348, 0, 0, 0, 1, 1, 0, 0, -8411.42, 705.503, 198.225, 1.12173, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55153, 51348, 0, 0, 0, 1, 1, 0, 0, -8400.6, 700.319, 198.225, 1.12169, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55160, 51348, 0, 0, 0, 1, 1, 0, 0, -8380.33, 637.741, 141.483, 1.52823, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55161, 51348, 0, 0, 0, 1, 1, 0, 0, -8388.02, 638.091, 141.483, 1.54903, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55171, 51348, 0, 0, 0, 1, 1, 0, 0, -8432.29, 576.596, 141.483, 0.707056, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55172, 51348, 0, 0, 0, 1, 1, 0, 0, -8404.32, 593.787, 137.351, 0.452592, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55204, 51348, 0, 0, 0, 1, 1, 0, 0, -8546.93, 730.49, 157.382, 5.58573, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55212, 51348, 0, 0, 0, 1, 1, 0, 0, -8548.66, 722.425, 148.67, 0.739359, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55213, 51348, 0, 0, 0, 1, 1, 0, 0, -8553.51, 728.471, 148.695, 0.731033, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55214, 51348, 0, 0, 0, 1, 1, 0, 0, -8559.38, 734.297, 148.67, 0.739137, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55215, 51348, 0, 0, 0, 1, 1, 0, 0, -8453.94, 649.443, 148.313, 2.04651, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55216, 51348, 0, 0, 0, 1, 1, 0, 0, -8442.93, 652.649, 148.554, 2.01651, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(55217, 51348, 0, 0, 0, 1, 1, 0, 0, -8436.15, 656.719, 148.314, 2.01559, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(63691, 51348, 0, 0, 0, 1, 1, 0, 0, -8280.34, 1157.21, 73.1883, 0.126612, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(63920, 51348, 0, 0, 0, 1, 1, 0, 0, -8922.52, 602.956, 170.513, 2.21306, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(63921, 51348, 0, 0, 0, 1, 1, 0, 0, -8914.83, 609.602, 170.331, 2.21159, 90, 20, 0, 1, 0, 1, 0, 0, 0),
(63942, 51348, 0, 0, 0, 1, 1, 0, 0, -8825.56, 827.493, 152.746, 4.92918, 90, 20, 0, 1, 0, 1, 0, 0, 0);