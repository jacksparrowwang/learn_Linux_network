#include <stdio.h>
#include <mysql.h>

void MYSQLInsert(char* IP, int port)
{
    MYSQL * fd = mysql_init(NULL);
    if (mysql_real_connect(fd, "127.0.0.1", "root", "", "client", 3306, NULL, 0) == NULL)
    {
        printf("connect failed!\n");
        return;
    }
    printf("connect mysql success!\n");

//    const char* sql = "insert into client_info (IP, port) values (\"192.168.1.1i\", \"234\") ";
    char sql[1024];

    sprintf(sql, "insert into client_info (IP, port) value (\"%s\", \"%d\")", IP, port);
    mysql_query(fd, sql);

    mysql_close(fd);
}

/* int main() */
/* { */
/*     printf("%s \n", mysql_get_client_info()); */
/*     Insert(); */
/*     return 0; */
/* } */
