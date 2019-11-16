#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

typedef struct vet{
        int *vetor;
        int top;
}arq;

arq *leitura(int argc,char *argv[]){
    
    FILE *fp[argc-4];
    int n_atual, cont_num = 0;
    int cont_arq = argc - 3;
    arq *vet ;
    
    vet = (arq*)malloc(sizeof(arq));
    vet->top = 0;
    //printf("args:%d  ultimo arg: %s",argc,argv[argc-1]);
    //printf("  .txt %d\n",cont_arq);

    for(int c = 0; c < cont_arq; c++){
        fp[c] = fopen(argv[c+2],"r");
        if(fp[c] == NULL)
            printf("arquivo vazio\n");
        else        
            while(fscanf(fp[c], "%d\n", &n_atual)!= EOF){
                vet->top++;
            }
    }
    for(int c = 0; c < cont_arq; c++){
        fclose(fp[c]);
    }
    vet->vetor = (int*)malloc(vet->top*sizeof(int));
    
    for(int c = 0; c<cont_arq; c++){
        fp[c] = fopen(argv[c+2],"r");
        if(fp[c]==NULL)
            printf("arquivo vazio\n");
        else        
            while(fscanf(fp[c],"%d\n",&n_atual)!=EOF){
                vet->vetor[cont_num] = n_atual;
                cont_num++;
            }vet->top = cont_num;
    }
    for(int c = 0;c<cont_arq;c++){
        fclose(fp[c]);
    }
    return vet;
}

void *ordena(void *desord){
    int proximo = 0;

    for(int i = 0; i < ((arq *)desord)->top; i++){
		for(int j = 0; j < (((arq *)desord)->top - 1); j++){
	        if(((arq *)desord)->vetor[j] > ((arq *)desord)->vetor[j+1]){
	        	proximo = ((arq *)desord)->vetor[j];
	        	((arq *)desord)->vetor[j] = ((arq *)desord)->vetor[j+1];
	        	((arq *)desord)->vetor[j+1] = proximo;
	        }
	    }
	 }

}
void escreve(arq *desord, char *argv){
    FILE * saida;
    saida = fopen(argv,"w");
    for(int c = 0; c < desord->top; c++)
        fprintf(saida,"%d\n",desord->vetor[c]);

}

int main(int argc, char *argv[]){
    clock_t start_time;
    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    

    arq *desord;
    desord = leitura(argc,argv);
    start_time = clock();

    for(int c  = 0; c < num_threads; c++)
		pthread_create(&threads[c], NULL, ordena, (void*)desord);

	for(int c  =0; c<num_threads;c++)
		pthread_join(threads[c], NULL);

    double time_final = (clock() - start_time) / (double)CLOCKS_PER_SEC;
    printf("Tempo de processamento usando %s threads: %f segundos\n", argv[1], time_final);
    escreve(desord, argv[argc-1]);

    return 0;
}
