# COMP345-F25
This is the repository for the Fall 2025 COMP345 Project. The latest successful builds will always be put onto this repo. Any collaboration is to be made on here.


# 🕹️ COMP 345 — Assignment 1 (Fall 2025)
**Course:** Concordia University – Department of Computer Science and Software Engineering  
**Project:** Advanced Program Design with C++  
**Team Members:**  
- Peter — Part 1: Map  
- Adam — Part 2: Player  
- Abdellah — Part 3: Orders List  
- Carlos — Part 4: Cards  
- Victor — Part 5: Game Engine  

---

## 📖 Overview
This project is the first team assignment for **COMP 345 (Advanced Program Design with C++)**.  
The goal is to implement a modular Warzone-style game engine in C++.  

The assignment is divided into **five parts**, each handled by one team member:
1. **Map** – Graph-based representation of the game map with validation.
2. **Player** – Represents players, their territories, and their decisions.
3. **Orders List** – Abstract base class for orders and an orders list manager.
4. **Cards** – Deck and hand system for Warzone cards.
5. **Game Engine** – State machine managing the flow of the game.

At this stage, each part is implemented separately with its own driver file for demonstration.

---

## 📂 Project Structure
├── src/
│ ├── Map.h / Map.cpp / MapDriver.cpp
│ ├── Player.h / Player.cpp / PlayerDriver.cpp
│ ├── Orders.h / Orders.cpp / OrdersDriver.cpp
│ ├── Cards.h / Cards.cpp / CardsDriver.cpp
│ ├── GameEngine.h / GameEngine.cpp / GameEngineDriver.cpp
│ └── MainDriver.cpp
├── CMakeLists.txt # Build configuration (if using CMake)
└── README.md # Project documentation


---

## 🛠️ Requirements
- All user-defined class members must be **pointers**.
- Each class implements:
  - Copy constructor  
  - Assignment operator  
  - Destructor  
  - Stream insertion operator (`operator<<`)  
- Code must avoid memory leaks.  
- No inline implementations (all functions in `.cpp`).  
- Drivers demonstrate functionality for each part.  

---

## 🚀 Build & Run
### Using CMake
```bash
mkdir build
cd build
cmake ..
make
./Comp345A1
