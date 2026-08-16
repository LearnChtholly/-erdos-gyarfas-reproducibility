#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std; using U=uint64_t;
struct Case{int B,E,sp,idx;string core;vector<pair<int,int>>ed;};
static void add(vector<U>&a,int u,int v){a[u]|=1ULL<<v;a[v]|=1ULL<<u;}static void del(vector<U>&a,int u,int v){a[u]&=~(1ULL<<v);a[v]&=~(1ULL<<u);}
static bool cycle_dfs(const vector<U>&a,int s,int v,int L,int dep,U used,int first){if(dep==L-1)return ((a[v]>>s)&1ULL)&&first<v;U c=a[v]&~used;while(c){U b=c&-c;c-=b;int w=__builtin_ctzll(b);if(cycle_dfs(a,s,w,L,dep+1,used|b,first))return true;}return false;}
static bool cycle_through(const vector<U>&a,int s,int L){U c=a[s];while(c){U b=c&-c;c-=b;int f=__builtin_ctzll(b);if(cycle_dfs(a,s,f,L,1,(1ULL<<s)|b,f))return true;}return false;}
static bool forbidden_through(const vector<U>&a,int s){return cycle_through(a,s,4)||cycle_through(a,s,8)||cycle_through(a,s,16);}static bool has_cycle_len(const vector<U>&a,int L){for(int s=0;s<(int)a.size();s++)if(cycle_through(a,s,L))return true;return false;}
static bool two_deg(int n,const vector<pair<int,int>>&ed){vector<U>a(n);for(auto [u,v]:ed)add(a,u,v);U alive=(1ULL<<n)-1;vector<int>d(n);for(int i=0;i<n;i++)d[i]=__builtin_popcountll(a[i]);for(int k=0;k<n;k++){int v=-1;for(int i=0;i<n;i++)if((alive>>i&1ULL)&&d[i]<=2){v=i;break;}if(v<0)return false;alive&=~(1ULL<<v);U c=a[v]&alive;while(c){U b=c&-c;c-=b;--d[__builtin_ctzll(b)];}}return true;}
static vector<Case>readcases(const string&p){ifstream in(p);string l;if(!getline(in,l)||l!="ENDPOINT_CASES_V1")throw runtime_error("head");vector<Case>o;int idx=0;while(getline(in,l)){if(l=="END")break;if(l.empty())continue;istringstream ss(l);string tag;Case c;ss>>tag>>c.B>>c.E>>c.core>>c.sp;getline(in,l);istringstream es(l);es>>tag;string x;while(es>>x){auto z=x.find(',');c.ed.push_back({stoi(x.substr(0,z)),stoi(x.substr(z+1))});}getline(in,l);c.idx=idx++;o.push_back(move(c));}return o;}
static int weighted_matching(const vector<vector<int>>&M){int n=M.size();vector<int>act;for(int i=0;i<n;i++){bool on=0;for(int j=0;j<n;j++)if(M[i][j]){on=1;break;}if(on)act.push_back(i);}int k=act.size();if(!k)return 0;unordered_map<U,int>memo;function<int(U)>dp=[&](U mask){if(!mask)return 0;auto it=memo.find(mask);if(it!=memo.end())return it->second;int a=__builtin_ctzll(mask);U rest=mask&~(1ULL<<a);int best=dp(rest);U c=rest;while(c){U bit=c&-c;c-=bit;int b=__builtin_ctzll(bit),w=M[act[a]][act[b]];if(w)best=max(best,w+dp(rest&~bit));}memo[mask]=best;return best;};return dp((1ULL<<k)-1);}
struct Res{bool ok=1;string why;vector<vector<int>>M;long long states=0,att=0,close=0;};
static Res calc(const Case&C){Res R;int B=C.B,EP=C.ed.size(),d=2*B-EP,y=2*d+C.E,base=B+EP;if(d<0){R.ok=0;R.why="d";return R;}vector<int>deg(B);set<pair<int,int>>seen;for(auto [u0,v0]:C.ed){int u=u0,v=v0;if(u>v)swap(u,v);if(u==v||!seen.insert({u,v}).second){R.ok=0;R.why="simple";return R;}deg[u]++;deg[v]++;}if(!two_deg(B,C.ed)){R.ok=0;R.why="2deg";return R;}vector<U>q(B);for(auto e:C.ed)add(q,e.first,e.second);if(has_cycle_len(q,4)||has_cycle_len(q,8)){R.ok=0;R.why="QC";return R;}vector<int>cap(B);for(int v=0;v<B;v++){cap[v]=4+((C.E&&v==C.sp)?1:0)-deg[v];if(cap[v]<0){R.ok=0;R.why="cap";return R;}}if(accumulate(cap.begin(),cap.end(),0)!=y){R.ok=0;R.why="capsum";return R;}
 int nmax=base+y;vector<U>g(nmax);for(int e=0;e<EP;e++){add(g,B+e,C.ed[e].first);add(g,B+e,C.ed[e].second);}R.M.assign(EP,vector<int>(EP));vector<int>rem=cap;
 for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++){int st=B+i,tg=B+j;function<void(int)>rec=[&](int dep){R.states++;if(dep>=y)return;int prev=dep?base+dep-1:st,node=base+dep;for(int lab=0;lab<B;lab++)if(rem[lab]){R.att++;add(g,node,lab);add(g,prev,node);rem[lab]--;if(!forbidden_through(g,node)){add(g,node,tg);R.close++;if(!forbidden_through(g,node))R.M[i][j]=R.M[j][i]=max(R.M[i][j],dep+1);del(g,node,tg);rec(dep+1);}rem[lab]++;del(g,prev,node);del(g,node,lab);}};rec(0);}return R;}
int main(int argc,char**argv){if(argc<2)return 2;auto cs=readcases(argv[1]);int lo=0,hi=cs.size();if(argc>=4){lo=stoi(argv[2]);hi=min((int)cs.size(),stoi(argv[3]));}map<pair<int,int>,int>cnt,mx;map<pair<int,int>,string>arg;long long S=0,A=0,C=0;for(int z=lo;z<hi;z++){auto r=calc(cs[z]);if(!r.ok){cerr<<"FAIL "<<z<<" "<<r.why<<"\n";return 10;}int w=weighted_matching(r.M),d=2*cs[z].B-(int)cs[z].ed.size(),y=2*d+cs[z].E;if(w>=y){cerr<<"WEIGHTFAIL "<<z<<" "<<cs[z].core<<" sp="<<cs[z].sp<<" w="<<w<<" y="<<y<<"\n";return 11;}auto sec=make_pair(cs[z].B,cs[z].E);cnt[sec]++;if(!mx.count(sec)||w>mx[sec]){mx[sec]=w;arg[sec]=cs[z].core+":"+to_string(cs[z].sp);}S+=r.states;A+=r.att;C+=r.close;}cout<<"PASS "<<lo<<" "<<hi<<" states="<<S<<" attempts="<<A<<" closures="<<C<<"\n";for(auto &[s,n]:cnt)cout<<"SECTOR "<<s.first<<","<<s.second<<" cases="<<n<<" maxW="<<mx[s]<<" arg="<<arg[s]<<"\n";}
