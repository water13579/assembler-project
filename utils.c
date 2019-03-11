#include "main.h"

/* returns the number of operands */
int op_num(char *line, int label_exists)
{
	int n;

	n = get_opcode(line, label_exists);

	printf("in opnum %d\n", n);
	if (n >= 0 && n <= 4)
		return 3;
	else if (n >= 5 && n <= 13)
		return 2;
	else if (n >= 14 && n <= 15)
		return 1;
	else
		return -1;
}

/* get the categorical number of the opcode */
/*int opcode_num(char *pc)
{
	int i;
	for(i = 0; i < 16; i++)
		if (strcmp(opcodes[i], pc) == 0)
			return i;
	printf("error: opcode does not exist\n");
	return -1;
}*/

int operand_type(char *op)
{
	int x;
	printf("in operand_type %s\n", op);
	if ((x = get_register(op)) != -1)
		return REGISTER;
	else if ((x = is_number(op)))
		return NUMBER;
	else 
		return LABEL;
}
/* builds binary word*/
word build_word(int opcode, int src, int dest, int are) 
{
	word w;

	w = (opcode << OPCODE_OFFSET) | (src << SRC_OFFSET) | (dest << DEST_OFFSET) | (are << ARE_OFFSET);
	return w;
}

/* returns the numbers of words we should add to a code line*/
int words_num(word w)
{
	int src = (w >> SRC_OFFSET) & 7; /* 111 in binary */
	int dest = (w >> DEST_OFFSET) & 7;
	/*printf("in words_num %d %d\n", src, dest);*/
	/*int opcode = (w >> OPCODE_OFFSET) & 15;*/
	printf("%d\n", w);
	if (src == REGISTER && dest == REGISTER) /* if we have two registers we can write them in a single word*/
		return 1;
	else if ((src == NONE) ^ (dest == NONE)) /* if we have command with only one operand */
		return 1;
	else if (src != NONE && dest != NONE) 
		return 2;
	else if (src == NONE && dest == NONE)
		return 0;
	return -1;
}

/* encodes the data to the data segment */
void encode_data(char *line, int *dc, int label_exists)
{
	int i;
	char *line_copy = malloc(sizeof(char) * 80);
	char *tok;

	strcpy(line_copy, line);
	tok = strtok(line_copy, " ,\t\n");
	if (label_exists)
		tok = strtok(NULL, " ,\t\n");

	if (strcmp(tok, ".string") == 0) {
		tok = strtok(NULL, " \",\t\n");
		for (i = 0; i <= strlen(tok); ++*dc, i++) 
			data[*dc] = tok[i]; /* encodes .string directives */
	} else if (strcmp(tok, ".data") == 0) {
		tok = strtok(NULL, " ,\t\n");
		while (tok != NULL) {
			data[(*dc)++] = atoi(tok);
			tok = strtok(NULL, " ,\t\n");
		}
	}
	printf("\n\nDC: %d\n\n", *dc);
	free(line_copy);
}

/* returns the name of the source operand */
char *get_src_name(char *line, int label_exists)
{
	char *pc;
	char *line_copy = malloc(sizeof(char) * 80);
	int x;

	strcpy(line_copy, line);
	x = op_num(line, label_exists);
	printf("in get_src_name %s\n", line_copy);
	if (x == 3) {

		pc = strtok(line_copy, " ,\t\n");
		if (label_exists)
			pc = strtok(NULL, " ,\t\n");
		pc = new_string(strtok(NULL, " ,\t\n"));
		printf("pc : %s\n", pc);
		free(line_copy);
		return pc;
	}
	return 0;
}
/* get the source type (REGISTER, LABEL or NUMBER) */
int get_src(char *line, int label_exists) 
{
	int op;
	char *pc;
	char *line_copy = malloc(sizeof(char) * 80);
	int x;

	strcpy(line_copy, line);
	x = op_num(line, label_exists);

	if (x == 3) {
		pc = strtok(line_copy, " ,\t\n");
		if (label_exists)
			pc = strtok(NULL, " ,\t\n");
		pc = strtok(NULL, " ,\t\n");
		printf("%s\n", pc);
		op = operand_type(pc);
		free(line_copy);
		return op;
	}
	return 0;
}

/* returns the name of the destination operand*/
char *get_dest_name(char *line, int label_exists)
{
	char *pc;
	char *line_copy = malloc(sizeof(char) * 80);
	int x;

	x = op_num(line, label_exists);
	strcpy(line_copy, line);

	if (x == 2) { /* means we have here opcode and single operand */
		pc = strtok(line_copy, " ,\t\n");
		if (label_exists)
			pc = strtok(NULL, " ,\t\n");
		pc = new_string(strtok(NULL, " ,\t\n"));
		free(line_copy);
		return pc;
	} else if (x == 3) {
		pc = strtok(line_copy, " ,\t\n");
		if (label_exists)
			pc = strtok(NULL, " ,\t\n");
		pc = strtok(NULL, " ,\t\n");
		pc = new_string(strtok(NULL, " ,\t\n"));
		free(line_copy);
		return pc;
	} 
	return 0;
}

/* returns the number of the destination operand */
int get_dest(char *line, int label_exists)
{
	int op = NONE;
	char *pc;
	char *line_copy = malloc(sizeof(char) * 80);
	int x = op_num(line, label_exists);

	strcpy(line_copy, line);
	printf("in get_dest %d\n", x);
	if (x == 2) {
		pc = strtok(line_copy, " ,\t\n");
		if (label_exists)
			pc = strtok(NULL, " ,\t\n");
		pc = strtok(NULL, " ,\t\n");
		printf("in get_dest op = %s x = 2\n", pc);
			
	} else if (x == 3) {
		pc = strtok(line_copy, " ,\t\n");
		if (label_exists)
			pc = strtok(NULL, " ,\t\n");
		pc = strtok(NULL, " ,\t\n");
		pc = strtok(NULL, " ,\t\n");
		printf("in get_dest op = %s\n", pc);

	} 
	if (x != 2 && x != 3){
		free(line_copy);
		return 0;
	}
	op = operand_type(pc);
	return op;
}

/* converts the integer to base64 representation */
char *encode_base64(word w)
{
    char table[] =  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                    'w', 'x', 'y', 'z', '0', '1', '2', '3',
                    '4', '5', '6', '7', '8', '9', '+', '/'};
    
    char* b64 = malloc(sizeof(char*) * 3);
    b64[0] = table[(w >> 6) & 63];
    b64[1] = table[w & 63];
    b64[2] = '\0';
    return b64;
}

/* checks if a string is a number */
int is_number(char *s) 
{ 
	int i;

	if (s[0] == '-' || s[0] == '+')
		s++;
    for (i = 0; i < strlen(s); i++) 
        if (!isdigit(s[i])) 
            return 0; 
  
    return 1; 
} 

/* builds the attached binary words that are added to the instruction line */
word add_word(tnode *head, char *line, int t, int label_exists)
{
	int op1, op2;
	tnode *cp;
	printf("%s\n", line);
	char *src  = get_src_name(line, label_exists);
	char *dest = get_dest_name(line, label_exists);

	/*printf("in add_word %s\n", line);*/
	op1 = get_register(src);
	op2 = get_register(dest);
	if (op1 != -1 && op2 != -1)
		return (op1 << 7) | (op2 << DEST_OFFSET) | A;
	if (t == SOURCE) {
		if (op1 != -1)
			return op1 << 7 | A;
		else if (is_number(src))
			return atoi(src) << 2 | A;
		else if ((cp = find_label(head, src))) {
			printf("-=-==============================-");
			if (cp->type == EXTERN)
				return E; /* label is relocatable */
			else 
				return cp->counter << 2 | R; /* label is relocatable */
		} else
			return 0;
		
	} else if (t == DESTINATION) {
		if (op2 != -1)
			return op2 << DEST_OFFSET | A;
		else if (is_number(dest)) 
			return atoi(dest) << 2 | A;
		else if ((cp = find_label(head, dest))) {
			if (cp->type == EXTERN)
				return E;
			else
				return cp->counter << 2 | R;
		} else 
			return 0;

	} else {
		printf("error: cannot add word\n");
		return 0;
	}
	
}



/* finds the register number if it is a register */
int get_register(char *reg) 
{
	int i;
	if (reg == NULL)
		return -1;
	/*printf("in get_register %s\n", reg);*/
	for (i = 0; i < 8; i++) {
/*		printf("reg %s\n", registers[i]);
*/		if (strcmp(reg, registers[i]) == 0)
			return i;
	}
	return -1;
}

/* checks whether a string has a label */
int has_label(char *line)
{
	if (isalpha(*line))
		while (isalnum(*line++))
			;
    if (*--line == ':')
		return 1;
	return 0;
}

/* */
char *get_label(char *line, int type)
{
	char *line_copy = malloc(sizeof(char) * 80);
    char *pc;
	strcpy(line_copy, line);

    pc = strtok(line_copy, " \t\n,");
    /* in case we have external or entry declerations */
	if (type == EXTERN || type == ENTRY) {
		pc = strtok(NULL, " \t\n,");
		return pc;
	}
    if (is_label(pc)) {
    	pc[strlen(pc)-1] = '\0';
        return pc;
    }
    free(line_copy);
/*	printf("in get_label: %s \n", pc);
*/    return NULL;
}

int is_label(char *label)
{
    if (isalpha(*label))
        while (isalnum(*label++))
			;
	/*printf("in is_label\n");*/
    if (*--label == ':') {
    	/*printf("in is_label true\n");*/
        return 1; 
    }
    return 0;
}

int direct_type(char *line)
{
	char *pc;
	char *line_copy = malloc(sizeof(char) * 80);

	if (line[0] == ';' || line[0] == '\n' || line[0] == '\0')
		return IGNORE;

	strcpy(line_copy, line);
	pc = strtok(line_copy, " ,\t\n");
	if (is_label(pc)) {
		pc = strtok(NULL, " ,\t\n");
		/*printf("in direct_type pc = %s\n", pc);*/
		if (strcmp(pc, ".extern") == 0) {
			free(line_copy);
			return EXTERN;
		}
		else if (strcmp(pc, ".entry") == 0) {
			free(line_copy);
			return ENTRY;
		}
	}

	if (strcmp(pc, ".extern") == 0) {
		free(line_copy);
		return EXTERN;
	}
	else if (strcmp(pc, ".entry") == 0) {
		free(line_copy);
		return ENTRY;
	}
	else if (strcmp(pc, ".data") == 0) {
		free(line_copy);
		return DATA;
	}
	else if (strcmp(pc, ".string") == 0){
		free(line_copy);
		return STRING;
	}
	free(line_copy);
	return NONE;
}

/* returns the opcode num */
int get_opcode(char *line, int label_exists)
{
	int i;
	char *pc;
	char *line_copy = malloc(sizeof(char) * 80);

	strcpy(line_copy, line);

    pc = strtok(line_copy, " \t\n,");

    if (label_exists)
        pc = strtok(NULL, " \t\n,");
    /*printf("in get_opcode %s\n", pc);*/
    for(i = 0; i < 16; i++)
        if (strcmp(opcodes[i], pc) == 0) {
        	free(line_copy);
            return i;
        }
    free(line_copy);
    return -1;
}

