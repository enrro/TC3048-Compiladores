#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//constants
#define PALABRAS_RESERVADAS       7
#define LARGO_PALABRAS_RESERVADAS 8
#define OPERADORES_LOGICOS        3
#define LARGO_OPERADORES_LOGICOS  4

//functions
void writeToFile(char *text);
void aceptarPalabra();
void aceptarComentario();
void aceptarEspacio();
void rechazarPalabra();
char obtenerSiguienteCaracter();
int isNormalChar(int c);
int isPuntuacion(int c);
int equalizer();
void operacionError();
int operadoresLogicos();
int palabraReservada();
int caracterDelimitacion();
int operadorAsignacion();
int operadoresAritmeticos();
int comentarios();
int signosPuntuacion();
int identificador();
int numeros();
int oprel();
int isfeof();


FILE *inicioLexema, *avance, *finalOfFile, *fp;
int numeroLinea = 1;
char ultimoLexema[100];
char arrOperadoresLogicos[OPERADORES_LOGICOS][LARGO_OPERADORES_LOGICOS] = {"and", "or", "not"};
char palabrasReservadas[PALABRAS_RESERVADAS][LARGO_PALABRAS_RESERVADAS] = {"program",
                                                                            "var","begin",
                                                                            "end","if",
                                                                            "then","else"};
char mensajedeError[] = "Error en la linea ";
int error_token = 0;

int main()
{
    inicioLexema = fopen("origen.txt", "r");
    avance = fopen("origen.txt", "r");
    finalOfFile = fopen("origen.txt", "r");
    fp=fopen("test.txt", "w");
    
    fseek(finalOfFile, 0, SEEK_END);

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
        else if(comentarios()!=-1)
        {
            aceptarComentario();
        }
        else if((c = numeros()) != -1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 2:
                    puts("Numero natural");
                    break;
                case 3:
                    puts("Numero octal");
                    break;
                case 4:
                    puts("Numero hexadecimal");
                    break;
                case 6:
                    puts("Numero de punto flotante");
                    break;
                case 8:
                    puts("Numero de punto flotante");
                    break;
            }
        }
        else if(palabraReservada()!=-1)
        {
            aceptarPalabra();
            puts("Palabra reservada");
        }
        else if(operadoresLogicos()!=-1)
        {
            aceptarPalabra();
            puts("Operador logico");
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
        else if((c=operadoresAritmeticos())!=-1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 1:
                    puts("suma");
                    break;
                case 2:
                    puts("resta");
                    break;
                case 3:
                    puts("multiplicacion");
                    break;
                case 4:
                    puts("division");
                    break;
            }
        }
        else if(operadorAsignacion()!=-1)
        {
            aceptarPalabra();
            puts("Asignacion");
        }
        else if((c=signosPuntuacion()) !=-1)
        {
            aceptarPalabra();
            switch(c)
            {
                case 1:
                    puts("Punto");
                    break;
                case 2:
                    puts("Coma");
                    break;
                case 3:
                    puts("Punto y coma");
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
    fclose(fp);
    
    //system("PAUSE");
    system("read -p 'Press Enter to continue...' var");
    //getc(stdin);
    return 0;
}

void writeToFile(char *text)
{
    char* str = "string";
    int x = 10;

    if(fp == NULL)
        exit(-1);
    fprintf(fp, "%s \n", text);
}


void aceptarPalabra()
{
    char buffer [100];

    
    long inicioPalabra = ftell(inicioLexema);
    long finPalabra = ftell(avance);
    if(ftell(avance)== ftell(finalOfFile))
    {
        ++finPalabra;
    }
    // fgets prints whatever is between the starting point and the end point
    fgets(buffer, finPalabra - inicioPalabra, inicioLexema);
    printf ("[%s]\t\t", buffer);
    writeToFile(buffer);
    fseek(inicioLexema, finPalabra, SEEK_SET);
}
void aceptarComentario()
{
    
    long inicioPalabra = ftell(inicioLexema);
    long finPalabra = ftell(avance);
    // fgets prints whatever is between the starting point and the end point
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

int isPuntuacion(int c)
{
    return (c != '.' && c != ',' && c != ';', c != '(', c != ')');
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

int operadoresLogicos()
{
    char c = obtenerSiguienteCaracter();
    char construirPalabra [LARGO_OPERADORES_LOGICOS];
    int contador, palabra, estadoActual;
    palabra = contador = estadoActual = 0;
    
    while(isNormalChar(c) && estadoActual != -1 && contador<LARGO_OPERADORES_LOGICOS)
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
    for(palabra = 0; palabra < OPERADORES_LOGICOS; ++palabra)
    {
        if(strcmp(arrOperadoresLogicos[palabra], construirPalabra)== 0)
        {
            return 1;
        }
    
    }


    rechazarPalabra();
    return -1;
}

int palabraReservada()
{
    char c = obtenerSiguienteCaracter();
    char construirPalabra [LARGO_PALABRAS_RESERVADAS];
    int contador, palabra, estadoActual, noAlfa;
    palabra = contador = estadoActual = noAlfa =  0;
    
    while(isNormalChar(c) && c != '(' && isPuntuacion(c) && estadoActual != -1 && contador<LARGO_PALABRAS_RESERVADAS)
    {
        if(isalpha(c))
        {
            construirPalabra[contador++] = c;
        }
        else if(!isalpha(c))
        {
            ++noAlfa;
        }
        else
        {
            estadoActual = -1;
        }
        c = obtenerSiguienteCaracter();
    }
    construirPalabra[contador]= '\0';
    if(noAlfa==0)
    {
        for(palabra = 0; palabra < PALABRAS_RESERVADAS; ++palabra)
        {
            if(strcmp(palabrasReservadas[palabra], construirPalabra)== 0)
            {
                return 1;
            }
        
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
    //ok so this seems to be the key to avoid using the hardcoded 1 at the top of acceptword
    //c = obtenerSiguienteCaracter();
    }
    if(estadoActual == 1 || estadoActual == 2 || estadoActual == 3 || estadoActual == 4)
    {
        return estadoActual;
    }
    rechazarPalabra();
    return -1;
}

int operadorAsignacion()
{
    char c = obtenerSiguienteCaracter();
    int estadoActual = 0;
    while(isNormalChar(c) && estadoActual != -1)
    {
        switch(estadoActual)
        {
        case 0:
            switch(c)
            {
            case '=':
                estadoActual = 1;
                break;
            default:
                estadoActual = -1;
                break;
            }
            break;
        default:
            estadoActual = -1;
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

int operadoresAritmeticos()
{
    char c = obtenerSiguienteCaracter();
    int estadoActual = 0;
    while(isNormalChar(c) && estadoActual != -1)
    {
        switch(estadoActual)
        {
        case 0:
            switch(c)
            {
            case '+':
                estadoActual = 1;
                break;
            case '-':
                estadoActual = 2;
                break;
            case '*':
                estadoActual = 3;
                break;
            case '/':
                estadoActual = 4;
                break;
            default:
                estadoActual = -1;
                break;
            }
            break;
        default:
            estadoActual = -1;
        }
        c = obtenerSiguienteCaracter();
        
    }
    
    if(estadoActual == 1 || estadoActual == 2 || estadoActual == 3 || estadoActual == 4)
    {
        return estadoActual;
    }
    rechazarPalabra();
    return -1;
}

int comentarios()
{
    char c = obtenerSiguienteCaracter();
    int estadoActual = 0;
    while(isNormalChar(c) && estadoActual != -1)
    {
        if(estadoActual == 0 && c == '/')
        {
            estadoActual = 1;
        }
        else if (estadoActual == 1 && c == '/')
        {
            estadoActual = 2;
            
        }
        else
        {
            estadoActual = -1;
        }
        c = obtenerSiguienteCaracter();
        
    }
    
    if(estadoActual == 2)
    {
        while( c != '\n' && c != EOF)
        {
            c = obtenerSiguienteCaracter();
        }

    }

    if(estadoActual == 2)
    {
        ++numeroLinea;
        
        return estadoActual;
    }
    rechazarPalabra();
    return -1;
}

int signosPuntuacion()
{
    char c = obtenerSiguienteCaracter();
    int estadoActual = 0;
    while(isNormalChar(c) && estadoActual != -1)
    {
        switch(estadoActual)
        {
        case 0:
            switch(c)
            {
            case '.':
                estadoActual = 1;
                break;
            case ',':
                estadoActual = 2;
                break;
            case ';':
                estadoActual = 3;
                break;
            default:
                estadoActual = -1;
                break;
            }
            break;
        default:
            estadoActual = -1;
        }
        c = obtenerSiguienteCaracter();
        
    }
    
    if(estadoActual == 1 || estadoActual == 2 || estadoActual == 3)
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


    while(isNormalChar(c) && c != ','  && c != ')' && estadoActual != -1)
    {
        switch(estadoActual)
        {
            case 0:
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
            break;
            case 1:
                if(isalpha(c))
                {
                    estadoActual = 1;
                }
                else if(c == '_' || isdigit(c))
                {
                    estadoActual = 1;
                }
                else
                {
                    estadoActual = -1;
                }
                break;
            case 2:
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
                break;
            default:
                estadoActual = -1;

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
int numeros()
{
    int estadoActual = 0;
    char c = obtenerSiguienteCaracter();
    

    while (isNormalChar(c) && estadoActual != -1 && c != ';')
    {
        switch (estadoActual){
        case 0:
            if(c == '0')
            {
                estadoActual = 1;
            }
            else if(isdigit(c))
            {
                estadoActual = 2;
            }
            else
            {
                estadoActual= -1;
            }
            break;
        case 1:
            if(c >= '1' && c <= '7')
            {
                estadoActual = 3;
            }
            else if(c == 'x' || c == 'X')
            {
                estadoActual = 4;
            }
            else if(c == '.')
            {
                estadoActual = 5;
            }
            else
            {
                estadoActual= -1;
            }
            break;
        case 2:
            if(isdigit(c))
            {
                estadoActual = 2;
            }
            else if(c == '.')
            {
                estadoActual = 5;
            }
            else
            {
                estadoActual = -1;
            }
            break;
        case 3:
            estadoActual = (c >= '0' && c <= '7') ? 3 : -1;
            break;
        case 4:
            estadoActual = (c >= '0' && c <= '9' || c >= 'a' && c <= 'f'|| c >= 'A' && c <= 'F') ? 4 : -1;
            break;
        case 5:
            estadoActual = (isdigit(c)) ? 6:-1;
            break;
        case 6:
            if(isdigit(c))
            {
                estadoActual = 6;
            }
            else if(c == 'e' || c == 'E')
            {
                estadoActual = 7;
            }
            else
            {
                estadoActual = -1;
            }
            break;
        case 7:
            if(isdigit(c))
            {
                estadoActual = 8;
            }
            else if(c == '+' || c == '-')
            {
                estadoActual = 9;
            }
            break;
        case 8:
            estadoActual = (isdigit(c)) ? 8 : -1;
            break;
        case 9:
            estadoActual = (isdigit(c)) ? 8 : -1;
            break;
        default:
            estadoActual = -1;
            break;
        }
        c = obtenerSiguienteCaracter();
    }

    if (estadoActual == 2 || estadoActual == 3 || estadoActual == 4 || estadoActual == 6 || estadoActual == 8)
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
