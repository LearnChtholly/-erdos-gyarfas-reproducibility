#include <bits/stdc++.h>
using namespace std;
struct E{int u,v;};
long long pairings=0,assign8=0,assign9=0,valid8=0,valid9=0;
vector<int> stubs={0,1,2,3,4,5,6,6,6,7,7,7};
vector<E> edges;
bool check(const vector<int>&k,int y){
 int n=8+y;vector<set<int>>adj(n);int nxt=8;set<pair<int,int>> direct;
 auto ae=[&](int u,int v)->bool{if(u==v)return false;if(adj[u].count(v))return false;adj[u].insert(v);adj[v].insert(u);return true;};
 vector<int> ynodes;
 for(int i=0;i<6;i++){int u=edges[i].u,v=edges[i].v,len=k[i];if(len==0){if(!ae(u,v))return false;}else{int prev=u;for(int j=0;j<len;j++){int w=nxt++;ynodes.push_back(w);if(!ae(prev,w))return false;prev=w;}if(!ae(prev,v))return false;}}
 if(nxt!=n)return false;
 // no two y vertices may share a common neighbor (simple distance-2 path)
 for(int i=0;i<(int)ynodes.size();i++)for(int j=i+1;j<(int)ynodes.size();j++){
   int a=ynodes[i],b=ynodes[j];for(int w:adj[a])if(adj[b].count(w))return false;
 }
 return true;
}
void assignk(int pos,int left,vector<int>&k,int target){if(pos==6){if(left)return;if(target==8)assign8++;else assign9++;if(check(k,target)){if(target==8)valid8++;else valid9++;cout<<"VALID y="<<target<<" edges";for(auto e:edges)cout<<" ("<<e.u<<","<<e.v<<")";cout<<" k";for(int x:k)cout<<" "<<x;cout<<"\n";}return;}for(int x=0;x<=2&&x<=left;x++){k[pos]=x;assignk(pos+1,left-x,k,target);}}
void pairrec(vector<int>&left){if(left.empty()){pairings++;vector<int>k(6);assignk(0,8,k,8);assignk(0,9,k,9);return;}int a=left[0];for(int i=1;i<(int)left.size();i++){int b=left[i];vector<int>n;for(int j=1;j<(int)left.size();j++)if(j!=i)n.push_back(left[j]);edges.push_back({a,b});pairrec(n);edges.pop_back();}}
int main(){auto x=stubs;pairrec(x);cerr<<"pairings="<<pairings<<" assign8="<<assign8<<" valid8="<<valid8<<" assign9="<<assign9<<" valid9="<<valid9<<"\n";}
