# Ricochet

Top down 2d game
player character has a knife they can throw that ricochets
- initially one bounce, then midway two
- throwing is controlled by mouse, movement by WASD + space

simple dodge mechanic

# Ideas

- Raycast drawing of ricochet bullet
- (advanced) light system with dynamic light mapping 
- futuristic style, ricochet is a laser that bounces

# To do

- [x] Simple movement
- [x] Basic walls
- [x] Basic ricochet
- [x] Basic enemy that stands still
- [x] Basic enemy that dies when hit
- [ ] Basic enemy that moves in a straight line

# Enemies

- Robots that move fast with a small field cone of view
  - Takes two hits to kill
  - Player shoots them once, and then they are stunned
  - No special pattern, just slightly faster than the player
- Flying drones
  - Long, narrow cone of view
  - Moderate speed, also takes two hits to kill
  - Approach the player, then do a charge attack
