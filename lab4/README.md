# ECE 477 Lab 4
## Part A
Write a program that takes inputs from two pushbutton switches (A and B). A single LED will be lit and will rotate across the row of LEDs from the LS position to the MS position (after leaving MS, the position shifts into the LS position.
Initially, the LED should move once every 1024ms. If switch A is pressed, then the delay between moves
should be cut in half. If button A is pressed when the delay is 32 ms, the delay should stay at 32 ms, but the direction
the of the LED should reverse. If button B is pushed, the delay should be doubled. If the delay is at 1024ms and button B is pressed, then the direction of the LED should reverse.
## Part B
A simple, but boring, pong game using the two switches and the row of LEDs.
- When the LED is at the rightmost position and button A is pressed, the LED will start moving left.
- When the LED is at the leftmost position and button B is pressed, the LED will start moving right.
- When a button is pressed when the LED is moving left or right and is not at the edge, the player who did not press the button gets a point.
- When the game starts, the LED does not move until button A is pressed, which will start the game.
- Whenever a point is scored, the LED will be at either the leftmost or rightmost edge (depending on who scored). And will not move until the button corresponding to that edge is pressed. (Call this serve)
- If the LED is not moving, and the player who is not the server presses the button, nothing happens.

If Ctrl+C (SIGINT) is sent to the program, the game will end and the final score will be printed.
