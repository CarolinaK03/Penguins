This program is a grid logic-based game, where the players are meant to move their penuins on a board (vertically and horizontally), which specific constraints, and attempt to collect as many fish
as possible while avoiding trapping their penguins and getting eliminated. 
The size of the grid is generated based on how many players (2-8) are playing, as well as how many penguins they would like to have each. 
Tiles are randomly assigned ice (1+), or water(0).
Every player begins by placing their penguin on the grid, with the contraints that they are not allowed to sit on the same ice tile as another penguin, and they are not allowed to collect more than 1 fish on the first round.
Once all penguins are placed, players can move their penguins (once per round), either horizontally or vertically on the grid. They are not allowed to jump over another penguin, or water tile.
If a penguin is trapped with null tiles on each side, they can no longer move.
The turns conitnue untill no one can move, and the player with the greatest amount of fish is the winner.
