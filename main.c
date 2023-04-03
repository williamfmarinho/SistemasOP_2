#include <stdio.h>
#include <stdlib.h>
#define processos 5
struct PCB{
    int pid;
    int chegada;
    int burst;
    int prioridade;
    struct PCB* proximo;
    struct PCB* anterior;
    struct PCB* pai;
};

struct PCB lista_PCB[processos];
int lista_Burst[processos];
int chegada = 0;

void setup_inicial(){
    srand(time(NULL));

    for (int i = 0; i < processos; i++){
        int burst = 1 + rand() % 10;
        int prioridade = 1 + rand() % 10;
        //printf("Burst gerado aleatorio: %d\n", burst);
        //printf("Prioridade gerada aleatoria: %d\n", prioridade);
        if (i < 2)
            chegada = 0;
        else
            chegada = i - 1;


        struct PCB pcb_i;
        pcb_i.pid = i;
        pcb_i.chegada = chegada;
        pcb_i.burst = burst;
        pcb_i.prioridade = prioridade;


        lista_PCB[i] = pcb_i;
        lista_Burst[i] = burst;

        //printf("Burst da lista de burst posicao %d: %d\n", i, lista_Burst[i]);
        //printf("Prioridade do pcb_i posicao %d: %d\n", i, lista_PCB[i].prioridade);

    }

}


struct lista_FCFS{
    int tamanho;
    struct PCB* head;
    struct PCB* tail;

};

void setup_FCFS(struct lista_FCFS* fcfs){
    fcfs->head = NULL;
    fcfs->tail = NULL;
    fcfs->tamanho = 0;
}


void push(struct lista_FCFS* fcfs, struct PCB* pcb){
    if (fcfs->tamanho == 0){
        fcfs->head = pcb;
        fcfs->tail = fcfs->head;
    }
    else{
        fcfs->tail->proximo = pcb;
        fcfs->tail = fcfs->tail->proximo;
    }

    fcfs->tamanho += 1;
}
struct PCB* pop(struct lista_FCFS* fcfs){
    if (fcfs->tamanho == 0)
        return;

    struct PCB* temporario = fcfs->head;
    fcfs->head = fcfs->head->proximo;
    fcfs->tamanho -= 1;
    return temporario;
}

void listar_todos_os_pcbs(){
    for (int i = 0; i < processos; i++)
        printf("PCB na posicao %d; Pid: %d, Chegada: %d, Burst: %d, Prioridade: %d\n", i, lista_PCB[i].pid, lista_PCB[i].chegada, lista_PCB[i].burst, lista_PCB[i].prioridade);
}

int main()
{
    setup_inicial();

    printf("-------------------------------------------------------------\n");

    int turnaround_FCFS[processos];
    int espera_FCFS[processos];
    struct lista_FCFS fila;
    setup_FCFS(&fila);
    listar_todos_os_pcbs();
    printf("-------------------------------------------------------------\n");
    printf("\n");
    for (int i = 0; i < processos; i++) {
        lista_PCB[i].burst = lista_Burst[i];
        espera_FCFS[i] = -lista_Burst[i];
        turnaround_FCFS[i] = 0;
        push(&fila, &lista_PCB[i]);
        //printf ("fila FCFS Head: %d\n",fila.head->pid);
        //printf ("fila FCFS Tail: %d\n",fila.tail->pid);
        //printf ("fila FCFS tamanho: %d\n",fila.tamanho);
}

    struct lista_FCFS* node;
    node->head = fila.head;

    printf ("\n----Visualizando a fila FCFS------\n");
        while(node->head->pid < processos){
            printf("Pid: %d, Chegada: %d, Burst: %d\n", node->head->pid, node->head->chegada, node->head->burst);
            node->head = node->head->proximo;
         }
    printf ("\n----Executando a fila FCFS------\n");
    int t = 0;
    node->head = fila.head;
    while(node->head->pid < processos){
        while(node->head->burst > 0){
            printf ("T: %d, Pid: %d, Burst: %d\n", t, node->head->pid, node->head->burst);
            t += 1;
            node->head->burst -= 1;
            if (node->head->burst == 0){
                turnaround_FCFS[node->head->pid] = t - node->head->chegada;
                printf("Turnaround: %d\n", turnaround_FCFS[node->head->pid]);
                espera_FCFS[node->head->pid] += turnaround_FCFS[node->head->pid];
                printf ("Terminou processo pid: %d, Proximo pid: %d\n", node->head->pid, node->head->proximo->pid);
            }
        }
        pop(&fila);
        node->head = fila.head;
    }
    printf("--------------------------------------//----------------------------------------\n");



    return 0;
}
