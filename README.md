# ShelfSafe

**ShelfSafe** is a C console application for keeping track of food and medicines at home. It records quantities and expiry dates, displays medicine schedule notes, and can show user-entered interaction reminders.

## The problem it addresses

Food and medicines can be forgotten in storage, and people may overlook expiry dates or their own schedule notes. ShelfSafe keeps these records together and makes it easier to review them from a simple menu.

## Features

1. **Add food items** with a name, quantity, unit, and expiry date.
2. **View the food inventory.**
3. **Add medicines** with a name, prescribed dose, schedule note, and expiry date.
4. **View saved medicines.**
5. **Check expiry dates** and see which items are expired or expiring soon.
6. **Save a user-verified interaction reminder** for two medicine names.
7. **View medicine schedule notes.**
8. **View matching interaction reminders** when both medicine names are in the inventory.

> Interaction reminders are entered by the user after checking with a doctor or pharmacist. ShelfSafe does not determine whether medicines are safe to combine, recommend doses, or replace professional advice.

## Screenshots

### Main Menu
![ShelfSafe main menu](Screenshot%202026-11-02%20233813.png)

### 1. Add Food Item
![Add food item](Screenshot%202026-11-02%20233900.png)

### 2. View Food Inventory
![View food inventory](Screenshot%202026-11-02%20233912.png)

### 3. Add Medicine
![Add medicine](Screenshot%202026-11-02%20233950.png)

### 4. View Medicines
![View medicines](Screenshot%202026-11-02%20234004.png)

### 5. Show Expiry Alerts
![Expiry alerts](Screenshot%202026-11-02%20234020.png)

### 6. Add Verified Interaction Reminder
![Add interaction reminder](Screenshot%202026-11-02%20234115.png)

### 7. View Medicine Schedule Notes
![Medicine schedule notes](Screenshot%202026-11-02%20234130.png)

### 8. View Matching Interaction Reminders
![Matching interaction reminders](Screenshot%202026-11-02%20234147.png)

## How to use

1. Start ShelfSafe from a terminal.
2. Choose an option from the main menu.
3. Enter item names, quantities, units, and dates when prompted.
4. Use the expiry option to review items that need attention.
5. Exit from the menu when finished.

Dates should be entered in `DD-MM-YYYY` format. The program displays schedule notes when requested; it does not send timed notifications.

## Build and run

### Requirements

- GCC or another compiler that supports C99
- Windows, macOS, or Linux

Open a terminal in the folder containing `ShelfSafe.c`.

#### Windows PowerShell

```powershell
gcc -std=c99 -Wall -Wextra -mconsole ShelfSafe.c -o ShelfSafe.exe
.\ShelfSafe.exe
```

#### macOS or Linux

```bash
gcc -std=c99 -Wall -Wextra ShelfSafe.c -o ShelfSafe
./ShelfSafe
```

## Data storage

ShelfSafe saves records in local text files in the folder where it runs. Keep those files if you want your records to be available the next time you start the program. Do not upload personal medicine or household information to a public repository.

## C concepts demonstrated

- Structures for representing food, medicine, and reminder records
- Arrays for storing multiple records
- Functions for organizing program features
- Loops and conditional statements for menus and checks
- Strings and input handling
- File handling for saving and loading records
- Date validation and comparison

## Project structure

```text
ShelfSafe/
├── ShelfSafe.c
├── README.md
├── .gitignore
└── Screenshot files
```

## Limitations

- ShelfSafe is a local, single-user console application; it does not sync data between devices.
- Schedule entries are notes, not automatic alarms.
- Interaction reminders are user-entered and are not generated from a medical database.

## License

No license has been added. Add a `LICENSE` file if you want to specify how others may use or modify this project.
