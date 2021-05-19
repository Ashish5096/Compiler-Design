#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char lhs[100][100], rhs[100][200];

void my_gets(char *, int);
void parse(void);
void parsing(void);
int check(void);

char t1[100], t2[100], input[300], temp[300], stack[300];
char start_symbol;

int n, top = -1;

int main()
{
	parse();
	return 0;
}

int check(void)
{
	if(strlen(stack) != 1)
		return 0;
	if(stack[0] == start_symbol)
		return 1;
	return 0;
}

void parsing(void)
{
	int len = strlen(input), i, l = 0, k;
	input[len] = ' ', input[len + 1] = '\0';
	len = strlen(input);
	printf("String %s\n", input);
	while(l < len)
	{
		i = 0;
		while(input[l] != ' ')
		{
			temp[i] = input[l];
			i++, l++;
		}
		temp[i] = '\0';
		for(k = 0; k < n; k++)
		{	
			if(!strcmp(temp, rhs[k]))
			{
				strcpy(temp, lhs[k]);
				break;
			}
		}
		strcat(stack, temp);
		printf("\nStack -> %s", stack);
		for(k = 0; k < n; k++)
		{
			if(!strcmp(stack, rhs[k]))
			{
				strcpy(stack, lhs[k]);
				break;
			}
		}
		l++;
	}
	printf("\nStack -> %s", stack);
	if(check())
	{
		printf("\n\nAccepted\n\n");
	}
	else
	{
		printf("\n\nRejected\n\n");
	}
}

void parse(void)
{
	//printf("\nNumber of productions : "), scanf("%d", &n);
	FILE *fp1 = NULL;
	char file_name[100];
	printf("\nFile name containing productions : "), my_gets(file_name, 100);
	printf("\nFile name : %s\n", file_name);
	fp1 = fopen(file_name, "r+");
	if(fp1 == NULL)
	{
		fprintf(stderr, "\n\nUnable to open file : %s\n\n", file_name);
		return;
	}
	int i = 0;
	n = 0;
	fscanf(fp1, "%c\n", &start_symbol);
	while(!feof(fp1))
	{
		fscanf(fp1, "%s %s\n", t1, t2);
		//printf("\n%s\t%s\n", t1, t2);
		strcpy(lhs[i], t1), strcpy(rhs[i], t2);
		i++, n++;	
	}
	printf("\nStart Symbol : %c", start_symbol);
	printf("\n\nProductions\n\n");
	for(i = 0; i < n; i++)
	{
		printf("%s -> %s\n", lhs[i], rhs[i]);
	}
	printf("\nInput String : "), my_gets(input, 100);
	printf("\nInput String : %s\n\n", input);
	parsing();
	fclose(fp1), fp1 = NULL;
}

void my_gets(char *arr, int n)
{
	int i = 0, c;
	while((c = getchar()) != '\n')
	{
		if(i > n - 2)
			break;
		arr[i++] = c;
	}
	arr[i] = '\0';
}
