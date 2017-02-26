#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "/usr/include/mysql/mysql.h"
#include <wiringPi.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "A"
#define CHOP(x) x[strlen(x) - 1] = ' '

#define HIGH 1
#define LOW 0
#define LED_pin 4

int main() {
    /*****************mysql************/
    MYSQL       *connection=NULL, conn;
    MYSQL_RES   *sql_result;
    MYSQL_ROW   sql_row;
    int       query_stat; 
    char query[255];

    char buffer[20];
    long longval;
    unsigned long* lengths;

    mysql_init(&conn);
    connection = mysql_real_connect(&conn, DB_HOST,
			DB_USER, DB_PASS,
	    DB_NAME, 3306,
	    (char *)NULL, 0);

    if (connection == NULL) {
			fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
			return 1;
    }

		if(wiringPiSetup() == -1) return 1;

		pinMode(LED_pin, OUTPUT);

    while(1) {
			query_stat = mysql_query(connection, "select * from table1");
			if (query_stat != 0){
	    	fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
	    	return 1;
			}

		sql_result = mysql_store_result(connection);

		if( (sql_row = mysql_fetch_row(sql_result)) != NULL ){
	  	  printf("idx:%s go:%s\n", sql_row[0], sql_row[1]);
		}

		lengths = mysql_fetch_lengths(sql_result);

		if(lengths[1]!=0){
	    memcpy(buffer, sql_row[1], lengths[1]);
	    buffer[lengths[1]]=0;
	    sscanf(sql_row[1], "%d", &longval);
		}
		printf("longval: %d\n", longval);
		mysql_free_result(sql_result);

		if(longval == 1) // alarm on
		{
	    printf("alarm on\n");
	    digitalWrite(LED_pin, HIGH);
	    delay(10);
		}
		else //alarm off
		{
	    printf("alarm off\n");
	    digitalWrite(LED_pin, LOW);
	    delay(10);
		}	
	}

exit:
	mysql_close(connection);

	return  0;

}
