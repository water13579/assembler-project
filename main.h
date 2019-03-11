#ifndef _MAIN_H_
#define _MAIN_H_

#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern int IC;
extern int DC;
extern word code[];
extern word data[];
extern char *opcodes[];
extern char *registers[];
extern int EXTERN_EXISTS;
extern int ENTRY_EXISTS;


typedef struct table_node {
	struct table_node *next;
	char *label;
	int counter;
	char type;
} tnode;

/* utils.c functions */
void encode_data(char line[], int *dc, int label_exists);
int words_num(word w);
int op_num(char *opcode, int label_exists);
int get_register(char *reg);
word add_word(tnode *head, char *line, int t, int label_exists);
int is_number(char *s);
char *encode_base64(word w);
int get_dest(char line[], int label_exists);
char *get_dest_name(char line[], int label_exists);
int get_src(char line[], int label_exists);
char *get_src_name(char line[], int label_exists);
void encode_data(char line[], int *dc, int label_exists);
int words_num(word w);
word build_word(int opcode, int src, int dest, int are);
int opcode_num(char *pc);
int operands_num(char *opcode);
int is_label(char *tok);
char *get_label(char *line, int type);
int direct_type(char *line);
int get_opcode(char *line, int label_exists);
char* new_string(char *str);
int operand_type(char *op);


/* tables.c functions */
tnode *find_label(tnode *head, char *label);
void free_tnode_list(tnode *head);
int label_exists(tnode *head, char *label);
tnode *get_last_item(tnode *node);
tnode *insert_symbol(tnode *head, char *label, int counter, int type);
int label_already_exists(tnode *head, char *label);
void print_list(tnode *head);
void update_data(tnode *head, int n);


/* first_scan.c functions and structures */
tnode *first_scan(FILE *fp, word code[], word data[], tnode *symbols_table);

/* second_scan.c functions and structures */
void second_scan(FILE *fp, word *code, tnode *symbols_table);


#endif


