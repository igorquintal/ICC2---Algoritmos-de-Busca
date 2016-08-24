#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


/* Funcao para preencher vetores com numeros aleatorios de 2 a 11 */
void geraVetor (int *vetor, long int tamanho)
{
	srand (time(NULL));
	int i;
		
	for (i = 0; i < (tamanho - 1); i++)
		vetor[i] = (rand()%10) + 2;
	
	vetor[i] = 1; //O vetor tera a configuracao do pior caso, com um unico elemento na primeira posicao

}

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
	int tamanho;

	/* Ponteiro para o vetor de inteiros */
	int *vetor;
	
	/* Variável para armazenar o elemento a ser buscado */
	const int elemento = atoi(argv[1]);

	/* Ponteiro para o arquivo que guardara os dados gerados */
	FILE *pfile;
	pfile = fopen ("dados_gerados.txt", "w");
	
	double tN, tLOGN;
	int repeticoesN, repeticoesLOGN;
	double start_time, stop_time;
	
	for (tamanho = 100; tamanho < 800000000; tamanho *= 1.1)
	{
		fprintf (pfile, "%d ", tamanho);

		vetor = (int *) malloc ( tamanho * sizeof (int) );
        
        printf ("Processando vetor de tamanho %d. Aguarde!\n", tamanho);
        
		geraVetor (vetor, tamanho);
		
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
			continue;
		}
		
		/* Saída dos dados */	
		fprintf (pfile, "%.10lf ", tN );
		fprintf (pfile, "%.10lf\n", tLOGN );
		
		free (vetor);
	}
		
	fclose (pfile);
    printf ("FIM\n");

	return 0;
}
