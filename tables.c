#include "main.h"


/* creates a duplicate string contains str */
char* new_string(char *str)
{
	int len;
	char *new_str;
	if (str == NULL)
		return NULL;
	len = strlen(str)+1; /* including null character */
	new_str = malloc(sizeof(char)*len);
	strcpy(new_str, str);
	return new_str;
}

/* inserts symbol in a linked list that represents symbols table */
tnode *insert_symbol(tnode *head, char *label, int counter, int type) 
{
	tnode *temp = head;
	if (head == NULL) {
		head = malloc(sizeof(tnode)); 
		head->next = NULL;
		head->label = new_string(label);
		head->counter = counter;
		head->type = type;
	} else if (!label_already_exists(head, label)) { /* the label does not already exists*/
		temp = get_last_item(head);
		temp->next = malloc(sizeof(tnode)); 
		temp->next->label = new_string(label);
		temp->next->counter = counter;
		temp->next->type = type;
		temp->next->next = NULL;
	}
	printf("in insert_symbol\n");
	return head;
}	

/* get last item from a linked list of symbols*/
tnode *get_last_item(tnode *node)
{
	if (node == NULL)
		return NULL;
	while (node->next != NULL) {
		node = node->next;
	}
	return node;
}

int label_already_exists(tnode *head, char *label)
{
	tnode *temp = head;

	if (head == NULL)
		return 0;
	while (temp != NULL) {
		if (strcmp(temp->label, label) == 0) {
			printf("error: label already exists\n");
			return 1;
		}
		temp = temp->next;
	}
	return 0; /* label does not exists */
}

void free_tnode_list(tnode *head)
{
	tnode *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp->label);
		free(temp);
	}
}
tnode *find_label(tnode *head, char *label)
{
	tnode *temp = head;
	printf("in find_label\n");
	if (temp == NULL) {
		printf("NULL is head\n");
		return NULL;
	}
	else {
		while (temp != NULL) {
/*			printf("label %s\n", temp->label);
*/			if (strcmp(temp->label, label) == 0)
				return temp;
			temp = temp->next;
		}
		printf("error: label does not exists\n");
		return NULL;
	}
}

/* for debugging purposes */
void print_list(tnode *head) 
{
	if (head == NULL)
		printf("head is NULL\n");
	while (head != NULL) {
		printf("%s\t%d\t%d\n", head->label, head->counter, head->type);
		head = head->next;
	}
}


void update_data(tnode *head, int n) /* updates the data section locations after the first scan */
{
	tnode *temp;

	for (temp = head; temp != NULL; temp = temp->next)
		if (temp->type == DATA)
			temp->counter += 100+n;
}
