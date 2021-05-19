#include<stdio.h>
#include<conio.h>
#include<string.h>
#define row 100
#define col 100
#define NULL 0

struct data
{
    int state_no;
    int count;
    char items[row][col];
    struct data *link2;
};

typedef struct data node;

void closure(char []);
void Goto(char [],int);
void CreateTable(int,char,char[],char[],char[]);

char Agumented[col]={'Q','-','>','.','S','$'};
int n,state=0,result=0;
char Grammar[row][col],P1[row][col],symbols[col],table[row][col],GrammarFinal[row][col];
node *start=NULL;

int main()
{
    char str[30],stack[30],temp[col];
    node *ptr1;

    int i,j,NoOfSymbols,flag=0,k,size;

    printf("\nEnter the number of rules:- ");
    scanf("%d",&n);
    fflush(stdin);
    printf("\nEnter the Start Symbol:- ");
    scanf("%c",&Agumented[4]);
    fflush(stdin);
    printf("Enter the terminal first Followed by Non terminals \n");
    scanf("%s",symbols);
    fflush(stdin);
    printf("\nEnter the rules one by one\n");
    for(i=0;i<n;i++)
    {
        printf("%d:- ",i+1);
        scanf("%s",temp);
        size=strlen(temp);

        strcpy(GrammarFinal[i],temp);
        GrammarFinal[i][size-1]='.';
        GrammarFinal[i][size]='$';
        GrammarFinal[i][size+1]='\0';

        for(j=0;j<=size+1;j++)
        {
            if(j==3)
                Grammar[i][j]='.';
            if(j>3)
                Grammar[i][j]=temp[j-1];
            if(j<3)
                Grammar[i][j]=temp[j];
            if(j==strlen(temp)+1)
               Grammar[i][j]='\0';
        }
    }

    closure(&Agumented[0]);

    ptr1=start;                                 // GOTO Function call
    while(ptr1!=NULL)
    {
        for(i=0;i<ptr1->count;i++)
        {
            Goto(ptr1->items[i],ptr1->state_no);
        }
        ptr1=ptr1->link2;
    }

    ptr1=start;
    printf("\n");
    while(ptr1!=NULL)
    {
        printf("<-------------STATE %d----------------------->\n",ptr1->state_no);
        for(i=0;i<ptr1->count;i++)
        {
            printf("%s\n",ptr1->items[i]);
        }
        ptr1=ptr1->link2;
    }
    printf("\n PARSING TABLE  \n"); // Printing Parsing Table
    for(j=0;j<strlen(symbols);j++)
    {
        printf("%c    ",symbols[j]);
    }
    printf("\n");

    for(i=0;i<=state;i++)
    {
        for(j=0;j<strlen(symbols);j++)
        {
            printf("%c",table[i][j*4]);
            printf("%c",table[i][(j*4)+1]);
            printf("%c  ",table[i][(j*4)+2]);
        }
        printf("\n");
    }
    if(result==0)
        printf("The Given Grammar is LR(0)\n");
    else
        printf("The Given Grammar is Not LR(0)\n");


return 0;
}



void closure(char a[])
{
   int i,j,k,c1=0,n1=0,flag=0,z;
   node *ptr,*ptr1=start;

   ptr=(node *)malloc(sizeof(node));
   ptr->state_no=state;
   ptr->link2=NULL;
   strcpy(ptr->items[c1],a);
   c1++;

   strcpy(P1[n1],a);
    n1++;

   for(k=0;k<n1;k++)
   {
        flag=0;
        for(i=0;i<strlen(P1[k]);i++)
        {
            if(P1[k][i]=='.' && P1[k][i+1]!='$')
            {
                for(z=1;z<n1;z++)
                {
                    if(P1[z][0]==P1[k][i+1])
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                   for(j=0;j<n;j++)
                   {
                        if(P1[k][i+1]==Grammar[j][0])
                        {
                            strcpy(P1[n1],Grammar[j]);
                            n1++;
                            strcpy(ptr->items[c1],Grammar[j]);
                            c1++;
                        }
                    }
                }

                break;
            }
        }
   }

    ptr->count=c1;
    if(start==NULL)
    {
        start=ptr;
    }
    else
    {
       while(ptr1->link2!=NULL)
           ptr1=ptr1->link2;

       ptr1->link2=ptr;
    }
}

void Goto(char a[],int s_no)
{
    char b[col],temp,buffer[3];
    int i,flag=0,j,pos=0,k;
    node *ptr1;

    for(i=0;i<strlen(a);i++)
    {
        if(a[i]=='.' && a[i+1]!='$')
        {
              b[i]=a[i+1];
              temp=a[i+1];
              b[i+1]='.';
              i=i+1;
        }
        else
        {
           b[i]=a[i];
        }
    }
    b[i]='\0';

    ptr1=start;
    while(ptr1!=NULL)
    {
        if(strcmp(b,ptr1->items[0])==0)
        {
            flag=1;

            sprintf(buffer,"%d",ptr1->state_no);
            CreateTable(s_no,temp,buffer,a,b);

            break;
        }
        ptr1=ptr1->link2;
    }

    if(flag==0)
    {
        state++;

        sprintf(buffer,"%d",state);
        CreateTable(s_no,temp,buffer,a,b);

        closure(&b[0]);
    }
}

void CreateTable(int s_no,char temp,char buffer[],char a[],char b[])
{
    int k,flag=0,i;

    for(k=0;k<strlen(symbols);k++)
    {
        if(temp==symbols[k])
        {
            flag=1;
            break;
        }
    }
    if(flag==0)
    {
        printf("Enter the symbols correctly \n");
        printf("%s %s",temp,symbols[k]);
        exit(0);
    }


    if(temp>=65 && temp<=90)                    //Goto Phase
    {
        table[s_no][k*4]=' ';
        table[s_no][(k*4)+1]=buffer[0];
        table[s_no][(k*4)+2]=buffer[1];
    }
    else if(strcmp(a,b)==0)                     //Reduce Phase
    {
        for(i=0;i<n;i++)
        {
            if(strcmp(a,GrammarFinal[i])==0)
            {
                sprintf(buffer,"%d",i+1);
                    break;
            }
        }
        for(k=0;k<strlen(symbols);k++)
        {
            if(a[0]=='Q' && symbols[k]=='$')
            {
                   table[s_no][k*4]='A';
            }
            else if(a[0]!='Q')
            {
                if((symbols[k]>=97 && symbols[k]<=122)||(symbols[k]>=35 && symbols[k]<=57))
                {
                    if(table[s_no][k*4]=='r'|| table[s_no][k*4]=='s')
                        result=1;
                    table[s_no][k*4]='r';
                    table[s_no][(k*4)+1]=buffer[0];
                    table[s_no][(k*4)+2]=buffer[1];
                }
                else
                    break;
            }
        }
    }
    else if(strcmp(a,b)!=0)                 //Shift Phase
    {
        if(table[s_no][k*4]=='r'|| table[s_no][k*4]=='s')
            result=1;
       table[s_no][k*4]='s';
       table[s_no][(k*4)+1]=buffer[0];
       table[s_no][(k*4)+2]=buffer[1];
    }

}
