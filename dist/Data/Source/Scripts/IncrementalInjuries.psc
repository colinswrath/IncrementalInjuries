Scriptname IncrementalInjuries Hidden

; NOTE: You cannot actually access these variables from your scripts.
; Copy the definitions to your own scripts if you want to use them.
int Property ACTOR_VALUE_HEALTH = 1 AutoReadOnly
int Property ACTOR_VALUE_STAMINA = 1 AutoReadOnly
int Property ACTOR_VALUE_MAGICKA = 1 AutoReadOnly

;Send one of the actor value properties above for the ActorValue

;Modifies an actor value mult by the specified amount
;ie. If you want to increase the mult by 10% , send "0.10". Decreasing works the same, but send a negative.
;I highly suggest using this over the set functions.
Function ModActorValueMult(int ActorValue, float modValue) global native     

;Restores all damage done by the mod to the specified actor value
Function RestoreActorValueDamage(int ActorValue) global native

;Set the specified mult for the appropriate AV.
;If you want to change the mult, I strongly suggest using ModActorValueMult instead, to make it more compatible with other mods
;Valid values are 0-5.0
;--------------------------------------------------------
Function SetHealthMult(float setVal) global native
Function SetStaminaMult(float setVal) global native
Function SetMagickaMult(float setVal) global native
;--------------------------------------------------------

;Same as the above, but for the limit
;Valid values are 0-1.0
;--------------------------------------------------------
Function SetHealthLimit(float setVal) global native
Function SetStaminaLimit(float setVal) global native
Function SetMagickaLimit(float setVal) global native
;--------------------------------------------------------

;Get the mult for the specified AV.
float Function GetHealthMult() global native
float Function GetStaminaMult() global native
float Function GetMagickaMult() global native

;Get the limit for the specified AV
float Function GetHealthLimit() global native
float Function GetStaminaLimit() global native
float Function GetMagickaLimit() global native




