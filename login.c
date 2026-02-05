#include <stdio.h>
#include <string.h>

#include "db.h"
#include "patient.h"
#include "doctor.h"
#include "appointment.h"
#include "medical.h"
#include "billing.h"

void login() {
    char username[50], password[50];
    char role[20];

    MYSQL *conn = connect_db();
    if (conn == NULL) {
        printf("Database connection failed!\n");
        return;
    }

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    char query[300];
    sprintf(query,
        "SELECT role FROM users WHERE username='%s' AND password='%s'",
        username, password);

    if (mysql_query(conn, query) != 0) {
        printf("Query failed!\n");
        close_db(conn);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);

    if (mysql_num_rows(res) == 1) {
        MYSQL_ROW row = mysql_fetch_row(res);
        strcpy(role, row[0]);

        printf("\nLogin successful! Role: %s\n", role);


        if (strcmp(role, "ADMIN") == 0) {
            doctor_menu();
            int choice;
            do {
                printf("\n--- ADMIN MENU ---\n");
                printf("1. Patient Management\n");
                printf("2. Doctor Management\n");
                printf("3. Appointment Management\n");
                printf("4. Medical Records\n");
                printf("5. Billing\n");
                printf("0. Logout\n");
                printf("Enter choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1: patient_menu(); break;
                    case 2: doctor_menu(); break;
                    case 3: appointment_menu(); break;
                    case 4: medical_menu(); break;
                    case 5: billing_menu(); break;
                    case 0: printf("Logging out...\n"); break;
                    default: printf("Invalid choice!\n");
                }
            } while (choice != 0);
        }

        else if (strcmp(role, "DOCTOR") == 0) {
            int choice;
            do {
                printf("\n--- DOCTOR MENU ---\n");
                printf("1. Medical Records\n");
                printf("2. View Appointments\n");
                printf("0. Logout\n");
                printf("Enter choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1: medical_menu(); break;
                    case 2: view_appointments(); break;
                    case 0: printf("Logging out...\n"); break;
                    default: printf("Invalid choice!\n");
                }
            } while (choice != 0);
        }

        else if (strcmp(role, "RECEPTIONIST") == 0) {
            int choice;
            do {
                printf("\n--- RECEPTIONIST MENU ---\n");
                printf("1. Patient Management\n");
                printf("2. Appointment Management\n");
                printf("3. Billing\n");
                printf("0. Logout\n");
                printf("Enter choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1: patient_menu(); break;
                    case 2: appointment_menu(); break;
                    case 3: billing_menu(); break;
                    case 0: printf("Logging out...\n"); break;
                    default: printf("Invalid choice!\n");
                }
            } while (choice != 0);
        }

        else {
            printf("Unknown role! Contact admin.\n");
        }
    }
    else {
        printf("Invalid username or password!\n");
    }

    mysql_free_result(res);
    close_db(conn);
}
