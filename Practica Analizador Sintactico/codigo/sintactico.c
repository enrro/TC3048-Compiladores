#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// constants
#define BUFFER 15
#define MAX_TOKENS 15
// methods
int programa();
int secuencia_sent();
int sentencia();
int sent_if();
int sent_repeat();
int sent_assign();
int sent_read();
int sent_write();
int expre();
int op_comparacion();
int exp_simple();
int opsum();
int term();
int opmult();
int factor();
void empatar();
void senial_error();
void cargarArchivo();
//globals
int iterador = 0;
char token[MAX_TOKENS][BUFFER];

int main()
{

    cargarArchivo();
    programa(token);
    printf("%s \n", "programa finalizado sin anomalias");
    system("read -p 'Press Enter to continue...' var");
    //windows sentences
    //system("PAUSE");
    //getc(stdin);
    return 0;
}


int programa()
{
    return secuencia_sent();
}


int secuencia_sent()
{
    int res = sentencia();
    while(strcmp(token[iterador], ";")==0)
    {
        empatar();
        res = sentencia();
    }
    return res;
}

int sentencia()
{
    if(sent_if())
    {

    }
    else if(sent_repeat())
    {

    }
    else if(sent_assign())
    {

    }
    else if(sent_read())
    {

    }
    else if(sent_write())
    {

    }
    else
    {
        senial_error();
    }
    return 1;
}

int sent_if()
{
    if((strcmp(token[iterador], "if") == 0))
    {
        empatar();
        if(expre())
        {
            if((strcmp(token[iterador], "then") == 0))
            {
                empatar();
                if(secuencia_sent())
                {
                    if((strcmp(token[iterador], "else") == 0))
                    {
                        empatar();
                        return secuencia_sent();
                    }
                    if((strcmp(token[iterador], "end") == 0))
                    {
                        empatar();
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int sent_repeat()
{
    if((strcmp(token[iterador], "repeat") == 0))
    {
        empatar();
        if(secuencia_sent())
        {
            if((strcmp(token[iterador], "until") == 0))
            {
                empatar();
                return expre();
            }
        }
    }
    return 0;
}

int sent_assign()
{
    if((strcmp(token[iterador], "identificador") == 0))
    {
        empatar();
        if((strcmp(token[iterador], ":=") == 0))
        {
            empatar();
            return expre();
        }
    }
    return 0;
}

int sent_read()
{
    if((strcmp(token[iterador], "read") == 0))
    {
        empatar();
        if((strcmp(token[iterador], "identificador") == 0))
        {
            empatar();
            return 1;
        }
    }    
    return 0;

}

int sent_write()
{
    if((strcmp(token[iterador], "write") == 0))
    {
        empatar();
        return expre();
    }
    return 0;
}

int expre()
{
    int res = exp_simple();
    if(res && op_comparacion())
    {
        return exp_simple();
    }
    return res;
}

int op_comparacion()
{
    if((strcmp(token[iterador], "<") == 0))
    {
        empatar();
        return 1;
    }
    else if ((strcmp(token[iterador], "=") == 0))
    {
        empatar();
        return 1;
    }
    else
        return 0;
}

int exp_simple()
{
    int res = term();
    if(res && opsum())
    {
        return term();
    }
    return res;

}

int opsum()
{
    if((strcmp(token[iterador], "+") == 0))
    {
        empatar();
        return 1;
    }
    else if ((strcmp(token[iterador], "-") == 0))
    {
        empatar();
        return 1;
    }
    else
        return 0;
}

int term()
{
    int res = factor();
    if(res && opmult())
    {
        return factor();
    }
    return res;
}

int opmult()
{
    if((strcmp(token[iterador], "*") == 0))
    {
        empatar();
        return 1;
    }
    else if ((strcmp(token[iterador], "/") == 0))
    {
        empatar();
        return 1;
    }
    else
        return 0;
}

int factor()
{
    if((strcmp(token[iterador], "(") == 0))
    {
        empatar();
        if(expre())
        {
            if((strcmp(token[iterador], ")") == 0))
                empatar();
                return 1;
        }
    }
    else if((strcmp(token[iterador], "numero") == 0))
    {
        empatar();
        return 1;
    }
    else if((strcmp(token[iterador], "identificador") == 0))
    {
        empatar();
        return 1;
    }
    else
    {
        return 0;
    }
}

void senial_error()
{
    printf("%s \n", "Señal de error");
    exit(0);
}

void empatar()
{
    ++iterador;
}

void cargarArchivo()
{
    FILE* inp;
    inp = fopen("origen.txt","r");		//filename of your data file
    int i = 0;
    while(1){
        char r = (char)fgetc(inp);
        int k = 0;
        while(r!=' ' && !feof(inp)){	//read till , or EOF
            token[i][k++] = r;			//store in array
            r = (char)fgetc(inp);
        }
        token[i][k]=0;		//make last character of string null 
        if(feof(inp)){		//check again for EOF
            break;
        }
        i++;
    }
    int j;

    printf("Los tokens leidos son: \n");	//print array
    for(j = 0;j<=i;j++){
        printf("%s\n",token[j] );	//print array
    }
}