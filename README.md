# My First 2D Platformer Game

https://github.com/adelayari/My-first-2D-platformer-game/assets/69399646/e9d469fa-d00e-4a90-955b-347368220e96

## Description

Welcome to "My First 2D Platformer Game," a simple game created using C/C++ and the [raylib](https://www.raylib.com/) library. Dive into the world of game development and learn while having fun.

## How to Play

- Use the arrow keys to control the character and make it jump.
- Avoid collisions with the moving obstacles.
- If you collide with an obstacle, the game is over.
- To restart the game, press the "R" key.
- Challenge yourself to reach the finish line and win the game.

## Author

**© 2023 AYARI Adel**
- LinkedIn: [AYARI Adel](https://www.linkedin.com/in/ayari-adel-esim/)

## License

This game is not licensed and is available for educational purposes.

## How to Use

To compile and run "My First Game," follow these steps:

1. Open your terminal or command prompt.

2. Navigate to the project directory.

3. Run the following command to build the game:

```bash
g++ src/main.cc -o main.exe -O2 -Wall -Wno-enum-compare -Wno-missing-braces -I./include -I./include/raylib-cpp -L./lib -lraylib -mwindows -lopengl32 -lgdi32 -lwinmm -mno-sse
