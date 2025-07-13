# MemoryManagerOOP

This project implements a custom memory management system in C++ using Object-Oriented Programming principles.
It was developed as part of an academic assignment in the Electrical and Electronics Engineering department at Ort Braude College.

## 📚 Project Overview

The system simulates memory allocation using a fixed-size memory pool and supports the following allocation strategies:

- **First Fit**
- **Best Fit**
- **Worst Fit**

Each strategy is implemented in a dedicated class that inherits from a shared abstract base class `MemoryManager`.

## 🧩 Components

- `Block` – Represents a single memory block in the pool.
- `MemoryManager` – Abstract base class for managing the memory pool.
- `FirstFitAllocator` / `BestFitAllocator` / `WorstFitAllocator` – Subclasses implementing allocation algorithms.
- `MemorySimulator` – Contains simulation scenarios to compare the algorithms.
- `Main.cpp` – Contains tests and verification for each class and scenario.

## ⚙️ Build Instructions

To compile the project using g++:

```bash
g++ -std=c++11 Main.cpp Block.cpp MemoryManager.cpp FirstFitAllocator.cpp BestFitAllocator.cpp WorstFitAllocator.cpp MemorySimulator.cpp -o memory_manager
```

To run:

```bash
./memory_manager
```

> Ensure you are in the directory containing all `.cpp` and `.h` files.

## 🧪 Testing

The project includes custom test functions within `Main.cpp` to verify:
- Correct block management (allocation, splitting, merging)
- Strategy-specific behavior
- Edge cases and exception handling
- Simulation performance and peak usage

## 🖼️ Simulation Output

Here is a sample from the simulator run (First Fit, Best Fit, Worst Fit):

### ✅ Part 1: First Fit & Best Fit Scenarios

![Simulation 1](./סימולציה%201.png)

### ✅ Part 2: Worst Fit & Summary

![Simulation 2](./סימולציה%202.png)


## 👤 Authors

- Adi Levi – [LinkedIn](https://www.linkedin.com/in/adi-levi-5008858269/)
- Aviv Friedman - [LinkedIn](https://www.linkedin.com/in/aviv-friedman-38719426b/)

## 📄 License

This academic project is shared for learning purposes.
