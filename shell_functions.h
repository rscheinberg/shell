struct redir {int arg; int position; char *type;};

char ** parse_args(int argc, char * line);
void cd (char *path);
char * remove_spaces(char * line);
char ** parse_args(int argc, char * line);
void leave ();
void initialize ();
int charfind(char s[], char c);
char ***semicolons (char *input);
void welcome(char * user);
struct redir * find_redir(char *** commands, int i);
void redir(char ** args, char * type, char * file);
void execreedvp(char ** args, struct redir *redir_info);