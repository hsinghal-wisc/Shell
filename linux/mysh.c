#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/wait.h>
#define SIZE 600

void removeSpaces(char *str) {
    // To keep track of non-space character count
    int count = 0, i, j = 0;
  while (str[j] == ' '  || str[j] == '\t')
            j++;
    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (i = j; str[i]; i++) {
        if (( str[i] == ' ' || str[i] == '\t' ) &&
(str[i-1] == '\t' || str[i-1] == ' '))
            { continue; }
        else
            { str[count++] = str[i]; }
        }
        if ( str[count-1] == ' ' || str[count-1] == '\t' )
    str[ count-1 ] = '\0';
        else
str[ count ] = '\0';
}
char str_amp[]= "&";
int main(int argc, char *argv[]) {
int jid = 1, pid = -1, rc, status = 0,
status2 = 0, p[100000] = {0}, map[100000] = {0}, map2[100000] = {0};
FILE* fd;
char ch[1000][SIZE];
int i, k = 0;
struct timeval stop, start;
char *cmd = (char *)malloc(SIZE * sizeof(char));
char *cmd2 = (char *)malloc(SIZE * sizeof(char));
char *cmd3 = (char *)malloc(SIZE * sizeof(char));
if(argc > 2 || argc < 1) {
fprintf(stderr, "Usage: mysh [batchFile]\n"); fflush(stderr); exit(1);
} else if (argc == 2) {
char* name = argv[1];
fd = fopen(name, "r");
if(fd == NULL) {
fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
fflush(stderr);
exit(1);}
while(fgets(cmd, SIZE, fd) != NULL) {
if(cmd[0] == '\0' || cmd[0] == EOF) break;
if (cmd[0] == '\n') {write(1, "\n", strlen("\n")); continue; }
strcpy(cmd3, cmd);
cmd[ strlen(cmd) - 1 ] = '\0';
cmd3[ strlen(cmd3) - 1 ] = '\0';
// fprintf(stdout,"before string%s\n",cmd);
removeSpaces(cmd);
// fprintf(stdout,"after string%s\n",cmd);

cmd[ strlen(cmd) ] = '\0';

strcpy(cmd2, cmd);

cmd2[ strlen(cmd2) ] = '\0';
// fprintf(stdout,"string%s\n",cmd);
// fprintf(stdout,"string 2%s\n",cmd2);
// fflush(stdout);


argc = 0;
fprintf(stdout, "%s\n", cmd3);
fflush(stdout);
if(strlen(cmd) > 512) {
fprintf(stderr, "An error has occurred\n"); fflush(stderr); continue; }
char *arg = strtok(cmd, " \t\n");
while (arg != NULL) {
argv[ argc++ ] = arg;
arg = strtok(NULL, " ");
}
argv[ argc ] = NULL;
// fprintf(stdout,"number %d\n",argc);
// fflush(stdout);
// for(i=0;i<argc;i++)
// {fprintf(stdout,"hi %s\n",argv[i]);fflush(stdout);}
while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
if(pid != -1 && p[pid]) {
p[pid] = 0;
pid = -1;
}}
if ((!strcmp (argv[0], "exit") || !strcmp (argv[0], "exit&"))
&& (argc == 1 || (argc == 2 && !strcmp(argv[1], "&"))))
exit(0);
if ((!strcmp (argv[0], "j") || !strcmp (argv[0], "j&")) &&
(argc == 1 || (argc == 2 && !strcmp(argv[1], "&")))) {
for (i=0; i < 1000; i++)
if(p[map2[i]]) {
fprintf(stdout, "%d : %s\n", i, ch[i]);
fflush(stdout);
}
continue;
}
if ((!strcmp (argv[0], "myw") || !strcmp (argv[0], "myw&"))
&& (argc == 2 || (argc == 3 && !strcmp(argv[2], "&")))) {
if(!map2[atoi(argv[1])]) {
fprintf(stderr, "Invalid jid %s\n", argv[1]); fflush(stderr); continue; }
if(!p[map2[atoi(argv[1])]]) {
fprintf(stdout, "0 : Job %s terminated\n", argv[1]); fflush(stdout);
continue; }
gettimeofday(&start, NULL);
(void) waitpid(map2[atoi(argv[1])], &status, 0);
gettimeofday(&stop, NULL);
fprintf(stdout, "%lu : Job %s terminated\n",
1000000 * (stop.tv_sec - start.tv_sec) + stop.tv_usec - start.tv_usec,
argv[1]);
fflush(stdout);
p[map2[atoi(argv[1])]] = 0;
continue;
}
if((argc == 1) && (!strcmp(argv[0], "&&"))) continue;
if((argc == 1) && (!strcmp(argv[0], "&"))) {
fprintf(stderr, "%s: Command not found\n", argv[0]);
fflush(stderr); continue; }
if (!strcmp(argv[argc - 1], "&")) {
// fprintf(stdout,"hii\n");fflush(stdout);
status2 = 1; argv[argc-1] = NULL; argc--; }
int s = strlen(argv[argc-1]);
// fprintf(stdout," %c",argv[argc-1][s-1]);fflush(stdout);
if(argv[argc-1][s-1] == '&') {
argv[argc-1][s-1] = '\0';
// fprintf(stdout,"hii\n");fflush(stdout);
status2 = 1; }
// if (!strcmp (argv[argc - 1],"&"))
// {
// status2=1;argv[argc-1] = NULL;}
rc = fork();
if(rc == 0) {
if(execvp(argv[0], argv) == -1) {
fprintf(stderr, "%s: Command not found\n", argv[0]);
fflush(stderr); exit(1); }
} else {
map[rc] = jid;
map2[jid] = rc;
jid++;
i = 0;
k = strlen(cmd2);
// fprintf(stdout,"string 1%s\n",cmd2);
// printf("len %d\n",k);
if(cmd2[k-1] == '&') {
for(i = 0; i < k-2; i++)
ch[map[rc]][i] = cmd2[i];
if(cmd2[k-2] != ' ') ch[map[rc]][i] = cmd2[i];
} else {
for(i = 0; i < k; i++)
ch[map[rc]][i] = cmd2[i];
if(cmd2[k-2] != ' ')
ch[map[rc]][i] = cmd2[i];
}
// fprintf(stdout,"string 2%s\n",ch[map[rc]]);
// fflush(stdout);
if(status2) {
p[rc] = 1; status2 = 0;
}
while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
if(pid != -1 && p[pid]) {
p[pid] = 0;
pid = -1;
}}

if(!p[rc])
(void) waitpid(rc, &status, 0);
}
// for(i=0;i<argc;i++)
// {free(argv[i]);argv[i]=NULL;}
// free(arg);arg=NULL;
// free(argv);argv=NULL;
// if(feof(fd))
// break;
// fflush(fd);
// cmd[0]='\0';
}
// for(i=0;i<argc;i++)
// {free(argv[i]);argv[i]=NULL;}
// free(name);name=NULL;
// free(argv);argv=NULL;
} else {
while (1) {
fprintf(stdout, "mysh> ");
fflush(stdout);
if (fgets(cmd, 515, stdin) == NULL) continue;
if(cmd[0] == '\0' || cmd[0] == EOF) continue;
if (cmd[0] == '\n') continue;
strcpy(cmd3, cmd);
cmd[ strlen(cmd) -1 ] = '\0';
cmd3[ strlen(cmd3) -1 ] = '\0';
// fprintf(stdout,"before string%s\n",cmd);
removeSpaces(cmd);
// fprintf(stdout,"after string%s\n",cmd);
cmd[ strlen(cmd) ] = '\0';
strcpy(cmd2, cmd);
cmd2[ strlen(cmd2) ] = '\0';
// fprintf(stdout,"string%s\n",cmd);
// fprintf(stdout,"string 2%s\n",cmd2);
// fflush(stdout);
argc = 0;
// fprintf(stdout,"%s\n",cmd3);
// fflush(stdout);
if(strlen(cmd) > 512) {
fprintf(stdout, "%s\n", cmd);
fflush(stdout); fprintf(stderr, "An error has occurred\n");
fflush(stderr); continue; }
char *arg = strtok(cmd, " \t\n");
while (arg != NULL) {
argv[ argc++ ] = arg;
arg = strtok(NULL, " ");
}
argv[ argc ] = NULL;
// fprintf(stdout,"number %d\n",argc);
// fflush(stdout);
// for(i=0;i<argc;i++)
// {fprintf(stdout,"hi %s\n",argv[i]);fflush(stdout);}
while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
if(pid != -1 && p[pid]) {
p[pid] = 0;
pid = -1;
}}
if ((!strcmp (argv[0], "exit") || !strcmp (argv[0], "exit&"))
&& (argc == 1 || (argc == 2 && !strcmp(argv[1], "&"))))
exit(0);
if ((!strcmp (argv[0], "j") || !strcmp (argv[0], "j&"))
&& (argc == 1 || (argc == 2 && !strcmp(argv[1], "&")))) {
for(i = 0; i <1000; i++)
if(p[map2[i]]) {
fprintf(stdout, "%d : %s\n", i, ch[i]);
fflush(stdout);
}
continue;
}
if ((!strcmp (argv[0], "myw") || !strcmp (argv[0], "myw&"))
&& (argc == 2 || (argc == 3 && !strcmp(argv[2], "&")))) {
if(!map2[atoi(argv[1])]) {
fprintf(stderr, "Invalid jid %s\n", argv[1]);
fflush(stderr); continue; }
if(!p[map2[atoi(argv[1])]]) {
fprintf(stdout, "0 : Job %s terminated\n", argv[1]);
fflush(stdout); continue;
}
gettimeofday(&start, NULL);
(void) waitpid(map2[atoi(argv[1])], &status, 0);
gettimeofday(&stop, NULL);
fprintf(stdout, "%lu : Job %s terminated\n",
1000000 * (stop.tv_sec - start.tv_sec) + stop.tv_usec - start.tv_usec,
argv[1]);
fflush(stdout);
p[map2[atoi(argv[1])]] = 0;
continue;
}
if((argc == 1) && (!strcmp(argv[0], "&&"))) continue;
if((argc == 1) && (!strcmp(argv[0], "&"))) {
fprintf(stderr, "%s: Command not found\n", argv[0]);
fflush(stderr); continue;
}
if(!strcmp(argv[argc - 1], "&")) {
// fprintf(stdout,"hii\n");fflush(stdout);
status2 = 1; argv[argc-1] = NULL; argc--; }
int s = strlen(argv[argc-1]);
// fprintf(stdout," %c",argv[argc-1][s-1]);fflush(stdout);
if(argv[argc-1][s-1] == '&') {
argv[argc-1][s-1] = '\0';
// fprintf(stdout,"hii\n");fflush(stdout);
status2 = 1;
}
// if (!strcmp (argv[argc - 1],"&"))
// {
// status2=1;argv[argc-1] = NULL;}
rc = fork();
if(rc == 0) {
if(execvp(argv[0], argv) == -1) {
fprintf(stderr, "%s: Command not found\n", argv[0]);
fflush(stderr); exit(1);
}
} else {
map[rc] = jid;
map2[jid] = rc;
jid++;
i = 0;
k = strlen(cmd2);
// fprintf(stdout,"string 1%s\n",cmd2);
// printf("len %d\n",k);
if(cmd2[k-1] == '&') {
for(i = 0; i < k-2; i++)
ch[map[rc]][i] = cmd2[i];
if(cmd2[k-2] != ' ') ch[map[rc]][i] = cmd2[i];
} else {
for(i = 0; i < k; i++)
ch[map[rc]][i] = cmd2[i];
if(cmd2[k-2] != ' ') ch[map[rc]][i] = cmd2[i];
}
// fprintf(stdout,"string 2%s\n",ch[map[rc]]);
// fflush(stdout);
if(status2) {
p[rc] = 1; status2 = 0;
}
while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
if(pid != -1 && p[pid]) {
p[pid] = 0;
pid = -1;
}}
if(!p[rc])
(void) waitpid(rc, &status, 0);
}
// for(i=0;i<argc;i++)
// {free(argv[i]);argv[i]=NULL;}
// free(arg);arg=NULL;
// free(argv);argv=NULL;
// if(feof(fd))
// break;
// fflush(fd);
// cmd[0]='\0';
}
// for(i=0;i<argc;i++)
// {free(argv[i]);argv[i]=NULL;}
// free(name);name=NULL;
// free(argv);argv=NULL;
}
fclose(fd);
free(cmd3);
free(cmd);
free(cmd2);
return 0;
}
