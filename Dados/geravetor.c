#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main ( int argc, char *argv[] )
{
	srand (time(NULL));
	int tamanho_maximo = atoi (argv[1]);
	int tamanho = 10;;
	int i;
	int iteracoes = 0; //Variavel para determinar se o tamanho sera multiplicado por 2 ou 5
    
	FILE *pfile;

	while ( tamanho <= tamanho_maximo )  
	{  	
  		char nome[100];
    	char tam[100];
		
		/* Conversao de int em string */
		sprintf ( tam, "%d", tamanho);

 	  	strcpy (nome, "dados_");
	    strcat (nome, tam);
   		strcat (nome, ".txt");
    	
		/* Criação de um arquivo txt com o vetor gerado */
   		pfile = fopen (nome, "w");
    
    	fprintf (pfile, "%d\n", tamanho);
	
		for (i = tamanho; i > 1; i--)
		{
			fprintf (pfile, "%d\n", (rand()%10) + 2); //Os vetores sempre são preenchidos com números de 2 a 10
		}

		fprintf (pfile, "%d\n", 1); //O elemento 1 aparecera um unica vesta, constituindo o pior caso para funcao de ordem log n
    
   		fclose (pfile);

		printf ("Arquivo de tamanho %s gerado!\n", tam);

		if ( tamanho < 100000000 )
		{
			if ( iteracoes % 2 == 0 ) //iteracoes par
				tamanho *= 5;
			
			else
				tamanho *= 2;		

		}

		else /* ( tamanho > 100000000 ) Regiao que devera ser mais preenchida no grafico, sendo necessario um numero maior de pontos */
			tamanho += 50000000;

		iteracoes++;
	}

	printf ("FIM\n");

	return 0;
}

