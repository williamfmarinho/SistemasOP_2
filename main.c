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
int lista_prioridade[processos];
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
        lista_prioridade[i] = prioridade;

        //printf("Burst da lista de burst posicao %d: %d\n", i, lista_Burst[i]);
        //printf("Prioridade do pcb_i posicao %d: %d\n", i, lista_PCB[i].prioridade);

    }

}
    //!------------------------------------------------------FIRST COME FIRST SERVED----------------------------------------------------------------

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
    //(*fcfs).head == fcfs->head
    struct PCB* temporario = fcfs->head;
    fcfs->head = fcfs->head->proximo;
    fcfs->tamanho -= 1;
    return temporario;
}

void listar_todos_os_pcbs(){
    for (int i = 0; i < processos; i++)
        printf("PCB na posicao %d; Pid: %d, Chegada: %d, Burst: %d, Prioridade: %d\n", i, lista_PCB[i].pid, lista_PCB[i].chegada, lista_PCB[i].burst, lista_PCB[i].prioridade);
}


    //!--------------------------------------------------PRIORIDADE---------------------------------------------------------------












typedef struct Heap_minimo {
    int capacidade_heap;
    int tamanho_heap;
    struct PCB* data;
} Heap_minimo;

Heap_minimo* Criar_heap_minimo(int capacidade_heap) {
    Heap_minimo* heap = malloc(sizeof(Heap_minimo));
    heap->capacidade_heap = capacidade_heap;
    heap->tamanho_heap = 0;
    heap->data = malloc(sizeof(struct PCB) * capacidade_heap);
    return heap;
}

void deletar_heap(Heap_minimo* heap) {
    free(heap->data);
    free(heap);
}

void trocar(struct PCB* a, struct PCB* b) {
    struct PCB temporario = *a;
    *a = *b;
    *b = temporario;
}

void heapify(Heap_minimo* heap, int i) {
    int menor = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < heap->tamanho_heap && heap->data[esquerda].prioridade < heap->data[menor].prioridade) {
        menor = esquerda;
    }

    if (direita < heap->tamanho_heap && heap->data[direita].prioridade < heap->data[menor].prioridade) {
        menor = direita;
    }

    if (menor != i) {
        trocar(&heap->data[i], &heap->data[menor]);
        heapify(heap, menor);
    }
}

void push_heap(Heap_minimo* heap, struct PCB item) {
    if (heap->tamanho_heap == heap->capacidade_heap) {
        return;
    }
    int i = heap->tamanho_heap;
    heap->data[i] = item;
    while (i != 0 && heap->data[(i - 1) / 2].prioridade > heap->data[i].prioridade) {
        trocar(&heap->data[(i - 1) / 2], &heap->data[i]);
        i = (i - 1) / 2;
    }

    heap->tamanho_heap++;

}

struct PCB heap_pop(Heap_minimo* heap) {
    if (heap->tamanho_heap == 0) {
        // heap is empty, return NULL
        struct PCB null_pcb = {-1, 0, 0, 0, NULL, NULL, NULL};
        return null_pcb;
    }

    // remove the root item (minimum value) from the heap
    struct PCB root = heap->data[0];
    heap->tamanho_heap--;

    // move the last item in the heap to the root position
    heap->data[0] = heap->data[heap->tamanho_heap];

    // fix the heap property by moving the root item down as necessary
    heapify(heap, 0);

    return root;
}





          //!--------------------------------------------------MAIN---------------------------------------------------------------

int main()
{
    setup_inicial();

    printf ("------------------------------FCFS------------------------\n");
    //!------------------------------------------------------FIRST COME FIRST SERVED----------------------------------------------------------------
    int turnaround_FCFS[processos];
    int espera_FCFS[processos];
    struct lista_FCFS fila;
    setup_FCFS(&fila);
    listar_todos_os_pcbs();
    printf("-------------------------------------------------------------\n");
    //!--------------------------------------------------------------------------------------------------------------------------------------------
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

    //struct lista_FCFS* node;
    struct PCB *pcb = pop(&fila);
    //struct PCB node;
    //node = pop(fila);
    //node->head = fila.head;

    printf ("\n----Visualizando a fila FCFS------\n");
    //    while(node->head->pid < processos){
    //        printf("Pid: %d, Chegada: %d, Burst: %d\n", node->head->pid, node->head->chegada, node->head->burst);
    //        node->head = node->head->proximo;
    //     }
    printf ("\n----Executando a fila FCFS------\n");
    int t = 0;
    //node->head = fila.head;
    while(pcb != NULL){
        while(pcb->burst > 0){
            printf ("T: %d, Pid: %d, Burst: %d\n", t, pcb->pid, pcb->burst);
            t += 1;
            pcb->burst -= 1;
            if (pcb->burst == 0){
                turnaround_FCFS[pcb->pid] = t - pcb->chegada;
                printf("Turnaround: %d\n", turnaround_FCFS[pcb->pid]);
                espera_FCFS[pcb->pid] += turnaround_FCFS[pcb->pid];
                //printf ("Terminou processo pid: %d, Proximo pid: %d\n", pcb->pid, pcb->proximo->pid);
            }

        }
        pcb = pop(&fila);
    }
    printf("--------------------------------------//----------------------------------------\n");

    //! Mostrando os turnaround do fcfs
    //for (int i = 0; i < processos; i++)
        //printf("%d\n", turnaround_FCFS[i]);

    for (int i = 0; i < 15; i++)
        printf("/\/\/\/\/\/\/\/\n");
    printf ("------------------------------PRIORIDADE------------------------------\n");
    //!------------------------------------------------------------PRIORIDADE------------------------------------------------------------------------------
    setup_inicial();
    listar_todos_os_pcbs();
    int t_prioridade = 0;
    int turnaround_prioridade[processos];
    int espera_prioridade[processos];
    printf ("-----------------------------------------------------------------------\n");



    //!Criando e alimentando a fila heap de prioridade
    Heap_minimo* heap = Criar_heap_minimo(processos);
    for (int i = 0; i < processos; i++){
        push_heap(heap, lista_PCB[i]);
        turnaround_prioridade[i] = 0;
        espera_prioridade[i] = -lista_Burst[i];

    }
    //!G-A-M-B-I-A-R-R-A: Realimentando os bursts pq por algum motivo deus sabe la qual nao tava passando

        for (int comparador = 0; comparador < processos; comparador++){
            int pid_atual = heap->data[comparador].pid;
            int burst_atual = lista_Burst[pid_atual];
            printf("Passando o burst da pid %d na posicao %d (%d) para a pid %d\n", pid_atual, comparador, burst_atual, heap->data[comparador].pid);
            heap->data[comparador].burst = burst_atual;
    }

    printf("\n");
    //!Visualizando a fila heap
    for (int i = 0; i < processos; i++){
        printf("Heap. PID: %d, burst: %d Prioridade: %d\n", heap->data[i].pid,heap->data[i].burst, heap->data[i].prioridade);
    }

    if ((heap->data[0].prioridade <= heap->data[1].prioridade && heap->data[2].prioridade) || (heap->data[1].prioridade <= heap->data[3].prioridade && heap->data[4].prioridade));
    //printf ("Push feito com sucesso, gracas a deus \n");

    //!Executando a fila
    struct PCB node_prioridade = heap_pop(heap);
    //printf("node removido pid: %d, burst %d, prioridade %d\n", node_prioridade.pid, node_prioridade.burst, node_prioridade.prioridade);
    printf("\n");
    while(node_prioridade.pid != -1){
            while(node_prioridade.burst > 0){
                printf ("T: %d, Prioridade: %d, PID: %d, Burst: %d\n", t_prioridade, node_prioridade.prioridade, node_prioridade.pid, node_prioridade.burst);
                t_prioridade += 1;
                node_prioridade.burst -= 1;
                if (node_prioridade.burst == 0){
                    turnaround_prioridade[node_prioridade.pid] = t_prioridade - node_prioridade.chegada;
                    espera_prioridade[node_prioridade.pid] += turnaround_prioridade[node_prioridade.pid];
                    printf("Turnaround do node %d: %d\n", node_prioridade.pid, turnaround_prioridade[node_prioridade.pid]);

                }


            }
            node_prioridade = heap_pop(heap);
    }


    return 0;
}
