# About

These are code snippets from Chroma, a third-person rogue-lite arena shooter built in
UE 5.3 and shipped on Steam. I was the gameplay engineer with ownership over the
Gameplay Ability System layer which backs every ability, attribute, and effect in the
game.

The C++ here is the framework the game is built on top of; the abilities, effects,
and enemies themselves are authored as Blueprint assets.

# Details

**Effect pipeline** (`EffectPipelineComponent`, `CalculateImpactCrit`) - Upgrades are
stored against a pipeline tag when the player picks them up, so gameplay asks the
pipeline to run a stage instead of every upgrade needing its own execution
calculation. `CalculateImpactCrit` is one such stage, applying the crit multiplier to
base impact damage before later upgrades stack on top of it.

**Blaster and backpack** (`PUBlaster`, `Backpack`) - The player's weapon and the
battery magazine feeding it. The backpack tracks the current battery, rechambers to
the next one when it discharges, and reloads once the last is spent. Both split into
an `_Exec` method that decides what happens and a `_BP` event that plays the
animation,A collection of example CPP work on Chroma, a 3rd-person Risk of Rain style shooter built in UE5.3
