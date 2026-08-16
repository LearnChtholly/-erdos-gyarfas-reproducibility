#include <bits/stdc++.h>
using namespace std; using U=uint64_t;
struct Case{int B,E,sp; string core; vector<pair<int,int>> ed;};
static bool exact_path(const vector<U>&a,int s,int t,int L){
 struct S{int v,d;U used;}; vector<S> st; st.reserve(512); st.push_back({s,0,1ULL<<s});
 while(!st.empty()){auto q=st.back();st.pop_back(); if(q.d==L){if(q.v==t)return true;continue;} U c=a[q.v]&~q.used; if(q.d<L-1)c&=~(1ULL<<t); while(c){U b=c&-c;c-=b;int w=__builtin_ctzll(b);st.push_back({w,q.d+1,q.used|b});}}
 return false;
}
static bool edge_bad(const vector<U>&a,int u,int v){return exact_path(a,u,v,3)||exact_path(a,u,v,7)||exact_path(a,u,v,15);}
static bool two_deg(vector<U>a){int n=a.size();U alive=(n==64?~0ULL:((1ULL<<n)-1));vector<int>d(n);for(int i=0;i<n;i++)d[i]=__builtin_popcountll(a[i]);for(int step=0;step<n;step++){int v=-1;for(int i=0;i<n;i++)if((alive>>i&1)&&d[i]<=2){v=i;break;}if(v<0)return false;alive&=~(1ULL<<v);U c=a[v]&alive;while(c){U b=c&-c;c-=b;d[__builtin_ctzll(b)]--;}}return true;}
static vector<Case> readcases(const string&path){ifstream in(path); if(!in)throw runtime_error("open");string l;getline(in,l);if(l!="ENDPOINT_CASES_V1")throw runtime_error("header");vector<Case> out;while(getline(in,l)){if(l=="END")break;if(l.empty())continue;istringstream is(l);string tag;Case c;is>>tag>>c.B>>c.E>>c.core>>c.sp;if(tag!="CASE")throw runtime_error("CASE");getline(in,l);istringstream es(l);es>>tag;if(tag!="EDGES")throw runtime_error("EDGES");string x;while(es>>x){auto p=x.find(',');c.ed.push_back({stoi(x.substr(0,p)),stoi(x.substr(p+1))});}getline(in,l);if(l!="ENDCASE")throw runtime_error("ENDCASE");out.push_back(move(c));}return out;}
struct Res{vector<int>a;long long states=0, attempts=0;};
static Res alpha(const Case&C){int B=C.B,EP=C.ed.size(),y=C.E?17:16,base=B+EP;if(EP!=2*B-8)throw runtime_error("edge count");
 vector<int>deg(B);set<pair<int,int>> seen;vector<U>q(B);for(auto [u0,v0]:C.ed){int u=u0,v=v0;if(u==v||u<0||v<0||u>=B||v>=B)throw runtime_error("bad edge");if(u>v)swap(u,v);if(!seen.insert({u,v}).second)throw runtime_error("parallel");deg[u]++;deg[v]++;q[u]|=1ULL<<v;q[v]|=1ULL<<u;}
 if(!two_deg(q))throw runtime_error("Q not 2-degenerate");
 // Independent base relevant-cycle check on Q: C4 and C8 are forbidden.
 for(auto [u,v]:seen)if(exact_path(q,u,v,3)||exact_path(q,u,v,7))throw runtime_error("Q has C4/C8");
 vector<int>cap(B);for(int v=0;v<B;v++){cap[v]=4+((C.E&&v==C.sp)?1:0)-deg[v];if(cap[v]<0)throw runtime_error("negative capacity");}if(accumulate(cap.begin(),cap.end(),0)!=y)throw runtime_error("capacity sum");
 if(C.E==0&&C.sp!=-1)throw runtime_error("sp E0"); if(C.E==1&&(C.sp<0||C.sp>=B))throw runtime_error("sp E1");
 vector<U> g(base+y);auto add=[&](int u,int v){g[u]|=1ULL<<v;g[v]|=1ULL<<u;};auto del=[&](int u,int v){g[u]&=~(1ULL<<v);g[v]&=~(1ULL<<u);};for(int e=0;e<EP;e++){add(B+e,C.ed[e].first);add(B+e,C.ed[e].second);}
 // Explicitly verify base skeleton has no C4,C8,C16.
 for(int u=0;u<base;u++){U c=g[u]&~((1ULL<<(u+1))-1);while(c){U bit=c&-c;c-=bit;int v=__builtin_ctzll(bit);if(edge_bad(g,u,v))throw runtime_error("base forbidden cycle");}}
 Res R;R.a.assign(EP,0);vector<int>rem=cap,seq;
 for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++){int st=B+i,tg=B+j;add(st,tg);if(!edge_bad(g,st,tg)){}del(st,tg);function<void(int)> rec=[&](int dep){R.states++;if(dep>=y)return;int prev=dep?base+dep-1:st,node=base+dep;for(int lab=0;lab<B;lab++)if(rem[lab]){R.attempts++;add(node,lab);add(prev,node);rem[lab]--;seq.push_back(lab);if(!edge_bad(g,prev,node)){add(node,tg);if(!edge_bad(g,node,tg)){int k=dep+1;R.a[i]=max(R.a[i],k);R.a[j]=max(R.a[j],k);}del(node,tg);rec(dep+1);}seq.pop_back();rem[lab]++;del(prev,node);del(node,lab);}};rec(0);}
 return R;
}
int main(int argc,char**argv){if(argc<2){cerr<<"usage cases [csv]\n";return 2;}auto cs=readcases(argv[1]);ofstream out;if(argc>=3){out.open(argv[2]);out<<"B,E,core,sp,sum,alpha\n";}map<pair<int,int>,int>cnt,mx;map<pair<int,int>,string>arg;long long states=0,attempts=0;for(auto&C:cs){auto r=alpha(C);int s=accumulate(r.a.begin(),r.a.end(),0),tw=C.E?34:32;auto key=make_pair(C.B,C.E);cnt[key]++;if(!mx.count(key)||s>mx[key])mx[key]=s,arg[key]=C.core+":"+to_string(C.sp);if(s>=tw){cerr<<"FAIL "<<C.B<<","<<C.E<<" "<<C.core<<" sp="<<C.sp<<" sum="<<s<<" 2y="<<tw<<"\n";return 10;}states+=r.states;attempts+=r.attempts;if(out){out<<C.B<<","<<C.E<<","<<C.core<<","<<C.sp<<","<<s<<",\"";for(size_t i=0;i<r.a.size();i++){if(i)out<<" ";out<<r.a[i];}out<<"\"\n";}}
 cout<<"TOTAL "<<cs.size()<<" STATES "<<states<<" ATTEMPTS "<<attempts<<"\n";for(auto[k,n]:cnt)cout<<"SECTOR "<<k.first<<","<<k.second<<" CASES "<<n<<" MAXSUM "<<mx[k]<<" ARGMAX "<<arg[k]<<" 2y "<<(k.second?34:32)<<"\n";}
