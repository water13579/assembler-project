#include "main.h"

tnode *first_scan(FILE *fp,word code[], word data[], tnode *symbols_table)
{
	int L;
	int opcode, src, dest;
	int LABEL_EXISTS; /* a flag */
	int t;
	char line[LINE];
	char *label;
	tnode *head = symbols_table;
	/*while (fgets(line, LINE, fp) != NULL) 
		printf("%s\n", line);*/
	while (fgets(line, LINE, fp) != NULL) 
	{
		printf("ic: %d\tdc: %d\n", IC, DC);
		if (IC >= 1024 || DC >= 1024) {
			printf("error: ran out of memory\n");
			break;
		}
		printf("%s\n", line);
		t = direct_type(line);

		if (t == IGNORE) 
			continue;
		printf("%d\n", t);

		label = get_label(line, t);
		

		if (label != NULL) {
			printf("hadf\n");
			LABEL_EXISTS = TRUE;
		}
		else 
			LABEL_EXISTS = FALSE;
		
/*		printf("label: %s - LABEL_EXISTS: %d\n", label, LABEL_EXISTS);
*/		/*printf("directive type: %d\n", t);*/
		if (t == STRING || t == DATA) {
			if (LABEL_EXISTS)
				head = insert_symbol(head, label, DC, DATA);
			encode_data(line, &DC, t);		
		} else if (t == EXTERN || t == ENTRY) {
			if (t == EXTERN) {
				head = insert_symbol(head, label, 0, EXTERN);
				EXTERN_EXISTS = 1;
			}
		} else {
			if (LABEL_EXISTS) 
				head = insert_symbol(head, label, IC+100, CODE);
			/*printf("code\n");*/
			opcode = get_opcode(line, LABEL_EXISTS);
			/*printf("opcode: %d\n", opcode);*/
			src = get_src(line, LABEL_EXISTS);
			/*printf("source: %d\n", src);*/
			dest = get_dest(line, LABEL_EXISTS);
			/*printf("dest: %d\n", dest);*/
			code[IC] = build_word(opcode, src, dest, A);
			L = words_num(code[IC])+1;
			/*printf("L: %d\n", L);
			printf("encoded: %s\n", encode_base64(code[IC]));*/
			IC = IC + L;
		}
/*		print_list(head);
*/	}
	printf("done with first_scan\n");
	return head;
}

