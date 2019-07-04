#include "./Lib/Grafo.h"

const int print_ele = 1, print_niv = 1;

void create_graph_connected(int per_connectivity, int number_vertices, Graph *graph, int cycles){
	per_connectivity = per_connectivity%101;
	int newVert = (per_connectivity * number_vertices * (number_vertices - 1))/200;
	graph->number_vertices = number_vertices;
	graph->adjList = (List*)malloc(sizeof(List) * number_vertices);
	graph->m = (int**)malloc(sizeof(int*) * number_vertices);
	for(int i=0; i<number_vertices; i++){
		graph->m[i] = (int*)malloc(sizeof(int) * number_vertices);
		for(int j=0; j<number_vertices; j++)
			graph->m[i][j] = 0;
	}
	if(newVert < 1)
		return;
	if(cycles == 0)
		newVert = number_vertices - 1;
	for(int j=1; j < number_vertices && newVert > 0; j++){
		for(int i=0; i<number_vertices && newVert > 0; i++){
			if(i+j<number_vertices){
				newVert--;
				graph->m[i+j][i] = graph->m[i][i+j] = 1;
			} 
		}
	}
}

void create_graph_acyclic(int per_connectivity, int number_vertices, Graph *graph){
	per_connectivity = per_connectivity%101;
	int newVert = (per_connectivity * (number_vertices - 1))/100;
	graph->number_vertices = number_vertices;
	graph->adjList = (List*)malloc(sizeof(List) * number_vertices);
	if(newVert < 1)
		return;
	for(int i=0; i<number_vertices; i++){
		if(newVert < 1)
			return;
		for(int j=i+1; j<number_vertices; j++){
			push_list(j, &graph->adjList[i]);
			newVert--;
			if(newVert < 1)
				return;
		}
	}
}

void DFS_Stack(Graph graph, int start){
	int top = 0;
	int *vis = (int*)malloc(sizeof(int) * graph.number_vertices);
	int *pilha = (int*)malloc(sizeof(int) * (graph.number_vertices + 1));
	for(int i=0; i<graph.number_vertices; i++)
		vis[i] = -1;
	vis[start] = 1;
	pilha[top] = start;
	do{
		int cur = pilha[top--];
		if(print_ele)
			printf("%d ", cur);
		for(int act = 0; act < graph.number_vertices; act++){
			if(graph.m[act][cur] == 1 && vis[act] == -1){
				vis[act] = 1;
				pilha[++top] = act;
			}
		}
	}while(top >= 0);
	if(print_ele)
		printf("\n");
	free(vis);
	free(pilha);
}

void DFS_Recursive_Caller(Graph graph, int start){
	int *vis = (int*)malloc(sizeof(int) * graph.number_vertices);
	for(int i=0; i<graph.number_vertices; i++)
		vis[i] = -1;
	vis[start] = 1;
	DFS_Recursive(start, &graph, vis);
	if(print_ele)
		printf("\n");
	free(vis);
}

void DFS_Recursive(int cur, Graph *graph, int *vis){
	if(print_ele)
		printf("%d ", cur);
	for(int act = 0; act < graph->number_vertices; act++){
		if(graph->m[act][cur]==1 && vis[act] == -1){
			vis[act] = 1;
			DFS_Recursive(act, graph, vis);
		}
	}
}

void BFS(Graph graph, int start){
	int niv, ini, fim;
	int *fila = (int*)malloc(sizeof(int) * (graph.number_vertices + 1));
	int *vis = (int*)malloc(sizeof(int) * graph.number_vertices);
	int *d = (int*)malloc(sizeof(int) * graph.number_vertices);
	for(int i=0; i<graph.number_vertices; i++) 
		vis[i] = -1, d[i] = 0;
	vis[start] = 1;
	ini = fim = niv = d[start] = 0;
	fila[ini] = start;
	if(print_niv)
		printf("Nivel %d: ", niv);
	do{
		int cur = fila[ini++];
		if(print_niv && niv == d[cur])
			printf("%d ", cur);
		else if(print_niv){
			niv++;
			printf("\nNivel %d: %d ", niv, cur);
		}
		for(int act = 0; act < graph.number_vertices; act++){
			if(graph.m[cur][act] == 1 && vis[act] == -1){
				d[act] = d[cur] + 1;
				vis[act] = 1;
				fila[++fim] = act;
			}
		}
	}while(ini <= fim);
	if(print_ele || print_niv)
		printf("\n");
	free(fila);
	free(vis);
	free(d);
}

int Dfs_Finding_Cycles(int cur, Graph *graph, int *color){
	color[cur] = 1;
	for(int act = 0; act < graph->number_vertices; act++){
		if(graph->m[cur][act] == 1 && color[act] == 0){
			graph->m[cur][act]++, graph->m[act][cur]++;
			if(Dfs_Finding_Cycles(act, graph, color) == 1){
				graph->m[cur][act]--, graph->m[act][cur]--;
				return 1;
			}
			graph->m[cur][act]--, graph->m[act][cur]--;
		}
		else if(graph->m[cur][act] == 1 && color[act] == 1)
			return 1;
	}
	color[cur] = 2;
	return 0;
}

int Finding_Cycles(Graph graph){
	int *color = (int*)malloc(sizeof(int) * graph.number_vertices);
	int *visited = (int*)malloc(sizeof(int) * graph.number_vertices);
	for(int i=0; i < graph.number_vertices; i++)
		visited[i] = color[i] = 0;
	for(int i=0; i < graph.number_vertices; i++){
		if(visited[i])
			continue;
		if(Dfs_Finding_Cycles(i,&graph,color)){
			free(color);
			free(visited);
			return 1;
		}
		for(int j=0; j < graph.number_vertices; j++){
			visited[j] += color[j];
			color[j] = 0;
		}
	}
	free(color);
	free(visited);
	return 0;
}

void all_way_graph_caller(Graph graph){
	int top = 0;
	int *visited = (int*)malloc(sizeof(int) * graph.number_vertices);
	int *pilha = (int*)malloc(sizeof(int) * (graph.number_vertices + 1));
	for(int i=0; i<graph.number_vertices; i++)
		visited[i] = 0;
	visited[0] = 1;
	pilha[top] = 0;
	all_way_graph(0, &graph, pilha, top, visited);
	free(pilha);
	free(visited);
}

void all_way_graph(int cur, Graph *graph, int pilha[], int top, int *visited){
	if(top+1 == graph->number_vertices){
		for(int i=0; i<=top; i++){
			if(i<top)
				printf("%d->", pilha[i]);
			else
				printf("%d\n", pilha[i]);
		}
		return;
	}	
	for(int act = 0; act < graph->number_vertices; act++){
		if(graph->m[act][cur] == 1 && visited[act] == 0){
			pilha[++top] = act;
			visited[act] = 1;
			all_way_graph(act, graph,pilha, top, visited);
			visited[act] = 0;
			pilha[top--];
		}
	}
}