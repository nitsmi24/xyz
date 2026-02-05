#include "db.h"
#include <stdio.h>

MYSQL* connect_db() {
    MYSQL *conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, "localhost", "root", "password",
                             "hospital_db", 0, NULL, 0)) {
        printf("Database connection failed!\n");
        return NULL;
    }
    return conn;
}

void close_db(MYSQL *conn) {
    mysql_close(conn);
}
