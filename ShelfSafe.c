#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 100
#define NAME_LEN 50
#define UNIT_LEN 20
#define DOSE_LEN 40
#define SCHEDULE_LEN 60
#define NOTE_LEN 100
#define LINE_LEN 256
#define FOOD_FILE "food.txt"
#define MEDICINE_FILE "medicines.txt"
#define INTERACTION_FILE "interaction_notes.txt"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char name[NAME_LEN];
    int quantity;
    char unit[UNIT_LEN];
    Date expiry;
} Food;

typedef struct {
    char name[NAME_LEN];
    char dose[DOSE_LEN];
    char schedule[SCHEDULE_LEN];
    Date expiry;
} Medicine;

typedef struct {
    char medicineA[NAME_LEN];
    char medicineB[NAME_LEN];
    char note[NOTE_LEN];
} InteractionNote;

Food foods[MAX_ITEMS];
Medicine medicines[MAX_ITEMS];
InteractionNote notes[MAX_ITEMS];

int foodCount = 0;
int medicineCount = 0;
int noteCount = 0;

/* ---------- Input helpers ---------- */

void readLine(const char *prompt, char *buffer, int size)
{
    int ch;

    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL) {
        printf("\nInput ended. Goodbye.\n");
        exit(0);
    }

    if (strchr(buffer, '\n') != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            /* Clear any extra characters from the input line. */
        }
    }

    /* The vertical bar separates fields in the saved files. */
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '|') {
            buffer[i] = '-';
        }
    }
}

void readText(const char *prompt, char *buffer, int size)
{
    do {
        readLine(prompt, buffer, size);

        if (buffer[0] == '\0') {
            printf("Please enter some text.\n");
        }
    } while (buffer[0] == '\0');
}

int readNumber(const char *prompt, int min, int max)
{
    char buffer[40];
    char *end;
    long value;

    for (;;) {
        readLine(prompt, buffer, sizeof(buffer));
        value = strtol(buffer, &end, 10);

        if (end != buffer && *end == '\0' && value >= min && value <= max) {
            return (int)value;
        }

        printf("Enter a whole number from %d to %d.\n", min, max);
    }
}

int isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) ||
           (year % 400 == 0);
}

int daysInMonth(int month, int year)
{
    switch (month) {
        case 2:
            return isLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

int isValidDate(Date date)
{
    if (date.year < 2000 || date.year > 2100) {
        return 0;
    }

    if (date.month < 1 || date.month > 12) {
        return 0;
    }

    return date.day >= 1 &&
           date.day <= daysInMonth(date.month, date.year);
}

Date readDate(const char *prompt)
{
    char buffer[50];
    Date date;
    int charsRead;

    for (;;) {
        readLine(prompt, buffer, sizeof(buffer));

        if (sscanf(buffer, "%d-%d-%d%n",
                   &date.day, &date.month, &date.year, &charsRead) == 3 &&
            buffer[charsRead] == '\0' &&
            isValidDate(date)) {
            return date;
        }

        printf("Enter a valid date in DD-MM-YYYY format, e.g. 05-11-2026.\n");
    }
}

/* Converts a date to a day count so dates can be compared. */
long dateNumber(Date date)
{
    long total = 0;

    for (int year = 1; year < date.year; year++) {
        total += isLeapYear(year) ? 366 : 365;
    }

    for (int month = 1; month < date.month; month++) {
        total += daysInMonth(month, date.year);
    }

    total += date.day;
    return total;
}

Date todayDate(void)
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    Date today;

    today.day = local->tm_mday;
    today.month = local->tm_mon + 1;
    today.year = local->tm_year + 1900;

    return today;
}

long daysUntil(Date date)
{
    Date today = todayDate();
    return dateNumber(date) - dateNumber(today);
}

/* ---------- Save and load ---------- */

void saveFoods(void)
{
    FILE *file = fopen(FOOD_FILE, "w");

    if (file == NULL) {
        printf("Could not save food records.\n");
        return;
    }

    for (int i = 0; i < foodCount; i++) {
        fprintf(file, "%s|%d|%s|%d|%d|%d\n",
                foods[i].name,
                foods[i].quantity,
                foods[i].unit,
                foods[i].expiry.day,
                foods[i].expiry.month,
                foods[i].expiry.year);
    }

    fclose(file);
}

void saveMedicines(void)
{
    FILE *file = fopen(MEDICINE_FILE, "w");

    if (file == NULL) {
        printf("Could not save medicine records.\n");
        return;
    }

    for (int i = 0; i < medicineCount; i++) {
        fprintf(file, "%s|%s|%s|%d|%d|%d\n",
                medicines[i].name,
                medicines[i].dose,
                medicines[i].schedule,
                medicines[i].expiry.day,
                medicines[i].expiry.month,
                medicines[i].expiry.year);
    }

    fclose(file);
}

void saveNotes(void)
{
    FILE *file = fopen(INTERACTION_FILE, "w");

    if (file == NULL) {
        printf("Could not save interaction reminders.\n");
        return;
    }

    for (int i = 0; i < noteCount; i++) {
        fprintf(file, "%s|%s|%s\n",
                notes[i].medicineA,
                notes[i].medicineB,
                notes[i].note);
    }

    fclose(file);
}

void loadFoods(void)
{
    FILE *file = fopen(FOOD_FILE, "r");
    char line[LINE_LEN];

    if (file == NULL) {
        return;
    }

    while (foodCount < MAX_ITEMS && fgets(line, sizeof(line), file) != NULL) {
        Food item;
        int fields = sscanf(line, "%49[^|]|%d|%19[^|]|%d|%d|%d",
                            item.name,
                            &item.quantity,
                            item.unit,
                            &item.expiry.day,
                            &item.expiry.month,
                            &item.expiry.year);

        if (fields == 6 && isValidDate(item.expiry)) {
            foods[foodCount++] = item;
        }
    }

    fclose(file);
}

void loadMedicines(void)
{
    FILE *file = fopen(MEDICINE_FILE, "r");
    char line[LINE_LEN];

    if (file == NULL) {
        return;
    }

    while (medicineCount < MAX_ITEMS &&
           fgets(line, sizeof(line), file) != NULL) {
        Medicine item;
        int fields = sscanf(line, "%49[^|]|%39[^|]|%59[^|]|%d|%d|%d",
                            item.name,
                            item.dose,
                            item.schedule,
                            &item.expiry.day,
                            &item.expiry.month,
                            &item.expiry.year);

        if (fields == 6 && isValidDate(item.expiry)) {
            medicines[medicineCount++] = item;
        }
    }

    fclose(file);
}

void loadNotes(void)
{
    FILE *file = fopen(INTERACTION_FILE, "r");
    char line[LINE_LEN];

    if (file == NULL) {
        return;
    }

    while (noteCount < MAX_ITEMS && fgets(line, sizeof(line), file) != NULL) {
        InteractionNote item;
        int fields = sscanf(line, "%49[^|]|%49[^|]|%99[^\n]",
                            item.medicineA,
                            item.medicineB,
                            item.note);

        if (fields == 3) {
            notes[noteCount++] = item;
        }
    }

    fclose(file);
}

/* ---------- Food module ---------- */

void addFood(void)
{
    if (foodCount >= MAX_ITEMS) {
        printf("Food list is full.\n");
        return;
    }

    Food item;

    readText("Food name: ", item.name, NAME_LEN);
    item.quantity = readNumber("Quantity: ", 1, 100000);
    readText("Unit (e.g. pieces, packets, kg): ", item.unit, UNIT_LEN);
    item.expiry = readDate("Expiry date (DD-MM-YYYY): ");

    foods[foodCount++] = item;
    saveFoods();

    printf("Food item saved.\n");
}

void listFoods(void)
{
    if (foodCount == 0) {
        printf("No food items saved yet.\n");
        return;
    }

    printf("\n--- Food Inventory ---\n");

    for (int i = 0; i < foodCount; i++) {
        long left = daysUntil(foods[i].expiry);

        printf("%d. %s — %d %s — expires %02d-%02d-%04d",
               i + 1,
               foods[i].name,
               foods[i].quantity,
               foods[i].unit,
               foods[i].expiry.day,
               foods[i].expiry.month,
               foods[i].expiry.year);

        if (left < 0) {
            printf(" [EXPIRED %ld day(s) ago]", -left);
        } else if (left == 0) {
            printf(" [EXPIRES TODAY]");
        } else if (left <= 7) {
            printf(" [EXPIRES IN %ld day(s)]", left);
        }

        printf("\n");
    }
}

/* ---------- Medicine module ---------- */

void addMedicine(void)
{
    if (medicineCount >= MAX_ITEMS) {
        printf("Medicine list is full.\n");
        return;
    }

    Medicine item;

    readText("Medicine name: ", item.name, NAME_LEN);
    readText("Dose as prescribed (e.g. 1 tablet): ", item.dose, DOSE_LEN);
    readText("Schedule note (e.g. morning after breakfast): ",
             item.schedule, SCHEDULE_LEN);
    item.expiry = readDate("Expiry date (DD-MM-YYYY): ");

    medicines[medicineCount++] = item;
    saveMedicines();

    printf("Medicine saved. Follow the instructions from your healthcare professional.\n");
}

void listMedicines(void)
{
    if (medicineCount == 0) {
        printf("No medicines saved yet.\n");
        return;
    }

    printf("\n--- Medicine List ---\n");

    for (int i = 0; i < medicineCount; i++) {
        long left = daysUntil(medicines[i].expiry);

        printf("%d. %s — %s — schedule: %s — expires %02d-%02d-%04d",
               i + 1,
               medicines[i].name,
               medicines[i].dose,
               medicines[i].schedule,
               medicines[i].expiry.day,
               medicines[i].expiry.month,
               medicines[i].expiry.year);

        if (left < 0) {
            printf(" [EXPIRED %ld day(s) ago]", -left);
        } else if (left == 0) {
            printf(" [EXPIRES TODAY]");
        } else if (left <= 7) {
            printf(" [EXPIRES IN %ld day(s)]", left);
        }

        printf("\n");
    }
}

/* ---------- User-entered interaction reminders ---------- */

int medicineIsListed(const char *name)
{
    for (int i = 0; i < medicineCount; i++) {
        if (strcmp(medicines[i].name, name) == 0) {
            return 1;
        }
    }

    return 0;
}

void addInteractionNote(void)
{
    if (noteCount >= MAX_ITEMS) {
        printf("Interaction reminder list is full.\n");
        return;
    }

    printf("\nOnly enter a reminder you have confirmed with a doctor or pharmacist.\n");

    InteractionNote item;

    readText("First medicine name (match its spelling in your list): ",
             item.medicineA, NAME_LEN);
    readText("Second medicine name: ", item.medicineB, NAME_LEN);
    readText("Your verified reminder note: ", item.note, NOTE_LEN);

    notes[noteCount++] = item;
    saveNotes();

    printf("Reminder saved. ShelfSafe does not determine medical interactions.\n");
}

void showInteractionNotes(void)
{
    int found = 0;

    printf("\n--- User-entered Interaction Reminders ---\n");

    for (int i = 0; i < noteCount; i++) {
        if (medicineIsListed(notes[i].medicineA) &&
            medicineIsListed(notes[i].medicineB)) {
            printf("Review combination: %s + %s\n",
                   notes[i].medicineA, notes[i].medicineB);
            printf("Reminder: %s\n", notes[i].note);
            found = 1;
        }
    }

    if (!found) {
        printf("No saved reminder matches two medicines in your list.\n");
    }

    printf("These are user-entered reminders, not medical advice.\n");
}

/* ---------- Alerts and menus ---------- */

void showExpiryAlerts(void)
{
    int found = 0;

    printf("\n--- Expired or Soon-to-Expire Items ---\n");

    for (int i = 0; i < foodCount; i++) {
        long left = daysUntil(foods[i].expiry);

        if (left <= 30) {
            printf("Food: %s — ", foods[i].name);
            if (left < 0) {
                printf("expired %ld day(s) ago\n", -left);
            } else if (left == 0) {
                printf("expires today\n");
            } else {
                printf("expires in %ld day(s)\n", left);
            }
            found = 1;
        }
    }

    for (int i = 0; i < medicineCount; i++) {
        long left = daysUntil(medicines[i].expiry);

        if (left <= 30) {
            printf("Medicine: %s — ", medicines[i].name);
            if (left < 0) {
                printf("expired %ld day(s) ago\n", -left);
            } else if (left == 0) {
                printf("expires today\n");
            } else {
                printf("expires in %ld day(s)\n", left);
            }
            found = 1;
        }
    }

    if (!found) {
        printf("No items expire within the next 30 days.\n");
    }
}

void showMedicineSchedules(void)
{
    if (medicineCount == 0) {
        printf("No medicine schedules saved.\n");
        return;
    }

    printf("\n--- Medicine Schedule Notes ---\n");
    for (int i = 0; i < medicineCount; i++) {
        printf("%s: %s (%s)\n",
               medicines[i].name,
               medicines[i].dose,
               medicines[i].schedule);
    }

    printf("These are reminders to review; follow your prescribed instructions.\n");
}

void showMenu(void)
{
    printf("\n========== SHELFSAFE ==========\n");
    printf("1. Add food item\n");
    printf("2. View food inventory\n");
    printf("3. Add medicine\n");
    printf("4. View medicines\n");
    printf("5. Show expiry alerts\n");
    printf("6. Add verified interaction reminder\n");
    printf("7. View medicine schedule notes\n");
    printf("8. View matching interaction reminders\n");
    printf("9. Exit\n");
}

int main(void)
{
    int choice;

    loadFoods();
    loadMedicines();
    loadNotes();

    printf("ShelfSafe — Home Health and Expiry Manager\n");
    printf("Loaded %d food item(s), %d medicine(s), and %d reminder(s).\n",
           foodCount, medicineCount, noteCount);

    do {
        showMenu();
        choice = readNumber("Choose an option: ", 1, 9);

        switch (choice) {
            case 1:
                addFood();
                break;
            case 2:
                listFoods();
                break;
            case 3:
                addMedicine();
                break;
            case 4:
                listMedicines();
                break;
            case 5:
                showExpiryAlerts();
                break;
            case 6:
                addInteractionNote();
                break;
            case 7:
                showMedicineSchedules();
                break;
            case 8:
                showInteractionNotes();
                break;
            case 9:
                printf("Goodbye. Your records are saved locally.\n");
                break;
        }
    } while (choice != 9);

    return 0;
}