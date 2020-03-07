# Info
Work in progress.
<br>
Many updates will occur in the upcoming week.
<br>
More details soon.

**This code will be refactored and modified in many ways. The AStar algorithm has not yet been implemented, as I'm working my way up from Breadth First Search. Comments are going to be added once everything is working properly. Many changes will occur in the upcoming week.**
# Usage

P = Start/Pause/Resume the path finding.
<br>
S = Enables the selection of the starting tile. When toggled on, the tile you select will be set as the starting point. (Only one tile can be the starting point. If selecting another tile while a start point was already set, the previous starting point will be replaced with the new one.)
<br>
T = Same thing as the "S" key, except it selects the target point.
<br>
R = Resets the tiles.
<br>
V = Toggles view-only mode, where you can't modify the grid/tiles. Enabled by default when starting the path finding (by pressing "P"). Disabled once you reset the grid (by pressing "R"). You can manually toggle it by pressing "V".

- Green tiles are "passable", which means the algorithm will look for those when finding the path.
<br>
- Red tiles are "impassable", which means the algorithm will ignore those when finding the path.
<br>
- Yellow tile is the starting point, from where the algorithm will start to look for the path.
<br>
- Blue tile is the target point, the goal, which the algorithm will find the path for.
<br>
- Purple tile means that the tile is a possible path and/or has been visited by the algorithm.
<br>

If you want, you can change the colors in the "Node.h" file. The tile colors are stored in the following variables, respectively: "passable", "impassable", "start", "target", "visited".
<br>
<br>
Each of the values in the "Uint8" array is in the range of 0-255 or 0x00-0xFF. They are, respectively, Red, Green, Blue and Alpha values.

# Compilation

Uses C++ and SDL2.
Compiled in Visual Studio Community 2019, targeted for x86. Release version not yet tested.
