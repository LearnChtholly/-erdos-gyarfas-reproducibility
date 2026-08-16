#include <bits/stdc++.h>
using namespace std; using U=uint64_t;
struct Case{int B,E,sp,idx; string core; vector<pair<int,int>> ed;};
static bool exact_path(const vector<U>&a,int s,int t,int L){
 struct S{int v,d;U used;}; vector<S> st; st.reserve(512); st.push_back({s,0,1ULL<<s});
 while(!st.empty()){auto q=st.back();st.pop_back();if(q.d==L){if(q.v==t)return true;continue;}U c=a[q.v]&~q.used;if(q.d<L-1)c&=~(1ULL<<t);while(c){U b=c&-c;c-=b;int w=__builtin_ctzll(b);st.push_back({w,q.d+1,q.used|b});}}
 return false;
}
static bool edge_bad(const vector<U>&a,int u,int v){return exact_path(a,u,v,3)||exact_path(a,u,v,7)||exact_path(a,u,v,15);} 
static bool two_deg(vector<U>a){int n=a.size();U alive=(1ULL<<n)-1;vector<int>d(n);for(int i=0;i<n;i++)d[i]=__builtin_popcountll(a[i]);for(int s=0;s<n;s++){int v=-1;for(int i=0;i<n;i++)if((alive>>i&1ULL)&&d[i]<=2){v=i;break;}if(v<0)return false;alive&=~(1ULL<<v);U c=a[v]&alive;while(c){U b=c&-c;c-=b;--d[__builtin_ctzll(b)];}}return true;}
static vector<Case> readcases(const string&p){ifstream in(p);string l;if(!getline(in,l)||l!="ENDPOINT_CASES_V1")throw runtime_error("header");vector<Case>o;int idx=0;while(getline(in,l)){if(l=="END")break;if(l.empty())continue;istringstream ss(l);string tag;Case c;ss>>tag>>c.B>>c.E>>c.core>>c.sp;if(tag!="CASE")throw runtime_error("CASE");getline(in,l);istringstream es(l);es>>tag;string x;while(es>>x){auto z=x.find(',');c.ed.push_back({stoi(x.substr(0,z)),stoi(x.substr(z+1))});}getline(in,l);c.idx=idx++;o.push_back(move(c));}return o;}

// exact maximum matching by memoized subset recursion, after restricting to active vertices.
static int matching_number(const vector<vector<int>>& M,int lev){
 int n=M.size(); vector<int> act;for(int i=0;i<n;i++){bool on=false;for(int j=0;j<n;j++)if(M[i][j]>=lev){on=true;break;}if(on)act.push_back(i);}int k=act.size();if(!k)return 0;if(k>22)throw runtime_error("too many active");
 vector<U> adj(k);for(int a=0;a<k;a++)for(int b=a+1;b<k;b++)if(M[act[a]][act[b]]>=lev){adj[a]|=1ULL<<b;adj[b]|=1ULL<<a;}
 unordered_map<U,int> memo; memo.reserve(1u<<min(k,18));
 function<int(U)> dp=[&](U mask)->int{if(!mask)return 0;auto it=memo.find(mask);if(it!=memo.end())return it->second;int i=__builtin_ctzll(mask);U rest=mask&~(1ULL<<i);int best=dp(rest);U c=adj[i]&rest;while(c){U b=c&-c;c-=b;best=max(best,1+dp(rest&~b));}memo.emplace(mask,best);return best;};
 return dp((1ULL<<k)-1);
}


static int weighted_matching_value(const vector<vector<int>>& M){
 int n=M.size(); vector<int> act;for(int i=0;i<n;i++){bool on=false;for(int j=0;j<n;j++)if(M[i][j]>0){on=true;break;}if(on)act.push_back(i);}int k=act.size();if(!k)return 0;
 unordered_map<U,int> memo;memo.reserve(1u<<min(k,18));
 function<int(U)> dp=[&](U mask)->int{if(!mask)return 0;auto it=memo.find(mask);if(it!=memo.end())return it->second;int a=__builtin_ctzll(mask);U rest=mask&~(1ULL<<a);int best=dp(rest);U c=rest;while(c){U bit=c&-c;c-=bit;int b=__builtin_ctzll(bit);int w=M[act[a]][act[b]];if(w)best=max(best,w+dp(rest&~bit));}memo.emplace(mask,best);return best;};
 return dp((1ULL<<k)-1);
}

struct Res{bool ok=true;string why;vector<vector<int>> pm;long long states=0,attempts=0;};
static Res pairmax_case(const Case&C){Res R;int B=C.B,EP=C.ed.size(),d=2*B-EP,y=2*d+C.E,base=B+EP;if(d<0){R.ok=false;R.why="edgecount";return R;}vector<int>deg(B);set<pair<int,int>>seen;vector<U>q(B);for(auto [u0,v0]:C.ed){int u=u0,v=v0;if(u>v)swap(u,v);if(u==v||!seen.insert({u,v}).second){R.ok=false;R.why="Qsimple";return R;}deg[u]++;deg[v]++;q[u]|=1ULL<<v;q[v]|=1ULL<<u;}if(!two_deg(q)){R.ok=false;R.why="2deg";return R;}for(auto [u,v]:seen)if(exact_path(q,u,v,3)||exact_path(q,u,v,7)){R.ok=false;R.why="QC4C8";return R;}vector<int>cap(B);for(int v=0;v<B;v++){cap[v]=4+((C.E&&v==C.sp)?1:0)-deg[v];if(cap[v]<0){R.ok=false;R.why="cap";return R;}}if(accumulate(cap.begin(),cap.end(),0)!=y){R.ok=false;R.why="capsum";return R;}
 vector<U>g(base+y);auto add=[&](int u,int v){g[u]|=1ULL<<v;g[v]|=1ULL<<u;};auto del=[&](int u,int v){g[u]&=~(1ULL<<v);g[v]&=~(1ULL<<u);};for(int e=0;e<EP;e++){add(B+e,C.ed[e].first);add(B+e,C.ed[e].second);}for(int u=0;u<base;u++){U c=g[u]&~((1ULL<<(u+1))-1);while(c){U bit=c&-c;c-=bit;int v=__builtin_ctzll(bit);if(edge_bad(g,u,v)){R.ok=false;R.why="basecycle";return R;}}}
 R.pm.assign(EP,vector<int>(EP));vector<int>rem=cap;for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++){int st=B+i,tg=B+j;function<void(int)>rec=[&](int dep){R.states++;if(dep>=y)return;int prev=dep?base+dep-1:st,node=base+dep;for(int lab=0;lab<B;lab++)if(rem[lab]){R.attempts++;add(node,lab);add(prev,node);rem[lab]--;if(!edge_bad(g,prev,node)){add(node,tg);if(!edge_bad(g,node,tg))R.pm[i][j]=R.pm[j][i]=max(R.pm[i][j],dep+1);del(node,tg);rec(dep+1);}rem[lab]++;del(prev,node);del(node,lab);}};rec(0);}return R;}

int main(int argc,char**argv){if(argc<2){cerr<<"usage cases [lo hi]\n";return 2;}auto cs=readcases(argv[1]);int lo=0,hi=cs.size();if(argc>=4){lo=stoi(argv[2]);hi=min((int)cs.size(),stoi(argv[3]));}map<pair<int,int>,int>cnt,mx,mxw;map<pair<int,int>,string>arg,argw;long long states=0,att=0;for(int z=lo;z<hi;z++){auto &C=cs[z];auto R=pairmax_case(C);if(!R.ok){cerr<<"FAIL "<<z<<" "<<R.why<<"\n";return 10;}int top=0;for(auto &a:R.pm)for(int v:a)top=max(top,v);int s=0;vector<int>nus;for(int lev=1;lev<=top;lev++){int nu=matching_number(R.pm,lev);nus.push_back(nu);s+=nu;}int wmv=weighted_matching_value(R.pm);int d=2*C.B-(int)C.ed.size();int y=2*d+C.E;if(wmv>=y){cerr<<"WEIGHT_REACH idx="<<z<<" "<<C.B<<","<<C.E<<" "<<C.core<<" sp="<<C.sp<<" w="<<wmv<<" y="<<y<<"\n";}auto sec=make_pair(C.B,C.E);cnt[sec]++;if(!mx.count(sec)||s>mx[sec]){mx[sec]=s;arg[sec]=C.core+":"+to_string(C.sp);}if(!mxw.count(sec)||wmv>mxw[sec]){mxw[sec]=wmv;argw[sec]=C.core+":"+to_string(C.sp);}states+=R.states;att+=R.attempts;}
 cout<<"PASS RANGE "<<lo<<" "<<hi<<" states="<<states<<" attempts="<<att<<"\n";for(auto &[sec,n]:cnt)cout<<"SECTOR "<<sec.first<<","<<sec.second<<" cases="<<n<<" max_matchlayers="<<mx[sec]<<" max_weight_matching="<<mxw[sec]<<" argW="<<argw[sec]<<" arg="<<arg[sec]<<"\n";
}
