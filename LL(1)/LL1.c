#include<stdio.h>
#define max 100


char NonTerminals[max],Terminals[max],production[max][max];
char follow[max];
int x=0,n;
void find_follow(char);
void find_first(char);

int main()
{

    int i,j,k=0,length,flag,l,matrix[max][max];

    printf("\nEnter the number of rules:- ");
    scanf("%d",&n);

    printf("\nEnter the rules one by one\n");
    for(i=0;i<n;i++)
    {
        printf("%d:- ",i+1);
        scanf("%s",production[i]);
        flag=0;
        for(l=0;l<k;l++)
        {
            if(NonTerminals[l]==production[i][0])
                flag=1;
        }
        if(flag==0)
            NonTerminals[k++]=production[i][0];
    }
    NonTerminals[k]='\0';
    k=0;
    printf("\n\n");
    for(i=0;i<n;i++)
    {
        length=strlen(production[i]);
        for(j=3;j<length;j++)
        {
            if(!(production[i][j]>=65 && production[i][j]<=90))
            {
                flag=0;
                for(l=0;l<k;l++)
                {
                    if(Terminals[l]==production[i][j])
                        flag=1;
                }
                if(flag==0)
                    Terminals[k++]=production[i][j];
            }

        }
    }
    Terminals[k]='\0';

    for(i=0;i<strlen(NonTerminals);i++)
    {
        for(j=0;j<n;j++)
        {
            if(NonTerminals[i]==production[j][0])
            {
                if(production[j][3]=='$')
                {
                    Array_Manipulation('$');

                    find_follow(production[j][0]);
                }
                else
                {
                    find_first(production[j][3]);

                }

                for(l=0;l<strlen(Terminals);l++)
                {
                    for(k=0;k<x;k++)
                    {
                        if(follow[k]==Terminals[l])
                            matrix[i][l]=j+1;
                    }
                }
                x=0;
            }
        }
        x=0;
    }
    printf("    LL(1) PARSING TABLE\n\n");

    for(i=0;i<strlen(Terminals);i++)
        printf("     %c",Terminals[i]);
    printf("\n\n");

    for(i=0;i<strlen(NonTerminals);i++)
    {
        printf("%c    ",NonTerminals[i]);
        for(j=0;j<strlen(Terminals);j++)
            if(matrix[i][j]==0)
                printf("      ");
            else
                printf("%d     ",matrix[i][j]);
        printf("\n");
    }

}

void find_follow(char ch)
{
      int i, j,length;

      if(production[0][0] == ch)
      {
            Array_Manipulation('$');
      }
      for(i = 0; i < n; i++)
      {
            length = strlen(production[i]);
            for(j = 3; j < length; j++)
            {
                  if(production[i][j] == ch)
                  {
                        if(production[i][j + 1] != '$')
                        {
                              find_first(production[i][j + 1]);
                        }
                        if(production[i][j + 1] == '$' && ch != production[i][0])
                        {
                              find_follow(production[i][0]);
                        }
                  }
            }
      }
}

void find_first(char ch)
{
      int k;
      if(!(isupper(ch)))
      {
            Array_Manipulation(ch);
      }
      for(k = 0; k <n; k++)
      {
            if(production[k][0] == ch)
            {
                  if(production[k][3] == '$')
                  {
                        find_follow(production[k][0]);;
                  }
                  else if(islower(production[k][3]))
                  {
                        Array_Manipulation(production[k][3]);
                  }
                  else
                  {
                        find_first(production[k][3]);
                  }
            }
      }
}

void Array_Manipulation(char ch)
{
      int i;
      for(i= 0;i< x;i++)
      {
            if(follow[i] == ch)
            {
                  return;
            }
      }
      follow[x++] = ch;
}
