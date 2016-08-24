#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

/* Função auxiliar para ser utilizada no qsort
 * Retorna -1 caso o primeiro elemento seja menor que o segundo
 * Retorna 0 caso sejam iguais
 * Retorna 1 caso o segundo elemento seja maior que o primeiro
 */
int compare ( const void * a, const void * b )
{
	return ( *(int *) a - *(int *) b );	
}

/* Função de complexidade O(N) para contar o número de repetições de certo elemento no vetor fornecido */
int conta_n ( int *vetor, int tamanho, int elemento )
{
	int i, repeticoes = 0;
	for (i = 0; i < tamanho; i++)
		if ( vetor[i] == elemento )
			repeticoes++;
	
	return repeticoes;
}

/* Função de complexidade O(Log(N)) para contar o número de repetições de certo elemento no vetor fornecido */
int conta_logn ( int *vetor, int esquerda, int direita, int elemento )
{
	int meio, aux;
	int primeiraMetade = 0, segundaMetade = 0;

	if ( vetor[esquerda] == elemento && vetor[direita] == elemento )
		return ( direita - esquerda ) + 1;
		
	if ( esquerda == direita )
		return 0;
			
	else
	{
		meio = ( esquerda + direita ) / 2;

		if ( vetor[meio] >= elemento )
			primeiraMetade = conta_logn ( vetor, esquerda, meio, elemento );
			
		if ( vetor[meio + 1] <= elemento )
			segundaMetade = conta_logn ( vetor, meio + 1, direita, elemento );
		
		return primeiraMetade + segundaMetade;
	
	}	
}

/* Função para calcular o tempo de execução de determinadas rotinas */
double getUnixTime(void)
{
    struct timespec tv;
    
    if(clock_gettime(CLOCK_REALTIME, &tv) != 0) 
	{
		return 0;
	}
    
    return (((double) tv.tv_sec) + (double) (tv.tv_nsec / 1000000000.0));
}

int main ( int argc, char *argv[] )
{
	/* Variável para guardar o tamanho do vetor */
	unsigned int tamanho;

	/* Ponteiro para o vetor de inteiros */
	int *vetor;
	
	/* Variável para armazenar o elemento a ser buscado */
	const int elemento = atoi(argv[1]);

	/* Ponteiro para o arquivo que guardara os dados gerados */
	FILE *pfile;
	pfile = fopen ("dados_gerados_com_modelos.txt", "w");

	/* Ponteiro para os arquivos com os modelos de vetores */
	FILE *pimput;

	/* String auxiliar */
	char tam[20];

	double tN, tLOGN;
	int repeticoesN, repeticoesLOGN;
	double start_time, stop_time;
	int j, lixo;
	int iteracoes = 0; //Variavel para determinar se o tamanho sera multiplicado por 2 ou 5

	tamanho = 10;

	while ( tamanho <= 500000000 )
	{			
		char nome[100] = "./Dados/dados_";
					
		sprintf ( tam, "%d", tamanho );	
		strcat ( nome, tam );
		strcat ( nome, ".txt" );
		printf ("%s a processar\n", nome);
		pimput = fopen ( nome, "r" );		

		fprintf (pfile, "%d ", tamanho);
			
		vetor = (int *) malloc ( tamanho * sizeof (int) );

		/* Cópia dos elementos do vetor fornecido pelo arquivo para o vetor alocado */
		fscanf (pimput, "%d", &lixo );			
		
		int i;
		for (i = 0; i < tamanho; i++)
		{	
			fscanf (pimput, "%u", &vetor[i]);	
		}
		
		/* Ordenação do vetor */
		qsort (vetor, tamanho, sizeof(int), compare);
	
		/* Execução da função de complexidade N e registro do seu tempo */
		start_time = getUnixTime();
		repeticoesN = conta_n (vetor, tamanho, elemento);
		stop_time = getUnixTime();
		tN = stop_time - start_time;
		
		/* Execução da função de complexidade LOG N e registro do seu tempo */
		start_time = getUnixTime();
		repeticoesLOGN = conta_logn (vetor, 0, tamanho -1, elemento);
		stop_time = getUnixTime();
		tLOGN = stop_time - start_time;
	
		if ( repeticoesN != repeticoesLOGN )
		{
			printf ("RELSULTADOS DISTINTOS PARA AS DUAS FUNCOES\n");
			tamanho = 2000000000;
			j = 2;
			//continue;
		}
		
		/* Saída dos dados */	
		fprintf (pfile, "%.10lf ", tN );
		fprintf (pfile, "%.10lf\n", tLOGN );
	
		free (vetor);
		fclose (pimput);

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
	
	printf ("Arquivos processados!!\n");	
	fclose (pfile);

	return 0;
}
