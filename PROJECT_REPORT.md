# E-Waste Management & Recycling System

## An Object-Oriented Approach to Sustainable E-Waste Management

**Student:** Kaushik B. Topale  
**Course:** Diploma Electronics & Computer Engineering (ECE)  
**Subject:** Object-Oriented Programming in C++  
**Language:** C++17  
**Application Type:** Offline console-based application  

---

## 1. Abstract

The E-Waste Management & Recycling System is a C++17 console application designed to manage the complete lifecycle of electronic waste. The system allows users to register electronic waste, classify it, assess its condition, recommend an appropriate action, calculate reward points, schedule collection, update collection status, search records, generate reports, and store data using files.

The project demonstrates important Object-Oriented Programming concepts including encapsulation, abstraction, inheritance, runtime polymorphism, composition, constructors, destructors, operator overloading, exception handling, and STL containers.

The application works completely offline and uses only the C++ standard library. No database, external library, internet connection, or GUI is required.

---

## 2. Introduction

Electronic waste includes unwanted or discarded electrical and electronic equipment such as mobile phones, laptops, batteries, printers, and televisions. Improper disposal of e-waste can cause environmental pollution and health risks because many devices contain hazardous materials.

This project provides a structured system for recording e-waste and managing its journey from registration to recycling. It also encourages responsible disposal by assigning reward points to registered items.

---

## 3. Problem Statement

Manual e-waste handling can cause the following problems:

- Difficulty maintaining item records
- No systematic classification of e-waste
- Lack of condition-based recommendations
- Poor collection tracking
- No reward calculation
- Difficulty preparing statistics and reports
- Loss of records after closing the program
- Lack of hazardous-item identification

The proposed system solves these problems through a modular, object-oriented, file-based application.

---

## 4. Objectives

The main objectives of the project are:

1. Register different types of electronic waste.
2. Generate a unique ID for every item.
3. Classify items automatically according to their type.
4. Store owner, item, condition, and weight details.
5. Recommend reuse, repair, refurbishment, recovery, or recycling.
6. Identify hazardous battery waste.
7. Calculate reward points using transparent rules.
8. Manage the collection lifecycle.
9. Search items by ID, owner, or category.
10. Update conditions, collection status, and collection details.
11. Delete records safely after confirmation.
12. Generate category, condition, status, weight, and reward reports.
13. Save data and transaction history using files.

---

## 5. Scope of the Project

The system supports the following e-waste types:

- Mobile
- Laptop
- Battery
- Printer
- Television

The system supports these item conditions:

- Working
- Partially Working
- Non-Working

The collection lifecycle is:

```text
Registered -> Scheduled -> Collected -> Sorted -> Recycled
```

The application is suitable for a small college project, e-waste collection centre, society collection drive, or offline recycling register.

---

## 6. System Workflow

```text
Start Program
      |
      v
Load Saved Data
      |
      v
Register E-Waste
      |
      v
Automatic Classification
      |
      v
Assess Condition
      |
      v
Generate Recommendation
      |
      v
Calculate Reward
      |
      v
Schedule Collection
      |
      v
Update Collection Status
      |
      v
Generate Reports
      |
      v
Save Data and Exit
```

---

## 7. Main Features

### 7.1 Register E-Waste

The user enters:

- Owner name
- E-waste type
- Item name or model
- Condition
- Weight

The system generates a unique ID such as:

```text
EW1001
```

### 7.2 Automatic Classification

| E-Waste Type | Category |
|---|---|
| Mobile | Small Electronics |
| Laptop | Computer Equipment |
| Battery | Hazardous E-Waste |
| Printer | Office Electronics |
| Television | Large Electronics |

### 7.3 Condition-Based Recommendation

| Condition | General Recommendation |
|---|---|
| Working | Reuse or Resale |
| Partially Working | Repair or Refurbish |
| Non-Working | Component Recovery and Recycling |

Battery items always display a hazardous handling warning.

### 7.4 Reward System

Reward points are calculated using:

```text
Reward Points = round(Weight x Type Rate x Condition Multiplier)
```

Type rates:

| Type | Rate |
|---|---:|
| Mobile | 18 points/kg |
| Laptop | 25 points/kg |
| Battery | 12 points/kg |
| Printer | 15 points/kg |
| Television | 10 points/kg |

Condition multipliers:

| Condition | Multiplier |
|---|---:|
| Working | 1.25 |
| Partially Working | 1.00 |
| Non-Working | 0.75 |

Reward levels:

| Points | Level |
|---:|---|
| 0-99 | Bronze |
| 100-249 | Silver |
| 250-499 | Gold |
| 500 or more | Eco Champion |

### 7.5 Collection Management

The system stores:

- Collection date
- Collection address
- Collection status

The system controls the correct status sequence and prevents invalid transitions.

### 7.6 Search

Records can be searched by:

- E-waste ID
- Owner name
- Category

### 7.7 Update

The system allows updating:

- Item condition
- Collection status
- Collection date
- Collection address

### 7.8 Delete

An item can be deleted only after the user confirms the deletion.

### 7.9 Reports

Reports include:

- Total registered items
- Total weight
- Total reward points
- Overall reward level
- Items by category
- Items by condition
- Items by status
- Hazardous item count
- Recycled item count

---

## 8. Object-Oriented Design

### 8.1 Abstract Base Class

The abstract class is:

```cpp
EWasteItem
```

It contains pure virtual functions such as:

```cpp
virtual std::string getType() const = 0;
virtual std::string getCategory() const = 0;
virtual int calculateReward() const = 0;
virtual std::string getRecommendedAction() const = 0;
```

### 8.2 Inheritance Hierarchy

```text
                         EWasteItem
                       /     |      \
                      /      |       \
                Mobile    Laptop    Battery
                                      /    \
                                   Printer Television
```

The logical inheritance structure is:

```text
EWasteItem
├── Mobile
├── Laptop
├── Battery
├── Printer
└── Television
```

### 8.3 Runtime Polymorphism

Objects are stored using base-class pointers:

```cpp
std::vector<EWasteItem*> items;
```

The program calls virtual functions through these pointers. The correct derived-class function is selected at runtime.

For example, the following call behaves differently for a laptop, battery, mobile, printer, or television:

```cpp
item->getRecommendedAction();
```

### 8.4 Encapsulation

Data members such as owner name, item name, weight, condition, and status are private inside `EWasteItem`. They are accessed through public methods.

### 8.5 Abstraction

The base class defines common operations without exposing the implementation details of each e-waste type.

### 8.6 Composition

The main controller uses manager objects:

- `CollectionManager`
- `RewardManager`
- `ReportManager`
- `FileManager`

`CollectionManager` also owns a map of collection records.

### 8.7 Constructors and Destructors

Every derived class calls the base-class constructor. `EWasteItem` has a virtual destructor so that derived objects can be safely deleted through base-class pointers.

### 8.8 Operator Overloading

The following operator compares two e-waste items based on reward points:

```cpp
bool operator>(const EWasteItem& other) const;
```

It is used to display items in descending reward order.

---

## 9. Class Responsibilities

### EWasteItem

- Abstract base class
- Stores common item information
- Validates IDs, weights, conditions, and statuses
- Defines common virtual behaviour

### Mobile

- Represents mobile phones
- Classifies items as Small Electronics
- Provides mobile-specific recommendations

### Laptop

- Represents laptops and notebooks
- Classifies items as Computer Equipment
- Provides laptop-specific recovery and repair recommendations

### Battery

- Represents batteries
- Classifies items as Hazardous E-Waste
- Displays hazardous handling warnings

### Printer

- Represents printers
- Classifies items as Office Electronics
- Provides printer-specific recommendations

### Television

- Represents televisions
- Classifies items as Large Electronics
- Provides television-specific recommendations

### CollectionManager

- Schedules collection
- Stores collection information
- Validates collection status transitions

### RewardManager

- Calculates reward levels
- Displays reward formula and calculation details

### ReportManager

- Calculates totals
- Generates map-based statistics
- Counts hazardous and recycled items

### FileManager

- Loads saved data
- Saves current data
- Creates the e-waste transaction file
- Displays transaction history
- Creates derived objects while loading records

---

## 10. Exception Handling

The project defines custom exceptions:

- `InvalidWeight`
- `InvalidEWasteType`
- `InvalidID`
- `InvalidInput`
- `ItemNotFound`

Examples of invalid operations handled by the system:

- Negative or zero weight
- Weight above 1000 kg
- Invalid ID format
- Invalid menu choice
- Invalid e-waste type
- Searching for an item that does not exist
- Invalid collection status transition
- Empty owner name or item name

---

## 11. STL Containers Used

### Vector

```cpp
std::vector<EWasteItem*> items;
```

Stores all registered e-waste objects using base-class pointers.

### Set

```cpp
std::set<std::string> ids;
```

Stores unique e-waste IDs and prevents duplicate IDs.

### Map

```cpp
std::map<std::string, int> byCategory;
std::map<std::string, int> byCondition;
std::map<std::string, int> byStatus;
```

Stores report statistics.

---

## 12. File Handling

### E-Waste Data File

```text
ewaste_data.txt
```

Stores item details such as:

- ID
- Type
- Owner
- Item name
- Condition
- Weight
- Collection status
- Collection date
- Collection address

### E-Waste Transaction File

```text
ewaste_transactions.log
```

Stores operations such as:

- Register
- Assess condition
- Schedule collection
- Update status
- Update collection details
- Delete
- Save

The e-waste program uses `ewaste_transactions.log` so that it does not mix its data with another project's `transactions.log` file.

The files are created automatically in the current program folder.

---

## 13. Menu Operations

```text
1.  Register E-Waste
2.  View All E-Waste
3.  Search E-Waste
4.  Assess Condition
5.  Get Recycling Recommendation
6.  Calculate Reward
7.  Schedule Collection
8.  Update Status
9.  Generate Reports
10. Update E-Waste
11. Delete E-Waste
12. View E-Waste Transaction Log
13. Save & Exit
```

---

## 14. Sample Demonstration

### Battery Registration

```text
Owner name: Neha Sharma
Type: Battery
Item name: Lithium-Ion Laptop Battery
Condition: Non-Working
Weight: 0.80 kg
```

System output:

```text
Automatic classification: Hazardous E-Waste
Initial recommendation: Hazardous Battery Recycling; never place in general waste
WARNING: isolate terminals, avoid heat or puncture, and use an authorised battery recycler.
```

### Laptop Reward Calculation

```text
Weight: 2.40 kg
Type rate: 25 points/kg
Condition multiplier: 0.75
```

```text
Reward points = round(2.40 x 25 x 0.75)
Reward points = 45
Reward level = Bronze
```

### Status Progression

```text
Registered -> Scheduled -> Collected -> Sorted -> Recycled
```

---

## 15. Testing Summary

| Test Case | Expected Result | Status |
|---|---|---|
| Register valid mobile | Mobile record created | Passed |
| Register valid laptop | Laptop record created | Passed |
| Register battery | Hazard warning displayed | Passed |
| Enter invalid weight | InvalidWeight exception | Passed |
| Enter invalid ID | InvalidID exception | Passed |
| Search existing item | Item details displayed | Passed |
| Search missing item | ItemNotFound exception | Passed |
| Calculate reward | Formula and reward displayed | Passed |
| Schedule collection | Status changes to Scheduled | Passed |
| Invalid status jump | InvalidInput exception | Passed |
| Generate report | Statistics displayed | Passed |
| Delete with confirmation | Item safely deleted | Passed |
| Save and restart | Records loaded from file | Passed |
| View transaction log | E-waste log displayed | Passed |

---

## 16. Limitations

- The system is console-based.
- The system works offline only.
- No database is used.
- No SMS or email notification is implemented.
- The system recommends repair or recycling but does not physically repair devices.
- The owner checks progress using the generated e-waste ID.
- Repair completion tracking can be added as a future enhancement.

---

## 17. Future Enhancements

Possible future improvements include:

- Add `Under Repair`, `Repaired`, and `Ready for Reuse` statuses.
- Add repair remarks and repair completion date.
- Add a separate owner profile module.
- Add export of reports to CSV or PDF.
- Add a graphical user interface.
- Add database connectivity.
- Add SMS or email notifications.
- Add QR code generation for e-waste IDs.
- Add login and role-based access.

---

## 18. Compilation and Execution

Compile using C++17:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic ewaste_single.cpp -o ewaste.exe
```

Run on Windows PowerShell:

```powershell
.\ewaste.exe
```

Run on Command Prompt:

```cmd
ewaste.exe
```

---

## 19. GitHub Repository Structure

Upload the following files to the root of the GitHub repository:

```text
E-waste-Management/
│
├── ewaste_single.cpp
├── PROJECT_REPORT.md
└── README.md
```

The following files are generated automatically while running the program and do not need to be uploaded:

```text
ewaste_data.txt

ewaste_transactions.log
```

Repository:

```text
https://github.com/kaushiktopale-eng/E-waste-Management
```

Recommended GitHub file placement:

```text
Repository root/
├── ewaste_single.cpp       ← Complete C++ source code
├── PROJECT_REPORT.md       ← This project report
└── README.md               ← Short project introduction
```

---

## 20. Conclusion

The E-Waste Management & Recycling System provides a complete offline solution for managing electronic waste. It demonstrates the practical use of Object-Oriented Programming in C++ through abstraction, encapsulation, inheritance, polymorphism, composition, exception handling, operator overloading, STL containers, and file handling.

The system manages e-waste from registration to collection and recycling while providing reward calculation, hazardous-item identification, reports, and persistent storage. It is a suitable diploma-level OOP microproject with clear real-world relevance.
