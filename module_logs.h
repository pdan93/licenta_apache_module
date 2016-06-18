
FILE * complete_file_log = -1;
/*
static void complete_log_request(request_rec *r) {
	
    complete_file_log = fopen ("/licenta/request.txt","w");
    if (complete_file_log!=NULL)
        {
		fprintf(complete_file_log, "Request protocol: %s\n",r->protocol);
		fprintf(complete_file_log, "Request hostname: %s\n",r->hostname);
		fprintf(complete_file_log, "Request status_line: %s\n",r->status_line);
		fprintf(complete_file_log, "Request method: %s\n",r->method);
		fprintf(complete_file_log, "Request range: %s\n",r->range);
		fprintf(complete_file_log, "Request content_type: %s\n",r->content_type);
		fprintf(complete_file_log, "Request handler: %s\n",r->handler);
		fprintf(complete_file_log, "Request content_encoding: %s\n",r->content_encoding);
		fprintf(complete_file_log, "Request user: %s\n",r->user);
		fprintf(complete_file_log, "Request ap_auth_type: %s\n",r->ap_auth_type);
		fprintf(complete_file_log, "Request unparsed_uri: %s\n",r->unparsed_uri);
		fprintf(complete_file_log, "Request uri: %s\n",r->uri);
		fprintf(complete_file_log, "Request filename: %s\n",r->filename);
		fprintf(complete_file_log, "Request canonical_filename: %s\n",r->canonical_filename);
		fprintf(complete_file_log, "Request path_info: %s\n",r->path_info);
		fprintf(complete_file_log, "Request args: %s\n",r->args);
		fprintf(complete_file_log, "Request log_id: %s\n",r->log_id);
		fprintf(complete_file_log, "Request useragent_ip: %s\n",r->useragent_ip);
		fprintf(complete_file_log, "Request time (time when request started): %d\n",r->request_time);
		fprintf(complete_file_log, "Request allowed(y/n): %d\n",r->allowed);
		fprintf(complete_file_log, "Request resource last modified time: %d\n",r->mtime);
		
		
			
			apr_sockaddr_t* useragent_addr= r->useragent_addr;
			fprintf(complete_file_log, "\tRequest UseragentAddr hostname: %s\n",useragent_addr->hostname);
			fprintf(complete_file_log, "\tRequest UseragentAddr servname: %s\n",useragent_addr->servname);
			fprintf(complete_file_log, "\tRequest UseragentAddr port: %d\n",useragent_addr->port);
			fprintf(complete_file_log, "\tRequest UseragentAddr ipaddr_len: %d\n",useragent_addr->ipaddr_len);
			fprintf(complete_file_log, "\tRequest UseragentAddr addr_str_len: %d\n",useragent_addr->addr_str_len);
			fprintf(complete_file_log, "\tRequest UseragentAddr family: %d\n",useragent_addr->family);
			//fprintf(complete_file_log, "\tRequest UseragentAddr ip: %s\n",useragent_addr->next->hostname);
			
			
		conn_rec* connection= r->connection;
		fprintf(complete_file_log, "Connection client_ip: %s\n",connection->client_ip);
		fprintf(complete_file_log, "Connection remote_host: %s\n",connection->remote_host);
		fprintf(complete_file_log, "Connection remote_logname: %s\n",connection->remote_logname);
		fprintf(complete_file_log, "Connection local_ip: %s\n",connection->local_ip);
		fprintf(complete_file_log, "Connection local_host: %s\n",connection->local_host);
		fprintf(complete_file_log, "Connection log_id: %s\n",connection->log_id);
		
		apr_sockaddr_t * client_addr = connection->client_addr;
			fprintf(complete_file_log, "\tConnection ClientAddr hostname: %s\n",client_addr->hostname);
			fprintf(complete_file_log, "\tConnection ClientAddr servname: %s\n",client_addr->servname);
			fprintf(complete_file_log, "\tConnection ClientAddr port: %d\n",client_addr->port);
			fprintf(complete_file_log, "\tConnection ClientAddr ipaddr_len: %d\n",client_addr->ipaddr_len);
			fprintf(complete_file_log, "\tConnection ClientAddr addr_str_len: %d\n",client_addr->addr_str_len);
			fprintf(complete_file_log, "\tConnection ClientAddr family: %d\n",client_addr->family);
			
		if (client_addr->next)
			{
			char * ip_addr;
			apr_sockaddr_ip_get(&ip_addr, client_addr->next);
			fprintf(complete_file_log, "\tConnection ClientAddr id_addr: %s\n",ip_addr);
			}
		
		
			
			//apr_uri_t parsed_uri = r->parsed_uri;
			//fprintf(complete_file_log, "\tRequest ParsedURI scheme: %s\n",parsed_uri->user);
		
		//fprintf(complete_file_log, "Requesr useragent_host: %s\n",r->useragent_host);
			server_rec * server= r->server;
			fprintf(complete_file_log, "\n");
			fprintf(complete_file_log, "Server defn_name: %s\n",server->defn_name);
			fprintf(complete_file_log, "Server server_scheme: %s\n",server->server_scheme);
			fprintf(complete_file_log, "Server server_admin: %s\n",server->server_admin);
			fprintf(complete_file_log, "Server server_hostname: %s\n",server->server_hostname);
			fprintf(complete_file_log, "Server path: %s\n",server->path);
			process_rec * process= server->process;
			fprintf(complete_file_log, "\tServer Process short_name: %s\n",process->short_name);
			fprintf(complete_file_log, "\tServer Process argv: %s\n",process->argv);
			//apr_file_t * error_log= server->error_log;
			//fprintf(complete_file_log, "\t\tServer ErrorLog fname: %s\n",error_log->fname);
			apr_port_t * port= server->port;
			fprintf(complete_file_log, "Server Port: %d\n",port);
			fprintf(complete_file_log, "Server Timeout(ms): %d\n",server->timeout);
			fprintf(complete_file_log, "Server Keep Alive Timeout(ms): %d\n",server->keep_alive_timeout);
			fprintf(complete_file_log, "Server Keep Alive Max: %d\n",server->keep_alive_max);
			fprintf(complete_file_log, "Server Keep Alive(yes/no): %d\n",server->keep_alive);
			fprintf(complete_file_log, "Server limit on size of the HTTP request line: %d\n",server->limit_req_line);
			fprintf(complete_file_log, "Server limit on size of any request header field: %d\n",server->limit_req_fieldsize);
			fprintf(complete_file_log, "Server limit on number of request header fields: %d\n",server->limit_req_fields);
		
		fprintf(complete_file_log,"\nHEADERS IN\n");
		printtable(r,r->headers_in,complete_file_log);
		//fprintf(complete_file_log,"\nHEADERS OUT\n");
		//printtable(r,r->headers_out,complete_file_log);
			
		
        
        
        
        }
        
}*/


static void log_bucket(apr_bucket *b) {
	FILE * f;
    f = fopen ("/licenta/bucket.txt","w");
    if (f!=NULL)
        {
			
        const char* buf ;
		size_t bytes ;
        apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
        
        fputs(buf, f);
        
        
        fclose(f);
        }
        
}

static void log_text(char *s) {
	FILE * f;
    f = fopen ("/licenta/text.txt","w");
    if (f!=NULL)
        {
		if (s!=NULL)
			fputs(s, f);
        fclose(f);
        }
        
}

static void log_nr(long long int nr) {
	FILE * f;
    f = fopen ("/licenta/text.txt","w");
    if (f!=NULL)
        {
        fprintf(f,"%d\n",nr);
        fclose(f);
        }
        
}
