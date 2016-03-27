-- Winter Hyacinth (item 45000) Send Script Event (20990) by right-click
DELETE FROM `event_scripts` WHERE `id`=20990;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(20990, 0, 10, 33273, 42000, 4602.977, 0, -1600.141, 156.7834, 0.7504916);

REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`unkInt32`, `AIName`, `ScriptName`) VALUES (206195, 22, 7490, 'Thundermar Ale Keg', '', '', '', 0, 0, 0.85, 0, 0, 0, 0, 0, 0, 86855, 0, 0, 0, 0, 10736, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '');

-- Громораан Искатель Ветра / http://ru.wowhead.com/quest=7786
-- loot
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-4 WHERE `entry`=12056 AND `item`=18563;
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-4 WHERE `entry`=12057 AND `item`=18564;
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=-100 WHERE `entry`=11502 AND `item`=19017;
UPDATE `creature_loot_template` SET `ChanceOrQuestChance`=100 WHERE  `entry`=14435 AND `item`=19018;
-- boss
DELETE FROM `creature` WHERE `id`=14435;
INSERT INTO `creature` (`guid`,`id`,`map`,`zone`,`area`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(999492, 14435, 1, 0, 0, 1, 2, 0, 0, -6237.06, 1718.68, 4.30388, 0.758073, 300, 0, 0, 333100, 0, 0, 0, 0, 0);

 REPLACE INTO `phase_definitions` (`zoneId`, `entry`, `phasemask`, `phaseId`, `originmap`, `terrainswapmap`, `flags`, `comment`) values('1377','1','3','0','0','0','0','Silithus - Quest Boss Phase');

REPLACE INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES 
(23, 1377, 1, 0, 0, 8, 0, 7786, 0, 0, 0, 0, 0, '', 'Phase Condition: Silithus - Quest Boss Phase'),
(23, 1377, 1, 0, 1, 14, 0, 7787, 0, 0, 1, 0, 0, '', 'Phase Condition: Silithus - Quest Boss Phase');