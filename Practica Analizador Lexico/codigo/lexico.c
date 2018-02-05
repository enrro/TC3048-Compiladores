#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define PALABRAS_RESERVADAS       7
#define LARGO_PALABRAS_RESERVADAS 8

FILE *inicioLexema, *avance;
int numeroLinea = 1;
char ultimoLexema[100];
char palabrasReservadas[PALABRAS_RESERVADAS][LARGO_PALABRAS_RESERVADAS] = {"program",
                                                                "var","begin",
                                                                "end","if",
                                                                "then","else"};
char mensajedeError[] = "Error en la linea ";
int error_token = 0;

void aceptarPalabra()
{
    char buffer [100];
    
    long inicioPalabra = ftell(inicioLexema);
    long finPalabra = ftell(avance);
    // fgets prints whatever is between the starting point and the end point
    fgets(buffer, finPalabra - inicioPalabra + 1, inicioLexema);
    printf ("[%s]\t", buffer);
    fseek(inicioLexema, finPalabra, SEEK_SET);
}

void aceptarEspacio()
{
    long finPalabra = ftell(avance);
    fseek(inicioLexema, finPalabra, SEEK_SET);
}

void rechazarPalabra()
{
    long inicioPalabra = ftell(inicioLexema);
    fseek(avance, inicioPalabra, SEEK_SET);
}

char obtenerSiguienteCaracter()
{
    return fgetc(avance);
}

int isNormalChar(int c)
{
    return (c != ' ' && c != '\n' && c != '\t' && c != EOF);
}

int equalizer()
{
    char c = obtenerSiguienteCaracter();
    
    if (c == ' ' || c =='\n' || c == '\t')
    {
        if(c == '\n')
        {
            ++numeroLinea;
        }
        return 1;
    }
    rechazarPalabra();
    return -1;
}

void operacionError()
{
    printf("%s %d\n", mensajedeError, numeroLinea);
    exit(0);
}

int palabraReservada()
{
    char c = obtenerSiguienteCaracter();
    char construirPalabra [LARGO_PALABRAS_RESERVADAS];
    int contador, palabra, estadoActual;
    palabra = contador = estadoActual = 0;
    
    while(isNormalChar(c) && estadoActual != -1 && contador<LARGO_PALABRAS_RESERVADAS)
    {
        if(isalpha(c))
        {
            construirPalabra[contador++] = c;
        }
        else
        {
            estadoActual = -1;
        }
        c = obtenerSiguienteCaracter();
    }
    construirPalabra[contador]= '\0';

    for(palabra = 0; palabra < PALABRAS_RESERVADAS; ++palabra)
    {
        if(strcmp(palabrasReservadas[palabra], construirPalabra)== 0)
        {
            return 1;
        }
    
    }


    rechazarPalabra();
    return -1;
}

int caracterDelimitacion()
{
    char c = obtenerSiguienteCaracter();
    int estadoActual = 0;

    switch(c)
    {
        case '(':
            estadoActual = 1;
            break;
        case ')':
            estadoActual = 2;
            break;
        case '[':
            estadoActual = 3;
            break;
        case ']':
            estadoActual = 4;
            break;
        default:
            estadoActual = -1;
            break;
    c = obtenerSiguienteCaracter();
    }
    if(estadoActual == 1 || estadoActual == 2 || estadoActual == 3 || estadoActual == 4)
    {
        return estadoActual;
    }
    rechazarPalabra();
    return -1;
}

int identificador()
{
    int estadoActual = 0;
    char c = obtenerSiguienteCaracter();
    while(isNormalChar(c) && estadoActual != -1)
    {
        if(isalpha(c))
        {
            estadoActual = 1;
        }
        else if(c == '_')
        {
            estadoActual = 2;
        }
        else
        {
            estadoActual = -1;
        }
        if(estadoActual == 1)
        {
            estadoActual = (isalpha(c) || isdigit(c) || c == '_') ? 1 : -1;
        }
        else if(estadoActual == 2)
        {
            if(isalpha(c))
            {
                estadoActual = 1;
            }
            else if(c == '_' || isdigit(c))
            {
                estadoActual = 2;
            }
            else
            {
                estadoActual = -1;
            }
        }
        c = obtenerSiguienteCaracter();
    }
    
    if(estadoActual == 1)
    {
        return estadoActual;
    }
    rechazarPalabra();
    return -1;
}

int oprel()
{
    int estadoActual = 0;
    int c = obtenerSiguienteCaracter();
     

    while (isNormalChar(c) && estadoActual != -1)
    {
        switch (estadoActual){
        case 0:
            switch (c){
            case '=':
                estadoActual = 1;
                break;
            case '!':
                estadoActual = 3;
                break;
            case '>':
                estadoActual = 5;
                break;
            case '<':
                estadoActual = 7;
                break;
            default:
                estadoActual = -1;
                break;
            }
            break;
        case 1:
            estadoActual = (c == '=') ? 2 : -1;
            break;
        case 3:
            estadoActual = (c == '=') ? 4 : -1;
            break;
        case 5:
            estadoActual = (c == '=') ? 6 : -1;
            break;
        case 7:
            estadoActual = (c == '=') ? 8 : -1;
            break;
        default:
            estadoActual = -1;
            break;
        }
        c = obtenerSiguienteCaracter();

    }

    if (estadoActual == 2 || estadoActual == 4 || estadoActual == 5 || estadoActual == 6 || estadoActual == 7 || estadoActual == 8)
    {
        return estadoActual;
    }
    // if the word is rejected then we need to move the pointer to the beginning if the word.
    rechazarPalabra();
    
    return -1;
}

int isfeof()
{
    char c = obtenerSiguienteCaracter();
    rechazarPalabra();
    return c == EOF;
}

int main()
{
    inicioLexema = fopen("origen.txt", "r");
    avance = fopen("origen.txt", "r");
    int c; 

    while (!isfeof())
    {
        if ((c = oprel()) != -1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 2:
                    printf("Operador igualdad \n");
                    break;
                case 4:
                    printf("Operador desigualdad \n");
                    break;
                case 5:
                    printf("Operador mayor que \n");
                    break;
                case 6:
                    printf("Operador mayor ó igual que \n");
                    break;
                case 7:
                    printf("Operador menor que \n");
                    break;
                case 8:
                    printf("Operador menor ó igual que \n");
                    break;
            }
        }
        else if(equalizer() != -1)
        {
            aceptarEspacio();
        }
        else if(palabraReservada()!=-1)
        {
            aceptarPalabra();
            puts("Palabra reservada");
        }
        else if(identificador() != -1)
        {
            aceptarPalabra();
            puts("Identificador");
        }
        else if((c=caracterDelimitacion())!=-1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 1:
                    puts("Parentesis izquierdo");
                    break;
                case 2:
                    puts("Parentesis derecho");
                    break;
                case 3:
                    puts("Corchete izquierdo");
                    break;
                case 4:
                    puts("Corchete derecho");
                    break;
            }
        }
        else
        {
            operacionError();
        }

    }

    fclose(inicioLexema);
    fclose(avance);
    //system("PAUSE");
    system("read -p 'Press Enter to continue...' var");
    //getc(stdin);
    return 0;
}