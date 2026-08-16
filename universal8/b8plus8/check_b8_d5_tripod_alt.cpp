#include <bits/stdc++.h>
using namespace std;using U=uint64_t;
static void add(vector<U>&a,int u,int v){a[u]|=1ULL<<v;a[v]|=1ULL<<u;}static void del(vector<U>&a,int u,int v){a[u]&=~(1ULL<<v);a[v]&=~(1ULL<<u);}
static bool exact_path(const vector<U>&a,int s,int t,int L){struct S{int v,d;U used;};vector<S>st{{s,0,1ULL<<s}};while(!st.empty()){auto q=st.back();st.pop_back();if(q.d==L){if(q.v==t)return true;continue;}U c=a[q.v]&~q.used;if(q.d<L-1)c&=~(1ULL<<t);while(c){U b=c&-c;c-=b;int w=__builtin_ctzll(b);st.push_back({w,q.d+1,q.used|b});}}return false;}
static bool edge_bad(vector<U>&g,int u,int v){del(g,u,v);bool x=exact_path(g,u,v,3)||exact_path(g,u,v,7)||exact_path(g,u,v,15);add(g,u,v);return x;}
static bool add_safe(vector<U>&g,int u,int v){add(g,u,v);if(edge_bad(g,u,v)){del(g,u,v);return false;}return true;}
int main(){
 vector<vector<pair<int,int>>> cores={
 {{0,1},{0,2},{0,4},{0,5},{1,2},{1,7},{2,3},{3,6},{4,5},{5,6},{6,7}},
 {{0,1},{0,4},{0,5},{1,2},{1,4},{1,7},{2,6},{3,5},{3,6},{3,7},{5,6}}
 };
 for(int ci=0;ci<2;ci++){auto ed=cores[ci];int B=8,EP=11,base=19;vector<int>deg(B);for(auto [u,v]:ed)deg[u]++,deg[v]++;vector<int>cap(B);for(int v=0;v<B;v++)cap[v]=4-deg[v];
  long long safePath=0,safeTrip=0,candTrip=0;vector<U>g(base+3);for(int e=0;e<EP;e++){add(g,B+e,ed[e].first);add(g,B+e,ed[e].second);} 
  // verify base has no forbidden cycles via every edge alternate path
  bool basebad=false;for(int u=0;u<base;u++){U c=g[u];while(c){U bit=c&-c;c-=bit;int v=__builtin_ctzll(bit);if(u<v&&edge_bad(g,u,v))basebad=true;}}
  for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++)for(int p=0;p<B;p++)for(int q=0;q<B;q++){
    if((p==q?2:1)>cap[p]|| (p!=q&&cap[q]<1))continue;auto h=g;int a=base,b=base+1;
    if(!add_safe(h,B+i,a))continue;if(!add_safe(h,a,p))continue;if(!add_safe(h,a,b))continue;if(!add_safe(h,b,q))continue;if(!add_safe(h,b,B+j))continue;safePath++;
  }
  for(int e0=0;e0<EP;e0++)for(int e1=e0+1;e1<EP;e1++)for(int e2=e1+1;e2<EP;e2++){int es[3]={e0,e1,e2};for(int lp=0;lp<3;lp++)for(int p=0;p<B;p++)for(int q=0;q<B;q++){
    if((p==q?2:1)>cap[p]|| (p!=q&&cap[q]<1))continue;candTrip++;auto h=g;int a=base,b=base+1,c=base+2;int le=es[lp];bool ok=true;
    ok&=add_safe(h,B+le,a);if(!ok)continue;ok&=add_safe(h,a,p);if(!ok)continue;ok&=add_safe(h,a,b);if(!ok)continue;ok&=add_safe(h,b,q);if(!ok)continue;ok&=add_safe(h,b,c);if(!ok)continue;
    for(int z=0;z<3;z++)if(z!=lp){ok&=add_safe(h,B+es[z],c);if(!ok)break;}
    if(ok)safeTrip++;
  }}
  cout<<"core "<<ci<<" basebad="<<basebad<<" safePathOrdered="<<safePath<<" candTrip="<<candTrip<<" safeTripOrdered="<<safeTrip<<"\n";
 }
}
