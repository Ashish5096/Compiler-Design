#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int getindex(char);

char stack[200];

int top = -1, i = 0, l;

char prec[][9]={

                            /*input*/

            /*stack    +    -   *   /   ^   i   (   )   $  */

            /*  + */  {'>', '>','<','<','<','<','<','>','>'},

            /*  - */  {'>', '>','<','<','<','<','<','>','>'},

            /*  * */  {'>', '>','>','>','<','<','<','>','>'},

            /*  / */  {'>', '>','>','>','<','<','<','>','>'},

            /*  ^ */  {'>', '>','>','>','<','<','<','>','>'},

            /*  i */  {'>', '>','>','>','>','e','e','>','>'},

            /*  ( */  {'<', '<','<','<','<','<','<','>','e'},

            /*  ) */  {'>', '>','>','>','>','e','e','>','>'},

            /*  $ */  {'<', '<','<','<','<','<','<','<','>'},

                };
                
char *handles[] = {")E(", "E*E", "E+E", "E-E", "E/E", "i", "E^E"}; 
//(E) becomes )E( when pushed to stack

void my_gets(char *, int);

char input[200];

void parse(void);
void shift(void);
void show_stack(void);
void show_input(void);
int reduce(void);
int check();

int h_index = -1;

int main()
{
	printf("\nString to parse : "), my_gets(input, 200);
	printf("\nString : %s\n", input);
	parse();
	return 0;
}

int reduce(void)
{
	int i, len, found, t;
	for(i = 0; i < 7; i++)
	{
		len = strlen(handles[i]);
		if(stack[top] == handles[i][0] && top + 1 >= len)
		{
			found = 1;
			for(t = 1; t < len; t++)
			{
				if(stack[top - t] != handles[i][t])
				{
					found = 0;
					break;
				}
			}
			if(found == 1)
			{
				stack[top - t + 1] = 'E';
				top = top - t + 1;
				h_index = i;
				stack[top + 1] = '\0';
				return 1;
			}
		}
	}
	return 0;
}

void show_stack(void)
{
	int j;
	for(j = 0; j <= top; j++)
		printf("%c", stack[j]);
	return;
}

void show_input(void)
{
	int j;
	for(j = i; j < l; j++)
		printf("%c", input[j]);
	return;
}

int check()
{
	if(top != 2)
		return 0;
	if(stack[0] == '$' && stack[1] == 'E' && stack[2] == '$')
		return 1;
	return 0;
}

void parse(void)
{
	l = strlen(input);
	stack[++top] = '$';
	printf("\nStack\t\t\tInput\t\t\tAction\n");
	int m, n;
	while(i < l)
	{
		shift();		
		printf("\n"), show_stack(), printf("\t\t\t");
		show_input(), printf("\t\t\t"), printf("Shift");
		if(top >= 0 && i < l)
		{
			m = getindex(stack[top]), n = getindex(input[i]);
			if(prec[m][n] == '>')
			{
				while(reduce())
				{
					printf("\n"), show_stack(), printf("\t\t\t");
					show_input(), printf("\t\t");
					printf("\tReduced: E->%s", handles[h_index]);
				}
			}
		}
	}
	if(check())
		printf("\n\nAccepted\n\n");
	else
		printf("\n\nNot Accepted\n\n");
}

void shift(void)
{
	stack[++top] = input[i++];
}

void my_gets(char *arr, int n)
{
	int i = 0, c;
	while((c = getchar()) != '\n')
	{
		if(i > n - 3)
			break;
		arr[i++] = c;
	}
	arr[i++] = '$';
	arr[i] = '\0';
}

int getindex(char c)
{
	switch(c)
    {
	    case '+':return 0;
	    case '-':return 1;
	    case '*':return 2;
	    case '/':return 3;
	    case '^':return 4;
	    case 'i':return 5;
	    case '(':return 6;
	    case ')':return 7;
	    case '$':return 8;
    }
    return -1;
}
