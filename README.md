# Warzone Risk Game — COMP 345 (Fall 2025)

**Course:** Advanced Program Design with C++  
**University:** Concordia University – Department of Computer Science and Software Engineering  
**Language:** C++17  
**Team Members:**  
- Peter —  
- Adam — 40237870 
- Abdellah — 
- Carlos —   
- Victor — 40284702  

---

## Project Overview
This repository contains our **semester-long team project** for COMP 345.  
The project is divided into **three assignments**, each building on the previous one, to gradually develop a playable **Risk-style game engine** based on the rules of [Warzone](https://www.warzone.com/).

- **Assignment 1:** Core architecture (Map, Player, Orders, Cards, Game Engine).  
- **Assignment 2:** Reinforcement rules, battle simulation, order execution logic.  
- **Assignment 3:** Full game flow with advanced features and final polish.  

The end goal is a complete **command-line playable Warzone game**.

---

## Game Description
- The **map** is a connected graph: nodes = territories, edges = adjacency.  
- **Continents** are connected subgraphs with control bonuses.  
- **Players** own territories and armies, issue orders, and attempt to conquer the map.  
- The **main loop** follows Warzone’s phases:  
  1. **Startup** → assign players & territories.  
  2. **Reinforcement** → armies allocated.  
  3. **Orders Issuing** → players declare moves.  
  4. **Orders Execution** → battles resolved, territories conquered.  
  5. **End** → winner declared.  

---

## Repository Structure
├── src/
│ ├── Map.h / Map.cpp / MapDriver.cpp
│ ├── Player.h / Player.cpp / PlayerDriver.cpp
│ ├── Orders.h / Orders.cpp / OrdersDriver.cpp
│ ├── Cards.h / Cards.cpp / CardsDriver.cpp
│ ├── GameEngine.h / GameEngine.cpp / GameEngineDriver.cpp
│ └── MainDriver.cpp
├── CMakeLists.txt
└── README.md


- Each **Assignment Part** has its own `.h/.cpp` duo + a **driver file** for testing.  
- `MainDriver.cpp` integrates everything and demonstrates the full assignment.  

---

## 📅 Progress
- **Assignment 1** — *In Progress*  
  - Implementing core classes & drivers.  
- **Assignment 2** — *Not Started*  
  - Will extend A1 with reinforcement & battle logic.  
- **Assignment 3** — *Not Started*  
  - Will finalize game engine & advanced features.  

