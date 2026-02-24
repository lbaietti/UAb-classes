#include<stdio.h>
#include<string.h>

#define MAXSTR 255

void strinv(char *str)
{
    int len=strlen(str);
    int i, j;
    char aux; 

    for(i=0, j=len-1;i<j; i++, j--)
    {
        aux=str[i];
        str[i]=str[j];
        str[j]=aux;
    }
}

int main()
{
    char str[MAXSTR];
    fgets(str, MAXSTR, stdin);
    str[strlen(str)-1]=0;
    strinv(str);
    printf("%s", str);
}