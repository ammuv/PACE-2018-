#include<iostream>
using namespace std;
#include<bits/stdc++.h>
#include<algorithm>
#include<limits.h>
#include<math.h>
#define MAX_LINE_SIZE 1024
#define MAX_SECTION_SIZE 256
char *inputfile=new char[17];
char *outputfile;
FILE *file1;
class tree{
    public:
        long n;
        long e;
        set < pair<long, long> > edges;
        /*tree(long n){
             this->n=n;
        }*/
        void tree_add_edge(long u,long v){
            if(v<u){
                long t=u;
                u=v;
                v=t;
            }
            pair<long,long> p = make_pair(u,v);
            edges.insert(p);
            e++;
        }
        void display(){
                for (set<pair<long,long> >::iterator it=edges.begin(); it!=edges.end(); ++it)
                    fprintf(stdout, "%ld %ld\n", (*it).first+1,(*it).second+1);
                fflush(stdout);
        }
}steinerTree;
class graph{
    public:
        long n;
        long e;
        long t;
        vector <long> terminals;
        vector < pair < pair<long, long>, long> > edges;
        vector < vector<long> > adj_matrix;
        vector < vector<long> > dist;
        vector < vector<long> > next;
        vector < vector< vector<long> > >path;
        void graph_add_edge(long u,long v, long w){
            pair<long,long> p = make_pair(u,v);
            pair < pair<long, long>, long> ed= make_pair(p,w);
            edges.push_back(ed);
        }
        void generate_adj_matrix(){
            for(long i=0;i<n;i++){
                vector<long> t(n,LONG_MAX);
                adj_matrix.push_back(t);
                adj_matrix[i][i]=0;
            }
            for(long i=0;i<e;i++){
                adj_matrix[edges[i].first.first][edges[i].first.second]=edges[i].second;
                adj_matrix[edges[i].first.second][edges[i].first.first]=edges[i].second;
            }
        }
        void display(){
                fprintf(stdout, "input: n = %ld, e = %ld, t = %ld ",n, e, t);
                fprintf(stdout, "\n");

                fprintf(stdout, "terminals:");
                for(long i=0; i<t; i++)
                    fprintf(stdout, " %ld", terminals[i]+1);
                fprintf(stdout, "\n");
                fflush(stdout);

                fprintf(stdout, "\n");
                fprintf(stdout, "Edges:");
                for(long i=0; i<e; i++)
                    fprintf(stdout, "E(u,v,w): %ld %ld %ld \n", edges[i].first.first+1,edges[i].first.second+1,edges[i].second);
                fflush(stdout);
        }
        void floydWarshall ()
        {
            long i, j, k;
            for(long i=0;i<n;i++){
                vector<long> t(n);
                vector<long> ne(n,-1);
                vector<vector<long> > p(n);
                dist.push_back(t);
                next.push_back(ne);
                path.push_back(p);
            }
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++){
                    dist[i][j] = adj_matrix[i][j];
                    if(dist[i][j]<LONG_MAX){
                            next[i][j]=j;
                    }
                }

            for (k = 0; k < n; k++)
            {
                for (i = 0; i < n; i++)
                {
                    for (j = 0; j < n; j++)
                    {
                        if (dist[i][k]!=LONG_MAX && dist[k][j]!=LONG_MAX)
                            if(dist[i][k] + dist[k][j] < dist[i][j]){
                                dist[i][j] = dist[i][k] + dist[k][j];
                                next[i][j] = next[i][k];
                            }
                    }
                }
            }
            //Shortest Path
            for(i=0;i<n;i++)
                for(j=0;j<n;j++){
                    Find_path(i,j);
                }
        }
        void Find_path(long u,long v){
            if(next[u][v]==-1)
                return;
            long i=u,j=v;
            path[i][j].push_back(u);
            while(u!=v){
                u=next[u][v];
                path[i][j].push_back(u);
            }
        }
        void printdist()
        {
            long i,j,k;
            printf ("\n\nFollowing matrix shows the shortest distances"
                    " between every pair of vertices \n");
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    if (dist[i][j] == LONG_MAX)
                        cout<<"INF";
                    else{
                        cout<<dist[i][j]<<" ";
                        for(k=0;k<path[i][j].size();k++){
                            cout<<path[i][j][k]<<"-";
                        }
                    }
                    cout<<"\t";
                }
                printf("\n");
            }
        }
        void printPath(long u,long v){
            for(long k=0;k<path[u][v].size()-1;k++)
                steinerTree.tree_add_edge(path[u][v][k],path[u][v][k+1]);
                //fprintf(stdout, "%ld %ld\n",path[u][v][k]+1,path[u][v][k+1]+1);
        }
};
graph* graph_load()
{
   // push_time();
    char buf[MAX_LINE_SIZE];
    char line[MAX_LINE_SIZE];
    long n = 0;
    long e = 0;
    long t = 0;
    long u, v, w;
    char section[MAX_SECTION_SIZE];

    int in_section = 0;
    graph *g = new graph();
    while(fgets(line, MAX_LINE_SIZE,stdin) != NULL)
    {
        strcpy(buf, line);
        char *c = strtok(buf," ");
        //cout<<c;
        if(!strcmp(c, "SECTION"))
        {
            if(in_section == 1) { printf("nested sections");}
            in_section = 1;

            if(sscanf(line, "SECTION %s", section) != 1)
                printf("invalid section line %s",section);

            if(!strcmp(section, "Graph"))
                continue;
            else if(!strcmp(section, "Terminals"))
                continue;
            else
                printf("invalid section");
        }
        else if(!strcmp(c, "END\n"))
        {
            if(in_section == 0) { printf("no section to end");}
            in_section = 0;
        }
        else if(!strcmp(c, "Nodes"))
        {
            if(sscanf(line, "Nodes %ld", &n) != 1)
                printf("invalid nodes line");
            g->n = n;
        }
        else if(!strcmp(c, "Edges"))
        {
            if(sscanf(line, "Edges %ld", &e) != 1)
                printf("invalid edges line");
            g->e = e;
        }
        else if(!strcmp(c, "Terminals"))
        {
            if(sscanf(line, "Terminals %ld", &t) != 1)
                printf("invalid terminals line");
            g->t = t;
        }
        else if(!strcmp(c, "E"))
        {
            if(sscanf(line, "E %ld %ld %ld", &u, &v, &w) != 3)
                printf("invalid edge line %s", line);
            g->graph_add_edge(u-1, v-1, w);
        }
        else if(!strcmp(c, "T"))
        {
            //cout<<"T";
            if(sscanf(line, "T %ld", &u) != 1)
                printf("invalid terminal line %s", line);
            g->terminals.push_back(u-1);
        }
        else if(!strcmp(c, "EOF"))
        {
            //cout<<"EOF";
            break;
        }
        else
        {
            continue;
        }
    }
    //cout<<"l";
    g->generate_adj_matrix();
    g->floydWarshall();
    //g->display();
    return g;
}
struct SteinerMin{
    long wt,minu,min_ind1,min_ind2;
};
void trace(vector< vector <SteinerMin> > ST,long index,long v, graph *g, vector<long> terminals){
        long u;
        if(!(index&(index-1))){
                u=ST[index][v].minu;
                g->printPath(u,v);
                return;
        }
        g->printPath(v,ST[index][v].minu);
        trace(ST,ST[index][v].min_ind1,ST[index][v].minu,g,terminals);
        trace(ST,ST[index][v].min_ind2,ST[index][v].minu,g,terminals);
}
void subsetUtil(vector< vector <SteinerMin> > &ST, vector<long> terminals, long r, vector<long> t1, long terminal_ind,graph *g,long tprime){
    if(t1.size()==r){
        long index=0;
        for(long k=0;k<r;k++){
            index+=(1<<t1[k]);
            //cout<<terminals[t1[k]]+1<<" ";
        }
        //cout<<"="<<index<<"\n";
        long min1,min2,min_ind1,min_ind2,sum,u,v,k,i,t1index,t2index,total=(1<<r);
        set<pair<long,long> >::iterator it;
        for(v=0;v<g->n;v++){
            if(r==g->t)
                v=tprime;
            ST[index][v].wt=LONG_MAX;
            for(u=0;u<g->n;u++){
                min1=g->dist[v][u];
                min2=LONG_MAX;
                for(i=1;i<total-1;i++){
                    t1index=0;
                    t2index=0;
                    for(k=0;k<r;k++){
                        if(i&(1<<k)){
                            t1index+=(1<<t1[k]);
                        }
                        else{
                            t2index+=(1<<t1[k]);
                        }
                    }
                    //ST[index][v]=min(dist(v,u)+min(ST[X' U u])+ST[X'' U u]))
                    sum=ST[t1index][u].wt+ST[t2index][u].wt;
                    if(min2>sum){
                        min2=sum;
                        min_ind1=t1index;
                        min_ind2=t2index;
                    }
                    //if(r==g->t)
                      //  cout<<"u="<<u<<","<<t1index<<"="<<ST[t1index][u]<<","<<t2index<<"="<<ST[t2index][u]<<","<<sum<<", min="<<min2<<"\n";
                }
                min1+=min2;
                if(min1<ST[index][v].wt){
                    ST[index][v].wt=min1;
                    ST[index][v].minu=u;
                    ST[index][v].min_ind1=t1index;
                    ST[index][v].min_ind2=t2index;
                    //ST[index][v].second.e=ST[min_ind1][minu].second.e+ST[min_ind2][minu].second.e+g->path[minu][v].size()-1;
                }
            }
            if(r==g->t){
               /* cout<<"\nMinimum Steiner Tree is:";
                ST[index][v].second.display();
                cout<<"Weight of Tree is: "<<ST[index][v].first;
                */
                fprintf(stdout,"VALUE %ld\n",ST[index][v].wt);
                steinerTree.n=g->n;
                trace(ST,index,v,g,terminals);
                steinerTree.display();
                break;
            }
        }
        //cout<<"\n";
        return;
    }
    if(terminal_ind>=terminals.size())
        return;
    t1.push_back(terminal_ind);//terminal_index included in t1
    subsetUtil(ST,terminals,r,t1,terminal_ind+1,g,tprime);
    t1.pop_back();//terminal_index is not included in t1
    subsetUtil(ST,terminals,r,t1,terminal_ind+1,g,tprime);
}
void getAllrSubsets(vector< vector <SteinerMin> >  &ST, vector<long>terminals,long r,graph *g,long tprime){
    vector<long> t1;
    subsetUtil(ST,terminals,r,t1,0,g,tprime);
}
void Dreyfus_Wagner(graph *g){
//1. Choose t' in Terminals
//2. T'=Terminals \ t'
//For all X \strict subset of T'
//3. ST[X U v], for all v \in V
//4. ST[X U v]=min(dist(v,u)+min(ST[X' U u])+ST[X'' U u])), for all u \in V and for all  X' \strict subset of X.
//5. Ans ST[T' U t']
//Base case:
//    |X|=1, ST[{x}U{v}] = shortest path between {x} and {v}
    long tprime=g->terminals[g->t-1];
    long u,index,v,i,k,siz=1<<g->t;
    vector< vector <SteinerMin> >ST(siz, vector< SteinerMin > (g->n));
    g->terminals.pop_back();
    g->t--;
    //Base case
    for(i=0;i<g->t;i++){
        index=1<<i;
        u=g->terminals[i];
        for(v=0;v<g->n;v++){
           // cout<<"\nu:"<<u<<", v:"<<v<<"\t";
            ST[index][v].wt=g->dist[u][v];
            ST[index][v].minu=u;
            /*ST[index][v].second.e=g->path[u][v].size()-1;
            for(k=0;k<g->path[u][v].size()-1;k++){
                ST[index][v].second.tree_add_edge(g->path[u][v][k],g->path[u][v][k+1]);
            }*/
            //ST[index][v].second.display();
        }
    }
    for(long X_size=2;X_size<=g->t;X_size++){
        getAllrSubsets(ST,g->terminals,X_size,g,tprime);
    }
}
int main(int argc, char** argv){
        /*int i;
        outputfile=argv[3];
        file1=fopen(outputfile,"w");
        for(i=0;i<11;i++)
           inputfile[i]=argv[3][i];
        strcat(inputfile,".gr");
        FILE *file=fopen(inputfile,"r");
        //cout<<file;
        //cout<<inputfile;
        */graph *g=graph_load();
        //g->display();
        Dreyfus_Wagner(g);
        //cout<<"over";
        //int k;
        //cin>>k;
        return 0;
}

