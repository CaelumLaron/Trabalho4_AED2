#include "./Lib/Grafo.h"

const int print_ele = 0, print_niv = 1;

void create_graph(int number_componentes, int per_connectivity, int number_vertices, Graph *graph){
	per_connectivity = per_connectivity%101;
	int newVert = (per_connectivity * (number_vertices - 1))/100;
	if(newVert < 1)
		return;
	graph->number_vertices = number_vertices;
	graph->adjList = (List *)malloc(sizeof(List) * number_vertices);
	int vertices_per_components = number_vertices/number_componentes;
	for(int i=0; i<number_componentes; ++i){
		for(int j=i*vertices_per_components; j<vertices_per_components*(i+1); ++j){
			if(j == ((vertices_per_components*(i+1))-1) )
				push_list(i*vertices_per_components, &graph->adjList[j]);
			else
				push_list(j+1, &graph->adjList[j]);
		}
	}
	newVert--;
	if(newVert < 1)
		return;
	for(int i=0; i<number_vertices; i++){
		int first_component = (i/number_componentes) * number_componentes;
		for(int j=0, k=0, vert = first_component; j<number_vertices, k<newVert; j++, vert++){
			vert = vert%number_vertices;
			if(vert != i){
				if(first_component+vertices_per_components-1 == i){
					if(vert != first_component){
						push_list(vert, &graph->adjList[i]);
						k++;
					}
				}
				else{
					if(vert != i+1){
						push_list(vert, &graph->adjList[i]);
						k++;
					}
				}
			}
		}
	}
}

void create_graph_connected(int per_connectivity, int number_vertices, Graph *graph){
	per_connectivity = per_connectivity%101;
	int newVert = (per_connectivity * (number_vertices - 1))/100;
	graph->number_vertices = number_vertices;
	graph->adjList = (List*)malloc(sizeof(List) * number_vertices);
	if(newVert < 1)
		return;
	for(int i=0; i<number_vertices; i++){
		int next_vertice = (i+1)%number_vertices;
		push_list(next_vertice, &graph->adjList[i]);
	}
	newVert--;
	if(newVert < 1)
		return;
	for(int i=0; i<number_vertices; i++){
		for(int j=0, k=0; j<number_vertices, k<newVert; j++){
			if(j != (i+1)%number_vertices && (j!= i)){
				push_list(j, &graph->adjList[i]);
				k++;
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

void show_adjList_vertices(Graph graph){
	int n = graph.number_vertices;
	for(int i=0; i<n; i++){
		printf("Vertice %d: \n", i);
		show_list(graph.adjList[i]);
	}
}

void DFS_Stack(Graph graph, int start){
	int *vis = (int*)malloc(sizeof(int) * graph.number_vertices);
	for(int i=0; i<graph.number_vertices; i++)
		vis[i] = -1;
	Stack stack;
	new_stack(&stack);
	push_stack(start, &stack);
	vis[start] = 1;
	while(!stack_empty(stack)){
		int cur = top(stack);
		if(print_ele)
			printf("%d ", cur);
		pop_stack(&stack);
		for(Element* it = graph.adjList[cur].first; it != NULL; it = it->prox){
			int act = it->data;
			if(vis[act] == -1){
				vis[act] = 1;
				push_stack(act, &stack);
			}
		}
	}
	if(print_ele)
		printf("\n");
}

void DFS_Recursive_Caller(Graph graph, int start){
	int *vis = (int*)malloc(sizeof(int) * graph.number_vertices);
	for(int i=0; i<graph.number_vertices; i++)
		vis[i] = -1;
	vis[start] = 1;
	DFS_Recursive(start, &graph, vis);
	if(print_ele)
		printf("\n");
}

void DFS_Recursive(int cur, Graph *graph, int *vis){
	if(print_ele)
		printf("%d ", cur);
	for(Element *it = graph->adjList[cur].first; it != NULL; it = it->prox){
		int act = it->data;
		if(vis[act] == -1){
			vis[act] = 1;
			DFS_Recursive(act, graph, vis);
		}
	}
}

void BFS(Graph graph, int start){
	int *vis = (int*)malloc(sizeof(int) * graph.number_vertices);
	int *d = (int*)malloc(sizeof(int) * graph.number_vertices);
	for(int i=0; i<graph.number_vertices; i++)
		vis[i] = -1, d[i] = 0;
	Queue queue;
	new_queue(&queue);
	push_queue(start, &queue);
	vis[start] = 1;
	d[start] = 0;
	int niv = 0;
	if(print_niv)
		printf("Nivel %d: ", niv);
	while(!queue_empty(queue)){
		int cur = front_queue(queue);
		if(print_ele)
			printf("%d ", cur);
		if(print_niv && niv == d[cur])
			printf("%d ", cur);
		else if(print_niv){
			niv++;
			printf("\nNivel %d: %d ", niv, cur);
		}
		pop_queue(&queue);
		for(Element *it = graph.adjList[cur].first; it != NULL; it = it->prox){
			int act = it->data;
			if(vis[act] == -1){
				d[act] = d[cur] + 1;
				vis[act] = 1;
				push_queue(act, &queue);
			}
		}
	}
	if(print_ele || print_niv)
		printf("\n");
}

int Dfs_Finding_Cycles(int cur, Graph *graph, int *color){
	color[cur] = 1;
	for(Element *it = graph->adjList[cur].first; it != NULL; it = it->prox){
		int act = it->data;
		if(color[act] == 0){
			if(Dfs_Finding_Cycles(act, graph, color) == 1)
				return 1;
		}
		else if(color[act] == 1)
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
		if(Dfs_Finding_Cycles(0,&graph,color))
			return 1;
		for(int i=0; i < graph.number_vertices; i++){
			visited[i] += color[i];
			color[i] = 0;
		}
	}
	return 0;
}

void all_way_graph_caller(Graph graph){
	Stack stack;
	new_stack(&stack);
	int *visited = (int*)malloc(sizeof(int) * graph.number_vertices);
	for(int i=0; i<graph.number_vertices; i++){
		for(int i=0; i<graph.number_vertices; i++)
			visited[i] = 0;
		visited[i] = 1;
		push_stack(i, &stack);
		all_way_graph(i, &graph, &stack, visited);
		pop_stack(&stack);
	}
}

void all_way_graph(int cur, Graph *graph, Stack *stack, int *visited){
	if(stack->size == graph->number_vertices){
		show_stack(*stack);
		return;
	}	
	for(Element *it = graph->adjList[cur].first; it != NULL; it = it->prox){
		int act = it->data;
		if(visited[act] == 0){
			push_stack(act, stack);
			visited[act] = 1;
			all_way_graph(act, graph,stack, visited);
			visited[act] = 0;
			pop_stack(stack);
		}
	}
}