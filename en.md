# Othello
This is something I made in the early days of learning C++.

[English](/en.md)

# Overview

## Display

2 color matrix LEDs are dynamically controlled for display.

## Base

When a piece is placed, it searches 8 directions (up, right, up, right...) from that square and reverses the color. When a piece is placed, the color of the square is reversed.

When a piece is placed in a square, the color of the square is reversed,
After a piece is placed, all squares (8*8) are searched to see if it can be placed.
If a piece can be placed in any square, the game continues. If not, the game skips or ends.

## Search method

To search upward with white

->If so, continue up one more square, otherwise search in the next direction.

->If there is a black square above the white square, then the white square is reversed.

## Computer mode

In computer mode, each square has its own priority and the pieces are placed in randomly selected squares if they have the same priority.

Priority (the higher the number, the higher the priority)

|   |   |   |   |   |   |   |   |
| - | - | - | - | - | - | - | - |
| 9 | 2 | 5 | 6 | 6 | 5 | 2 | 9 |
| 2 | 1 | 3 | 4 | 4 | 3 | 1 | 2 |
| 5 | 3 | 8 | 7 | 7 | 8 | 3 | 5 |
| 6 | 4 | 7 | 0 | 0 | 7 | 4 | 6 |
| 6 | 4 | 7 | 0 | 0 | 7 | 4 | 6 |
| 5 | 3 | 8 | 7 | 7 | 8 | 3 | 5 |
| 2 | 1 | 3 | 4 | 4 | 3 | 1 | 2 |
| 9 | 2 | 5 | 6 | 6 | 5 | 2 | 9 |

In the beginning of the game, I try to give priority to the center 4×4 to prevent the 4 corners from being taken.

In the middle game we try to avoid squares near the corners to prevent the corners from being taken.

I don't know about the endgame. It's luck. I'm going for the corner.

## Else

The game is too small and dirty for the next version

Translated by DeepL
