# Program to track the execution of space invaders given assembly tags

tags = {
    0x0000:"Reset",
    0x0008:"ScanLine96",
    0x0010:"ScanLine224",
    0x00B1:"InitRack",
    0x0100:"DrawAlien",
    0x0141:"CursorNextAlien",
    0x017A:"GetAlienCoords",
    0x01A1:"MoveRefAlien",
    0x01C0:"InitAliens",
    0x01CD:"ReturnTwo",
    0x01CF:"DrawBottomLine",
    0x01D9:"AddDelta",
    0x01E4:"CopyRAMMirror",
    0x01EF:"DrawShieldPl1",
    0x01F5:"DrawShieldPl2",
    0x0209:"RememberShields1",
    0x020E:"RememberShields2",
    0x0213:"RestoreShields2",
    0x021A:"RestoreShields1",
    0x021E:"CopyShields",
    0x0248:"RunGameObjs",
    0x028E:"GameObj0",
    0x0381:"MovePlayerRight",
    0x038E:"MovePlayerLeft",
    0x039B:"DrawPlayerDie",
    0x03BB:"GameObj1",
    0x03FA:"InitPlyShot",
    0x040A:"MovePlyShot",
    0x0426:"MY_CollisionDetected",
    0x0430:"ReadPlyShot",
    0x0436:"EndOfBlowup",
    0x0476:"GameObj2",
    0x04AB:"ResetShot",
    0x04B6:"GameObj3",
    0x0550:"ToShotStruct",
    0x055B:"FromShotStruct",
    0x0563:"HandleAlienShot",
    0x062F:"FindInColumn",
    0x0644:"ShotBlowingUp",
    0x0682:"GameObj4",
    0x0765:"WaitForStart",
    0x0798:"NewGame",
    0x081F:"GameLoop",
    0x0886:"GetAlRefPtr",
    0x088D:"PromptPlayer",
    0x08D1:"GetShipsPerCred",
    0x08D8:"SpeedShots",
    #0x08F3:"PrintMessage",
    #0x08FF:"DrawChar",
    0x0913:"TimeToSaucer",
    0x097C:"AlienScoreValue",
    0x0988:"AdjustScore",
    0x09AD:"Print4Digits",
    0x09B2:"DrawHexByte",
    0x09D6:"ClearPlayField",
    0x0A5F:"ScoreForAlien",
    0x0A80:"Animate",
    0x0A93:"PrintMessageDel",
    0x0AAB:"SplashSquiggly",
    0x0AB1:"OneSecDelay",
    0x0AB6:"TwoSecDelay",
    0x0ABB:"SplashDemo",
    #0x0ABF:"ISRSplTasks",
    0x0AD7:"WaitOnDelay",
    0x0AE2:"IniSplashAni",
    0x1400:"DrawShiftedSprite",
    0x1424:"EraseSimpleSprite",
    #0x1439:"DrawSimpSprite",
    0x1452:"EraseShifted",
    0x1474:"CnvtPixNumber",
    0x147C:"RememberShields",
    0x1491:"DrawSprCollision",
    0x14CB:"ClearSmallSprite",
    0x14D8:"PlayerShotHit",
    0x1504:"CodeBug1",
    0x1538:"AExplodeTime",
    0x1554:"Cnt16s",
    0x1562:"FindRow",
    0x156F:"FindColumn",
    0x1581:"GetAlienStatPtr",
    0x1590:"WrapRef",
    0x1597:"RackBump",
    0x15D3:"DrawSprite",
    0x15F3:"CountAliens",
    0x1611:"GetPlayerDataPtr",
    0x1618:"PlrFireOrDemo",
    0x170E:"AShotReloadRate",
    0x172C:"ShotSound",
    0x1740:"TimeFleetSound",
    0x1775:"FleetDelayExShip",
    0x17B4:"SndOffExtPly",
    0x17C0:"ReadInputs",
    #0x17CD:"CheckHandleTilt",
    0x1804:"CtrlSaucerSound",
    0x1815:"DrawAdvTable",
    0x1856:"ReadPriStruct",
    0x1868:"SplashSprite",
    0x18D4:"init",
    0x18FA:"SoundBits3On",
    0x1904:"InitAliensP2",
    0x190A:"PlyrShotAndBump",
    0x1910:"CurPlyAlive",
    0x191A:"DrawScoreHead",
    0x1931:"DrawScore",
    0x1947:"DrawNumCredits",
    0x1950:"PrintHiScore",
    0x1956:"DrawStatus",
    0x199A:"CheckHiddenMes",
    0x19BE:"MessageTaito",
    0x19D1:"EnableGameTasks",
    0x19D7:"DsableGameTasks",
    0x19DC:"SoundBits3Off",
    0x19E6:"DrawNumShips",
    0x1A06:"CompYToBeam",
    #0x1A32:"BlockCopy",
    0x1A3B:"ReadDesc",
    0x1A47:"ConvToScr",
    0x1A5C:"ClearScreen",
    0x1A69:"RestoreShields",
    0x1A7F:"RemoveShip",
}

with open('log.txt', 'r') as f:
    Lines = f.readlines()
    for line in Lines:
        number = int(line, 16)
        value = tags.get(number)
        if value:
            print(hex(number), value)









