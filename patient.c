#include <stdio.h>
#include "db.h"
#include "patient.h"

void add_patient() {
    MYSQL *conn = connect_db();
    char name[100], gender[10], phone[15], address[200];
    int age;

    printf("Enter Name: ");
    scanf(" %[^\n]", name);

    printf("Enter Age: ");
    scanf("%d", &age);

    printf("Enter Gender: ");
    scanf("%s", gender);

    printf("Enter Phone: ");
    scanf("%s", phone);

    printf("Enter Address: ");
    scanf(" %[^\n]", address);

    char query[500];
    sprintf(query,
        "INSERT INTO patients(name, age, gender, phone, address) "
        "VALUES('%s', %d, '%s', '%s', '%s')",
        name, age, gender, phone, address);

    if (mysql_query(conn, query) == 0)
        printf("Patient added successfully!\n");
    else
        printf("Error adding patient!\n");

    close_db(conn);
}
void view_patients() {
    MYSQL *conn = connect_db();
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(conn, "SELECT * FROM patients");
    res = mysql_store_result(conn);

    printf("\nID | Name | Age | Gender | Phone | Address\n");
    printf("---------------------------------------------\n");

    while ((row = mysql_fetch_row(res))) {
        printf("%s | %s | %s | %s | %s | %s\n",
               row[0], row[1], row[2], row[3], row[4], row[5]);
    }

    close_db(conn);
}
void update_patient() {
    MYSQL *conn = connect_db();
    int id, age;
    char phone[15];

    printf("Enter Patient ID to update: ");
    scanf("%d", &id);

    printf("Enter new Age: ");
    scanf("%d", &age);

    printf("Enter new Phone: ");
    scanf("%s", phone);

    char query[300];
    sprintf(query,
        "UPDATE patients SET age=%d, phone='%s' WHERE patient_id=%d",
        age, phone, id);

    if (mysql_query(conn, query) == 0)
        printf("Patient updated successfully!\n");
    else
        printf("Update failed!\n");

    close_db(conn);
}
void delete_patient() {
    MYSQL *conn = connect_db();
    int id;

    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);

    char query[200];
    sprintf(query,
        "DELETE FROM patients WHERE patient_id=%d", id);

    if (mysql_query(conn, query) == 0)
        printf("Patient deleted successfully!\n");
    else
        printf("Delete failed!\n");

    close_db(conn);
}
void patient_menu() {
    int choice;
    do {
        printf("\n--- Patient Management ---\n");
        printf("1. Add Patient\n");
        printf("2. View Patients\n");
        printf("3. Update Patient\n");
        printf("4. Delete Patient\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: add_patient(); break;
            case 2: view_patients(); break;
            case 3: update_patient(); break;
            case 4: delete_patient(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
}
