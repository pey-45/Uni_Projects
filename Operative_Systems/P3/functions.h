#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "aux_functions.h"

void f_authors(char ** command);
void f_pid(char ** command);
void f_chdir(char ** command);
void f_time(char ** command);
void f_hist(char ** command, tList * command_history);
void f_command(char ** command, int (*main)(int, char**, char**), char ** envp, tList * command_history, tList * open_files, tList * memory, tList * shared_memory, tList * mmap_memory);
void f_open(char ** command, tList * open_files, bool show_message);
void f_close (char ** command, tList * open_files);
void f_dup(char ** command, tList * open_files);
void f_listopen(char ** command, tList open_files);
void f_infosys();
void f_help(char ** command);
void f_quit(tList * command_history, tList * open_files, tList * memory, tList * shared_memory, tList * mmap_memory);
void f_invalid();
void f_create(char ** command, tList *open_files);
void f_stat(char ** command);
void f_list(char ** command);
void f_delete(char ** command);
void f_malloc(char ** command, tList * memory);
void f_mmap(char ** command, tList * mmap_memory);
void f_read(char ** command);
void f_write(char ** command);
void f_memdump(char ** command);
void f_memfill(char ** command);
void f_mem(char ** command, tList * memory, tList * shared_memory, tList * mmap_memory);
void f_recurse(char ** command);
void f_uid(char ** command);
void f_showvar(char ** command, int (*main)(int, char**, char**), char ** envp);
void f_changevar(char ** command, int (*main)(int, char**, char**), char ** envp);
void f_subs(char ** command, int (*main)(int, char**, char**), char ** envp);


void processCommand(char ** command, int (*main)(int, char**, char**), char ** envp, tList * command_history, tList * open_files, tList * memory, tList * shared_memory, tList * mmap_memory);

#endif //FUNCTIONS_H