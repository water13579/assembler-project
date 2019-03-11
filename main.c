#include "main.h"

word code[1024]; /* can be a maximum of 1024 words */
word data[1024]; 
tnode *head = NULL; /* the head of the symbols table */
tnode *extern_list = NULL; /* the head of the extern_list that contains all the data for the .ext file */
tnode *entry_list = NULL; /* the head of the entry_list that contains all the data for the .ent file */

int IC = 0;
int DC = 0;
int ENTRY_EXISTS = 0;
int EXTERN_EXISTS = 0;

int main(int argc, char *argv[])
{
	FILE *fp;
	char filename[50];
	char infile[50];
	char outfile[50];
	char extfile[50];
	char entfile[50];
	int i;
	tnode *temp = NULL;

	if (argc == 1)
		printf("No files entered\n");

	argv++;
	while (--argc) {
		strcpy(filename, *argv++);
		strcpy(infile, filename);
		strcat(infile, ".as");
		if ((fp = fopen(infile, "r")) == NULL) 
			printf("error: failed to open %s\n", infile);
		else {
			head = first_scan(fp, code, data, head);
			fclose(fp);
			fp = fopen(infile, "r");
			update_data(head, IC);
			second_scan(fp, code, head);
			fclose(fp);
			if ((IC + DC) >= 1024) /* we have only 1024 words of 12 bits in our ccomputer */
				printf("error: ran out of memory\n");
			strcpy(outfile, filename);
			strcat(outfile, ".ob");
			printf("%s\n", outfile);
			fp = fopen(outfile, "w");
			fprintf(fp, "%d\t%d\n", IC, DC);
			for (i = 0; i < IC; i++) {
				fprintf(fp, "%s\n", encode_base64(code[i]));
				printf("%s\n", encode_base64(code[i]));
			}

			printf("IC: %d\tDC: %d\n", IC, DC);
			for (i = 0; i < DC; i++)
				fprintf(fp, "%s\n", encode_base64(data[i]));
			fclose(fp);
			/* creates the .ent and .ext files */
			if (EXTERN_EXISTS) {
				strcpy(extfile, filename);
				strcat(extfile, ".ext");
				fp = fopen(extfile, "w");
				temp = head;
				printf("%s\n", head->label);
				while (temp != NULL) {
					if (temp->type == EXTERN)
						fprintf(fp, "%s\t\t%d\n", temp->label, temp->counter);
					temp = temp->next;
				}
				fclose(fp);
			}
			if (ENTRY_EXISTS) {
				strcpy(entfile, filename);
				strcat(entfile, ".ent");
				fp = fopen(entfile, "w");
				temp = head;
				while (temp != NULL) {
					if (temp->type == ENTRY)
						fprintf(fp, "%s\t\t%d\n", temp->label, temp->counter);	
					temp = temp->next;			
				}
				fclose(fp);
			}

		}

	}
	print_list(head);
	free_tnode_list(head);

	return 0;
}

