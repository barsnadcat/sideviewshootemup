# HMS "Sunflower" project.
Physics based arcade ship builder.

## Question:
What is the optimal way to replicate the behavior of Highfleet ships in Unreal engine?

Requirements:
* Ship is collections of parts that rigidly joined together affected by gravity and moves using thrusters
* Each part takes damage individually, can be destroyed so ship can be split
* Guns can be placed inside ship -- projectiles they spawn do not collide with ship itself, only with enemy ship, other projectiles
* Player character can run inside the ship, and is affected by ship movement

## Constraints approach
Ship parts connected together using FConstraintInstance.

Nice:
* Very simple code
* We can add snapping and bending as features

Bad:
* Physic simulation take a lot of CPU - this will no run on mobile for sure

Really bad:
* Ship is not rigid


### More constraints approach
Add long distance constraints.

Nice:
* We can add snapping and bending as features

Not great:
* Messy code to split ship
* Ship is sort of rigid but no 100%

Bad:
* Even worse performance

## Welding approach
Ship parts connected together using AttachToComponent with bWeldSimulatedBodies = true

Really nice:
* Very cheap simulation - should run on mobile

Nice:
* Ship is 100% rigid

Bad:
* Attachment is directed graph, wile ship is undirected - so additional layer of conde is needed

Really bad:
* Unwelding parts generates spikes up to 200ms on big ships

### Batch unweld chaos mod
Add Batch unweld to MeshComponent, FBodyInstance and chaos.

Not tested yet, performance fix is not given.

Bad:
* Extra maintenance cost associated with engine modification.

## Lego fortnite approach
Use FClusterUnion and FGeometryCollection.

- FClusterUnionManager
  - FClusterUnionPhysicsProxy
    - UClusterUnionComponent (5.3)
      - AClusterUnionActor 
      - ChaosModularVehicle (5.4)
        
