#include <bits/stdc++.h>
using namespace std; using U=uint64_t;
struct Case{int B,E,sp,idx;string core;vector<pair<int,int>>ed;};
static void add(vector<U>&a,int u,int v){a[u]|=1ULL<<v;a[v]|=1ULL<<u;}static void del(vector<U>&a,int u,int v){a[u]&=~(1ULL<<v);a[v]&=~(1ULL<<u);}
static bool cycdfs(const vector<U>&a,int s,int v,int L,int dep,U used,int first){if(dep==L-1)return ((a[v]>>s)&1ULL)&&first<v;U c=a[v]&~used;while(c){U b=c&-c;c-=b;int w=__builtin_ctzll(b);if(cycdfs(a,s,w,L,dep+1,used|b,first))return true;}return false;}
static bool cycle_through(const vector<U>&a,int s,int L){U c=a[s];while(c){U b=c&-c;c-=b;int f=__builtin_ctzll(b);if(cycdfs(a,s,f,L,1,(1ULL<<s)|b,f))return true;}return false;}
static bool bad(const vector<U>&a,int s){return cycle_through(a,s,4)||cycle_through(a,s,8)||cycle_through(a,s,16);} 
static vector<Case> readcases(const string&p){ifstream in(p);string l;getline(in,l);vector<Case>o;int idx=0;while(getline(in,l)){if(l=="END")break;if(l.empty())continue;istringstream ss(l);string t;Case c;ss>>t>>c.B>>c.E>>c.core>>c.sp;getline(in,l);istringstream es(l);es>>t;string x;while(es>>x){auto z=x.find(',');c.ed.push_back({stoi(x.substr(0,z)),stoi(x.substr(z+1))});}getline(in,l);c.idx=idx++;o.push_back(c);}return o;}

struct Opt{int i,j; array<uint8_t,8> cnt;};
static uint32_t packcnt(const array<uint8_t,8>&c){uint32_t x=0;for(int i=0;i<8;i++)x|=(uint32_t)c[i]<<(3*i);return x;}
struct KeyState{uint16_t mask; uint32_t rem; bool operator==(KeyState const&o)const{return mask==o.mask&&rem==o.rem;}};
struct H{size_t operator()(KeyState const&s)const{return ((uint64_t)s.mask<<32)^s.rem;}};

int main(int argc,char**argv){auto cs=readcases(argv[1]);set<int> survivors; if(argc>=3){ifstream f(argv[2]);int x;while(f>>x)survivors.insert(x);} long long totalopts=0,totalstates=0;int sat=0,unsat=0;
 for(auto&C:cs){if(!survivors.empty()&&!survivors.count(C.idx))continue;int B=C.B,EP=C.ed.size(),d=2*B-EP,y=2*d+C.E,base=B+EP;vector<int>deg(B);for(auto [u,v]:C.ed)deg[u]++,deg[v]++;array<int,8> cap{};for(int v=0;v<B;v++)cap[v]=4+((C.E&&v==C.sp)?1:0)-deg[v];
  vector<U>g(base+y);for(int e=0;e<EP;e++){add(g,B+e,C.ed[e].first);add(g,B+e,C.ed[e].second);} vector<vector<vector<array<uint8_t,8>>>> pc(EP,vector<vector<array<uint8_t,8>>>(EP));
  // r=0 direct path options
  for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++){int st=B+i,tg=B+j;add(g,st,tg);bool ok=!bad(g,st);del(g,st,tg);if(ok){array<uint8_t,8> z{};pc[i][j].push_back(z);}}
  vector<int>rem(B);for(int v=0;v<B;v++)rem[v]=cap[v];array<uint8_t,8> cnt{};vector<int>word;
  for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++){int st=B+i,tg=B+j;set<uint32_t> seen;
   function<void(int)> rec=[&](int dep){if(dep>=y)return;int prev=dep?base+dep-1:st,node=base+dep;for(int lab=0;lab<B;lab++)if(rem[lab]){add(g,node,lab);add(g,prev,node);rem[lab]--;cnt[lab]++;if(!bad(g,node)){add(g,node,tg);bool ok=!bad(g,node);del(g,node,tg);if(ok){auto k=packcnt(cnt);if(seen.insert(k).second)pc[i][j].push_back(cnt);}rec(dep+1);}cnt[lab]--;rem[lab]++;del(g,prev,node);del(g,node,lab);}};rec(0);
  }
  long long opts=0;for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++)opts+=pc[i][j].size();totalopts+=opts;
  // encode rem quotas 3 bits each
  auto packrem=[&](array<int,8> const&r){uint32_t x=0;for(int v=0;v<8;v++)x|=(uint32_t)r[v]<<(3*v);return x;};
  array<int,8> init{};for(int v=0;v<8;v++)init[v]=cap[v];uint32_t initrem=packrem(init);
  unordered_set<KeyState,H> dead; long long states=0;
  function<bool(uint16_t,uint32_t)> dfs=[&](uint16_t mask,uint32_t remp)->bool{states++; if(mask==(1u<<EP)-1){return remp==0;}KeyState ks{mask,remp};if(dead.count(ks))return false;int i=0;while(mask>>i&1)i++;for(int j=i+1;j<EP;j++)if(!(mask>>j&1)){
    for(auto &c:pc[i][j]){uint32_t nr=remp;bool ok=1;for(int v=0;v<8;v++){int rv=(nr>>(3*v))&7;if(c[v]>rv){ok=0;break;}nr-=((uint32_t)c[v])<<(3*v);}if(ok&&dfs(mask|(1u<<i)|(1u<<j),nr))return true;}
   }dead.insert(ks);return false;};
  bool yes=dfs(0,initrem);totalstates+=states;if(yes){sat++;cout<<"SAT idx="<<C.idx<<" "<<C.core<<" E="<<C.E<<" sp="<<C.sp<<" opts="<<opts<<" states="<<states<<"\n";}else{unsat++;cout<<"UNSAT idx="<<C.idx<<" "<<C.core<<" E="<<C.E<<" sp="<<C.sp<<" opts="<<opts<<" states="<<states<<"\n";}
 }
 cerr<<"SUMMARY sat="<<sat<<" unsat="<<unsat<<" totalopts="<<totalopts<<" totalstates="<<totalstates<<"\n";
}
