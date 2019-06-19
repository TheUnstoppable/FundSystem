# FundSystem
A building fund system plugin for C&amp;C Renegade. Made by The Unstoppable.
The configuration INI below is needed to make plugin work.

1.0 - First Release
1.1 - Fixed ConstantFundAmount

CONFIGURATION:

[Fund]
;Plugin made by MasterCan.
;This settings can be edited from game mode ini files as well.
;By default, these values will be used.


;Enable or disable funding system.
EnableFund=1

;Decide if the fund amounts of buildings are constant or increases when a player joins.
ConstantFundAmount=0

;If your constant fund amount is 0, you can determine how much price will be added when a player joins to fund cost.
;Max = 1.0, Min = 0.1, else multiplier will be set to 1.
FundAddMultiplier=1.0

; ---------------------------------- Building Fund Ticks ----------------------------------
;If ConstantFundAmount is 1, specified amounts will be constant fund aount of buildings.
;If ConstantFundAmount is 0, building fund cost will be increased by specified amounts.
;
;Leaving fund enabled for a building and setting tick to 0 will cause plugin to misbehave.
; -----------------------------------------------------------------------------------------
Tick_PowerPlant=450
Tick_Refinery=500
Tick_VehicleFactory=600
Tick_SoldierFactory=550
Tick_Defense=575
Tick_ConYard=300
Tick_ComCenter=325
Tick_TibSilo=350
Tick_RepairPad=200
Tick_Shrine=175

; -------------------------------- Building Fund Allowance --------------------------------
; Specify the building funds you want to disable.
; Use 1, for enable funding. Use 0 for disabling.
;
; Using values except 1 and 0 automatically disables funding for that building.
; -----------------------------------------------------------------------------------------
Fund_PowerPlant=1
Fund_Refinery=1
Fund_VehicleFactory=1
Fund_SoldierFactory=1
Fund_Defense=1
Fund_ConYard=1
Fund_ComCenter=1
Fund_TibSilo=1
Fund_RepairPad=1
Fund_Shrine=1
