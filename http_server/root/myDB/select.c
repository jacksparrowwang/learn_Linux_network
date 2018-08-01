#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

int main()
{
    // 建立句柄
    MYSQL* connect_fd = mysql_init(NULL);
    // 建立连接
    if (mysql_real_connect(connect_fd, "127.0.0.1", "root", "", "py", 3306, NULL, 0) == NULL)
    {
        fprintf(stderr, "mysql_readl_connect failed\n");
        return 1;
    }
    fprintf(stderr, "mysql_readl_connect OK\n");

    const char* sql = "select * from server";
    int ret = mysql_query(connect_fd, sql);
    if (ret < 0)
    {
        fprintf(stderr, "mysql_query failed\n");
        return 1;
    }

    // 遍历结果集合
    // 拿到result就可以通过result来进行获取
    MYSQL_RES* result = mysql_store_result(connect_fd);
    if (result == NULL)
    {
        fprintf(stderr, "mysql_store_result failed\n");
        return 1;
    }

    printf("<html>");
    // 得到有几行几列
    int rows = mysql_num_rows(result);
    int fields = mysql_num_fields(result);

    // 这是获取到一个表头
    MYSQL_FIELD* f = mysql_fetch_field(result);
    while (f != NULL)
    {
        // f->name 表示这列的具体列名字
        printf("%s\t", f->name);
        f = mysql_fetch_field(result);
    }
    printf("<br>");
    int i = 0;
    for (; i < rows; ++i)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        int j = 0;
        for (; j < fields; ++j)
        {
            printf("%s\t", row[j]);
        }
    }
    printf("<br>");

    printf("</html>");
    mysql_close(connect_fd);
    return 0;
}

