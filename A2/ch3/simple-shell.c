/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>



#define MAX_LINE		80 /* 80 chars per line, per command */
#define HIST_SIZE 		100
int count = 0;

struct node {
	char data[50];
	struct node *next;
};


struct node *head = NULL;

void insert_at_begin(char* x) { // use strcpy to copy string
	struct node *t;

	t = (struct node*)malloc(sizeof(struct node));

	if (head == NULL) {
		head = t;
		strcpy(head->data,x);
		head->next = NULL;
		count++;
		return;
	}

	strcpy(t->data,x);
	t->next = head;

	head = t;
	count++;

}
int check_list(char *x) {
	int present = 0;
	struct node *current = head;

	while (current != NULL) {
		if(current->data == x)
		{
			present = 1;
		}
		current = current->next;
	}

	return present;
}


void traverse() {
	struct node *t;

	t = head;

	if (t == NULL) {
		printf("Linked list is empty.\n");
		return;
	}

	if(count>10)
	{
		int i = 0;
		while(i<10)
		{

			printf("%s\n", t->data);
			t = t->next;
			i++;
		}
	} 
	else
	{ 
		while (t != NULL) {

			printf("%s\n", t->data);
			t = t->next;
		}
	}

}

/*HISTORY FEATURE*/

int retrieveHistory(char *input, char **args, int check)
{

	if(!strcmp(args[0],"history"))
	{
		if(count<=0)
		{
			printf("No commands in the history!\n");
		}	
		else
		{
			traverse();
		}

		return -1;
	}
	else if (!strcmp(args[0],"!!"))
	{
		if(count>0)
		{
			char *param;

			param = strtok(head->data," \n");
			args[0] = param;
			param = strtok(NULL," \n");
		}
		else
		{
			printf("No commands in the history!\n");
		}
		
	}
	else if(input[0] == '!' && isdigit(input[1]))
	{

		int a = input[1] - '0';
		a--;
		struct node * temp = head;
		int i = 0;
		while(i<a)
		{
			temp = temp->next;
			i++;
		}

		char *param;

		param = strtok(temp->data," \n");
		args[0] = param;
		param = strtok(NULL," \n");
	}
	
	return 1;
}




int parseinput(char *input, char **args)
{
	int n_cmd = 0;
	int i = 0;
	char *param;

	param = strtok(input," \n");

	while(param!=NULL)
	{
		args[i] = (char*)malloc(MAX_LINE*sizeof(char));
		args[i] = param;

		i++;
		param = strtok(NULL," \n");
	}

	args[i] = NULL;
	return i;

}

int CheckAmpersand(int arg, char **args)
{	
	char* c;
	int yes = 0;
	int i;
	for(i = 0; i<arg; i++)
	{
		c = args[i];
		if((strcmp(c,"&") == 0) && (i == (arg-1)))
		{
			yes = 1;
			break;
		}
	}

	if(yes)
	{
		args[arg-1] = NULL;
	}


	return yes;
	
}



int main(void)
{
	int iter = 0;
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	int should_run = 1;
	char input[MAX_LINE];
	int arg = 0;
	int check;
	pid_t pid;
	int i, upper;

	while (should_run){   
		printf("osh>");
		fflush(stdout);

		fgets(input, MAX_LINE,stdin);
		if(input[0] == '\n')
		{
			continue;
		}
		if(!strcmp(input,"exit\n"))
		{
			should_run = 0;
			return 0;
		}
		/* NOT PUSHING HISTORY OR !!, !%d! */
		if(strcmp(input, "!!\n") != 0)
		{
			if(strcmp(input, "history\n") != 0)
			{
				if(input[0] != '!' && !isdigit(input[1]))
				{
					insert_at_begin(input);
				}
			}
		}

		arg = parseinput(input,args);


		check = CheckAmpersand(arg, args);

		if(-1 != retrieveHistory(input,args,check))
		{
			pid = fork();

			if(pid < 0)
			{
				fprintf(stderr,"No child process created\n");
				exit(1);
			}
			else if(pid == 0)
			{
				execvp(args[0],args);
				printf("Invalid command!\n");
				return 0;
			}
			else
			{
				if(!check)
				{


					wait(NULL);
					fflush(stdout);
					printf("Child complete!\n");



				}
			}
		}



	}


	return 0;
}



