#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<limits.h>
#include<string.h>

#define L_T 1
#define L_E 2
#define G_T 3
#define G_E 4
#define NOT 5
#define NOT_E 6
#define Assign 7
#define EQUAL 8
#define XOR 9
#define MOD 10
#define PLUS 11
#define MINUS 12
#define STAR 13
#define DIVIDE 14
#define B_AND 15
#define B_OR 16

#define ERROR -1

#define L_PAREN 17
#define R_PAREN 18
#define L_BRAC 19
#define R_BRAC 20
#define L_BIG_BRAC 21
#define R_BIG_BRAC 22
#define SEMI_COLON 23
#define COMMA 24

#define CHAR_SET_SIZE 128
#define KEY_WORDS 26

int first_op[CHAR_SET_SIZE], second_op[CHAR_SET_SIZE];

int delimiter[CHAR_SET_SIZE];

char lex[200];

char *key_words[KEY_WORDS] = {
	"void", "int", "double", "char", "long", "float", "switch", "case", "short",
	"if", "else", "for", "while", "break", "return", "continue", "default", "static", 
	"sizeof", "struct", "union", "default", "signed", "unsigned", "const", "do"};
	
void memory_set_call(void);
void my_gets(char *, int);
void lexical_analysis(char *);
int iskeyword(char *);
	
int main(int argc, char **argv[])
{
	memory_set_call();
	char file_name[100];
	printf("\nEnter file name : "), my_gets(file_name, 100);
	printf("\n\n");
	lexical_analysis(file_name);
	return 0;
}

void lexical_analysis(char *file_name)
{
	FILE *fp1 = NULL;
	fp1 = fopen(file_name, "r+");
	if(fp1 == NULL)
	{
		fprintf(stderr, "\n\nUnable to open file : %s\n\n", file_name);
		return;
	}
	int i, c1, c2, line_number = 1, d_flag = 0, e_flag;
	int k_words = 0, identifiers = 0, num = 0, ch = 0, str = 0, del = 0, op = 0;
	while(!feof(fp1))
	{
		c1 = fgetc(fp1);
		if(c1 == EOF)
			break;
		if(isdigit(c1)) // for numeric constants
		{
			i = 0, e_flag = 0;
			while(1)
			{
				lex[i++] = c1;
				c1 = fgetc(fp1);
				if(isdigit(c1))
					continue;
				else if(delimiter[c1] != ERROR || c1 == ' ' || c1 == '\t' || c1 == '\n')
				{
					fseek(fp1, -1, SEEK_CUR);
					lex[i] = '\0';
					break;
				}
				else
				{
					if(c1 == '.' && d_flag == 0)
					{
						d_flag = 1;
						continue;
					}
					else
					{
						e_flag = 1;
						break;	
					}	
				}
			}
			if(!e_flag)
			{
				printf("Token : %s\tLexeme : %s\n", "Numeric Constant", lex);
				num++;
			}
			else
			{
				fprintf(stderr, "\nError found at line %d\n", line_number);
				break;
			}
		}
		else if(isalpha(c1)) // for identifiers
		{
			i = 0;
			while(1)
			{
				lex[i++] = c1;
				c1 = fgetc(fp1);
				if(isalnum(c1))
					continue;
				else
				{
					fseek(fp1, -1, SEEK_CUR);
					lex[i] = '\0';
					break;
				}
			}
			if(iskeyword(lex))
			{
				k_words++;
				printf("Token : %s\tLexeme : %s\n", "Key Word", lex);
			}
			else
			{
				identifiers++;
				printf("Token : %s\tLexeme : %s\n", "Identifier", lex);
			}
		}
		else if(c1 == '\'') // character constant
		{
			c2 = fgetc(fp1);
			c1 = fgetc(fp1);
			if(c1 == '\'')
			{
				printf("Token : %s\tLexeme : %c\n", "Character Constant", c2);
				ch++;
			}
			else
			{
				fprintf(stderr, "\nError found at line %d\n", line_number);
				break;
			}
		}
		else if(c1 == '\"') // string constant
		{
			i = 0;
			while(1)
			{
				lex[i++] = c1;
				c1 = fgetc(fp1);
				if(c1 == '\"')
				{
					lex[i] = '\0';
					break;
				}
			}
			str++;
			printf("Token : %s\tLexeme : %s\n", "String Constant", lex);
		}
		else
		{
			if(first_op[c1] != ERROR) //for operators
			{
				c2 = fgetc(fp1);
				if(second_op[c2] == ERROR)
				{
					fseek(fp1, -1, SEEK_CUR); // SEEK_END(Last point), SEEK_CUR (Current position), SEEK_SET (Starting point)
					lex[0] = c1, lex [1] = '\0';
					printf("Token : %s\tLexeme : %s\n", "Operator", lex);
				}
				else
				{
					lex[0] = c1, lex[1] = c2, lex[2] = '\0';
					printf("Token : %s\tLexeme : %s\n", "Operator", lex);
				}
				op++;
			}
			else if(delimiter[c1] != ERROR) //for delimiters
			{
				lex[0] = c1, lex[1] ='\0';
				printf("Token : %s\tLexeme : %s\n", "Delimiter", lex);
				del++;
			}
			else //for errors
			{
				if(c1 == ' ' || c1 == '\n' || c1 == '\t') //not an error
				{
					if(c1 == '\n')
					{
						line_number++;
						printf("\n");
					}
					continue;
				}
				else //error
				{
					fprintf(stderr, "\nError found at line %d\n", line_number);
					break;
				}
			}
		}
	}
	printf("\nKeywords : %d\nIdentifiers : %d\n", k_words, identifiers);
	printf("Operators : %d\tDelimiters : %d\n", op, del);
	printf("Numeric Constants : %d\tCharacter Constants : %d\n", num, ch);
	printf("String Constants : %d\n\n", str);
	fclose(fp1), fp1 = NULL;	
}

int iskeyword(char *arr)
{
	int i, f = 0;
	for(i = 0; i < KEY_WORDS; i++)
	{
		if(!strcmp(key_words[i], arr))
		{
			f = 1;
			break;
		}
	}
	return f;	
}

void my_gets(char *arr, int size)
{
	int i = 0, c;
	while((c = getchar()) != '\n')
	{
		if(i > size - 2)
			break;
		arr[i++] = c;
	}
	arr[i] = '\0';
	return;
}

void memory_set_call(void)
{
	memset(first_op, -1, CHAR_SET_SIZE * sizeof(int));
	memset(second_op, -1, CHAR_SET_SIZE * sizeof(int));
	memset(delimiter, -1, CHAR_SET_SIZE * sizeof(int));
	first_op['<'] = L_T, first_op['>'] = G_T, first_op['!'] = NOT, first_op['='] = Assign,
	first_op['%'] = MOD, first_op['+'] = PLUS, first_op['-'] = MINUS, first_op['*'] = STAR,
	first_op['/'] = DIVIDE, first_op['&'] = B_AND, first_op['|'] = B_OR;
	delimiter['{'] = L_PAREN, delimiter['}'] = R_PAREN, delimiter['('] = L_BRAC, delimiter[')'] = R_BRAC, 
	delimiter['['] = L_BIG_BRAC, delimiter[']'] = R_BIG_BRAC, delimiter[';'] = SEMI_COLON, delimiter[','] = COMMA;
	second_op['='] = EQUAL;
	return;
}
