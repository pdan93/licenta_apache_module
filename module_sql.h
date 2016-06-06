char *Module_SQL_server = "localhost";
char *Module_SQL_user = "honeypot";
char *Module_SQL_password = "honeypot123"; /* set me first */
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
	strcat(query,"INSERT INTO `logs`(`id`, `r_protocol`, `r_hostname`, `r_status_line`, `r_method`, `r_range`, `r_content_type`, `r_handler`, `r_content_encoding`, `r_user`, `r_ap_auth_type`, `r_unparsed_uri`, `r_uri`, `r_filename`, `r_canonical_filename`, `r_path_info`, `r_args`, `r_log_id`, `r_useragent_ip`, `r_request_time`, `r_allowed`, `r_mtime`, `r_UA_hostname`, `r_UA_servname`, `r_UA_port`, `r_UA_ipaddr_len`, `r_UA_addr_str_len`, `r_UA_family`, `c_client_ip`, `c_remote_host`, `c_remote_logname`, `c_local_ip`, `c_local_host`, `c_log_id`, `c_CA_hostname`, `c_CA_servname`, `c_CA_port`, `c_CA_ipaddr_len`, `c_CA_addr_str_len`, `c_CA_family`, `s_defn_name`, `s_server_scheme`, `s_server_admin`, `s_server_hostname`, `s_path`, `s_P_short_name`, `s_P_argv`, `s_port`, `s_timeout`, `s_keep_alive_timeout`, `s_keep_alive_max`, `s_keep_alive`, `s_limit_req_line`, `s_limit_req_fieldsize`, `s_limit_req_fields`, `headers_in`, `headers_out`, `request_body`) VALUES (NULL,");
	
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
	if (input_has_work)//request body if any
		escape_and_add(conn,query,input_buffer,0);
		else
		escape_and_add(conn,query,"",0);
	
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