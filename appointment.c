#include <stdio.h>
#include "db.h"
#include "appointment.h"

void add_appointment() {
    MYSQL *conn = connect_db();
    int patient_id, doctor_id;
    char date[15], time[10];

    printf("Enter Patient ID: ");
    scanf("%d", &patient_id);

    printf("Enter Doctor ID: ");
    scanf("%d", &doctor_id);

    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%s", date);

    printf("Enter Time (HH:MM): ");
    scanf("%s", time);

    // Conflict check
    char check_query[300];
    sprintf(check_query,
        "SELECT * FROM appointments "
        "WHERE doctor_id=%d AND date='%s' AND time='%s'",
        doctor_id, date, time);

    mysql_query(conn, check_query);
    MYSQL_RES *res = mysql_store_result(conn);

    if (mysql_num_rows(res) > 0) {
        printf("Doctor already has an appointment at this time!\n");
        close_db(conn);
        return;
    }

    char query[400];
    sprintf(query,
        "INSERT INTO appointments(patient_id, doctor_id, date, time, status) "
        "VALUES(%d, %d, '%s', '%s', 'SCHEDULED')",
        patient_id, doctor_id, date, time);

    if (mysql_query(conn, query) == 0)
        printf("Appointment scheduled successfully!\n");
    else
        printf("Error scheduling appointment!\n");

    // Logging
    FILE *fp = fopen("logs/system.log", "a");
    fprintf(fp, "Appointment scheduled: Patient %d with Doctor %d\n",
            patient_id, doctor_id);
    fclose(fp);

    close_db(conn);
}
void view_appointments() {
    MYSQL *conn = connect_db();
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(conn,
        "SELECT a.appointment_id, p.name, d.name, a.date, a.time, a.status "
        "FROM appointments a "
        "JOIN patients p ON a.patient_id=p.patient_id "
        "JOIN doctors d ON a.doctor_id=d.doctor_id");

    res = mysql_store_result(conn);

    printf("\nID | Patient | Doctor | Date | Time | Status\n");
    printf("-------------------------------------------------\n");

    while ((row = mysql_fetch_row(res))) {
        printf("%s | %s | %s | %s | %s | %s\n",
               row[0], row[1], row[2], row[3], row[4], row[5]);
    }

    close_db(conn);
}
void update_appointment_status() {
    MYSQL *conn = connect_db();
    int id;
    char status[20];

    printf("Enter Appointment ID: ");
    scanf("%d", &id);

    printf("Enter Status (COMPLETED / CANCELLED): ");
    scanf("%s", status);

    char query[300];
    sprintf(query,
        "UPDATE appointments SET status='%s' WHERE appointment_id=%d",
        status, id);

    if (mysql_query(conn, query) == 0)
        printf("Appointment updated successfully!\n");
    else
        printf("Update failed!\n");

    close_db(conn);
}
void appointment_menu() {
    int choice;
    do {
        printf("\n--- Appointment Management ---\n");
        printf("1. Add Appointment\n");
        printf("2. View Appointments\n");
        printf("3. Update Appointment Status\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: add_appointment(); break;
            case 2: view_appointments(); break;
            case 3: update_appointment_status(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
}
