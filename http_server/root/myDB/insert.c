#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include "commit.h"

int main()
{
    // 获取数据
    char buf[1024*10] = {0};
    if (GetQueryString(buf) < 0)
    {
        fprintf(stderr, "GetQueryString failed\n");
        return 1;
    }
    int id = 0;
    char name[100] = {0};
    sscanf(buf, "id=%d&name=%s", &id, name);

    MYSQL* connect_fd = mysql_init(NULL);
    if (mysql_real_connect(connect_fd, "127.0.0.1", "root", "", "py", 3306, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_real_connect failed\n");
        return 1;
    }
    char sql[1024] = {0};
    sprintf(sql, "insert into server values (%d, '%s')", id, name);
    int ret = mysql_query(connect_fd,sql);
    if (ret < 0)
    {
        fprintf(stderr, "mysql_query failed\n");
        return 1;
    }
    printf("<html>insert seccess </html>");

    mysql_close(connect_fd);
    return 0;
}

