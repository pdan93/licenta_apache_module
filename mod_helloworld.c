#include <httpd.h>
#include <http_protocol.h>
#include <http_config.h>
#include <util_filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include <regex.h>
#include <time.h>


FILE * content_file;
FILE * content_file2;
int output_has_work = 0;
int input_has_work = 0;
char * input_buffer;
int sql_logged = 0;
int AttackType = 0;
int SpecificAttackType = 0;
int hasOwnDb = 0;
char * OwnDbNr[5];
int StopHim = 0;

#include "module_logs.h"
#include "module_helpers.h"
#include "module_sql.h"


//project honeypot access key wjrkudvgxlao

static int helloworld_handler(request_rec *r) {
    /*if (!r->handler || strcmp(r->handler, "helloworld")) {
        return DECLINED;
    }
    if (r->method_number != M_GET) {
        return HTTP_METHOD_NOT_ALLOWED;
    }
    ap_set_content_type(r, "text/html;charset=ascii");
    ap_rputs("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\">\n", r);
    ap_rputs("<html><heade><title> Apache HelloWorld Module</title></head>", r);
    ap_rputs(mysql_get_client_info(), r);
    mysql_test(r);
    
    ap_rputs("</body></html>", r);
    
	log_request(r);*/
	//ap_rputs("hey",r);
	
    return OK;
}




static int output_filter_init(ap_filter_t* f) {
	
	if (output_has_work==1)
		{
		content_file = fopen ("/licenta/output_filter.txt","w");
		}
	//fprintf(content_file,"Start3\n");

	return OK ;
}

static int output_filter(ap_filter_t* f, apr_bucket_brigade* bb_in) {
	if (output_has_work==1)
		{
		apr_bucket* b ;
		apr_bucket_brigade *bb_out; 
		
		request_rec *r = f->r; 
		conn_rec *c = r->connection; 
		apr_pool_t *p = r->pool;
		
		
		
		bb_out = apr_brigade_create(p, c->bucket_alloc);
		write_brigade(bb_out,r,""); 
		
		int ok = 0;
		
		char *data = "hey";
		char *data2 = "heyend";
		
		
		/*fprintf(fortest,"what we get: %d\n",strstr(r->the_request,".php"));
		int length;
		apr_brigade_length(bb_in,1,&length);
		fprintf(fortest,"Brigade length %d\n",length);*/
		
		
		for ( b = APR_BRIGADE_FIRST(bb_in); b != APR_BRIGADE_SENTINEL(bb_in); b = APR_BUCKET_NEXT(b) ) 
			{
			char* buf ;
			size_t bytes ;
			if ( APR_BUCKET_IS_EOS(b) ) {
				
				//fprintf(content_file,"END\n");
				fclose(content_file);
				
				//ap_add_output_filter("last_filter",)
				
				
				read_in_buffer("/licenta/output_filter.txt",&buf,f->r);
				
				//buf = str_replace(buf,"99","yy");
				
				if (AttackType==1)//sql inj
					{
					
					if (SpecificAttackType>2)
						write_brigade(bb_out,r,"HTTP/1.1 500 Internal Server Error");
						else
						write_brigade(bb_out,r,buf); 
					}
					else
					write_brigade(bb_out,r,buf); 
					
					
				//write_brigade(bb_out,r,"HTTP/1.1 500 Internal Server Error");
				
				
				//write_brigade(bb_out,r,"y"); 
				APR_BRIGADE_INSERT_TAIL(bb_out,apr_bucket_eos_create(c->bucket_alloc)); 
				
				if (complete_file_log!=-1)
					{
					fclose(complete_file_log); //closing the complete log file descriptor
					complete_file_log = -1;
					}
				if (sql_logged==0)
					{
					mysql_log(f->r);
					sql_logged = 1;
					}
				}
				else 
				{
				apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
				buf[bytes]=0;
				fprintf(content_file,"%s\n",buf);
				//log_bucket(b);
				/*char * pch;
				pch = strstr(buf,"daniel");
				int len = pch-buf;
				
				char content[10000];
				content[0]=0;
				
				strncat(content,buf,len);
				strcat(content,"DANIEL");
				strcat(content,pch+6);
				
				
				
				write_brigade(bb_out,r,content); 
				
				
				write_brigade(bb_out,r,buf);*/
				
				//ok=0;
				
				
				}
			}
		//APR_BRIGADE_INSERT_TAIL(bb_in,apr_bucket_eos_create(c->bucket_alloc)); 
		//ap_remove_output_filter(f); 
		
	 
		//if (ok==0)
		//	return ap_pass_brigade(f->next, bb_in) ;
		//	else
			return ap_pass_brigade(f->next, bb_out) ;
		}
		else
		{
		if (complete_file_log!=-1)
			{
			fclose(complete_file_log); //closing the complete log file descriptor
			complete_file_log = -1;
			}
		if (sql_logged==0)
			{
			mysql_log(f->r);
			sql_logged = 1;
			}
		return ap_pass_brigade(f->next, bb_in);
		}
}	

int attack_listen(ap_filter_t* f) {
	char ip_map_line[100];
	FILE * ip_map = fopen("/var/www/ip_map","r");
	while (fgets(ip_map_line, 100, ip_map) != NULL) {
		if (strstr(ip_map_line,f->r->useragent_ip)!=NULL)
			{
			ip_map_line[strlen(ip_map_line)-1]=0;
			hasOwnDb=1;
			OwnDbNr[0]=0;
			strcat(OwnDbNr,ip_map_line+strlen(f->r->useragent_ip)+1);
			break;
			}
			
	}
	fclose(ip_map);
	//log_nr(hasOwnDb);
	//verify_ip(f->r);
	
		
	
	int ok=0;
	if (my_regex("(POST|PUT|DELETE)",f->r->method))
		ok=1;
	if (ok==1)
		{
		input_has_work = 1;
		output_has_work = 1;
		input_buffer = -1;
		}
		else
		{		
		if (hasOwnDb)
			changefilepath(f->r);
		}
		
	return 0;
}

void changefilepath(request_rec* r) {
	char correct_path[100];
	correct_path[0]=0;
	sprintf(correct_path,"/var/www/html/copy%s",OwnDbNr);
	char * correct_filename = apr_pcalloc(r->pool,sizeof(char)*100);
	strcat(correct_filename,r->filename);
	correct_filename = str_replace(correct_filename,"/var/www/html",correct_path,r);
	
	r->filename = apr_pcalloc(r->pool,sizeof(char)*strlen(correct_filename));
	strcat(r->filename,correct_filename);
}

int categorize_attack(request_rec* r) {
	input_buffer = urlDecode(input_buffer,r);
	if (strlen(input_buffer)>0)
		{
		if (my_regex("(.*')|(.*OR.*=)|(;)|(ALTER|CREATE|DELETE|DROP|EXEC(UTE){0,1}|INSERT(\s+INTO){0,1}|MERGE|SELECT|UPDATE|UNION(\s+ALL){0,1})",input_buffer))
			{
			AttackType = 1;//sql inj
			
			struct post_body pb = break_post_body(input_buffer);
			
			SpecificAttackType = categorize_sql_injection(pb.values[0]);
			}
			else
			{
			struct post_body pb = break_post_body(input_buffer);
			int ok_user_email=0,ok_pass=0;
			for (int i=0; i<pb.nr; i++)
				{
				if (strcmp(pb.keys[i],"email")==0)
					ok_user_email=1;
				if (strcmp(pb.keys[i],"username")==0)
					ok_user_email=1;
				if (strcmp(pb.keys[i],"password")==0)
					ok_pass=1;
				}
			if (ok_user_email==1 || ok_pass==1)
				{
				int count = get_last_guessings(r);
				log_nr(count);
				if (count>2)
					{
					AttackType = 2;//brute force guessing
					if (ok_user_email==1 && ok_pass==1)
						SpecificAttackType = 3;
						else if (ok_user_email==1)
							SpecificAttackType = 1;
							else
							SpecificAttackType = 2;
					if (count>100)
						StopHim=1;
					}
				}
			//SELECT COUNT(*) as 'count' FROM `logs` WHERE (user_post!='' OR pass_post!='') AND r_useragent_ip='89.136.122.93' AND timestamp>'2016-06-09 00:38:55'
			}
		}
	if (AttackType>0)
		{
		verify_ip(r);
		if (SpecificAttackType>0 && hasOwnDb==0)
			clonedb(r);
		}
	
	if (hasOwnDb)
		changefilepath(r);		
				
}

static int input_filter_init(ap_filter_t* f) {
	attack_listen(f);
		
	//f->r->filename = apr_pcalloc(f->r->pool,sizeof(char)*100);
	//strcat(f->r->filename,"/var/www/html/test2.php");
		
	
	return OK ;
}


int input_filter(ap_filter_t* f, apr_bucket_brigade *bb, ap_input_mode_t mode, apr_read_type_e block, apr_off_t readbytes) {
	int rv;
	
	rv = ap_get_brigade(f->next, bb, mode, block, readbytes);
	if (input_has_work==1)//if we have to parse the input request
		{		
		int bb_length;
		apr_brigade_length(bb,1,&bb_length);//get size of input body so that we can malloc it
		if (input_buffer==-1)
			{
			input_buffer = apr_pcalloc(f->r->pool,sizeof(char)*bb_length);// malloc (sizeof(char)*bb_length);
			//input_buffer[0]=0;
			}
			
		apr_bucket* b ;
		for ( b = APR_BRIGADE_FIRST(bb); b != APR_BRIGADE_SENTINEL(bb); b = APR_BUCKET_NEXT(b) ) 
			{
			
			char* buf ;
			size_t bytes ;
			if ( APR_BUCKET_IS_EOS(b) ) {
				categorize_attack(f->r);
				input_has_work=-1;
				if (StopHim==1)
					{
					mysql_log(f->r);
					exit(0);
					}
				}
				else 
				{
				apr_bucket_read(b, &buf, &bytes, APR_BLOCK_READ);
				buf[bytes]=0;
				strcat(input_buffer,buf);
				//char *	buf2 = apr_punescape_url(f->r->pool,buf);
				//fprintf(complete_file_log,"%s\n",buf);
				
				}
			}
		}
	return rv;
}

static void helloworld_hooks(apr_pool_t *pool) {
    //ap_hook_handler(helloworld_handler, NULL, NULL, APR_HOOK_LAST);ap_hook_translate_name
	//ap_register_output_filter("helloworld", last_filter, NULL, AP_FTYPE_RESOURCE) ;
	//ap_hook_translate_name(translate_name_handler, NULL, NULL, APR_HOOK_MIDDLE);
	//ap_hook_post_read_request(post_config_handler, NULL, NULL, APR_HOOK_MIDDLE);
	ap_register_output_filter("helloworld", output_filter, output_filter_init, AP_FTYPE_RESOURCE);
	ap_register_input_filter("elixir_output_filter", input_filter, input_filter_init, AP_FTYPE_CONTENT_SET);//AP_FTYPE_CONNECTION);
}


module AP_MODULE_DECLARE_DATA helloworld_module = {
  STANDARD20_MODULE_STUFF,
  NULL,  
  NULL,
  NULL,
  NULL,
  NULL,
  helloworld_hooks
};