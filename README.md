# Info
You can change the color of the tiles at the "Node.h" file, in the "colors" struct.
The colors, of type "Uint8", can be in the ranges 0-255 and 0x00-0xFF.

For each Uint8 array, the colors are, respectively: Red, Green, Blue, Alpha.

**This is a work-in-progress project. There are going to be many changes in the upcoming week.**<br>
**A Star algorithm not yet implemented, as I'm working my way up from other algorithms.**<br>
**The latest implemented algorithm was Dijkstra's.**<br>
**There's a lot of redundant code, performance issues, lack of comments, documentation, and more. Those are intended to be fixed once I'm satisfied with the core functionalities.**
# Usage

There are 5 types of tiles:

1. "Passable" - Algorithm goes through to find the path.
2. "Impassable" - Algorithm ignores these tiles.
3. "Start" - From where the algorithm starts off to find the path. If one has already been set as "start", it will be replaced with the new one.
4. "Target" - Target for the path to be found. Can only be one tile. If one has already been set as "target", it will be replaced with the new one.
5. "Visited" - Tiles that the algorithm has already visited.

To start finding a path, you should press the "S" key, to pick a starting tile. Once that's done, do the same with the "T" key, to pick the target tile, which is the tile you want to "go" to.<br>
You can left-click "passable" tiles to make them "impassable".<br>
Once you're happy with how the tiles are setup, you can press "P" to start the path finding.

The default algorithm does not account for tile costs.<br> 
If you want to, you can press the "C" key, to get into the "cost" mode, which currently uses Dijkstra's algorithm.<br>

Once the path has been found, you can make changes and restart the path finding by pressing "P" again. If you want to get rid of the "visited" purple tiles in order to make changes, you can press the "K" key, for better visualization.<br>

If you want to reset all tiles to "passable" green tiles, you can press the "R" key.<br>

## Colors

| Tile | Color |
|------|-------|
| Passable | Green (0, 255, 0) |
| Impassable | Red (255, 0, 0) |
| Start | Yellow (255, 255, 0) |
| Target | Blue (0, 255, 255) |
| Visited | Purple (125, 5, 88) |

## Keymap
| Key | Description |
|-----|-------------|
| S | Selection mode for starting point. |
| T | Selection mode for target point. |
| P | Start/pause/resume the path finding. |
| R | Resets all tiles. |
| C | Changes the algorithm to use tile costs. You can increase a tile's cost by right clicking it. Once the cost is 5, it becomes "impassable". |
| K | After the path was found, pressing this key converts the tiles in the path from "visited" to "passable". Used for better visualization. |
| V | Toggles view-only mode. |

# Compilation
Compiled in Visual Studio Community 2019, in C++. Using SDL2 and SDL_ttf as external libraries.