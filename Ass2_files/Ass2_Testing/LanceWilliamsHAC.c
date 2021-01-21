/*
Author: Kim
Date: 2019/11/21
Student ID: z5191441
Purpose: Apply LW Algo to derive a Denogram tree with a given Graph.
Structure: Using single linkage or complete linkage to calculate. Implement recursively from down to top. For 2 vertex dist is calcualted as the dist of 1/max weighted egde and for 2  multiple leaf nodes cluster we apply the algorithm with specified linkage. 
Constrains: Only a small portion of LW linkage are implemented in this code. Lot more might be implemented and studied in the future.
Reference: <stdio.h> library
           <stdlib.h> library
           "Graph.h" given by unsw comp2521 acadamic group
           "BSTree.h" given by unsw comp2521 acadamic group
           "LanceWilliamsHAC.h" given by unsw comp2521 acadamic group and implemented by kim.
*/

#include"LanceWilliamsHAC.h"
#include<stdlib.h>
#include"Graph.h"
#include<stdio.h>
#include "BSTree.h"

//Create a new dendrogram
Dendrogram new_D(int i);
//fill a double array to be value
void fill_zero(double *seen,int num,int value);
//check if only one node left to be NON-NULL in the array
int check_full(DNode** d_array,int num);
//fill the dist array to be -1
void fill_dist_negative(double **dist,int num);
//fill the distant array(with function above)
double ** distant(int num,Graph g);
//return a smaller value 
int small_one(int a,int b);
//return the larger one
int large_one(int a,int b);
//update the dist matrix after merging the clusters
void calculate_new_dist(int num,double **dist,int s,int b,int method,DNode** d_array); 
//find the minimal node value(LW ALGO)
double find_min(double **dist,DNode* dnode,int i);
//find the max node value(LW ALGO)
double find_max(double **dist,DNode* dnode,int i);
//free the dist array
void destroy_dist(double ** dist,int num);
//free the node array
void destroy_node(DNode **d_array,int num); 

//function to do the lw algo
Dendrogram LanceWilliamsHAC(Graph g, int method){
    //initialize the graph
    int num = GraphNumVertices(g);
    //initialize the DNODE array 
    DNode** d_array = malloc(num*sizeof(DNode*));
    //initialize each node in the d_array
    for(int i =0; i< num; i++){
        d_array[i]=malloc(sizeof(DNode));
        d_array[i]->vertex = -1;
    } 
    //make the left point to a vertex node;  
    for(int i =0; i<num; i++){
        d_array[i]->left = new_D(i);
        d_array[i]->right = NULL;
    }        
    //create a distance function
    double **dist = distant(num,g);   
    //while not only one pointer point to the nodes,keeps going
    while(check_full(d_array,num)){    
        int i = 0;
        int row = 0;//i
        int col = 0;//j
        //find the smallest i and j
        double small = dist[0][0];
        while(i<num){
            int j = 0;
            while(j<num){
                if((small > dist[i][j] && dist[i][j] >= 0 )|| small == -1){
                    small = dist[i][j];
                    row = i;
                    col = j;
                }
                j++;
            }
            i++;
        }
        //row and col records the closest vertex;  
        //create the parent node      
        DNode* node_parent = new_D(-1);        
        node_parent->left = d_array[row]->left;
        node_parent->right = d_array[col]->left;
        int s = small_one(row,col);
        int b = large_one(row,col);
        //up_date the dist array            
        //calculate the new cluster distance and put into small arrray                                        
        calculate_new_dist(num,dist,s,b,method,d_array); 
        //set the large array to be infinite (-1)
        fill_zero(dist[b],num,-1);
        //update the d_array
        d_array[s] ->left = node_parent;
        d_array[b]->left = NULL;
    }
    //go to the one with node(should be only one)
    int i  = 0;
    while(i<num && d_array[i]->left==NULL){
        i++;
    }  
    DNode * LW = d_array[i]->left;
    d_array[i]->left = NULL;
    //free the dist array and the node array here   
    destroy_dist(dist,num);
    destroy_node(d_array,num);   
    return LW;
}

//free dist
void destroy_dist(double ** dist,int num){
    for(int i = 0;i<num;i++){
        free(dist[i]);
    }
    free(dist);
}

//free node array
void destroy_node(DNode **d_array,int num){
    for(int i = 0;i<num;i++){
        free(d_array[i]);
    }
    free(d_array);
}

//update dist array after merge 
void calculate_new_dist(int num,double **dist,int s,int b,int method,DNode* *d_array){
    int i =0;//vertex i;
    //use while calculate the dist for the new clusters
    while(i<num){
        if(method ==SINGLE_LINKAGE){            
            double s_v =find_min(dist,d_array[s],i);
            double b_v =find_min(dist,d_array[b],i);
            //return smaller one
            if(s_v>b_v && b_v>0 ){
                s_v = b_v;
            }else if(s_v< 0 && b_v> 0){
                s_v=b_v;
            }
            //update symmetric          
            dist[s][i] =s_v;  
            dist[i][s]=dist[s][i];       
        }else if(method == COMPLETE_LINKAGE){
            double s_v =find_max(dist,d_array[s],i);
            double b_v =find_max(dist,d_array[b],i);
            if(s_v<b_v){
                s_v = b_v;
            }  
            dist[s][i] =s_v ;
            dist[i][s]=dist[s][i];       
        }
        i++;
    }
    //fix some nodes (diagonal and internal)
    dist[s][b]=-1;
    dist[b][s]=-1;
    dist[s][s]=-1;
    dist[b][b]=-1;
    //fill the col be -1
    for(int i = 0; i<num;i++){
        dist[i][b]=-1;
    }
}

//find the minimal values of all given leaf node
double find_min(double **dist,DNode* dnode,int i){
    //base case:
    if(dnode == NULL){
        return -1;
    }
    if(dnode->left == NULL){
        return dist[dnode->vertex][i];
    }
    //compare left right and return the smaller one
    double left = find_min(dist,dnode->left,i);
    double right = find_min(dist,dnode->right,i);
    if(left > 0 &&right > 0){
        if(left<=right){
            return left;
        }else{
            return right;
        }
    }
    if(left > 0 && right <=0){
        return left;
    }
    if(right > 0 && left <= 0){
        return right;
    }
    //both smaller than zero
    else{
        return -1;
    }
}
//find max value of the given leaf nodes
double find_max(double **dist,DNode *dnode,int i){
    //base case:
    if(dnode == NULL){
        return -1;
    }
    if(dnode->left == NULL){
        return dist[dnode->vertex][i];
    }    
    //compare and return the large one
    double left = find_max(dist,dnode->left,i);
    double right = find_max(dist,dnode->right,i);
    if(left>=right){
        return left;
    }else{
        return right;
    }
}

//return small int
int small_one(int a,int b){
    if(a>=b){
        return b;
    }else{
        return a;
    }
}
//return large int
int large_one(int a,int b){
    if(a<=b){
        return b;
    }else{
        return a;
    }
}

//fill dist array be -1
void fill_dist_negative(double **dist,int num){
    int i = 0;
    while(i<num){
        dist[i] = malloc(sizeof(double)*num);
        int j = 0;
        while(j<num){
            dist[i][j] = -1;
            j++;
        }
        i++;
    }
}

//fill in the distant array
double** distant(int num,Graph g){
    //malloc and initialize
    double **dist = malloc(sizeof(double*)*num);
    fill_dist_negative(dist,num);
    int i = 0; // vertex i
    //loop to every vertex
    while(i<num){
        AdjList out = GraphOutIncident(g, i);
        while(out != NULL){
            //fill dist
            dist[i][out->v]=1.0/(out->weight);
            //if both >0 and ..... or one larger than 0 one smaller than 0
            if((dist[i][out->v] < dist[out->v][i] && dist[i][out->v]>0)||(dist[i][out->v]>0 &&dist[out->v][i]<0)){
                //update symmetric item
                dist[out->v][i] = dist[i][out->v];
            }
            out = out->next;
        }
        i++;
    }
    return dist;
}


//if full return 0;not full return 1;
//full means the dist array only has one non_null child node
int check_full(DNode** d_array,int num){
    int i = 0;
    int count =0;
    while(i<num){
        if(d_array[i]->left != NULL){
            count++;
        }
        i++;
    }
    if(count == 1){
        return 0;
    }else{
        return 1;
    }
}

//fill the double array be zero
void fill_zero(double *seen,int num,int value){
    int i = 0;
    while(i<num){
        seen[i]=value;
        i++;
    }
}
    
//malloc a new DNode*
Dendrogram new_D(int i){
    Dendrogram new = malloc(sizeof(DNode));
    new->vertex = i;
    new->left =NULL;
    new->right =NULL;
    return new;
}

//free the Dendrogram and related memory(recursive);
void freeDendrogram(Dendrogram d){
    //basecase
    if(d == NULL){
        return;
    }
    if(d->left == NULL){
        free(d);
        return;
    }
    freeDendrogram(d->left);
    freeDendrogram(d->right);    
}
