#include "aux_functions.h"

void f_authors(char ** command);
void f_pid(char ** command);
void f_chdir(char ** command);
void f_date();
void f_time();
void f_hist(char ** command, tList * command_history);
void f_command(char ** command, tList * command_history, tList * open_files);
void f_open(char ** command, tList * open_files, bool show_message);
void f_close (char ** command, tList * open_files);
void f_dup(char ** command, tList * open_files);
void f_listopen(char ** command, tList open_files);
void f_infosys();
void f_help(char ** command);
void f_quit(tList * command_history, tList * open_files);
void f_invalid();
void f_create(char ** command, tList *open_files);
void f_stat(char ** command);
void f_list(char ** command);
void f_delete(char ** command);

void processCommand(char ** command, tList * command_history, tList * open_files);
