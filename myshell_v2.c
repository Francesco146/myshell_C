
//myshell di marastoni francesco iniziata 05.12.2019
//contributo di avesani dennis, bertoncini federico, ruffo alessandro, internet
//C program

/*
          ____                                                                               
        ,'  , `.                       .--.--.      ,---,                  ,--,      ,--,    
     ,-+-,.' _ |                      /  /    '.  ,--.' |                ,--.'|    ,--.'|    
  ,-+-. ;   , ||                     |  :  /`. /  |  |  :                |  | :    |  | :    
 ,--.'|'   |  ;|                     ;  |  |--`   :  :  :                :  : '    :  : '    
|   |  ,', |  ':       .--,          |  :  ;_     :  |  |,--.    ,---.   |  ' |    |  ' |    
|   | /  | |  ||     /_ ./|           \  \    `.  |  :  '   |   /     \  '  | |    '  | |    
'   | :  | :  |,  , ' , ' :            `----.   \ |  |   /' :  /    /  | |  | :    |  | :    
;   . |  ; |--'  /___/ \: |            __ \  \  | '  :  | | | .    ' / | '  : |__  '  : |__  
|   : |  | ,      .  \  ' |           /  /`--'  / |  |  ' | : '   ;   /| |  | '.'| |  | '.'| 
|   : '  |/        \  ;   :          '--'.     /  |  :  :_:,' '   |  / | ;  :    ; ;  :    ; 
;   | |`-'          \  \  ;            `--'---'   |  | ,'     |   :    | |  ,   /  |  ,   /  
|   ;/               :  \  \                      `--''        \   \  /   ---`-'    ---`-'   
'---'                 \  ' ;                                    `----'                       
                       `--`                                                                  
*/

#include "myshell_v2.h" //includo la libreria personalizzata

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

int main(void)  //main del programma
{               //dichiarazioni di tutte le variabili come 2 vettori, 1 matrice un char* e una serie di counter
    ctrlz();    //funzione per ignorare il CTRL+Z
    openFile(); //funzione per il File di Log
    do          //ciclo che itera finche il comando inserito non è "exit"
    {
        initShell();                  //scrivo la command line dove l'utente scriverà i comandi
        input();                      //funzione per prendere l'input
        buildCommand();               //costruisco il vettore del comando e dei suoi parametri
        if (!strcmp(command, "help")) //se il comando digitato è "help"
        {
            help(); //funzione che restituisce un comando help personalizzato
        }
        if (!strncmp(command, "set color", 9)) //se il comando digitato senza guardare i parametri è "set color"
        {
            setC(); //funzione che mi fa cambiare il colore della shell
        }
        if (!strncmp(command, "cd", 2)) //se il comando digitato è "cd" senza guardare i parametri
        {
            cd(); //funzione che mi fa il comando -build.in- della bash originale "cd"
        }
        else if (a != 1 && check != 1) //se non è tra i comandi "help" "set color" "cd"
        {
            runCommand(); //esegue il comando con execvp classsico
        }
        printf("%s", colorS);          //setta il colore con il colore personalizzato (se non è stato cambiato mette il default)
        check = 0;                     //variabile usata per capire se l'input era tra i comandi "help" "set color" "cd"
        fflush(stdout);                //pulisco la standard output
    } while (strcmp(command, "exit")); //ciclo che itera finche il comando inserito non è "exit" ovvero se la strcmp() non ritorna 0
    close(fd);                         //chiudo il file di log, log.md
    printf("%s", reset);               //setto il colore standard
    return 0;                          //il programma è stato eseguito correttamente e ritorna 0
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void openFile(void) //apro il file
{
    if (access("log.md", F_OK) != -1) //se il file esiste
    {
        fd = open("log.md", O_RDWR | O_TRUNC, 07770); //lo apro e lo pulisco
    }
    else //se non esiste
    {
        fd = open("log.md", O_RDWR | O_CREAT | O_TRUNC, 07770); //lo creo e lo apro
    }
    if (fd == -1) //se mi da qualuque errore
    {
        printf("[!] Errore nella Apertura del file!"); //lo scrivo in output
    }
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void input(void) //funzione per l'input
{
    do
    {
        printf("%s", White);
        a = read(0, command, lenght); //uso la read per l'input
    } while (a < 0);                  //lo legge finchè l'utente scrive effettivamente qualcosa
    command[a - 1] = 0;               //regola della read di mettere a 0 l'ultimo valore input
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

char *replaceWord(char *s, char *oldW, char *newW) //funzione per sostituire una parola con un'altra (usata per sostituire il percorso assoluto con il carattere "~")
{
    char *result;
    int iii = 0, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    for (iii = 0; s[iii] != '\0'; iii++) //conto quante volte la parola si ripete
    {
        if (strstr(&s[iii], oldW) == &s[iii])
        {
            cnt++;
            //salto all'indice dopo la vecchia parola.
            iii += oldWlen - 1;
        }
    }
    // faccio una nuova stringa di quelle dimensioni tramite "malloc"
    result = (char *)malloc(iii + cnt * (newWlen - oldWlen) + 1);
    iii = 0;
    while (*s)
    {
        //confronto la sottostringa con il risultato
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[iii], newW);
            iii += newWlen;
            s += oldWlen;
        }
        else
        {
            result[iii++] = *s++;
        }
    }
    result[iii] = '\0'; //metto il tappo alla fine della nuova stringa
    return result;
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

const char *getUserName(void) //funzione per prendere il nome utente tramite funzioni incluse nel file .h
{
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid); //prendo il nome
    if (pw)                            //se è andato tutto a buon fine
    {
        return pw->pw_name; //restituisco il nome
    }
    else //altrimenti
    {
        return ""; //restituisco una stringa vuota
    }
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void initShell(void) //funzione per scrivere la CLI
{
    sprintf(x, "/home/%s", getUserName());       //metto in x la string /home/%s do %s sarà il nome utente
    gethostname(hostbuffer, sizeof(hostbuffer)); //processo per prendere l'hostname tramite funzioni dichiarate nel file .h
    gethostbyname(hostbuffer);
    printf("%s%s@%s%s%s:%s%s%s$ ", colorS, getUserName(), hostbuffer, BoldBlue, White, BoldBlue, replaceWord(getcwd(cwd, sizeof(cwd)), x, w), White); // scrivo la CLI
    fflush(stdout);                                                                                                                                   //pulisco lo standard output
    nSpace = 0, lastSpace = 0, j = 0, a = 0;                                                                                                          //setto le varibili che mi serviranno
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void setC(void) //funzione per cambiare il colore
{
    if (arguments[2] == NULL) //se l'utente ha digitato solamente "set color"
    {
        printf("Color:\n 1 = Red\t 2 = BoldRed\t 3 = Green\n 4 = BoldGreen\t 5 = Yellow\t 6 = BoldYellow\n 7  = Blue\t 8 = BoldBlue\t 9 = Magenta\n 10 = BoldMagenta\t 11 = Cyan\t 12 = BoldCyan\t 13 = White\n"); //scrivo il menù per il colore
        if (scanf("%lld", &color))                                                                                                                                                                                 //chiedo in input un numero che verrà messo in "color", se va ha buon fine passa avanti
        {
        }
        else //altrimenti
        {
            printf("[!] Input Error!"); //scrivo all'utente che qualcosa è andato storto
            color = 0;                  //setto "color" a 0
        }
    }
    else //altrimenti
    {
        color = strtoll(arguments[2], &prt, 10); //metto il numero che ha messo dopo "set color" in "color"
    }
    switch (color) //se "color" è
    {
    case 1:                     //1, allora metto il colore rosso ed esco dallo "switch"
        memcpy(colorS, Red, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 2:                         //2, allora metto il colore rosso grassetto ed esco dallo "switch"
        memcpy(colorS, BoldRed, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 3:                       //3, allora metto il colore Verde ed esco dallo "switch"
        memcpy(colorS, Green, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 4:                           //4, allora metto il colore Verde grassetto ed esco dallo "switch"
        memcpy(colorS, BoldGreen, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 5:                        //5, allora metto il colore Giallo ed esco dallo "switch"
        memcpy(colorS, Yellow, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 6:                            //6, allora metto il colore Giallo grassetto ed esco dallo "switch"
        memcpy(colorS, BoldYellow, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 7:                      //7, allora metto il colore blu ed esco dallo "switch"
        memcpy(colorS, Blue, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 8:                          //8, allora metto il colore blu grassetto ed esco dallo "switch"
        memcpy(colorS, BoldBlue, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 9:                         //9, allora metto il colore Magenta ed esco dallo "switch"
        memcpy(colorS, Magenta, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 10:                            //10, allora metto il colore Magenta grassetto ed esco dallo "switch"
        memcpy(colorS, BoldMagenta, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 11:                     //11, allora metto il colore ciano ed esco dallo "switch"
        memcpy(colorS, Cyan, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 12:                         //12, allora metto il colore ciano grassetto ed esco dallo "switch"
        memcpy(colorS, BoldCyan, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    case 13:                      //13, allora metto il colore bianco ed esco dallo "switch"
        memcpy(colorS, White, 8); //metto il colore selezione in "colorS" e pulisco la variabile "color"
        printf("%s", colorS);
        color = 0;
        break;
    default: //se non soddifa nessuna delle condizioni antecedenti, scrive all'utente che fa errato e esce dallo "switch"
        printf("[!] Error Choice\n");
        color = 0; //pulisco la variabile "color"
        break;
    }
    check = 1; //setto la variabile "check" a 1, questo serve che non esegua il comando con l'execvp
    fflush(stdout);
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void trimLeading(char *str) //funzione serve a togliere gli spazi (" ") all'inizio del comando
{
    int index = 0, iiii = 0; //Trova l'ultimo indice del carattere degli spazi bianchi
    while (str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }
    if (index != 0)
    { //tutti gli spazi finali alla sua sinistra
        iiii = 0;
        while (str[iiii + index] != '\0')
        {
            str[iiii] = str[iiii + index];
            iiii++;
        }
        str[iiii] = '\0'; // tappo
    }
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void buildCommand(void) //funzione per costruire il vettore con il comando e i suoi parametri
{
    trimLeading(command);        //pulisco il comando input dagli spazi iniziali
    for (i = 0; command[i]; i++) // implementazione con matrice
    {
        if (command[i] == ' ')
        {
            m[j][i - lastSpace] = 0;
            j++;
            nSpace++;
            lastSpace = i + 1;
        }
        else
        {
            m[j][i - lastSpace] = command[i];
        }
    }
    for (i = lastSpace; command[i]; i++) // per ultima parola dopo l'ultimo spazio
    {
        m[j][i - lastSpace] = command[i];
    }
    m[j][i - lastSpace] = 0;
    for (i = 0; i < nSpace + 1; i++) // costruzione di arguments da matrice input
    {
        arguments[i] = m[i];
    }
    arguments[i] = NULL;
    for (counter = 0; command[counter]; counter++) //calcolo la lunghezza del comando
    {
    }
    if (write(fd, command, counter) == -1 || write(fd, "\n", 1) == -1) //scrivo il comando e il "\n" nel file log.md, se da errore la scrittura sul file
    {
        printf("[!] Errore Nel File"); //lo segnalo all'utente
    }
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void help(void) //funzione per il comando "help" personalizzato
{
    printf("myshell_v2.c © \nprogrammed by Marastoni Francesco, Avesani Dennis, Bertoncini Federico,\n Ruffo Alessandro, Internet. Custom Command: \t 'set color'\n"); //scrivo l'autore e il contributo
    check = 1;                                                                                                                                                         //setto la variabile "check" a 1, questo serve che non esegua il comando con l'execvp
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void cd(void) //funzione per il comando "cd"
{
    if (arguments[1] == NULL) //se scrive solo "cd" allora
    {
        sprintf(x, "/home/%s", getUserName()); //metto in x la root del perchè
        if (chdir(x) != 0)                     //mi sposto nella root del sistema, se da errore
        {
            printf("[!] Error"); //lo segnalo
        }
    }
    else if (chdir(arguments[1]) != 0) //altrimenti se scrive una directory, mi sposto li, se dovesse andare male allora
    {
        perror("[!] Error"); //lo segnalo
    }
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void ctrlc() //funzione che se premo il ctrl+c va a capo e scrive nuovamente la CLI
{
    printf("\n"); //vado a capo
    initShell();  //scrivo la CLI
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void ctrlz(void) //setto il comportamento di CTRL+C e del CTRL+Z
{
    signal(SIGINT, ctrlc);    //se l'utente fa CTRL+C allora invoco la funzione "ctrlc"
    signal(SIGTSTP, SIG_IGN); //se l'utente fa CTRL+Z allora ignoro il segnale
}

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

void runCommand(void) //funzione per eseguire il comando
{
    if (strcmp(command, "exit")) // se il comando non è exit, esegue comando, con gestioni errori
    {
        id = fork(); //creo un figlio
        if (id)      //se sono padre id=1
        {
            wait(&s); //aspetto la morte del figlio
        }
        else if (id == -1) //se la fork() non va, quindi restituisce -1 dice all'utente che è successo un errore interno
        {
            printf("[ERROR] Internal Error!"); //segnalo all'utente che è successo un errore
            exit(1);                           //uccido il figlio
        }
        else if (!id) //infine se sono il figlio id= 0
        {
            execvp(arguments[0], arguments); //eseguo il comando mettendo i parametri e il comando in un vettore
            perror("[!] Error");             //se non và l'execvp() manda un segnale di errore, giustificando il perchè di questo
            exit(1);                         //uccido il figlio
        }
    }
}
