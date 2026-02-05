#include <stdio.h>
#include "db.h"
#include "medical.h"

void add_medical_record() {
    MYSQL *conn = connect_db();
    int patient_id, doctor_id;
    char diagnosis[300], prescription[300], date[15];

    printf("Enter Patient ID: ");
    scanf("%d", &patient_id);

    printf("Enter Doctor ID: ");
    scanf("%d", &doctor_id);

    printf("Enter Diagnosis: ");
    scanf(" %[^\n]", diagnosis);

    printf("Enter Prescription: ");
    scanf(" %[^\n]", prescription);

    printf("Enter Visit Date (YYYY-MM-DD): ");
    scanf("%s", date);

    char query[600];
    sprintf(query,
        "INSERT INTO medical_records(patient_id, doctor_id, diagnosis, prescription, visit_date) "
        "VALUES(%d, %d, '%s', '%s', '%s')",
        patient_id, doctor_id, diagnosis, prescription, date);

    if (mysql_query(conn, query) == 0)
        printf("Medical record added successfully!\n");
    else
        printf("Error adding medical record!\n");

    // File report (important)
    FILE *fp = fopen("reports/patient_reports.txt", "a");
    fprintf(fp,
        "Patient ID: %d | Doctor ID: %d | Date: %s\nDiagnosis: %s\nPrescription: %s\n\n",
        patient_id, doctor_id, date, diagnosis, prescription);
    fclose(fp);

    close_db(conn);
}
void view_medical_records() {
    MYSQL *conn = connect_db();
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(conn,
        "SELECT m.record_id, p.name, d.name, m.diagnosis, m.prescription, m.visit_date "
        "FROM medical_records m "
        "JOIN patients p ON m.patient_id=p.patient_id "
        "JOIN doctors d ON m.doctor_id=d.doctor_id");

    res = mysql_store_result(conn);

    printf("\nID | Patient | Doctor | Date | Diagnosis | Prescription\n");
    printf("----------------------------------------------------------\n");

    while ((row = mysql_fetch_row(res))) {
        printf("%s | %s | %s | %s | %s | %s\n",
               row[0], row[1], row[2], row[5], row[3], row[4]);
    }

    close_db(conn);
}
void medical_menu() {
    int choice;
    do {
        printf("\n--- Medical Records ---\n");
        printf("1. Add Medical Record\n");
        printf("2. View Medical Records\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: add_medical_record(); break;
            case 2: view_medical_records(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
}
