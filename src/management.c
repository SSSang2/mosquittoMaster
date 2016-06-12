#include "management.h"

void deleteAllSlave(){
	MYSQL       *connection=NULL, conn;
        MYSQL_RES   *sql_result;
        MYSQL_ROW   sql_row;
        int       query_stat;

        char query[255];
        int result =0;
        mysql_init(&conn);
	
        connection = mysql_real_connect(&conn, DB_HOST,
                        DB_USER, DB_PASS,
                        DB_NAME, 3306,
                        (char *)NULL, 0);

        if (connection == NULL)
        {
                fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
                return 1;
        }

        sprintf(query, "delete from slaveList;");

        query_stat = mysql_query(connection, query);
        if (query_stat != 0)
        {
                fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
                return 1;
        }
	mysql_free_result(sql_result);
	mysql_close(connection);


}
void updateIPNode(const char _name[], int node){
        MYSQL       *connection=NULL, conn;
        MYSQL_RES   *sql_result;
        MYSQL_ROW   sql_row;
        int       query_stat;

        char query[255];
        int result =0;
        struct client_config *pub_config;
        struct client_config *sub_config;
        int port_index =5;
        char outer_port_tmp[5];
        int outer_port =0;
	int bChange=0, aChange = 0;

        mysql_init(&conn);

        connection = mysql_real_connect(&conn, DB_HOST,
                        DB_USER, DB_PASS,
			DB_NAME, 3306,
			(char *)NULL, 0);

	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	sprintf(query, "select node from slaveList "
			"where name = '%s';",
			_name );

	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	sql_result = mysql_store_result(connection);
	while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
	{
		if(atoi(sql_row[0]) == MAX_CLIENT)
			bChange = atoi(sql_row[0]);
	}


	while(_name[port_index]){
		outer_port_tmp[port_index-5] = _name[port_index];
		port_index++;
	}
	outer_port = 10010 + atoi(outer_port_tmp);
	sprintf(query, "update slaveList set "
			"node = '%d' where name = '%s';",
			node, _name );


	printf("===========================================\n");
	printf("------------ updateNode -----------------\n");
	printf("#name   : %s\n", _name);
	printf("#node   : %d\n", node);
	printf("-------------------------------------------\n");
	printf("===========================================\n");

	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	if( !(node ==MAX_CLIENT && bChange==MAX_CLIENT) ){
		if(node == MAX_CLIENT){
			char name[10] = "Slave";
			char index[3];//(slaveCount+1) -'0';

			_itoa(*sCount+1, index);
			strcat(name,index);
			printf("Resgist New Slave : %s, %d, %d\n", name, bChange, node);
			if(!checkSlaveId(name))
				registSlave(name);
		}
	}
	mysql_free_result(sql_result);
	mysql_close(connection);

}

void updateIPAddr(const char _name[], const char _ip[], const int port, const int sock){
	MYSQL       *connection=NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;

	char name[255];
	char ip[255];
	int node;
	char query[255];
	int result =0;
	struct client_config *pub_config;
	struct client_config *sub_config;
	int port_index =5;
	char outer_port_tmp[5];
	int outer_port =0;

	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST,
			DB_USER, DB_PASS,
			DB_NAME, 3306,
			(char *)NULL, 0);

	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	sprintf(name, _name);
	sprintf(ip, _ip);
	while(_name[port_index]){
		outer_port_tmp[port_index-5] = _name[port_index];
		port_index++;
	}	
	outer_port = 10010 + atoi(outer_port_tmp);
	sprintf(query, "update slaveList set "
			"ip = '%s', port = '%d', outer_port = '%d', sock = '%d' where name = '%s';",
			ip, port, outer_port,sock, name );

	printf("===========================================\n");
	printf("------------ updateIPAddr -----------------\n");
	printf("#name	: %s\n", name);
	printf("#ip	: %s\n", ip);
	printf("#port	: %d\n", port);
	printf("#outP	: %d\n", outer_port);
	printf("#sock	: %d\n", sock);
	printf("-------------------------------------------\n");
	printf("===========================================\n");

	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	mysql_free_result(sql_result);
	mysql_close(connection);
}
int registSlave(const char _name[]){

	MYSQL       *connection=NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;

	char name[255];
	int node;
	char query[255];
	char command[255];
	int result =0;
	printf("\n=====================================\n");
	printf("-------------------------------------\n");
	printf("#Regist Slave Brocker!\n");
	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST,
			DB_USER, DB_PASS,
			DB_NAME, 3306,
			(char *)NULL, 0);

	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	sprintf(name, _name);
	sprintf(query, "insert into slaveList values "
			"('%s', '%d', '%s', '%d', '%d', '%d');",
			name, 0, NULL, 0, 0, 0);
	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	strcpy(command, "docker start ");
	strcat(command, _name);
	printf("Commad : %s\n", command);
	system(command);

	strcpy(command, "docker exec ");
	strcat(command, _name);
	strcat(command, " mosquitto &");	
	printf("Commad : %s\n", command);
	system(command);
	*sCount = *sCount+1;
	printf("-------------------------------------\n");
	printf("=====================================\n\n");
    mysql_free_result(sql_result);
	mysql_close(connection);
}
int checkSlaveId(const char _name[])
{
	MYSQL       *connection=NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;

	char name[255];
	char ip[255];
	int node;
	char query[255];
	int result =0;
	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST,
			DB_USER, DB_PASS,
                                    DB_NAME, 3306,
                                    (char *)NULL, 0);

    if (connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 1;
    }

    printf("Name :");
	sprintf(name, _name);
    //fgets(name, 255, stdin);
    //CHOP(name);

    sprintf(query, "select exists (select * from slaveList where name = '%s');", name);
    query_stat = mysql_query(connection, query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    sql_result = mysql_store_result(connection);
    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
	if(atoi(sql_row[0]) == 1){
		printf("Already exist name!!\n");
	}
	else if (atoi(sql_row[0]) == 0){
		printf("Input new Slave : %s\n", name);
	}
	result = atoi(sql_row[0]);
        //printf("%s\n", sql_row[0]);
    }
    mysql_free_result(sql_result);

    mysql_close(connection);

	return result;
}
void getSlaveInfo(char _ip[]){
	MYSQL       *connection=NULL, conn;
	MYSQL_RES   *sql_result;
	MYSQL_ROW   sql_row;
	int       query_stat;
	int	get=0;
	char query[255];
	mysql_init(&conn);

	connection = mysql_real_connect(&conn, DB_HOST,
			DB_USER, DB_PASS,
			DB_NAME, 3306,
			(char *)NULL, 0);

	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	sprintf(query, "select node, outer_port from slaveList where node = (select MIN(node) from slaveList);");
	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}
	sql_result = mysql_store_result(connection);
	while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
	{
		printf("\n\n getInfo : %d, %d\n", atoi(sql_row[0]), MAX_CLIENT);
		if( atoi(sql_row[0]) < MAX_CLIENT){
			strcat(_ip, ":");
			strcat(_ip, sql_row[1]);
			get=1;
			break;
		}
	}
/*
	if(get==0){
		char name[10] = "Slave";
		char index[3];//(slaveCount+1) -'0';	
		int port = 10011 + sCount;
		char _port[6];//  (char)(port) -'0';

		_itoa(port, _port);
		_itoa(sCount+1, index);
		strcat(name,index);
		printf("Resgist New Slave : %s\n", name);
		if(!checkSlaveId(name))
			registSlave(name);
		strcat(_ip, ":");
		strcat(_ip, _port);
	}

*/
	mysql_free_result(sql_result);
	mysql_close(connection);

}
void _itoa(int a, char* buf){
	int tmp = a;
	char result[100];
	int i=0, j=0;
	int b=0;
	memset(result, NULL, sizeof(result));
	if(a>0){
		while(tmp>0){
			b = tmp % 10;
			result[i++] = (char)(b+'0');
			tmp /= 10;
		}
		for(j=0; j<i; j++)
			buf[j] = result[i-j-1];
		buf[i] = '\0';
	}
	else{
		buf[0] = '0';
		buf[1] = '\0';
	}
}
