#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 50
#define M 30

double currentincome = 0;
double currentexpense = 0;
int next_id = 1; // Global ID counter

struct node {
    int id;
    char date[M];
    double amount;
    char category[N];
    struct node *next;
} *income = NULL, *expense = NULL;

struct record {
    double x, y;
} *point = NULL;

// Function prototypes
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
void search_by_category(struct node *head, char category[]);
void filter_by_date_range(struct node *head, const char *start_date_str, const char *end_date_str);

int main() {
    int option, value;
    double b;
    char c[N], a[M];
    char s1[15], s2[15], s3[15];

    // Load existing data
    if (fopen("Record.bin", "rb") != NULL) {
        point = readrecord();
        currentincome = point->x;
        currentexpense = point->y;
        free(point);
    }

    if (fopen("myincome.bin", "rb") != NULL) {
        income = readincome(income);
        // Find the highest existing ID for income records
        struct node *temp = income;
        while (temp != NULL) {
            if (temp->id >= next_id) next_id = temp->id + 1;
            temp = temp->next;
        }
    }

    if (fopen("myexpense.bin", "rb") != NULL) {
        expense = readexpense(expense);
        // Find the highest existing ID for expense records
        struct node *temp = expense;
        while (temp != NULL) {
            if (temp->id >= next_id) next_id = temp->id + 1;
            temp = temp->next;
        }
    }

    do {
        printf("\n_______________________________________________\n");
        printf("|     YOUR INCOME   =      %.2lf INR      \n", currentincome);      
        printf("|     YOUR EXPENSE  =      %.2lf INR     \n", currentexpense);
        printf("|     YOUR BALANCE  =      %.2lf INR     \n", currentincome - currentexpense);
        printf("|_______________________________________________\n");
        printf("\nMAIN MENU:\n");
        printf("1. ADD INCOME\n");
        printf("2. ADD EXPENSE\n");
        printf("3. VIEW INCOME RECORD\n");
        printf("4. VIEW EXPENSE RECORD\n");
        printf("5. SEARCH BY CATEGORY\n");
        printf("6. FILTER BY DATE RANGE\n");
        printf("7. EXIT\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch(option) {
            case 1: {
                printf("\n************** ADD INCOME **************\n");
                printf("Enter Date (DD MMM YYYY, e.g., 15 Jan 2023): ");
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

            case 2: {
                printf("\n************** ADD EXPENSE **************\n");
                printf("Enter Date (DD MMM YYYY, e.g., 15 Jan 2023): ");
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
                break;
            }

            case 3: {
                printf("\n********** INCOME RECORD **********\n");
                display(3);
                break;
            }

            case 4: {
                printf("\n********** EXPENSE RECORD **********\n");
                display(4);
                break;
            }

            case 5: {
                char search_category[N];
                printf("\nEnter category to search: ");
                scanf("%s", search_category);
                printf("Search in (1) Income or (2) Expense? ");
                int choice;
                scanf("%d", &choice);
                if (choice == 1) search_by_category(income, search_category);
                else if (choice == 2) search_by_category(expense, search_category);
                else printf("Invalid choice!\n");
                break;
            }

            case 6: {
    char start_date[20] = {0}, end_date[20] = {0};
    
    // Clear input buffer
    while (getchar() != '\n');
    
    printf("Enter start date (DD MMM YYYY): ");
    fgets(start_date, sizeof(start_date), stdin);
    start_date[strcspn(start_date, "\n")] = 0; // Remove newline
    
    printf("Enter end date (DD MMM YYYY): ");
    fgets(end_date, sizeof(end_date), stdin);
    end_date[strcspn(end_date, "\n")] = 0; // Remove newline
    
    printf("Filter (1) Income or (2) Expense? ");
    int choice;
    scanf("%d", &choice);
    
    if (choice == 1) filter_by_date_range(income, start_date, end_date);
    else if (choice == 2) filter_by_date_range(expense, start_date, end_date);
    else printf("Invalid choice!\n");
    break;
}

            case 7: {
                point = (struct record*)malloc(sizeof(struct record));
                point->x = currentincome;
                point->y = currentexpense;
                write(point);
                free(point);
                printf("\nData saved successfully. Exiting...\n");
                break;
            }

            default:
                printf("\nInvalid option! Please try again.\n");
                break;
        }
    } while (option != 7);

    // Free memory before exiting
    deleterecord(income);
    deleterecord(expense);
    
    return 0;
}

// Function to create a new record
void create(char x[], double y, char z[], struct node **temp) {
    struct node *newnode = (struct node*)malloc(sizeof(struct node));
    newnode->id = next_id;
    strcpy(newnode->date, x);
    newnode->amount = y;
    strcpy(newnode->category, z);
    newnode->next = NULL;

    if (*temp == NULL) {
        *temp = newnode;
    } else {
        struct node *ptr = *temp;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = newnode;
    }
}

// Display income or expense records
void display(int a3) {
    struct node *ptr = (a3 == 3) ? income : expense;
    
    if (ptr == NULL) {
        printf("No records available!\n");
        return;
    }

    while (ptr != NULL) {
        printf("ID: %d | Date: %s | Amount: %.2lf | Category: %s\n", 
              ptr->id, ptr->date, ptr->amount, ptr->category);
        ptr = ptr->next;
    }
}

// Convert date string to comparable integer
int date_to_int(const char date[]) {
    int day, year;
    char month[10] = {0};
    
    // More flexible sscanf pattern
    if (sscanf(date, "%d %9s %d", &day, month, &year) != 3) {
        return -1;
    }

    // Convert month to lowercase
    for (int i = 0; month[i]; i++) {
        month[i] = tolower(month[i]);
    }

    const char *months[] = {"jan", "feb", "mar", "apr", "may", "jun",
                           "jul", "aug", "sep", "oct", "nov", "dec"};
    
    int month_num = 0;
    for (int i = 0; i < 12; i++) {
        if (strcmp(month, months[i]) == 0) {
            month_num = i + 1;
            break;
        }
    }

    if (month_num == 0) return -1;
    if (day < 1 || day > 31) return -1;
    if (year < 2000 || year > 2100) return -1;

    return year * 10000 + month_num * 100 + day;
}

// Search transactions by category
void search_by_category(struct node *head, char category[]) {
    if (head == NULL) {
        printf("No records available!\n");
        return;
    }

    int found = 0;
    struct node *current = head;
    
    printf("\n=== Transactions in Category: %s ===\n", category);
    while (current != NULL) {
        if (strcasecmp(current->category, category) == 0) {
            printf("ID: %d | Date: %s | Amount: %.2lf\n", 
                  current->id, current->date, current->amount);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No transactions found in this category.\n");
    }
}

// Filter transactions by date range
void filter_by_date_range(struct node *head, const char *start_date_str, const char *end_date_str) {
    if (head == NULL) {
        printf("No records available!\n");
        return;
    }

    int start_date = date_to_int(start_date_str);
    int end_date = date_to_int(end_date_str);

    if (start_date == -1 || end_date == -1) {
        printf("Invalid date format! Use 'DD MMM YYYY' (e.g., '15 Jan 2023')\n");
        return;
    }

    if (start_date > end_date) {
        printf("Error: Start date must be before end date.\n");
        return;
    }

    printf("\n=== Transactions Between %s and %s ===\n", start_date_str, end_date_str);
    
    int found = 0;
    struct node *current = head;
    
    while (current != NULL) {
        int current_date = date_to_int(current->date);
        if (current_date == -1) {
            current = current->next;
            continue;
        }

        if (current_date >= start_date && current_date <= end_date) {
            printf("ID: %d | Date: %s | Amount: %.2lf | Category: %s\n",
                  current->id, current->date, current->amount, current->category);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("No transactions found in this date range.\n");
    }
}









void deleterecord(struct node *ptr)
{
struct node *freeme =ptr;
struct node *holdme=NULL;
while(freeme!=NULL)
{
holdme=freeme->next;
free(freeme);
freeme=holdme;
}
}






struct node *readnext(struct node *ptr,FILE *fpointer)
{

if(ptr==NULL)
{
ptr=(struct node *)malloc(sizeof(struct node));
fread(ptr,sizeof(struct node),1,fpointer);
ptr->next=NULL;
}
else
{
struct node *ptr1=ptr;
struct node *ptr2=(struct node *)malloc(sizeof(struct node));
while(ptr1->next!=NULL)
{
ptr1=ptr1->next;
}
fread(ptr2,sizeof(struct node),1,fpointer);
ptr1->next=ptr2;
ptr2->next=NULL;
}
return ptr;
}





struct node *readincome(struct node *ptr)
{
FILE *fpointer;
fpointer=fopen("myincome.bin","rb");
if(fpointer!=NULL)
{
deleterecord(ptr);
ptr=NULL;
fseek(fpointer,0,SEEK_END);
long filesize=ftell(fpointer);
rewind(fpointer);
int entries=(int)(filesize/(sizeof(struct node)));
for(int i=0;i<entries;i++)
{
fseek(fpointer,(sizeof(struct node)*i),SEEK_SET);
ptr=readnext(ptr,fpointer);
}
}
else
{
printf("ERROR IN OPENINNG FILE\n");
}
return ptr;
}








void writeincome(struct node *ptr)
{
FILE *fpointer;
fpointer=fopen("myincome.bin","wb");    
if(fpointer!=NULL)
{
struct node *ptr1=ptr;
struct node *holdnext=NULL;
while(ptr1!=NULL)
{
holdnext=ptr1->next;
ptr1->next=NULL;
fseek(fpointer,0,SEEK_END);
fwrite(ptr1,sizeof(struct node),1,fpointer);/*everytime we write into file,it will overwrite the data......                                                      whole data will be deleted and new data willl be written intofile*/
ptr1->next=holdnext;
holdnext=NULL;
ptr1=ptr1->next;
}
fclose(fpointer);
fpointer=NULL;
printf("\nINCOME SAVED SUCCESSFULLY\n\n");
 printf("________________________________________________________________________________________________________________\n\n");

}
else{
printf("\nCANNOT SAVE INCOME..TRY AGAIN\n");
 printf("________________________________________________________________________________________________________________\n\n");

}
}



void writeexpense(struct node *ptr)
{
FILE *fpointer;
fpointer=fopen("myexpense.bin","wb");    
if(fpointer!=NULL)
{
struct node *ptr1=ptr;
struct node *holdnext=NULL;
while(ptr1!=NULL)
{
holdnext=ptr1->next;
ptr1->next=NULL;
fseek(fpointer,0,SEEK_END);
fwrite(ptr1,sizeof(struct node),1,fpointer);/*everytime we write into file,it will overwrite the data.....
                                                     whole data will be deleted and new data willl be written intofile*/
ptr1->next=holdnext;
holdnext=NULL;
ptr1=ptr1->next;
}
fclose(fpointer);
fpointer=NULL;
printf("\nEXPENSE SAVED SUCCESSFULLY\n\n");
 printf("________________________________________________________________________________________________________________\n\n");

}
else{
printf("\nCANNOT SAVE EXPENSE..TRY AGAIN\n\n");
 printf("________________________________________________________________________________________________________________\n\n");

}
}





struct node *readexpense(struct node *ptr)
{
FILE *fpointer;
fpointer=fopen("myexpense.bin","rb");
if(fpointer!=NULL)
{
deleterecord(ptr);
ptr=NULL;
fseek(fpointer,0,SEEK_END);
long filesize=ftell(fpointer);
rewind(fpointer);
int entries=(int)(filesize/(sizeof(struct node)));
for(int i=0;i<entries;i++)
{
fseek(fpointer,(sizeof(struct node)*i),SEEK_SET);
ptr=readnext(ptr,fpointer);
}
}
else
{
printf("cannonot open file\n");

}
return ptr;
}








void write(struct record *point)
{
FILE *fpointer;
fpointer=fopen("Record.bin","wb");    
if(fpointer!=NULL)
{

fseek(fpointer,0,SEEK_END);
fwrite(point,sizeof(struct record),1,fpointer);/*everytime we write into file,it will overwrite the data.....
                                                     whole data will be deleted and new data willl be written intofile*/
}
else{
printf("FILEOPEN ERROR\n");
}
fclose(fpointer);
fpointer=NULL;

}






struct record *readrecord()
{
FILE *fpointer;
fpointer=fopen("Record.bin","rb");
struct record *ptr=NULL;

if(fpointer!=NULL)
{

fseek(fpointer,0,SEEK_SET);

ptr=(struct record *)malloc(sizeof(struct record));
fread(ptr,sizeof(struct record),1,fpointer);


}
else
{
printf("CANNOT OPEN FILE\n");
}
return ptr;
}
