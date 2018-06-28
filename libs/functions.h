/*
Autore: Lorenzo Damiano
Github: https://github.com/SoStealth
*/
//----------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

/*
get_table
Legge una tabella di numeri interi da file, alloca una matrice, copia la tabella 
nella matrice e ne restituisce il puntatore
Riceve:
- *filename: percorso del file da cui leggere la tabella
- rows: numero di righe della tabella da leggere
- columns: numero di colonne della tabella da leggere
Restituisce:
- puntatore ad una matrice di interi contenente i dati della tabella
Questa funzione usa la malloc(), pertanto è necessario eseguire la free() della
matrice ricevuta dopo la chiamata
*/
int** get_table(char* filename, int rows, int columns);

/*
expression
Converte un espressione di lancio (esempio: 1d6+2) in valori interi utilizzando
la split()
Riceve:
- *s: espressione da convertire
- *times: puntatore ad intero in cui verrà memorizzato il numero di lanci di dado
- *value: puntatore ad intero in cui verrà memorizzato il valore del dado
- *bonus: puntatore ad intero in cui verrà memorizzato il bonus al lancio
*/
void expression(char* s, int* times, int* value, int* bonus);

/*
start_log
Inizializza un file creandolo se non esiste o resettandolo se esiste già
Riceve:
- *filepath: percorso dove salvare il file
*/
void start_log(char* filepath);

/*
write_log
Scrive una stringa al fondo di un file
Riceve:
- *filepath: percorso del file su cui scrivere
- *s: stringa da scrivere sul file
*/
void write_log(char* filepath,char* s);

/*
split
Divide una stringa in più sottostringhe in base ad un carattere divisore
Riceve:
- *str: stringa da dividere
- c: carattere divisore
- ***arr: puntatore a matrice di record in cui salvare le varie sottostringhe
Restituisce:
- numero di sottostringhe trovate
*/
int split (char *str, char c, char ***arr);

//--------------------------------------------------------------------------------
int** get_table(char* filename, int rows, int columns) {
	char* line;			//buffer dove inserire man mano le righe lette dal file
	size_t len = 0;		//richiesta dalla funzione readline()
	char** temp = NULL;	//contiene i dati del file separati tramite split()
	FILE* file = fopen(filename,"r");	//apre il file in lettura
	if (file == NULL) {		//se il file non è stato trovato, esce
        printf("Error: file pointer is null.\n");
        exit(1);
    }
	int** table = (int**)malloc(sizeof(int)*rows*columns);	//alloca spazio per la matrice
	for(int k=0;k<rows;k++) table[k]=(int*)malloc(sizeof(int)*columns);	//alloca spazio per ogni riga
	for(int i=0;i<rows;i++) {	//per ogni riga della matrice
		getline(&line, &len, file);	//legge una riga dal file
		split(line,',',&temp);		//divide la riga per ','
		for(int j=0;j<columns;j++) {	//per ogni valore trovato nella riga
			table[i][j] = atoi(temp[j]);	//salva il valore nella matrice
		}
		free(temp);	//dealloca lo spazio allocato nella split()
	}
	fclose(file);	//chiude il file
	return table;	//restituisce il puntatore alla matrice creata
}
//---------------------------------------------------------------------------------
void expression(char* s, int* times, int* value, int* bonus) {
	char** temp;	//contiene i dati separati dalla split()
	int count = split(s,'d',&temp);	//divide l'espressione per 'd'
	if(count == 1) {	//se la d non è inclusa nell'espressione
		*times = 0;	//non ci sono lanci di dado
		*value = 0;	//il valore del dado è 0
		*bonus = atoi(temp[0]);	//il valore bonus equivale al valore dell'espressione
		return;
	}
	if(strlen(temp[0])==0) {	//se non è presente un numero di fronte alla 'd'
		*times = 1;	//c'è un lancio di dado
	} else {	//altrimenti
		*times = atoi(temp[0]);	//i lanci di dado equivalgono al numero scritto
	}
	char** temp2;	//contiene i dati separati dalla split()
	char* s2 = strdup(temp[1]);	//prende la seconda parte dell'espressione
	count = split(s2,'+',&temp2);	//suddivide per '+'
	if(count==2) {	//se è presente un bonus
		*value = atoi(temp2[0]);	//salva il valore del dado
		*bonus = atoi(temp2[1]);	//salva il bonus
		return;
	}
	count = split(s2,'-',&temp2);	//suddivide per '-'
	if(count==2) {	//se è presente un malus
		*value = atoi(temp2[0]);	//salva il valore del dado
		*bonus = atoi(temp2[1]);	//salva il valore del malus
		*bonus = *bonus * -1;	//inverte il valore del malus così che sia negativo
		return;
	}	//altrimenti
	*value = atoi(temp2[0]);	//salva il valore del dado
	*bonus = 0;		//imposta il bonus a 0 in quanto non c'è nessun bonus
	free(s2);		//dealloca lo spazio utilizzato per le split()
	free(temp);
	free(temp2);
	return;
}
//-------------------------------------------------------------------------------------------
void start_log(char* filepath) {
	FILE* file = fopen(filepath,"w");	//crea o resetta il file
	fclose(file);	//chiude il file
}
//-------------------------------------------------------------------------------------------
void write_log(char* filepath,char* s) {
	FILE* file = fopen(filepath,"a");	//apre il file
	fputs(s,file);	//aggiunge la stringa in coda
	fclose(file);	//chiude il file
}
//-------------------------------------------------------------------------------------------
/*
Split taken from:
http://source-code-share.blogspot.it/2014/07/implementation-of-java-stringsplit.html
*/
int split (char *str, char c, char ***arr)
{
  int count = 1;
  int token_len = 1;
  int i = 0;
  char *p;
  char *t;

  p = str;
  while (*p != '\0')
    {
      if (*p == c)
        count++;
      p++;
    }

  *arr = (char**) malloc(sizeof(char*) * count);
  if (*arr == NULL)
    exit(1);

  p = str;
  while (*p != '\0')
    {
      if (*p == c)
      {
        (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
        if ((*arr)[i] == NULL)
          exit(1);

        token_len = 0;
        i++;
      }
      p++;
      token_len++;
    }
  (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
  if ((*arr)[i] == NULL)
    exit(1);

  i = 0;
  p = str;
  t = ((*arr)[i]);
  while (*p != '\0')
    {
      if (*p != c && *p != '\0')
      {
        *t = *p;
        t++;
      }
      else
      {
        *t = '\0';
        i++;
        t = ((*arr)[i]);
      }
      p++;
    }
  *t = '\0';    //Aggiunta: nella versione originale, l'ultima stringa non aveva il carattere di chiusura

  return count;
}