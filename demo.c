#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define N 50
#define M 30

double currentincome = 0;
double currentexpense = 0;
double monthly_budget = 0;
int next_id = 1;

struct node
{
    int id;
    char date[M];
    double amount;
    char category[N];
    struct node *next;
} *income = NULL, *expense = NULL;

struct record
{
    double x, y, monthly_budget;
} *point = NULL;

void create(char x[], double y, char z[], struct node **temp);
void display(int a3);
struct node *readnext(struct node *ptr, FILE *fpointer);
void writeincome(struct node *ptr);
void writeexpense(struct node *ptr);
void deleterecord(struct node *ptr);
struct node *readincome(struct node *ptr);
struct node *readexpense(struct node *ptr);
void write(struct record *point);
struct record *readrecord();
int date_to_int(const char date[]);
int get_month(const char *date);
void search_by_category(struct node *head, char category[]);
void filter_by_date_range(struct node *head, const char *start_date_str, const char *end_date_str);
void export_to_csv(struct node *head, const char *filename, const char *type);
void generate_report();
void check_monthly_budget(struct node *expense, double budget);

int main()
{
    int option;
    double b;
    char c[N], a[M];
    char s1[15], s2[15], s3[15];

    if (fopen("Record.bin", "rb") != NULL)
    {
        point = readrecord();
        currentincome = point->x;
        currentexpense = point->y;
        monthly_budget = point->monthly_budget;
        free(point);
    }

    if (fopen("myincome.bin", "rb") != NULL)
    {
        income = readincome(income);
        struct node *temp = income;
        while (temp != NULL)
        {
            if (temp->id >= next_id)
                next_id = temp->id + 1;
            temp = temp->next;
        }
    }

    if (fopen("myexpense.bin", "rb") != NULL)
    {
        expense = readexpense(expense);
        struct node *temp = expense;
        while (temp != NULL)
        {
            if (temp->id >= next_id)
                next_id = temp->id + 1;
            temp = temp->next;
        }
    }

    do
    {
        printf("\n_______________________________________________\n");
        printf("|     YOUR INCOME   =      %.2lf INR      \n", currentincome);
        printf("|     YOUR EXPENSE  =      %.2lf INR     \n", currentexpense);
        printf("|     YOUR BALANCE  =      %.2lf INR     \n", currentincome - currentexpense);
        printf("|     MONTHLY BUDGET=      %.2lf INR     \n", monthly_budget);
        printf("|_______________________________________________\n");
        printf("\nMAIN MENU:\n");
        printf("\nMAIN MENU:\n");
        printf("1. ADD INCOME\n");
        printf("2. ADD EXPENSE\n");
        printf("3. SET MONTHLY BUDGET\n"); 
        printf("4. VIEW INCOME RECORD\n");
        printf("5. VIEW EXPENSE RECORD\n");
        printf("6. SEARCH BY CATEGORY\n");
        printf("7. FILTER BY DATE RANGE\n");
        printf("8. EXPORT TO CSV\n");
        printf("9. GENERATE REPORT\n");
        printf("10. EXIT\n"); 
        printf("11. VISUALIZE EXPENSES (Python Graph)\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
        {
            printf("\n************** ADD INCOME **************\n");
            printf("Enter Date (DD MMM YYYY): ");
            scanf("%s %s %s", s1, s2, s3);
            snprintf(a, M, "%s %s %s", s1, s2, s3);

            printf("Enter Amount: ");
            scanf("%lf", &b);

            printf("Enter Category: ");
            scanf("%s", c);

            currentincome += b;
            create(a, b, c, &income);
            income->id = next_id++;
            writeincome(income);
            break;
        }
        case 2:
        {
            printf("\n************** ADD EXPENSE **************\n");
            printf("Enter Date (DD MMM YYYY): ");
            scanf("%s %s %s", s1, s2, s3);
            snprintf(a, M, "%s %s %s", s1, s2, s3);

            printf("Enter Amount: ");
            scanf("%lf", &b);

            printf("Enter Category: ");
            scanf("%s", c);

            currentexpense += b;
            create(a, b, c, &expense);
            expense->id = next_id++;
            writeexpense(expense);
            check_monthly_budget(expense, monthly_budget);
            break;
        }
        case 3:
            // SET MONTHLY BUDGET
            printf("Enter new monthly budget: ");
            scanf("%lf", &monthly_budget);
            printf("Monthly budget set to %.2lf INR\n", monthly_budget);
            break;
        case 4:
            printf("\n********** INCOME RECORD **********\n");
            display(3);
            break;
        case 5:
            printf("\n********** EXPENSE RECORD **********\n");
            display(4);
            break;
        case 6:
        {
            char search_category[N];
            printf("\nEnter category to search: ");
            scanf("%s", search_category);
            printf("Search in (1) Income or (2) Expense? ");
            int choice;
            scanf("%d", &choice);
            if (choice == 1)
                search_by_category(income, search_category);
            else if (choice == 2)
                search_by_category(expense, search_category);
            else
                printf("Invalid choice!\n");
            break;
        }
        case 7:
        {
            char start_date[20] = {0}, end_date[20] = {0};
            while (getchar() != '\n')
                ;
            printf("Enter start date (DD MMM YYYY): ");
            fgets(start_date, sizeof(start_date), stdin);
            start_date[strcspn(start_date, "\n")] = 0;

            printf("Enter end date (DD MMM YYYY): ");
            fgets(end_date, sizeof(end_date), stdin);
            end_date[strcspn(end_date, "\n")] = 0;

            printf("Filter (1) Income or (2) Expense? ");
            int choice;
            scanf("%d", &choice);

            if (choice == 1)
                filter_by_date_range(income, start_date, end_date);
            else if (choice == 2)
                filter_by_date_range(expense, start_date, end_date);
            else
                printf("Invalid choice!\n");
            break;
        }
        case 8:
        {
            printf("\nExport (1) Income or (2) Expense records? ");
            int choice;
            scanf("%d", &choice);

            if (choice == 1)
            {
                export_to_csv(income, "income_records.csv", "Income");
                printf("Income records exported to income_records.csv\n");
            }
            else if (choice == 2)
            {
                export_to_csv(expense, "expense_records.csv", "Expense");
                printf("Expense records exported to expense_records.csv\n");
            }
            else
            {
                printf("Invalid choice!\n");
            }
            break;
        }
        case 9:
            generate_report();
            break;
        case 10:
            point = (struct record *)malloc(sizeof(struct record));
            point->x = currentincome;
            point->y = currentexpense;
            point->monthly_budget = monthly_budget;
            write(point);
            free(point);
            printf("\nData saved successfully. Exiting...\n");
            break;
        case 11:
            printf("\nLaunching visualization...\n");
            system("python visualize.py");
            break;
        default:
            printf("\nInvalid option! Please try again.\n");
            break;
        }
    } while (option != 10);

    deleterecord(income);
    deleterecord(expense);
    return 0;
}

void create(char x[], double y, char z[], struct node **temp)
{
    struct node *newnode = (struct node *)malloc(sizeof(struct node));
    newnode->id = next_id;
    strcpy(newnode->date, x);
    newnode->amount = y;
    strcpy(newnode->category, z);
    newnode->next = NULL;

    if (*temp == NULL)
    {
        *temp = newnode;
    }
    else
    {
        struct node *ptr = *temp;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = newnode;
    }
}

void display(int a3)
{
    struct node *ptr = (a3 == 3) ? income : expense;

    if (ptr == NULL)
    {
        printf("No records available!\n");
        return;
    }

    printf("ID\tDate\t\tAmount\tCategory\n");
    printf("------------------------------------------------\n");
    while (ptr != NULL)
    {
        printf("%d\t%s\t%.2lf\t%s\n",
               ptr->id, ptr->date, ptr->amount, ptr->category);
        ptr = ptr->next;
    }
}

int date_to_int(const char date[])
{
    int day, year;
    char month[10] = {0};

    if (sscanf(date, "%d %9s %d", &day, month, &year) != 3)
    {
        return -1;
    }

    for (int i = 0; month[i]; i++)
    {
        month[i] = tolower(month[i]);
    }

    const char *months[] = {"jan", "feb", "mar", "apr", "may", "jun",
                            "jul", "aug", "sep", "oct", "nov", "dec"};

    int month_num = 0;
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(month, months[i]) == 0)
        {
            month_num = i + 1;
            break;
        }
    }

    if (month_num == 0)
        return -1;
    if (day < 1 || day > 31)
        return -1;
    if (year < 2000 || year > 2100)
        return -1;

    return year * 10000 + month_num * 100 + day;
}

void search_by_category(struct node *head, char category[])
{
    if (head == NULL)
    {
        printf("No records available!\n");
        return;
    }

    int found = 0;
    struct node *current = head;

    printf("\n=== Transactions in Category: %s ===\n", category);
    printf("ID\tDate\t\tAmount\n");
    printf("--------------------------------\n");
    while (current != NULL)
    {
        if (strcasecmp(current->category, category) == 0)
        {
            printf("%d\t%s\t%.2lf\n",
                   current->id, current->date, current->amount);
            found = 1;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("No transactions found in this category.\n");
    }
}

void filter_by_date_range(struct node *head, const char *start_date_str, const char *end_date_str)
{
    if (head == NULL)
    {
        printf("No records available!\n");
        return;
    }

    int start_date = date_to_int(start_date_str);
    int end_date = date_to_int(end_date_str);

    if (start_date == -1 || end_date == -1)
    {
        printf("Invalid date format! Use 'DD MMM YYYY' (e.g., '15 Jan 2023')\n");
        return;
    }

    if (start_date > end_date)
    {
        printf("Error: Start date must be before end date.\n");
        return;
    }

    printf("\n=== Transactions Between %s and %s ===\n", start_date_str, end_date_str);
    printf("ID\tDate\t\tAmount\tCategory\n");
    printf("----------------------------------------\n");

    int found = 0;
    struct node *current = head;

    while (current != NULL)
    {
        int current_date = date_to_int(current->date);
        if (current_date == -1)
        {
            current = current->next;
            continue;
        }

        if (current_date >= start_date && current_date <= end_date)
        {
            printf("%d\t%s\t%.2lf\t%s\n",
                   current->id, current->date, current->amount, current->category);
            found = 1;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("No transactions found in this date range.\n");
    }
}

void export_to_csv(struct node *head, const char *filename, const char *type)
{
    if (head == NULL)
    {
        printf("No %s records to export!\n", type);
        return;
    }

    FILE *csv_file = fopen(filename, "w");
    if (csv_file == NULL)
    {
        printf("Error creating CSV file!\n");
        return;
    }

    fprintf(csv_file, "ID,Date,Amount,Category\n");

    struct node *current = head;
    while (current != NULL)
    {
        fprintf(csv_file, "%d,%s,%.2lf,%s\n",
                current->id, current->date, current->amount, current->category);
        current = current->next;
    }

    fclose(csv_file);
}

void generate_report()
{
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char timestamp[50];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local);

    printf("\n=== FINANCIAL REPORT ===\n");
    printf("Generated on: %s\n", timestamp);
    printf("Total Income: %.2lf\n", currentincome);
    printf("Total Expense: %.2lf\n", currentexpense);
    printf("Net Balance: %.2lf\n", currentincome - currentexpense);

    export_to_csv(income, "income_report.csv", "Income");
    export_to_csv(expense, "expense_report.csv", "Expense");

    printf("\nReports exported to:\n");
    printf("- income_report.csv\n");
    printf("- expense_report.csv\n");
}

void deleterecord(struct node *ptr)
{
    struct node *freeme = ptr;
    struct node *holdme = NULL;
    while (freeme != NULL)
    {
        holdme = freeme->next;
        free(freeme);
        freeme = holdme;
    }
}

struct node *readnext(struct node *ptr, FILE *fpointer)
{
    if (ptr == NULL)
    {
        ptr = (struct node *)malloc(sizeof(struct node));
        fread(ptr, sizeof(struct node), 1, fpointer);
        ptr->next = NULL;
    }
    else
    {
        struct node *ptr1 = ptr;
        struct node *ptr2 = (struct node *)malloc(sizeof(struct node));
        while (ptr1->next != NULL)
        {
            ptr1 = ptr1->next;
        }
        fread(ptr2, sizeof(struct node), 1, fpointer);
        ptr1->next = ptr2;
        ptr2->next = NULL;
    }
    return ptr;
}

struct node *readincome(struct node *ptr)
{
    FILE *fpointer;
    fpointer = fopen("myincome.bin", "rb");
    if (fpointer != NULL)
    {
        deleterecord(ptr);
        ptr = NULL;
        fseek(fpointer, 0, SEEK_END);
        long filesize = ftell(fpointer);
        rewind(fpointer);
        int entries = (int)(filesize / (sizeof(struct node)));
        for (int i = 0; i < entries; i++)
        {
            fseek(fpointer, (sizeof(struct node) * i), SEEK_SET);
            ptr = readnext(ptr, fpointer);
        }
    }
    else
    {
        printf("ERROR IN OPENING FILE\n");
    }
    return ptr;
}

void writeincome(struct node *ptr)
{
    FILE *fpointer;
    fpointer = fopen("myincome.bin", "wb");
    if (fpointer != NULL)
    {
        struct node *ptr1 = ptr;
        struct node *holdnext = NULL;
        while (ptr1 != NULL)
        {
            holdnext = ptr1->next;
            ptr1->next = NULL;
            fseek(fpointer, 0, SEEK_END);
            fwrite(ptr1, sizeof(struct node), 1, fpointer);
            ptr1->next = holdnext;
            holdnext = NULL;
            ptr1 = ptr1->next;
        }
        fclose(fpointer);
        fpointer = NULL;
        printf("\nINCOME SAVED SUCCESSFULLY\n\n");
        printf("________________________________________________________________________________________________________________\n\n");
    }
    else
    {
        printf("\nCANNOT SAVE INCOME..TRY AGAIN\n");
        printf("________________________________________________________________________________________________________________\n\n");
    }
}

void writeexpense(struct node *ptr)
{
    FILE *fpointer;
    fpointer = fopen("myexpense.bin", "wb");
    if (fpointer != NULL)
    {
        struct node *ptr1 = ptr;
        struct node *holdnext = NULL;
        while (ptr1 != NULL)
        {
            holdnext = ptr1->next;
            ptr1->next = NULL;
            fseek(fpointer, 0, SEEK_END);
            fwrite(ptr1, sizeof(struct node), 1, fpointer);
            ptr1->next = holdnext;
            holdnext = NULL;
            ptr1 = ptr1->next;
        }
        fclose(fpointer);
        fpointer = NULL;
        printf("\nEXPENSE SAVED SUCCESSFULLY\n\n");
        printf("________________________________________________________________________________________________________________\n\n");
    }
    else
    {
        printf("\nCANNOT SAVE EXPENSE..TRY AGAIN\n\n");
        printf("________________________________________________________________________________________________________________\n\n");
    }
}

struct node *readexpense(struct node *ptr)
{
    FILE *fpointer;
    fpointer = fopen("myexpense.bin", "rb");
    if (fpointer != NULL)
    {
        deleterecord(ptr);
        ptr = NULL;
        fseek(fpointer, 0, SEEK_END);
        long filesize = ftell(fpointer);
        rewind(fpointer);
        int entries = (int)(filesize / (sizeof(struct node)));
        for (int i = 0; i < entries; i++)
        {
            fseek(fpointer, (sizeof(struct node) * i), SEEK_SET);
            ptr = readnext(ptr, fpointer);
        }
    }
    else
    {
        printf("cannot open file\n");
    }
    return ptr;
}

void write(struct record *point)
{
    FILE *fpointer;
    fpointer = fopen("Record.bin", "wb");
    if (fpointer != NULL)
    {
        fseek(fpointer, 0, SEEK_END);
        fwrite(point, sizeof(struct record), 1, fpointer);
    }
    else
    {
        printf("FILEOPEN ERROR\n");
    }
    fclose(fpointer);
    fpointer = NULL;
}

struct record *readrecord()
{
    FILE *fpointer;
    fpointer = fopen("Record.bin", "rb");
    struct record *ptr = NULL;

    if (fpointer != NULL)
    {
        fseek(fpointer, 0, SEEK_SET);
        ptr = (struct record *)malloc(sizeof(struct record));
        fread(ptr, sizeof(struct record), 1, fpointer);
    }
    else
    {
        printf("CANNOT OPEN FILE\n");
    }
    return ptr;
}

int get_month(const char *date)
{
    int day, year;
    char month[10];
    if (sscanf(date, "%d %s %d", &day, month, &year) != 3)
        return -1;

    for (int i = 0; month[i]; i++)
        month[i] = tolower(month[i]);

    const char *months[] = {
        "jan", "feb", "mar", "apr", "may", "jun",
        "jul", "aug", "sep", "oct", "nov", "dec"};

    for (int i = 0; i < 12; ++i)
    {
        if (strcmp(month, months[i]) == 0)
            return (year * 100) + (i + 1); 
    }
    return -1;
}

void check_monthly_budget(struct node *expense, double budget)
{
    if (budget <= 0 || expense == NULL)
        return;

    struct node *last = expense;
    while (last->next != NULL)
        last = last->next;

    int current_month = get_month(last->date);
    double total = 0;

    struct node *temp = expense;
    while (temp != NULL)
    {
        if (get_month(temp->date) == current_month)
            total += temp->amount;
        temp = temp->next;
    }

    if (total > budget)
    {
        printf("\n Alert: Monthly expense of %.2lf has exceeded your budget of %.2lf INR!\n", total, budget);
    }
}
