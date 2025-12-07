#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREDENTIAL_FILE "credentials.txt"
#define STUDENT_FILE "students_records.txt"
#define TEMP_FILE "tmp_records.txt"

struct Student
{
    char roll[32];
    char name[50];
    float physics;
    float chemistry;
    float maths;
    float english;
};

char currentRole[20], currentUser[50];

// FUNCTION DECLARATIONS
int loginSystem(void);
void mainMenu(void);
void adminMenu(void);
void staffMenu(void);
void userMenu(void);
void guestMenu(void);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

int main(void)
{
    FILE *f = fopen(CREDENTIAL_FILE, "r");
    if (!f)
    {
        f = fopen(CREDENTIAL_FILE, "w");
        fprintf(f, "admin admin123 ADMIN\n");
        fprintf(f, "staff staff123 STAFF\n");
        fprintf(f, "user user123 USER\n");
        fprintf(f, "guest guest123 GUEST\n");
        fclose(f);
        printf("Created default '%s' (admin/admin123).\n", CREDENTIAL_FILE);
    }
    else
        fclose(f);

    if (loginSystem())
        mainMenu();
    else
        printf("Login Failed. Exiting...\n");

    return 0;
}

/* ================= LOGIN ================= */

int loginSystem(void)
{
    char username[50], password[50];
    char u[50], p[50], r[20];

    printf("Username: ");
    scanf("%49s", username);
    printf("Password: ");
    scanf("%49s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp)
    {
        printf("Error: %s not found!\n", CREDENTIAL_FILE);
        return 0;
    }

    while (fscanf(fp, "%49s %49s %19s", u, p, r) == 3)
    {
        if (strcmp(username, u) == 0 && strcmp(password, p) == 0)
        {
            strcpy(currentUser, u);
            strcpy(currentRole, r);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* ================= MENUS ================= */

void mainMenu(void)
{
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else if (strcmp(currentRole, "GUEST") == 0)
        guestMenu();
    else
        printf("Access Denied! Invalid Role.\n");
}

void adminMenu(void)
{
    int choice;
    do
    {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            displayStudents();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            updateStudent();
            break;
        case 5:
            deleteStudent();
            break;
        case 6:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (1);
}

void staffMenu(void)
{
    int choice;
    do
    {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayStudents();
            break;
        case 2:
            searchStudent();
            break;
        case 3:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (1);
}

void userMenu(void)
{
    int choice;
    do
    {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayStudents();
            break;
        case 2:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice!\n");
        }
    } while (1);
}

void guestMenu(void)
{
    printf("\nWelcome GUEST: %s\n", currentUser);
    printf("You have read-only access.\n");
    displayStudents();
}

/* ================= ADD ================= */

void addStudent(void)
{
    struct Student st;
    FILE *f = fopen(STUDENT_FILE, "a");
    if (!f)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Roll Number: ");
    scanf("%31s", st.roll);

    printf("Enter Name: ");
    getchar();
    fgets(st.name, 50, stdin);
    st.name[strcspn(st.name, "\n")] = 0;

    printf("Enter Physics Marks: ");
    scanf("%f", &st.physics);

    printf("Enter Chemistry Marks: ");
    scanf("%f", &st.chemistry);

    printf("Enter Maths Marks: ");
    scanf("%f", &st.maths);

    printf("Enter English Marks: ");
    scanf("%f", &st.english);

    fprintf(f, "%s,%s,%.2f,%.2f,%.2f,%.2f\n",
            st.roll, st.name,
            st.physics, st.chemistry,
            st.maths, st.english);

    fclose(f);
    printf("Student Added Successfully!\n");
}

/* ================= DISPLAY ================= */

void displayStudents(void)
{
    struct Student st;
    FILE *f = fopen(STUDENT_FILE, "r");
    if (!f)
    {
        printf("No data available.\n");
        return;
    }

    printf("\n==================== STUDENT LIST ====================\n");
    printf("%-10s | %-25s | %8s | %8s | %8s | %8s\n",
           "Roll", "Name", "Physics", "Chem", "Maths", "English");
    printf("---------------------------------------------------------------\n");

    int count = 0;
    while (fscanf(f, "%31[^,],%49[^,],%f,%f,%f,%f\n",
                  st.roll, st.name,
                  &st.physics, &st.chemistry,
                  &st.maths, &st.english) == 6)
    {
        printf("%-10s | %-25s | %8.2f | %8.2f | %8.2f | %8.2f\n",
               st.roll, st.name,
               st.physics, st.chemistry, st.maths, st.english);
        count++;
    }

    if (count == 0)
        printf("(No records found)\n");

    printf("---------------------------------------------------------------\n");
    printf("Total records: %d\n", count);

    fclose(f);
}

/* ================= SEARCH ================= */

void searchStudent(void)
{
    struct Student st;
    char r[32];
    int found = 0;

    printf("Enter Roll Number to Search: ");
    scanf("%31s", r);

    FILE *f = fopen(STUDENT_FILE, "r");
    if (!f)
    {
        printf("No data found.\n");
        return;
    }

    while (fscanf(f, "%31[^,],%49[^,],%f,%f,%f,%f\n",
                  st.roll, st.name,
                  &st.physics, &st.chemistry,
                  &st.maths, &st.english) == 6)
    {
        if (strcmp(st.roll, r) == 0)
        {
            printf("\nStudent Found!\n");
            printf("Roll: %s\nName: %s\nPhysics: %.2f\nChemistry: %.2f\nMaths: %.2f\nEnglish: %.2f\n",
                   st.roll, st.name,
                   st.physics, st.chemistry,
                   st.maths, st.english);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student Not Found!\n");

    fclose(f);
}

/* ================= UPDATE ================= */

void updateStudent(void)
{
    struct Student st;
    char r[32];
    int found = 0;

    printf("Enter Roll Number to Update: ");
    scanf("%31s", r);

    FILE *f = fopen(STUDENT_FILE, "r");
    FILE *t = fopen(TEMP_FILE, "w");

    if (!f || !t)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(f, "%31[^,],%49[^,],%f,%f,%f,%f\n",
                  st.roll, st.name,
                  &st.physics, &st.chemistry,
                  &st.maths, &st.english) == 6)
    {
        if (strcmp(st.roll, r) == 0)
        {
            found = 1;

            printf("Current Name: %s\n", st.name);
            printf("Current Marks -> Physics: %.2f, Chemistry: %.2f, Maths: %.2f, English: %.2f\n",
                   st.physics, st.chemistry, st.maths, st.english);

            printf("Enter New Name: ");
            getchar();
            fgets(st.name, 50, stdin);
            st.name[strcspn(st.name, "\n")] = 0;

            printf("Enter New Physics Marks: ");
            scanf("%f", &st.physics);

            printf("Enter New Chemistry Marks: ");
            scanf("%f", &st.chemistry);

            printf("Enter New Maths Marks: ");
            scanf("%f", &st.maths);

            printf("Enter New English Marks: ");
            scanf("%f", &st.english);

            printf("Record Updated!\n");
        }

        fprintf(t, "%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                st.roll, st.name,
                st.physics, st.chemistry,
                st.maths, st.english);
    }

    fclose(f);
    fclose(t);

    remove(STUDENT_FILE);
    rename(TEMP_FILE, STUDENT_FILE);

    if (!found)
        printf("Record not found!\n");
}

/* ================= DELETE ================= */

void deleteStudent(void)
{
    struct Student st;
    char r[32];
    int found = 0;

    printf("Enter Roll Number to Delete: ");
    scanf("%31s", r);

    FILE *f = fopen(STUDENT_FILE, "r");
    FILE *t = fopen(TEMP_FILE, "w");

    if (!f || !t)
    {
        printf("Error opening file!\n");
        if (f)
            fclose(f);
        if (t)
            fclose(t);
        return;
    }

    while (fscanf(f, "%31[^,],%49[^,],%f,%f,%f,%f\n",
                  st.roll, st.name,
                  &st.physics, &st.chemistry,
                  &st.maths, &st.english) == 6)
    {
        if (strcmp(st.roll, r) == 0)
        {
            found = 1;
            printf("Record Deleted!\n");
            // Don't write this record to temp file (skip it)
            continue;
        }

        // Copy all other records
        fprintf(t, "%s,%s,%.2f,%.2f,%.2f,%.2f\n",
                st.roll, st.name,
                st.physics, st.chemistry,
                st.maths, st.english);
    }

    fclose(f);
    fclose(t);

    if (!found)
    {
        printf("Record not found!\n");
        // No deletion happened; you may want to remove temp
        remove(TEMP_FILE);
    }
    else
    {
        // Replace original file with updated temp file
        remove(STUDENT_FILE);
        rename(TEMP_FILE, STUDENT_FILE);
    }
}
