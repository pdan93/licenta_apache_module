#include <stdio.h>
#include <stdlib.h>

void read_in_buffer(char * filepath, char ** buffer ) {
	FILE * content_file2;
	content_file2 = fopen (filepath,"r");
	
  fseek (content_file2 , 0 , SEEK_END);
  long lSize = ftell (content_file2);
  rewind (content_file2);

  // allocate memory to contain the whole file:
  *buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  size_t result = fread (*buffer,1,lSize,content_file2);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  
  //printf("%s",buffer);
	  
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
	char *orig_copy;
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements
	orig_copy = orig;
    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
	free(orig_copy);
    return result;
}



int main() {
	
	char * buf ;
	read_in_buffer("/licenta/testing.txt",&buf);
	buf = str_replace(buf,"99","yy");
	printf("%s",buf);
	return 1;
}