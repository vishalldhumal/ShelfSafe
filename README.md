# ShelfSafe

ShelfSafe is a C console application for organizing food and medicine records, checking expiry dates, and displaying medicine schedule notes. Users can also save interaction reminders they have verified with a doctor or pharmacist.

## Features

1. Add a food item with its quantity, unit, and expiry date.
2. View the food inventory.
3. Add a medicine with its dose, schedule note, and expiry date.
4. View saved medicines.
5. Check for expired items and items expiring soon.
6. Add a user-verified medicine interaction reminder.
7. View medicine schedule notes.
8. View interaction reminders that match medicines in the inventory.

> ShelfSafe displays user-entered reminders. It does not determine whether medicines are safe to combine and does not replace advice from a doctor or pharmacist.

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

## Requirements

- GCC or another C compiler that supports C99
- Windows, macOS, or Linux

## Compile and run

Open a terminal in the folder containing `ShelfSafe.c`.

### Windows PowerShell

```powershell
gcc -std=c99 -Wall -Wextra -mconsole ShelfSafe.c -o ShelfSafe.exe
.\ShelfSafe.exe
```

### macOS or Linux

```bash
gcc -std=c99 -Wall -Wextra ShelfSafe.c -o ShelfSafe
./ShelfSafe
```

## C concepts demonstrated

- Structures and arrays
- Functions
- Loops and conditional statements
- Strings and input handling
- File handling
- Date validation and comparison

## Notes

- Data is stored locally in text files created by the program.
- Keep those data files beside the program if you want to retain your records.
- The screenshots are saved in the repository root.

## License

No license has been added. Add a `LICENSE` file if you want to specify how others may use or modify this project.
