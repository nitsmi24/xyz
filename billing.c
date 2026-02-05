#include <stdio.h>
#include "db.h"
#include "billing.h"

void generate_bill() {
    MYSQL *conn = connect_db();
    int patient_id;
    float amount;
    char date[15];

    printf("Enter Patient ID: ");
    scanf("%d", &patient_id);

    printf("Enter Bill Amount: ");
    scanf("%f", &amount);

    printf("Enter Billing Date (YYYY-MM-DD): ");
    scanf("%s", date);

    char query[300];
    sprintf(query,
        "INSERT INTO billing(patient_id, amount, payment_status, payment_date) "
        "VALUES(%d, %.2f, 'PENDING', '%s')",
        patient_id, amount, date);

    if (mysql_query(conn, query) == 0)
        printf("Bill generated successfully!\n");
    else
        printf("Error generating bill!\n");

    // File logging
    FILE *fp = fopen("logs/system.log", "a");
    fprintf(fp, "Bill generated for Patient ID %d | Amount %.2f\n",
            patient_id, amount);
    fclose(fp);

    close_db(conn);
}
void view_bills() {
    MYSQL *conn = connect_db();
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(conn,
        "SELECT b.bill_id, p.name, b.amount, b.payment_status, b.payment_date "
        "FROM billing b "
        "JOIN patients p ON b.patient_id=p.patient_id");

    res = mysql_store_result(conn);

    printf("\nID | Patient | Amount | Status | Date\n");
    printf("----------------------------------------\n");

    while ((row = mysql_fetch_row(res))) {
        printf("%s | %s | %s | %s | %s\n",
               row[0], row[1], row[2], row[3], row[4]);
    }

    close_db(conn);
}
void update_payment_status() {
    MYSQL *conn = connect_db();
    int bill_id;
    char status[20];

    printf("Enter Bill ID: ");
    scanf("%d", &bill_id);

    printf("Enter Status (PAID / PENDING): ");
    scanf("%s", status);

    char query[200];
    sprintf(query,
        "UPDATE billing SET payment_status='%s' WHERE bill_id=%d",
        status, bill_id);

    if (mysql_query(conn, query) == 0)
        printf("Payment status updated!\n");
    else
        printf("Update failed!\n");

    close_db(conn);
}
void billing_menu() {
    int choice;
    do {
        printf("\n--- Billing & Payments ---\n");
        printf("1. Generate Bill\n");
        printf("2. View Bills\n");
        printf("3. Update Payment Status\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: generate_bill(); break;
            case 2: view_bills(); break;
            case 3: update_payment_status(); break;
            case 0: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 0);
}
