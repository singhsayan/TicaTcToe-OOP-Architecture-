# Tic-Tac-Toe — Architecture & Design Documentation

## Table of Contents
1. [Overview](#overview)  
2. [System Features](#system-features)  
3. [Class Diagram](#class-diagram)  
4. [Design Patterns Used](#design-patterns-used)  
5. [Component-Level Details](#component-level-details)  
6. [Workflow](#workflow)  
7. [Game Output](#game-output)  

---

## Overview
This project implements a **modular, object-oriented Tic-Tac-Toe game** using C++.  
The design emphasizes **extendability, maintainability, and clean separation of concerns** through classic design patterns like *Observer*, *Factory*, and *Strategy (via Rules abstraction)*.

The game supports:
- Dynamic board sizes  
- Multiple players  
- Custom rules (via `Rules` base class)  
- Observer-based notifications (e.g., console logging)  

---

## System Features
### Dynamic Game Size  
Players can choose any `n x n` board.

### Observer-Based Notifications  
Every action (move played, win, draw) notifies observers such as:
- Console loggers  
- (Extendable: GUI loggers, File loggers, Remote loggers)

### Rule Abstraction  
Rules are encapsulated inside `Rules` and `StandardRules`, making it easy to support custom variants.

### Factory-Based Game Creation  
Centralized creation of game objects via `GameFactory`.

### Player & Symbol Abstraction  
Players have:
- Name  
- Score  
- Symbol (X, O, etc.)

---

## Class Diagram
*(Reference to the diagram provided in the prompt — not embedded here)*

Key classes demonstrated:
- **Game**, **Board**, **Player**, **Symbol**, **Rules**, **StandardRules**  
- **IObserver**, **ConsoleNotifier**  
- **GameFactory**

Relationships:
- Game → Board (composition)  
- Game → Player (deque)  
- Game → Rules  
- Game → Observer list  
- Player → Symbol  
- Board → Symbol grid  

---

## Design Patterns Used

### **Observer Pattern**
Used for broadcasting state changes (moves, win announcements, draws).  
- **Subject:** `Game`  
- **Observers:** Implementations of `IObserver` (e.g., `ConsoleNotifier`)

### **Factory Method Pattern**
Game creation is abstracted inside `GameFactory`.

### **Strategy Pattern (Rules Engine)**
The `Rules` interface allows plugging different win/draw logic.

### **Encapsulation & Abstraction**
Classes like `Board`, `Player`, `Symbol` hide internal details.

---

## Component-Level Details

### **1. IObserver & ConsoleNotifier**
Responsible for reacting to game events.

```cpp
class IObserver {
public:
    virtual void update(string message) = 0;
};
```
### **2. Symbol**

 - Represents a player’s assigned marker (X, O, etc.).

### **3. Board**

Holds:

 - Grid of Symbol*

 - Validates moves

 - Displays board

### **4. Player**

## Stores:

 - ID

 - Name

 - Symbol

 - Score

### **5. Rules & StandardRules**

## Implements:

 - Valid move check

 - Winner logic (rows, columns, diagonals)

 - Draw logic

### **6. Game**

## Coordinates:

 - Players

 - Board

 - Rules engine

 - Observer notifications

 - Turn rotation

### **7. GameFactory**

Creates game objects based on type.

## Workflow

 - User selects board size

 - GameFactory builds a new game

 - Players join

 - Observers are attached

 - Game loop begins:

 - Current player inputs a move

 - Move validated & placed

 - Check for win/draw

 - Observers notified

 - Turn rotates

## Output
