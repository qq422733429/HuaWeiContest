
#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define INF 9999999

static int min_weight=1000000;
Path *result_path = (Path*)malloc(sizeof(Path));
int must_though_list[52]={0};
int must_though_num;
int origin_graph[600][600];
int route_graph[600][600];
int route_num;
int vertex_num;
int temp_bool_vertex[600];
int start_vertex;
int end_vertex;
int must_though_vertex[600]={0};
int alread_in_path[52];
int alread_in_path_num;



//你要完成的功能总入口
void search_route(char *topo[4801], int edge_num, char *demand)
{
			    int there_is_no_result=0;
			    for(int i =0;i<edge_num;i++){
			        	char *delim = ",";
			            char *p;
			            p =strtok(topo[i], delim);
			            int route_id=atoi(p);
			            p =strtok(NULL, delim);
			            int start_id=atoi(p);
			            p =strtok(NULL, delim);
			            int end_id=atoi(p);
			            p =strtok(NULL, delim);
			            int route_wight=atoi(p);
			            if(start_id==end_id)
			            	continue;

			            //To judge whether or not to have the edge
			            if(origin_graph[start_id][end_id]!=0){
			            	//To determine whether or not smaller than the original
			            	if(origin_graph[start_id][end_id]>route_wight){
			            		route_graph[start_id][end_id]=route_id;
			            		origin_graph[start_id][end_id]=route_wight;
			            	}
			            }else{
			            	route_graph[start_id][end_id]=route_id;
			            	origin_graph[start_id][end_id]=route_wight;
			            	route_num++;
			            	if(temp_bool_vertex[start_id]==0){
			            		temp_bool_vertex[start_id]=1;
			            		vertex_num++;
			            	}
			            	if(temp_bool_vertex[end_id]==0){
			            		temp_bool_vertex[end_id]=1;
			            		vertex_num++;
			            	}
			            }
			        }
			    char *delim = ",";
			    char *temp;
			    temp=strtok(demand, delim);
			    start_vertex=atoi(temp);
			    temp=strtok(NULL, delim);
			    end_vertex=atoi(temp);
			    temp=strtok(NULL, delim);
			    char *p;
			    p=strtok(temp, "|");
			    must_though_vertex[start_vertex]=1;
			    must_though_list[must_though_num++]=start_vertex;
			    do{
			    	must_though_vertex[atoi(p)]=1;
			    	must_though_list[must_though_num++]=atoi(p);
			    }while((p=strtok(NULL, "|"))!=NULL);
			    must_though_vertex[end_vertex]=1;
			    must_though_list[must_though_num++]=end_vertex;
//			    for(int i =0;i<vertex_num;i++){
//			    	for(int j=0;j<vertex_num;j++){
//			    		printf("%d  ",origin_graph[i][j]);
//			    	}
//			    	printf("\n");
//			    }

			    //检查是否有必经节点没有下一跳
			    for(int i =0;i<must_though_num;i++){
			    	int temp_check=0;
			    	for(int j=0;j<vertex_num;j++){
			    		if(origin_graph[must_though_list[i]][j]!=0&&j!=end_vertex){
			    			temp_check++;
			    		}
			    	}
			    	if(temp==0){
			    		there_is_no_result=1;
			    	}
			    }
			    printf("vertex num : %d \n",vertex_num);
			    printf("route_num : %d \n",route_num);
			    printf("must_though_num : %d \n",must_though_num);

			    if(there_is_no_result==0){
						if (route_num<200) {
							Path *start = (Path*)malloc(sizeof(Path));
							start->exist_path[start->exist_vertex_num]=start_vertex;
							start->exist_vertex_num++;
							start->last_vertex=start_vertex;
							if(must_though_vertex[start_vertex]==0)
								start->must_through_num=must_though_num;
							else
								start->must_through_num=must_though_num-1;
							int bool_graph[600];
							bool_graph[start_vertex]=1;
							find_route_dfs_1(vertex_num,origin_graph,start,bool_graph,end_vertex,must_though_vertex);
						}else{

							alread_in_path[alread_in_path_num++]=start_vertex;
							dijkstra_first();
							int temp_must_though_num;
							int temp_must_though_list[50];
							for(int i=0;i<must_though_num-2;i++){
								for(int k=0;k<temp_must_though_num;k++)
									temp_must_though_list[k]=0;
								temp_must_though_num=0;
								for(int j=0;j<must_though_num;j++){
									int status=0;
									for(int k=0;k<alread_in_path_num;k++){
										if(alread_in_path[k]==must_though_list[j]){
											status=1;
											break;
										}
									}
									if(status!=1)
										temp_must_though_list[temp_must_though_num++]=must_though_list[j];
								}
								search_and_add(temp_must_though_list,temp_must_though_num);
							}

						}
			    }
			    printf("weight:%d\n",min_weight);
				for (int i = 0; i < result_path->exist_vertex_num; i++)
					record_result(route_graph[result_path->exist_path[i]][result_path->exist_path[i+1]]);
				for(int i =0;i<result_path->exist_vertex_num;i++)
					printf("%d  ",result_path->exist_path[i]);

}

void search_and_add(int temp_must_though_list[50],int temp_must_though_num){
	Path *first_path;
	Path *last_path;
	Path *path2;
	int best_vertex;
	Path *temp_best=(Path *)malloc(sizeof(Path));
	temp_best->weight=999999;
	Path *temp;
	for(int i=0;i<temp_must_though_num;i++){
		int current_must_though_vertex=temp_must_though_list[i];
		if(current_must_though_vertex!=end_vertex){
		for(int j =0;j<alread_in_path_num-1;j++){
			first_path=(Path *)malloc(sizeof(Path));
			last_path=(Path *)malloc(sizeof(Path));
			int start=alread_in_path[j];
			int end=alread_in_path[j+1];
			path2=(Path *)malloc(sizeof(Path));
			int switchs=0;
			for(int k =0;k<result_path->exist_vertex_num;k++){

				if(result_path->exist_path[k]==start){
									switchs=1;
								}
				if(switchs==0){
					path2->exist_path[path2->exist_vertex_num++]=result_path->exist_path[k];
				}
				if(result_path->exist_path[k]==end){
									switchs=0;
								}
			}
			for(int k=0;k<temp_must_though_num;k++){
				if(temp_must_though_list[k]!=current_must_though_vertex)
					path2->exist_path[path2->exist_vertex_num++]=temp_must_though_list[k];
			}
			int result1 = dijkstra_p2p(path2,start,current_must_though_vertex,first_path);
			if(result1==1){
				for(int k=0;k<first_path->exist_vertex_num;k++)
					path2->exist_path[path2->exist_vertex_num++]=first_path->exist_path[k];
				path2->exist_path[path2->exist_vertex_num++]=alread_in_path[i];
				int result2 = dijkstra_p2p(path2,current_must_though_vertex,end,last_path);
				if(result2==1){
					temp=(Path *)malloc(sizeof(Path));
					for(int x=0;x<result_path->exist_vertex_num;x++){
						if(result_path->exist_path[x]==end){
								switchs=0;
						}
						if(switchs==0){
							temp->exist_path[temp->exist_vertex_num++]=result_path->exist_path[x];
						}
						if(result_path->exist_path[x]==start){
								switchs=1;
								for(int y=first_path->exist_vertex_num-1;y>0;y--)
									temp->exist_path[temp->exist_vertex_num++]=first_path->exist_path[y];
								temp->exist_path[temp->exist_vertex_num++]=current_must_though_vertex;
								for(int y=last_path->exist_vertex_num-1;y>0;y--)
									temp->exist_path[temp->exist_vertex_num++]=last_path->exist_path[y];
						}
					}
								//suan quanzhi
								int weitht_count=count(temp);
									if(weitht_count<temp_best->weight){
									temp_best=(Path *)malloc(sizeof(Path));
									for(int i=0;i<temp->exist_vertex_num;i++)
										temp_best->exist_path[temp_best->exist_vertex_num++]=temp->exist_path[i];
									best_vertex=current_must_though_vertex;
									temp_best->weight=weitht_count;
				}



				}
			}
			}


				last_path=(Path *)malloc(sizeof(Path));
				path2=(Path *)malloc(sizeof(Path));
				for(int k =0;k<result_path->exist_vertex_num;k++){
					path2->exist_path[path2->exist_vertex_num++]=result_path->exist_path[k];
				}
				int start=path2->exist_path[--path2->exist_vertex_num];
				path2->exist_path[path2->exist_vertex_num]=0;
				for(int k=0;k<temp_must_though_num;k++){
					if(temp_must_though_list[k]!=current_must_though_vertex)
						path2->exist_path[path2->exist_vertex_num++]=temp_must_though_list[k];
					}
				int result2 = dijkstra_p2p(path2,start,current_must_though_vertex,last_path);
				if(result2==1){
					temp=(Path *)malloc(sizeof(Path));
								for(int k =0;k<result_path->exist_vertex_num;k++){
									temp->exist_path[temp->exist_vertex_num++]=result_path->exist_path[k];
								}
								for(int y=last_path->exist_vertex_num-1;y>0;y--)
									temp->exist_path[temp->exist_vertex_num++]=last_path->exist_path[y];
								temp->exist_path[temp->exist_vertex_num++]=current_must_though_vertex;
								int weitht_count=count(temp);
									if(weitht_count<temp_best->weight){
									temp_best=(Path *)malloc(sizeof(Path));
									for(int i=0;i<temp->exist_vertex_num;i++)
										temp_best->exist_path[temp_best->exist_vertex_num++]=temp->exist_path[i];
										best_vertex=current_must_though_vertex;
										temp_best->weight=weitht_count;
								}

							}

		}else{
			last_path=(Path *)malloc(sizeof(Path));
			path2=(Path *)malloc(sizeof(Path));
			for(int k =0;k<result_path->exist_vertex_num;k++){
				path2->exist_path[path2->exist_vertex_num++]=result_path->exist_path[k];
			}
			int start=path2->exist_path[--path2->exist_vertex_num];
			path2->exist_path[path2->exist_vertex_num]=0;
			for(int k=0;k<temp_must_though_num;k++){
				if(temp_must_though_list[k]!=current_must_though_vertex)
					path2->exist_path[path2->exist_vertex_num++]=temp_must_though_list[k];
			}
			int result2 = dijkstra_p2p(path2,start,current_must_though_vertex,last_path);
			if(result2==1){
				temp=(Path *)malloc(sizeof(Path));
				for(int k =0;k<result_path->exist_vertex_num;k++){
					temp->exist_path[temp->exist_vertex_num++]=result_path->exist_path[k];
				}
				for(int y=last_path->exist_vertex_num-1;y>0;y--)
					temp->exist_path[temp->exist_vertex_num++]=last_path->exist_path[y];
				temp->exist_path[temp->exist_vertex_num++]=current_must_though_vertex;
				int weitht_count=count(temp);
				if(weitht_count<temp_best->weight){
					temp_best=(Path *)malloc(sizeof(Path));
					for(int i=0;i<temp->exist_vertex_num;i++)
						temp_best->exist_path[temp_best->exist_vertex_num++]=temp->exist_path[i];
						best_vertex=current_must_though_vertex;
						temp_best->weight=weitht_count;
				}
			}

		}



		}
		result_path->exist_vertex_num=0;
		alread_in_path_num=0;
		for(int i=0;i<temp_best->exist_vertex_num;i++){
			result_path->exist_path[result_path->exist_vertex_num++]=temp_best->exist_path[i];
			if(must_though_vertex[temp_best->exist_path[i]]==1)
				alread_in_path[alread_in_path_num++]=temp_best->exist_path[i];
		}





	}

int count(Path *path){
	int result=0;
	for(int i=0;i<path->exist_vertex_num-1;i++){
		result+=origin_graph[path->exist_path[i]][path->exist_path[i+1]];
	}
	return result;

}






int sssss;
int dijkstra_p2p(Path *current_path, int start,int end,Path *path){
	int prev[600];
	int dist[600];
	int i,j,k;
	int min;
	int tmp;
	int flag[600];	  // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
	int matrix[600][600];
	printf("%d \n ",sssss++);
	for(int i =0;i<vertex_num;i++){
				for(int j=0;j<vertex_num;j++){
					if(origin_graph[i][j]!=0)
						matrix[i][j]=origin_graph[i][j];
					else
						matrix[i][j]=INF;

				}
			}
	for(int i=0;i<current_path->exist_vertex_num;i++){
		for(int j=0;j<vertex_num;j++){
			for(int k=0;k<vertex_num;k++){
				matrix[j][current_path->exist_path[i]]=INF;
				matrix[current_path->exist_path[i]][j]=INF;
			}
		}
	}



	for (i = 0; i < vertex_num; i++)
		  {
			flag[i] = 0;
		    prev[i] = start;			  // 顶点i的前驱顶点为0。
		    dist[i] = matrix[start][i];// 顶点i的最短路径为"顶点vs"到"顶点i"的权。
		  }

		  // 对"顶点vs"自身进行初始化
		  flag[start] = 1;
		  dist[start] = 0;
		  prev[start] = start;

  // 遍历G.vexnum-1次；每次找出一个顶点的最短路径。
  for (i = 1; i < vertex_num; i++)
  {
    // 寻找当前最小的路径；
    // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
    min = INF;
    for (j = 0; j < vertex_num; j++)
    {
      if (flag[j]==0 && dist[j]<min)
      {
        min = dist[j];
        k = j;
      }
    }
    // 标记"顶点k"为已经获取到最短路径
    flag[k] = 1;

    // 修正当前最短路径和前驱顶点
    // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
    for (j = 0; j < vertex_num; j++)
    {
      tmp = (matrix[k][j]==INF ? INF : (min + matrix[k][j])); // 防止溢出
      if (flag[j] == 0 && (tmp  < dist[j]) )
      {
        dist[j] = tmp;
        prev[j] = k;
      }
    }
  }
  if(dist[end]>=INF||dist[end]==0)
	  return 0;
  else{
		  int temp=prev[end];

		  while(temp!=start){
			  path->exist_path[path->exist_vertex_num++]=temp;
			  temp=prev[temp];
		  }
		  return 1;
  }
}


void dijkstra_first(){
	Path *path;
	int prev[600];
	int dist[600];
	int i,j,k;
	int min;
	int tmp;
	int flag[600];	  // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
	int matrix[600][600];
	for(int i =0;i<vertex_num;i++){
			for(int j=0;j<vertex_num;j++){
				if(origin_graph[i][j]!=0)
					matrix[i][j]=origin_graph[i][j];
				else
					matrix[i][j]=INF;

			}
		}

	  // 初始化
	  for (i = 0; i < vertex_num; i++)
	  {
		  flag[i] = 0;
	    prev[i] = start_vertex;			  // 顶点i的前驱顶点为0。
	    dist[i] = matrix[start_vertex][i];// 顶点i的最短路径为"顶点vs"到"顶点i"的权。
	  }

	  // 对"顶点vs"自身进行初始化
	  flag[start_vertex] = 1;
	  dist[start_vertex] = 0;
	  prev[start_vertex] = start_vertex;

	  // 遍历G.vexnum-1次；每次找出一个顶点的最短路径。
	  for (i = 1; i < vertex_num; i++)
	  {
	    // 寻找当前最小的路径；
	    // 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
	    min = INF;
	    for (j = 0; j < vertex_num; j++)
	    {
	      if (flag[j]==0 && dist[j]<min)
	      {
	        min = dist[j];
	        k = j;
	      }
	    }
	    // 标记"顶点k"为已经获取到最短路径
	    flag[k] = 1;

	    // 修正当前最短路径和前驱顶点
	    // 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
	    for (j = 0; j < vertex_num; j++)
	    {
	      tmp = (matrix[k][j]==INF ? INF : (min + matrix[k][j])); // 防止溢出
	      if (flag[j] == 0 && (tmp  < dist[j]) )
	      {
	        dist[j] = tmp;
	        prev[j] = k;
	      }
	    }
	  }
  for(int i =0;i<must_though_num;i++){
	  if(must_though_list[i]!=start_vertex&&dist[must_though_list[i]]<INF&&dist[must_though_list[i]]<min_weight){
		  	  	  path=(Path *)malloc(sizeof(Path));
				  path->weight=dist[must_though_list[i]];
				  path->exist_path[path->exist_vertex_num++]=must_though_list[i];
				  int temp=prev[must_though_list[i]];

				  while(temp!=start_vertex){
					  path->exist_path[path->exist_vertex_num++]=temp;
					  temp=prev[temp];
				  }
				  for(int k=0;k<result_path->exist_vertex_num;k++)
					  result_path->exist_path[k]=0;
				  result_path->exist_vertex_num=0;
				  result_path->exist_path[0]=start_vertex;
				  result_path->exist_vertex_num++;
				  min_weight=path->weight;
				  for(int k=path->exist_vertex_num-1;k>-1;k--)
					  result_path->exist_path[result_path->exist_vertex_num++]=path->exist_path[k];


		  }
	  }

  alread_in_path[alread_in_path_num++]=result_path->exist_path[result_path->exist_vertex_num-1];


}







//
//void init(int cishu)
//{
//       int i,j;
//       for(i=0;i<=cishu;i++)
//              for(j=0;j<=cishu;j++)
//              {
//            	  floyd_grgph[i][j]=9999999;
//                  path[i][j]=j;
//              }
//
//}
//
//void floyd(int cishu)
//{
//       int i,j,k;
//       for(k=0;k<=cishu;k++)
//              for(i=0;i<=cishu;i++)
//                     for(j=0;j<=cishu;j++)
//                            if(floyd_grgph[i][j]>floyd_grgph[i][k]+floyd_grgph[k][j])
//                            {
//                            	floyd_grgph[i][j]=floyd_grgph[i][k]+floyd_grgph[k][j];
//                            	path[i][j]=path[i][k];
//                            }
//}
//
//



void find_route_dfs_1(int vertex_num,int origin_graph[][600],Path *route,int though_vertex[600],int end_vertex,int must_though_vertex[600]){
	//寻找下一条边
	for(int i=0;i<vertex_num;i++){
		if(origin_graph[route->last_vertex][i]!=0){
			//该点未经过
			if(though_vertex[i]==0&&route->weight+origin_graph[route->last_vertex][i]<=min_weight){

					though_vertex[i]=1;
					route->exist_path[route->exist_vertex_num++]=i;
					route->weight+=origin_graph[route->last_vertex][i];
					if(must_though_vertex[i]!=0)
						route->must_through_num--;
				if(i!=end_vertex){
					route->last_vertex=i;
					find_route_dfs_1(vertex_num,origin_graph,route,though_vertex,end_vertex,must_though_vertex);

				}else{
					if(route->must_through_num==0){
							for(int k=0;k<result_path->exist_vertex_num;k++)
								result_path->exist_path[k]=0;
							result_path->exist_vertex_num=0;
							min_weight=route->weight;
							for(int i =0;i<route->exist_vertex_num;i++)
								result_path->exist_path[result_path->exist_vertex_num++]=route->exist_path[i];
							result_path->weight=route->exist_vertex_num;
					}
						though_vertex[i]=0;
						route->weight-=origin_graph[route->last_vertex][i];
						route->exist_path[--route->exist_vertex_num]=0;
						if(must_though_vertex[i]!=0)
								route->must_through_num++;

				}
			}
		}

	}
	though_vertex[route->last_vertex]=0;
	route->weight-=origin_graph[route->exist_path[route->exist_vertex_num-2]][route->last_vertex];
	if(must_though_vertex[route->last_vertex]!=0)
				route->must_through_num++;
	route->last_vertex=route->exist_path[route->exist_vertex_num-2];
	route->exist_path[--route->exist_vertex_num]=0;


}




