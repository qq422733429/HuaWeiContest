#ifndef __ROUTE_H__
#define __ROUTE_H__
struct Path{
	int exist_path[600]={0};
	int exist_vertex_num=0;
	int last_vertex;
	int must_through_num = 0;
	int weight=0;
};




void search_route(char *graph[5000], int edge_num, char *condition);
void find_route_dfs_1(int vertex_num,int origin_graph[][600],Path *route,int though_vertex[600],int end_vertex,int must_though_vertex[600]);
void init(int cishu);
void floyd(int cishu);
int dijkstra_p2p(Path *current_path, int start,int end,Path *path);
void dijkstra_first();
void search_and_add(int temp_must_though_list[50],int temp_must_though_num);
int count(Path *path);
#endif
