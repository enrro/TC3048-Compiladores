#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

//globals
int iterador=0;
char token[2][10] = {"write", "1"};

int main()
{
    int i;

    for(i = 0; i<2; i++)
    {
        printf("%s, \t  %d\n",token[i], i);
    }
    programa(token);



    //system("PAUSE");
    system("read -p 'Press Enter to continue...' var");
    //getc(stdin);
    return 0;
}

void empatar()
{
    ++iterador;
}

int programa()
{
    printf("%s\n", token[iterador]);
    secuencia_sent();
    return 1;
}


int secuencia_sent()
{
    sentencia();
    while(token[iterador] == ';')
    {
        empatar();
        sentencia();
    }
    return 1;
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
}
int sent_if()
{
    return 1;
}
int sent_repeat()
{

}
int sent_assign()
{

}
int sent_read()
{

}
int sent_write()
{

}

int expre()
{

}
int op_comparacion()
{

}
int exp_simple()
{

}
int opsum()
{

}
int term()
{

}
int opmult()
{

}
int factor()
{

}

void senial_error()
{
    printf("%s \n", "Señal de error");
    exit(0);
}