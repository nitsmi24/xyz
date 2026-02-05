#include <stdio.h>
#include "db.h"
#include "doctor.h"

void add_doctor() {
    MYSQL *conn = connect_db();
    char name[100], specialization[100], phone[15];

    printf("Enter Doctor Name: ");
    scanf(" %[^\n]", name);

    printf("Enter Specialization: ");
    scanf(" %[^\n]", specialization);

    printf("Enter Phone: ");
    scanf("%s", phone);

    char query[400];
    sprintf(query,
        "INSERT INTO doctors(name, specialization, phone) "
        "VALUES('%s', '%s', '%s')",
        name, specialization, phone);

    if (mysql_query(conn, query) == 0)
        printf("Doctor added successfully!\n");
    else
        printf("Error adding doctor!\n");

    // Logging
    FILE *fp = fopen("logs/system.log", "a");
    fprintf(fp, "Doctor added: %s (%s)\n", name, specialization);
    fclose(fp);

    close_db(conn);
}
void view_doctors() {
    MYSQL *conn = connect_db();
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(conn, "SELECT * FROM doctors");
    res = mysql_store_result(conn);

    printf("\nID | Name | Specialization | Phone\n");
    printf("------------------------------------\n");

    while ((row = mysql_fetch_row(res))) {
        printf("%s | %s | %s | %s\n",
               row[0], row[1], row[2], row[3]);
    }

    close_db(conn);
}
void update_doctor() {
    MYSQL *conn = connect_db();
    int id;
    char phone[15];

    printf("Enter Doctor ID to update: ");
    scanf("%d", &id);

    printf("Enter new Phone: ");
    scanf("%s", phone);

    char query[300];
    sprintf(query,
        "UPDATE doctors SET phone='%s' WHERE doctor_id=%d",
        phone, id);

    if (mysql_query(conn, query) == 0)
        printf("Doctor updated successfully!\n");
    else
        printf("Update failed!\n");

    close_db(conn);
}
void delete_doctor() {
    MYSQL *conn = connect_db();
    int id;

    printf("Enter Doctor ID to delete: ");
    scanf("%d", &id);

    char query[200];
    sprintf(query,
        "DELETE FROM doctors WHERE doctor_id=%d", id);

    if (mysql_query(conn, query) == 0)
        printf("Doctor deleted successfully!\n");
    else
        printf("Delete failed!\n");

    close_db(conn);
}
void doctor_menu() {
    int choice;
    do {
        printf("\n--- Doctor Management ---\n");
        printf("1. Add Doctor\n");
        printf("2. View Doctors\n");
        printf("3. Update Doctor\n");
        printf("4. Delete Doctor\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: add_doctor(); break;
            case 2: view_doctors(); break;
            case 3: update_doctor(); break;
            case 4: delete_doctor(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
}
