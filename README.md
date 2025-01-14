# C++ CLI Chess Game

Welcome to my C++ Command-Line Interface (CLI) Chess project! This project is the culmination of what I have learned so far as a Computer Science student. I knew going into the project that this would be challenging and that it would certainly not be perfect, but I am somewhat happy with what I was able to achieve.

---

## Table of Contents
1. [Overview](#overview)
2. [Reflection](#what-i-learned)
3. [Future Improvements](#future-improvements)
4. [Closing Remarks](#closing-remarks)

---

## Overview

The main goal of this project was to implement a fully functional chess game within a CLI environment. The game includes several key features:
- **Player vs AI:** Using the Minimax algorithm optimized with Alpha-Beta pruning, the AI can make determine moves.
- **Player vs Player:** Two players can compete against each other in a traditional chess match.
- **Piece Movement and Move Validation:** The program ensures that only legal moves are allowed based on chess rules.
- **Draw and Checkmate Detection:** Basic rules for detecting draws and checkmates are implemented.
- **Full Chessboard Display:** A text-based board that clearly displays the state of the game.

While the project is far from perfect, it has been an excellent opportunity to apply and expand my knowledge of game development, C++, and data structures.

---

## Reflection

1. **Planning:**
   Reflecting on the challenges I faced during this project, I realize that many of the issues stemmed from poor initial planning. I made several changes to my original plan throughout the project, which led to messy code and inefficiencies in my approach. In hindsight, I learned that thorough planning is crucial for the success of a project. If I were to approach a similar project in the future, I would dedicate more time to outlining what the program needs to do, how I plan to achieve it, and the structure of the code before diving into implementation.
3. **Codebase Orgnization:**
   As mentioned earlier, my lack of careful planning made the organization of my codebase more difficult than it needed to be. Throughout the project, I had to continuously refactor and reorganize classes, sometimes changing their responsibilities entirely. This constant restructuring, combined with the growing complexity of the code, made it harder to stay motivated and focused on the project. I often found myself caught in larger problems that I had not anticipated, which delayed progress on smaller tasks. In the future, I will prioritize better planning and clearer organization from the start, which should prevent these issues and make the process smoother.
4. **The Engine:**
   Unfortunately, the engine is far from perfect. While I was able to create a basic game where moves can be played, the AI is far from competitive and prone to crashing. I am proud of my implementation of the Minimax algorithm, Alpha-Beta pruning, and the generation of the game tree, but the engine’s performance is not as good as I had hoped. This is largely a result of my poor planning in the early stages, where I did not properly consider the need for efficient code to support the AI. By the time I reached the engine’s development, I was facing burnout and didn’t have the energy to go back and redo the inefficient parts of the code. This experience has taught me the importance of optimizing code early on, especially when working with algorithms that require high performance.
---

## Future Improvements

1. **Bitboard and Zobrist Hashing:**
   - One major improvement I would like to make is changing the way the board is represented. Instead of using a traditional 8x8 array of pointers, I would use a bitboard representation. This would involve using bit manipulation to store the state of the board in a much more compact and efficient way.
   - Additionally, I would like to implement Zobrist Hashing, which would allow for the optimal storage of information for each piece on the board. This would not only improve performance, but also make it easier to store and manipulate data, such as tracking past moves, available moves for each piece, and comparing different board states efficiently.

2. **Polishing the UI:**
   - Initially, I considered implementing a graphical user interface (GUI) using a C++ GUI library, but I ultimately decided against it, as it would detract from the focus of the project. However, I do recognize the value of learning how to build a GUI, and I believe adding one in the future would be an interesting challenge. While not the core focus, a GUI could make the game more accessible and visually engaging for users.
     
4. **Rule Completeness:**
   - Although I implemented complex moves such as en passant and castling, there are still some niche rules that I missed, including the threefold repetition rule and the fifty-move rule. Adding these advanced rules would improve the overall game experience and ensure the program adheres to the full set of chess rules.
   - I would also like to add more detailed detection for different types of draws and refine how the game handles endgame scenarios.

5. **Refactoring and Optimization:**
   - A significant part of the future work will involve refactoring the code for better performance and readability. Many areas of the current code are inefficient and could benefit from simplification.
   - I also plan to implement comprehensive unit tests to ensure that the game works as expected under various scenarios, which will help catch bugs early and make the code more robust.
   - Ultimately, I believe many of the improvements I plan to make come down to better planning in the initial stages of development. Having a clear plan for how to structure the code, optimize key parts, and test thoroughly will make the future development process more efficient and enjoyable.

---

## Closing Remarks

While I have decided to stop working on this project for now, I view it as a stepping stone rather than a finished product. I am proud of the effort I put into this project, and although the final result isn’t perfect, it has been a valuable learning experience. I know there are many areas where I can improve, and I am confident that with more knowledge and experience, I could revisit this project in the future and rewrite or enhance the code.

This project has highlighted both my strengths and weaknesses, and it has given me a deeper understanding of what it takes to bring a complex idea like a chess game to life. I’m excited to continue learning, refining my skills, and applying them to new challenges. I look forward to the possibility of returning to this project later on, armed with a stronger foundation and a clearer vision of how to take it to the next level.
