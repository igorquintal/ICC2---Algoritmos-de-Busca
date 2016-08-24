#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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
	/* Percorre-se todo o vetor, contando o número de vezes que o elemento aparece repetido */
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
    
    /* Variáveis para armazenar a quantidade de repetições do elemento na primeira e na segunda metade do vetor, respectivamente */
	int primeiraMetade = 0, segundaMetade = 0;

    /* Caso o primeiro e o último elemento do vetor são iguais ao elemento procurado, a quantidade de repetições é o tamanho do vetor, já que ele está ordenado */
	if ( vetor[esquerda] == elemento && vetor[direita] == elemento )
		return ( direita - esquerda ) + 1;
		
    /* Caso se chegue a um vetor de tamanho 1 que não contenha o elemento procurado */
	if ( esquerda == direita )
		return 0;
		
	/* Caso o vetor não seja todo formado pelo elemento procurado, divide-se ele pela metade e analisa-se as duas metades geradas, tomando o seguinte cuidado:*/
	else
	{
		meio = ( esquerda + direita ) / 2;

        /* Como o vetor está ordenado, caso o último elemento da primeira metade seja menor que o elemento procurado, podemos descartar essa metade. Caso contrário, analisamos a primeira metade */
		if ( vetor[meio] >= elemento )
			primeiraMetade = conta_logn ( vetor, esquerda, meio, elemento );
			
        /* Caso o primeiro elmento da segunda metade seja maior que o elemento procurado, podemos descartar essa metade. Caso contrário, analisamos o vetor formado pela segunda metade */
		if ( vetor[meio + 1] <= elemento )
			segundaMetade = conta_logn ( vetor, meio + 1, direita, elemento );
		
        /* Soma-se a quantidade de repetições encontradas na primeira e na segunda metades. Caso uma das funções acima não seja executada, como primeiraMetade e segundaMetade foram inicializadas com zero, não há problema na soma */
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
	
	/* Variável para armazenar o elemento a ser buscado */
	const int elemento = atoi(argv[2]);
	
	/* Ponteiro para o arquivo */
	FILE *pfile;
	
	/* Abertura do arquivo */
	pfile = fopen (argv[1], "r");
	
	if (pfile == NULL)
	{
		printf ("ERRO ao abrir arquivo de dados!!\n");
		return 1;
	}
		
	/* Leitura do tamanho do vetor */ 
	fscanf (pfile, "%d", &tamanho);
	
	/* Alocação de um vetor com o tamanho fornecido pelo arquivo */
	int *vetor;
	vetor = (int *) malloc ( tamanho * sizeof (int) );
	
	/* Cópia dos elementos do vetor fornecido pelo arquivo para o vetor alocado */
	int i;
	for (i = 0; i < tamanho; i++)
	{	
		fscanf (pfile, "%d", &vetor[i]);	
	}
	
	/* Ordenação do vetor */
	qsort (vetor, tamanho, sizeof(int), compare);

	double tN, tLOGN;
	int repeticoesN, repeticoesLOGN;
	
	double start_time, stop_time;
	
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
	
	/* Saída dos dados */
	printf ("Funcao de complexidade N\n"
			"\tRepeticoes: %d\n"
			"\tTempo: %.10lfs\n\n", repeticoesN, tN );
	printf ("Funcao de complexidade LOG N\n"
			"\tRepeticoes: %d\n"
			"\tTempo: %.10lfs\n\n", repeticoesLOGN, tLOGN );
    
    fclose (pfile);
    
    free (vetor);
		
	return 0;
}
