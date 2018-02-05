#include <stdio.h>
#include <stdlib.h>

FILE *inicioLexema, *avance;
int numeroLinea = 1;
char ultimoLexema[100];
char keywords[7][7] = {"program",
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
    fgets(buffer, finPalabra - inicioPalabra+1, inicioLexema);
    printf ("[%s] ", buffer);
    fseek(inicioLexema, finPalabra, SEEK_SET);
}

void aceptarEspacio()
{
    long inicioPalabra = ftell(inicioLexema);
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
    return -1;
}

void operacionError()
{
    printf("%s %d\n", mensajedeError, numeroLinea);
    exit(0);
}

int oprel()
{
    int estadoActual = 0;
    char c = obtenerSiguienteCaracter();

    while (c != ' ' && c != '\n' && c != '\t' && c != EOF && estadoActual != -1)
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
        
        printf("arriba %c %d\n", c, c);
        // printf("%d\n", estadoActual);
        // using the function below is a little noisy with the equalizer function, since it avoid
        // the detection of the space that follows the relation operator. In action is not really much
        // important because after a word is defined there always follows a space. 
        // some cases where this is important is where there is an error after the expression like in 
        // ===. more testing is required. 
        c = obtenerSiguienteCaracter();
        printf("abajo %c %d\n", c, c);
        printf("%d\n",(c != ' ' && c != '\n' ));
        // despues de mas experimentacion me di cuenta de que este metodo es de gran ayuda. trabaja en 
        // conjunto con las condiciones que se encuentran en la parte superior y acotan la participacion
        // de ciertos caracteres dentro del programa. Ya que la unica forma de salir es mediante 
        // caracteres blancos. ahora === no permite dar una salida al bucle
        // this example could lead to some kind of error detection.
    }

    if (estadoActual == 2 || estadoActual == 4 || estadoActual == 5 || estadoActual == 6 || estadoActual == 7 || estadoActual == 8)
    {
        return estadoActual;
    }
    rechazarPalabra;
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
                    printf("Operador relacion \n");
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
        else
        {
            rechazarPalabra();
        }

    }

    fclose(inicioLexema);
    fclose(avance);
    //system("PAUSE");
    system("read -p 'Press Enter to continue...' var");
    //getc(stdin);
    return 0;
}