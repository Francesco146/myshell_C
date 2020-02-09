#include <sys/types.h>                              //per diversi comandi
#include <unistd.h>                                 //per diversi comandi
#include <stdlib.h>                                 //per diversi comandi
#include <string.h>                                 //per strcmp()
#include <stdio.h>                                  //per perror()
#include <sys/wait.h>                               //per wait()
#include <pwd.h>                                    //per nome user
#include <netdb.h>                                  //per diversi comandi
#include <limits.h>                                 //per diversi comandi
#include <sys/socket.h>                             //per diversi comandi
#include <signal.h>                                 //per diversi comandi
#include <sys/stat.h>                               //per diversi comandi
#include <fcntl.h>                                  //per diversi comandi
#include <ctype.h>                                  //per diversi comandi
#include <malloc.h>                                 //per diversi comandi
#define Red "\e[0;31m"                              //definisco tutti i colori
#define BoldRed "\e[1;31m"                          //definisco tutti i colori
#define Green "\e[0;32m"                            //definisco tutti i colori
#define BoldGreen "\e[1;32m"                        //definisco tutti i colori
#define Yellow "\e[0;33m"                           //definisco tutti i colori
#define BoldYellow "\e[01;33m"                      //definisco tutti i colori
#define Blue "\e[0;34m"                             //definisco tutti i colori
#define BoldBlue "\e[1;34m"                         //definisco tutti i colori
#define Magenta "\e[0;35m"                          //definisco tutti i colori
#define BoldMagenta "\e[1;35m"                      //definisco tutti i colori
#define Cyan "\e[0;36m"                             //definisco tutti i colori
#define BoldCyan "\e[1;36m"                         //definisco tutti i colori
#define White "\e[0;37m"                            //definisco tutti i colori
#define reset "\e[0m"                               //definisco il codice per far tornare il colore normale
#define lenght 1024                                 //definisco la lunghezza standard generale

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

char *replaceWord(char *s, char *oldW, char *newW); //defisco tutte le funzioni che userò nel programma
const char *getUserName(void);                      //defisco tutte le funzioni che userò nel programma
void initShell(void);                               //defisco tutte le funzioni che userò nel programma
void setC(void);                                    //defisco tutte le funzioni che userò nel programma
void buildCommand(void);                            //defisco tutte le funzioni che userò nel programma
void help(void);                                    //defisco tutte le funzioni che userò nel programma
void cd(void);                                      //defisco tutte le funzioni che userò nel programma
void ctrlz(void);                                   //defisco tutte le funzioni che userò nel programma
void input(void);                                   //defisco tutte le funzioni che userò nel programma
void runCommand(void);                              //defisco tutte le funzioni che userò nel programma
void openFile(void);                                //defisco tutte le funzioni che userò nel programma
void trimLeading(char *str);                        //defisco tutte le funzioni che userò nel programma
void ctrlc();                                       //defisco tutte le funzioni che userò nel programma

/* ##################################################################### */
/* ##################################################################### */
/* ##################################################################### */

char hostbuffer[lenght], command[lenght], m[lenght][lenght], cwd[lenght], colorS[8] = "\e[1;32m", x[lenght], w[1] = "~", *prt, *arguments[lenght];
long long a = 0, i = 0, nSpace = 0, lastSpace = 0, j = 0, id = 0, color = 0, check = 0, k = 0;
int s = 0, fd = 0, counter = 0;
//hostbuffer per il nome della macchina usata
//command per il comando input da elaborare
//m per la elaborazione del comando
//cwd per la il percorso della myshell
//colorS per il colore selezionata / default
//x per i vari sprintf
//w per il carattere che sostituisce il percorso root con il carattere "~"
//prt per la funzione strtoll in setC
//arguments è il vettore dove andrà messo il comando aleaborato
//a per il numero dei caratteri insteriti in input dall'utente, usata se l'utente preme invio senza mettere nessun comando
//id per la fork()
//color per la funzione setC
//check per vedere se il comando non è un comando "help", "set color" o "cd"
//i per i counter generali
//j e k e lastSpace e nSpace per la funzione buildCommand(), usati per la matrice
//s è per la wait()
//counter è per la funzione buildCommand()
//fd è il file descriptor del log.md