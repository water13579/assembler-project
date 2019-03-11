#include "main.h"

void second_scan(FILE *fp, word *code, tnode *symbols_table)
{
	int t;
	int L;
	int LABEL_EXISTS;
	char line[LINE];
	tnode *head = symbols_table;
	char *label;

	IC = 0; /* initializes IC to 0   */
	printf("started second_scan\n");
	while(fgets(line, LINE, fp) != NULL) 
	{

		printf("line: %s IC: %d\n", line, IC);
		t = direct_type(line);
		if (t == IGNORE)
			continue;
		if ((label = get_label(line, t)))
			LABEL_EXISTS = TRUE;
		else 
			LABEL_EXISTS = FALSE;
		/*printf("LABEL_EXISTS: %d\n", LABEL_EXISTS);*/
		if (t == DATA || t == STRING || t == EXTERN) /* finds kind of directive */
			continue;
		else if (t == ENTRY) {
			insert_symbol(head, label, 0, ENTRY);
			ENTRY_EXISTS = 1;
		} else {
			L = words_num(code[IC]); /* checks how many words to add to the command word */
			printf("L = %d", L);
			if (L == 1) {
				code[IC+1] = add_word(head, line, DESTINATION, LABEL_EXISTS);
			} else if (L == 2) {
				printf("HELLO\n");
				code[IC+1] = add_word(head, line, SOURCE, LABEL_EXISTS); /* builds the rest of the block */
				code[IC+2] = add_word(head, line, DESTINATION, LABEL_EXISTS);
				printf("%d %d \n", code[1], code[2]);
			}
			IC += L+1; /* get to the next words block */

		}
	}
	printf("done with second_scan\n");
	fclose(fp);
}



