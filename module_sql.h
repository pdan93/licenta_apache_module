char *Module_SQL_server = "localhost";
char *Module_SQL_user = "root";
char *Module_SQL_password = "Honey230793"; /* set me first */
char *Module_SQL_database = "honeypot";

escape_and_add(MYSQL *conn,char * query,char * whattoadd,int comma) {
	sprintf(query + strlen(query),"'");
	if (whattoadd!=NULL)
		mysql_real_escape_string(conn,query + strlen(query),whattoadd,strlen(whattoadd));
	if (comma==1)
		sprintf(query + strlen(query),"',");
		else
		sprintf(query + strlen(query),"'");
}

static void mysql_log(request_rec *r) {
	
	
	
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn, Module_SQL_server,
		 Module_SQL_user, Module_SQL_password, Module_SQL_database, 0, NULL, 0)) {
	  //printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	char query[50000];
	query[0]=0;
	strcat(query,"INSERT INTO `logs`(`id`, `is_attack`, `attack_type`, `specific_attack_type`, `timestamp`, `r_protocol`, `r_hostname`, `r_status_line`, `r_method`, `r_range`, `r_content_type`, `r_handler`, `r_content_encoding`, `r_user`, `r_ap_auth_type`, `r_unparsed_uri`, `r_uri`, `r_filename`, `r_canonical_filename`, `r_path_info`, `r_args`, `r_log_id`, `r_useragent_ip`, `r_request_time`, `r_allowed`, `r_mtime`, `r_UA_hostname`, `r_UA_servname`, `r_UA_port`, `r_UA_ipaddr_len`, `r_UA_addr_str_len`, `r_UA_family`, `c_client_ip`, `c_remote_host`, `c_remote_logname`, `c_local_ip`, `c_local_host`, `c_log_id`, `c_CA_hostname`, `c_CA_servname`, `c_CA_port`, `c_CA_ipaddr_len`, `c_CA_addr_str_len`, `c_CA_family`, `s_defn_name`, `s_server_scheme`, `s_server_admin`, `s_server_hostname`, `s_path`, `s_P_short_name`, `s_P_argv`, `s_port`, `s_timeout`, `s_keep_alive_timeout`, `s_keep_alive_max`, `s_keep_alive`, `s_limit_req_line`, `s_limit_req_fieldsize`, `s_limit_req_fields`, `headers_in`, `headers_out`, `request_body`, `user_post`, `pass_post`) VALUES (NULL,");
	
	if (AttackType>0)
		sprintf(query + strlen(query),"%d,",1);
		else
		sprintf(query + strlen(query),"%d,",0);
	
	sprintf(query + strlen(query),"%d,",AttackType);
	sprintf(query + strlen(query),"%d,",SpecificAttackType);
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	sprintf(query + strlen(query),"'%d-%d-%d %d:%d:%d',", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	
	escape_and_add(conn,query,r->protocol,1);
	escape_and_add(conn,query,r->hostname,1);
	escape_and_add(conn,query,r->status_line,1);
	escape_and_add(conn,query,r->method,1);
	escape_and_add(conn,query,r->range,1);
	escape_and_add(conn,query,r->content_type,1);
	escape_and_add(conn,query,r->handler,1);
	escape_and_add(conn,query,r->content_encoding,1);
	escape_and_add(conn,query,r->user,1);
	escape_and_add(conn,query,r->ap_auth_type,1);
	escape_and_add(conn,query,r->unparsed_uri,1);
	escape_and_add(conn,query,r->uri,1);
	escape_and_add(conn,query,r->filename,1);
	escape_and_add(conn,query,r->canonical_filename,1);
	escape_and_add(conn,query,r->path_info,1);
	escape_and_add(conn,query,r->args,1);
	escape_and_add(conn,query,r->log_id,1);
	escape_and_add(conn,query,r->useragent_ip,1);
	sprintf(query + strlen(query),"%d,",r->request_time);
	sprintf(query + strlen(query),"%d,",r->allowed);
	sprintf(query + strlen(query),"%d,",r->mtime);
	
	apr_sockaddr_t* useragent_addr= r->useragent_addr;
	escape_and_add(conn,query,useragent_addr->hostname,1);
	escape_and_add(conn,query,useragent_addr->servname,1);
	sprintf(query + strlen(query),"%d,",useragent_addr->port);
	sprintf(query + strlen(query),"%d,",useragent_addr->ipaddr_len);
	sprintf(query + strlen(query),"%d,",useragent_addr->addr_str_len);
	sprintf(query + strlen(query),"%d,",useragent_addr->family);
	
	conn_rec* connection= r->connection;
	escape_and_add(conn,query,connection->client_ip,1);
	escape_and_add(conn,query,connection->remote_host,1);
	escape_and_add(conn,query,connection->remote_logname,1);
	escape_and_add(conn,query,connection->local_ip,1);
	escape_and_add(conn,query,connection->local_host,1);
	escape_and_add(conn,query,connection->log_id,1);
	
	apr_sockaddr_t * client_addr = connection->client_addr;	
	escape_and_add(conn,query,client_addr->hostname,1);
	escape_and_add(conn,query,client_addr->servname,1);
	sprintf(query + strlen(query),"%d,",client_addr->port);
	sprintf(query + strlen(query),"%d,",client_addr->ipaddr_len);
	sprintf(query + strlen(query),"%d,",client_addr->addr_str_len);
	sprintf(query + strlen(query),"%d,",client_addr->family);
	
	server_rec * server= r->server;
	escape_and_add(conn,query,server->defn_name,1);
	escape_and_add(conn,query,server->server_scheme,1);
	escape_and_add(conn,query,server->server_admin,1);
	escape_and_add(conn,query,server->server_hostname,1);
	escape_and_add(conn,query,server->path,1);
	
	
	process_rec * process= server->process;
	escape_and_add(conn,query,process->short_name,1);
	escape_and_add(conn,query,process->argv,1);
	
	apr_port_t * port= server->port;
	sprintf(query + strlen(query),"%d,",port);
		
	sprintf(query + strlen(query),"%d,",server->timeout);
	sprintf(query + strlen(query),"%d,",server->keep_alive_timeout);
	sprintf(query + strlen(query),"%d,",server->keep_alive_max);
	sprintf(query + strlen(query),"%d,",server->keep_alive);
	sprintf(query + strlen(query),"%d,",server->limit_req_line);
	sprintf(query + strlen(query),"%d,",server->limit_req_fieldsize);
	sprintf(query + strlen(query),"%d,",server->limit_req_fields);
	
	char headers_in[10000];
	char headers_out[10000];
	
	printtable(r,r->headers_in,headers_in);
	printtable(r,r->headers_out,headers_out);
	escape_and_add(conn,query,headers_in,1);//headers_in
	escape_and_add(conn,query,headers_out,1);//headers_out`
	if (input_has_work!=0)//request body if any
		{
		escape_and_add(conn,query,input_buffer,1);
		struct post_body pb = break_post_body(input_buffer);
		char user_email[255],pass[255];
		user_email[0]=0;pass[0]=0;
		for (int i=0; i<pb.nr; i++)
			{
			if (strcmp(pb.keys[i],"email")==0)
				strcat(user_email,pb.values[i]);
			if (strcmp(pb.keys[i],"username")==0)
				strcat(user_email,pb.values[i]);
			if (strcmp(pb.keys[i],"password")==0)
				strcat(pass,pb.values[i]);
			}
		escape_and_add(conn,query,user_email,1);
		escape_and_add(conn,query,pass,0);
		
		}
		else
		{
		escape_and_add(conn,query,"",1);
		escape_and_add(conn,query,"",1);
		escape_and_add(conn,query,"",0);
		}
	
	sprintf(query + strlen(query),");");
	
	
	if (mysql_query(conn, query)) {
		
	  //printf("%s\n", mysql_error(conn));
	  log_text(mysql_error(conn));
	  //exit(1);
	}
	/*
	// send SQL query 
	if (mysql_query(conn, "show tables")) {
	  printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	res = mysql_use_result(conn);
	// output table name 
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
	  printf("%s \n", row[0]);
	// close connection 
	mysql_free_result(res);*/
	mysql_close(conn);
}

int get_last_guessings(request_rec *r) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn, Module_SQL_server,
		 Module_SQL_user, Module_SQL_password, Module_SQL_database, 0, NULL, 0)) {
	  //printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	char query[50000];
	query[0]=0;
	int count=0;
	//SELECT COUNT(*) as 'count' FROM `logs` WHERE (user_post!='' OR pass_post!='') AND r_useragent_ip='89.136.122.93' AND timestamp>'2016-06-09 00:38:55'
	strcat(query,"SELECT COUNT(*) as 'count' FROM `logs` WHERE (user_post!='' OR pass_post!='') AND r_useragent_ip='");
	sprintf(query + strlen(query),"%s' AND timestamp>'",r->useragent_ip);
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	rawtime -= 60*1;//last 5 minutes
	timeinfo = localtime ( &rawtime );
	sprintf(query + strlen(query),"%d-%d-%d %d:%d:%d'", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		
	
	if (mysql_query(conn, query)) {
		  //printf("%s\n", mysql_error(conn));
		  //exit(1);
		}
		else
		{
		res = mysql_use_result(conn);
		// output table name 
		
		while ((row = mysql_fetch_row(res)) != NULL)
			{
			count = atoi(row[0]);
			}
		  //printf("%s \n", row[0]);
	  
		// close connection 
		mysql_free_result(res);
		}
	mysql_close(conn);
	return count;
}


int get_last_requests(request_rec *r) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn, Module_SQL_server,
		 Module_SQL_user, Module_SQL_password, Module_SQL_database, 0, NULL, 0)) {
	  //printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	char query[50000];
	query[0]=0;
	int count=0;
	//SELECT COUNT(*) as 'count' FROM `logs` WHERE (user_post!='' OR pass_post!='') AND r_useragent_ip='89.136.122.93' AND timestamp>'2016-06-09 00:38:55'
	strcat(query,"SELECT COUNT(*) as 'count' FROM `logs` WHERE r_useragent_ip='");
	sprintf(query + strlen(query),"%s' AND timestamp>'",r->useragent_ip);
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	rawtime -= 30*1;//last 30 secs
	timeinfo = localtime ( &rawtime );
	sprintf(query + strlen(query),"%d-%d-%d %d:%d:%d'", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		
	
	if (mysql_query(conn, query)) {
		  //printf("%s\n", mysql_error(conn));
		  //exit(1);
		}
		else
		{
		res = mysql_use_result(conn);
		// output table name 
		
		while ((row = mysql_fetch_row(res)) != NULL)
			{
			count = atoi(row[0]);
			}
		  //printf("%s \n", row[0]);
	  
		// close connection 
		mysql_free_result(res);
		}
	mysql_close(conn);
	return count;
}



int verify_ip(request_rec *r) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn, Module_SQL_server,
		 Module_SQL_user, Module_SQL_password, Module_SQL_database, 0, NULL, 0)) {
	  //printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	char * query = apr_pcalloc(r->pool,sizeof(char)*1000);
	query[0]=0;
	int count=0;
	//SELECT COUNT(*) as 'count' FROM `logs` WHERE (user_post!='' OR pass_post!='') AND r_useragent_ip='89.136.122.93' AND timestamp>'2016-06-09 00:38:55'
	strcat(query,"SELECT COUNT(*) as 'counnt' FROM `ips` WHERE ip='");
	sprintf(query + strlen(query),"%s'",r->useragent_ip);
	
	
	if (mysql_query(conn, query)) {
		  //printf("%s\n", mysql_error(conn));
		  //exit(1);
		}
		else
		{
		res = mysql_use_result(conn);
		// output table name 
		
		while ((row = mysql_fetch_row(res)) != NULL)
			{
			count = atoi(row[0]);
			}
		  //printf("%s \n", row[0]);
	  
		// close connection 
		mysql_free_result(res);
		}
	if (count==0)//ip doesn't exist and we have to call dnsverifier
		{
		char buf[1000];
		FILE *fp;
		char path[1035];
		char command[1000];
		command[0]=0;
		strcat(command,"php -f /licenta/dnsverify.php -- '");
		sprintf(command + strlen(command),"%s'",r->useragent_ip);

		/* Open the command for reading. */
		fp = popen(command, "r");
		if (fp == NULL) {
		exit(1);
		}
		buf[0]=0;
		int threat = 0;
		fscanf(fp,"%s",&buf);
		fscanf(fp,"%d",&threat);
		
		/* close */
		pclose(fp);
		
		//WE NOW SAVE THE IP DATA
		query[0]=0;
		strcat(query,"INSERT INTO ips (`id`,`ip`,`threat`,`type`) VALUES (NULL,");
		escape_and_add(conn,query,r->useragent_ip,1);
		sprintf(query + strlen(query),"%d,",threat);
		escape_and_add(conn,query,buf,0);
		
		sprintf(query + strlen(query),");");
		mysql_query(conn, query);
		
		}
	mysql_close(conn);
	return count;
}

void clonedb(request_rec *r) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn, Module_SQL_server,
		 Module_SQL_user, Module_SQL_password, Module_SQL_database, 0, NULL, 0)) {
	  //printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	
	int last_sql_clone_nr;
	FILE * last_sql = fopen("/var/www/last_sql_clone.txt","r");
	fscanf(last_sql,"%d",&last_sql_clone_nr);
	fclose(last_sql);
	last_sql_clone_nr++;
	
	char query[50000];
	query[0]=0;
	sprintf(query + strlen(query),"CREATE DATABASE elixir_fashion%d;",last_sql_clone_nr);
	inclone++;
	log_nr2(inclone);
	if (mysql_query(conn, query)) {
	  //printf("%s\n", mysql_error(conn));
	  //log_text(query);
	  //exit(1);
	}
	query[0]=0;
	sprintf(query + strlen(query),"GRANT ALL PRIVILEGES ON elixir_fashion%d.* To 'elixir%duser'@'localhost' IDENTIFIED BY 'test%dpass';",last_sql_clone_nr,last_sql_clone_nr,last_sql_clone_nr);
	if (mysql_query(conn, query)) {
	  //printf("%s\n", mysql_error(conn));
	  //log_text(mysql_error(conn));
	  //exit(1);
	}
	
	query[0]=0;
	sprintf(query + strlen(query),"mysqldump -h localhost -u root -p'Honey230793' elixir_data | mysql -h localhost -u elixir%duser -p'test%dpass' elixir_fashion%d",last_sql_clone_nr,last_sql_clone_nr,last_sql_clone_nr);
	system(query);
	//log_text(query);
	
	mysql_close(conn);
	
	
	query[0]=0;
	sprintf(query,"elixir_fashion%d",last_sql_clone_nr);
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, Module_SQL_server,
		 Module_SQL_user, Module_SQL_password, query, 0, NULL, 0)) {
	  //printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	
	
	query[0]=0;
	sprintf(query,"DELETE FROM cart ORDER BY RAND() LIMIT %d",rand_interval(8000,9500));
	mysql_query(conn, query);
	query[0]=0;
	sprintf(query,"DELETE FROM cart_products ORDER BY RAND() LIMIT %d",rand_interval(9000,9500));
	mysql_query(conn, query);
	query[0]=0;
	sprintf(query,"DELETE FROM orders ORDER BY RAND() LIMIT %d",rand_interval(9900,9990));
	mysql_query(conn, query);
	query[0]=0;
	sprintf(query,"DELETE FROM products ORDER BY RAND() LIMIT %d",rand_interval(9900,9990));
	//mysql_query(conn, query);
	query[0]=0;
	sprintf(query,"DELETE FROM users ORDER BY RAND() LIMIT %d",rand_interval(8500,9500));
	mysql_query(conn, query);
	
	mysql_close(conn);
	// COPY_ING the files!
	query[0]=0;
	sprintf(query,"mkdir /var/www/html/copy%d",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/add_to_cart.php /var/www/html/copy%d/add_to_cart.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/cart.php /var/www/html/copy%d/cart.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/checkout.php /var/www/html/copy%d/checkout.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/logout.php /var/www/html/copy%d/logout.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/product.php /var/www/html/copy%d/product.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/signup.php /var/www/html/copy%d/signup.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/test.php /var/www/html/copy%d/test.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/test2.php /var/www/html/copy%d/test2.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/index.php /var/www/html/copy%d/index.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/my_account.php /var/www/html/copy%d/my_account.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/remove_from_cart.php /var/www/html/copy%d/remove_from_cart.php",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp /var/www/html/style.css /var/www/html/copy%d/style.css",last_sql_clone_nr);
	system(query);
	
	query[0]=0;
	sprintf(query,"cp -avr /var/www/html/includes /var/www/html/copy%d/includes",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp -avr /var/www/html/js /var/www/html/copy%d/js",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp -avr /var/www/html/css /var/www/html/copy%d/css",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp -avr /var/www/html/fonts /var/www/html/copy%d/fonts",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp -avr /var/www/html/images /var/www/html/copy%d/images",last_sql_clone_nr);
	system(query);
	query[0]=0;
	sprintf(query,"cp -avr /var/www/html/lib /var/www/html/copy%d/lib",last_sql_clone_nr);
	system(query);
	
	
	
	
	
	
	last_sql = fopen("/var/www/last_sql_clone.txt","w");
	fprintf(last_sql,"%d\n",last_sql_clone_nr);
	fclose(last_sql);
	last_sql = fopen("/var/www/ip_map","a+");
	fprintf(last_sql,"%s %d\n",r->useragent_ip,last_sql_clone_nr);
	fclose(last_sql);
	hasOwnDb = 1;
	OwnDbNr[0]=0;
	sprintf(OwnDbNr,"%d",last_sql_clone_nr);
	
	
	
	char * db_config_contents;
	query[0]=0;sprintf(query,"/var/www/html/copy%d/includes/initialize.php",last_sql_clone_nr);
	read_in_buffer(query,&db_config_contents,r);
	query[0]=0;sprintf(query,"elixir%duser",last_sql_clone_nr);
	db_config_contents = str_replace(db_config_contents,"elixir_user",query,r);
	query[0]=0;sprintf(query,"test%dpass",last_sql_clone_nr);
	db_config_contents = str_replace(db_config_contents,"elixir_fashion123",query,r);
	query[0]=0;sprintf(query,"elixir_fashion%d",last_sql_clone_nr);
	db_config_contents = str_replace(db_config_contents,"elixir_fashion",query,r);
	log_text2(db_config_contents);
	
	query[0]=0;sprintf(query,"/var/www/html/copy%d/includes/initialize.php",last_sql_clone_nr);
	FILE * db_config = fopen(query,"w");
	fprintf(db_config,"%s",db_config_contents);
	fclose(db_config);
	
	
}


struct MCookie get_last_set_cookie(request_rec* r) {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	char * return_val = NULL;
	struct MCookie rcookie;
	rcookie.key=NULL;
	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, Module_SQL_server,
		 Module_SQL_user, Module_SQL_password, Module_SQL_database, 0, NULL, 0)) {
	  //printf("%s\n", mysql_error(conn));
	  exit(1);
	}
	char query[1000];
	query[0]=0;
	int count=0;
	//SELECT COUNT(*) as 'count' FROM `logs` WHERE (user_post!='' OR pass_post!='') AND r_useragent_ip='89.136.122.93' AND timestamp>'2016-06-09 00:38:55'
	sprintf(query,"SELECT headers_out FROM  `logs` WHERE headers_out LIKE '%%Set-Cookie%%' AND r_useragent_ip='%s' ORDER BY TIMESTAMP DESC LIMIT 1;",r->useragent_ip);
	
	//log_text(query);
	if (mysql_query(conn, query)) {
		  //printf("%s\n", mysql_error(conn));
		  //log_text(mysql_error(conn));
		  //exit(1);
		}
		else
		{
		res = mysql_use_result(conn);
		// output table name 
		
		while ((row = mysql_fetch_row(res)) != NULL)
			{
			return_val = apr_pcalloc(r->pool, sizeof(char)*strlen(row[0]));
			strcat(return_val,row[0]);
			}
		  //printf("%s \n", row[0]);
	  
		// close connection 
		mysql_free_result(res);
		}
	mysql_close(conn);
	if (return_val!=NULL)
		{
		char * temp = apr_pcalloc(r->pool, sizeof(char)*strlen(return_val));
		log_nr(strlen(return_val));
		strcat(temp,return_val);
		return_val[0]=0;
		
		strcat(return_val,temp+strpos(temp,"Set-Cookie")+12);
		if (strpos(return_val,";"))
			return_val[strpos(return_val,";")]=0;
		return parse_cookie(r,return_val);
		
		
		}
	return rcookie;
}