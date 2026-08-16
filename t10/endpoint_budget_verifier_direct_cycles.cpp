#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;
using U = uint64_t;

struct Case { int B,E,sp,idx; string core; vector<pair<int,int>> ed; };
struct Budget { int B,E,sp,sum; string core; vector<int> lam; };
using Key = tuple<int,int,string,int>;
static Key key(const Case& c){return {c.B,c.E,c.core,c.sp};}
static Key key(const Budget& b){return {b.B,b.E,b.core,b.sp};}

static void add(vector<U>&a,int u,int v){a[u]|=1ULL<<v;a[v]|=1ULL<<u;}
static void del(vector<U>&a,int u,int v){a[u]&=~(1ULL<<v);a[v]&=~(1ULL<<u);}

static bool cycle_dfs(const vector<U>&a,int s,int v,int L,int depth,U used,int first){
    if(depth==L-1){
        if(((a[v]>>s)&1ULL) && first<v) return true; // orientation symmetry only
        return false;
    }
    U c=a[v]&~used;
    while(c){U b=c&-c;c-=b;int w=__builtin_ctzll(b);
        if(cycle_dfs(a,s,w,L,depth+1,used|b,first))return true;
    }
    return false;
}
static bool cycle_through(const vector<U>&a,int s,int L){
    U c=a[s];
    while(c){U b=c&-c;c-=b;int f=__builtin_ctzll(b);
        if(cycle_dfs(a,s,f,L,1,(1ULL<<s)|b,f))return true;
    }
    return false;
}
static bool forbidden_through(const vector<U>&a,int s){
    return cycle_through(a,s,4)||cycle_through(a,s,8)||cycle_through(a,s,16);
}
static bool has_cycle_len(const vector<U>&a,int L){
    for(int s=0;s<(int)a.size();++s) if(cycle_through(a,s,L)) return true;
    return false;
}
static bool two_deg(int n,const vector<pair<int,int>>&ed){
    vector<U>a(n);for(auto [u,v]:ed)add(a,u,v);U alive=(1ULL<<n)-1;vector<int>d(n);
    for(int i=0;i<n;i++)d[i]=__builtin_popcountll(a[i]);
    for(int k=0;k<n;k++){int v=-1;for(int i=0;i<n;i++)if((alive>>i&1ULL)&&d[i]<=2){v=i;break;}
        if(v<0)return false;alive&=~(1ULL<<v);U c=a[v]&alive;while(c){U b=c&-c;c-=b;--d[__builtin_ctzll(b)];}}
    return true;
}
static vector<Case> read_cases(const string&p){
    ifstream in(p);string l;if(!getline(in,l)||l!="ENDPOINT_CASES_V1")throw runtime_error("case header");
    vector<Case> out;int idx=0;
    while(getline(in,l)){if(l=="END")break;if(l.empty())continue;istringstream ss(l);string tag;Case c;ss>>tag>>c.B>>c.E>>c.core>>c.sp;if(tag!="CASE")throw runtime_error("CASE");
        if(!getline(in,l))throw runtime_error("EDGES missing");istringstream es(l);es>>tag;if(tag!="EDGES")throw runtime_error("EDGES");string x;
        while(es>>x){auto z=x.find(',');c.ed.push_back({stoi(x.substr(0,z)),stoi(x.substr(z+1))});}
        if(!getline(in,l)||l!="ENDCASE")throw runtime_error("ENDCASE");c.idx=idx++;out.push_back(move(c));}
    return out;
}
static vector<string> csv_split(const string&s){vector<string>v;string x;bool q=false;for(char ch:s){if(ch=='\"'){q=!q;continue;}if(ch==','&&!q){v.push_back(x);x.clear();}else x+=ch;}v.push_back(x);return v;}
static vector<Budget> read_budgets(const string&p){
    ifstream in(p);string l;if(!getline(in,l))throw runtime_error("budget header");vector<Budget> out;
    while(getline(in,l)){if(l.empty())continue;auto f=csv_split(l);if(f.size()!=6)throw runtime_error("budget csv fields");Budget b;b.B=stoi(f[0]);b.E=stoi(f[1]);b.core=f[2];b.sp=stoi(f[3]);b.sum=stoi(f[4]);istringstream ss(f[5]);int x;while(ss>>x)b.lam.push_back(x);out.push_back(move(b));}
    return out;
}
struct Result{bool ok=true;string why;long long states=0,attempts=0,closures=0;int longest=-1;pair<int,int> longpair{-1,-1};vector<int> longword;vector<int> cap;};

static Result verify(const Case&C,const Budget&Bd,bool collect_witness){
    Result R;int B=C.B,E=C.E,EP=C.ed.size(),y=E?17:16;
    if(key(C)!=key(Bd)){R.ok=false;R.why="key mismatch";return R;}
    if((int)Bd.lam.size()!=EP){R.ok=false;R.why="lambda length";return R;}
    for(int x:Bd.lam)if(x<0||x>2){R.ok=false;R.why="lambda outside 0,1,2";return R;}
    if(accumulate(Bd.lam.begin(),Bd.lam.end(),0)!=Bd.sum||Bd.sum>=y){R.ok=false;R.why="lambda sum";return R;}
    if(EP!=2*B-8){R.ok=false;R.why="edge count";return R;}
    vector<int>deg(B);set<pair<int,int>>seen;vector<pair<int,int>>ed=C.ed;
    for(auto [u0,v0]:ed){int u=u0,v=v0;if(u==v||u<0||v<0||u>=B||v>=B){R.ok=false;R.why="bad Q edge";return R;}if(u>v)swap(u,v);if(!seen.insert({u,v}).second){R.ok=false;R.why="parallel Q edge";return R;}deg[u]++;deg[v]++;}
    if(!two_deg(B,ed)){R.ok=false;R.why="Q not 2deg";return R;}
    if((E==0&&C.sp!=-1)||(E==1&&(C.sp<0||C.sp>=B))){R.ok=false;R.why="sp";return R;}
    R.cap.assign(B,0);for(int v=0;v<B;v++){R.cap[v]=4+((E&&v==C.sp)?1:0)-deg[v];if(R.cap[v]<0){R.ok=false;R.why="negative cap";return R;}}
    if(accumulate(R.cap.begin(),R.cap.end(),0)!=y){R.ok=false;R.why="cap sum";return R;}
    // Validate Q has no C4/C8 by direct cycle enumeration. Since the local base
    // skeleton is the 1-subdivision of simple Q, this also excludes base C8/C16;
    // base C4 would require parallel Q edges, already rejected above.
    vector<U>q(B);for(auto [u,v]:ed)add(q,u,v);
    if(has_cycle_len(q,4)||has_cycle_len(q,8)){R.ok=false;R.why="Q C4/C8";return R;}

    int base=B+EP,nmax=base+y;vector<U>g(nmax);
    for(int e=0;e<EP;e++){add(g,B+e,ed[e].first);add(g,B+e,ed[e].second);}
    vector<int>rem=R.cap,word;
    for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++){
        int st=B+i,tg=B+j,lim=Bd.lam[i]+Bd.lam[j];
        function<bool(int)> rec=[&](int dep)->bool{
            R.states++;if(dep>=y)return true;int prev=dep?base+dep-1:st,node=base+dep;
            for(int lab=0;lab<B;lab++)if(rem[lab]){
                R.attempts++;add(g,node,lab);add(g,prev,node);rem[lab]--;word.push_back(lab);
                bool bad=forbidden_through(g,node);
                if(!bad){
                    // For correctness we only need to close when k > budget limit.
                    // In witness mode we close at every depth to record longest safe path.
                    int k=dep+1;
                    if(k>lim || collect_witness){
                        add(g,node,tg);R.closures++;bool cbad=forbidden_through(g,node);del(g,node,tg);
                        if(!cbad){
                            if(k>R.longest){R.longest=k;R.longpair={i,j};R.longword=word;}
                            if(k>lim){R.ok=false;R.why="budget violation endpoints="+to_string(i)+","+to_string(j)+" r="+to_string(k);return false;}
                        }
                    }
                    if(!rec(dep+1))return false;
                }
                word.pop_back();rem[lab]++;del(g,prev,node);del(g,node,lab);
            }
            return true;
        };
        if(!rec(0))return R;
    }
    return R;
}

int main(int argc,char**argv){
    if(argc<3){cerr<<"usage: verifier cases cert [start end] [--witness]\n";return 2;}
    auto cs=read_cases(argv[1]);auto bs=read_budgets(argv[2]);
    map<Key,int> cc,bc;for(auto&c:cs)cc[key(c)]++;for(auto&b:bs)bc[key(b)]++;
    int dupc=0,dupb=0;for(auto&[k,n]:cc)if(n!=1)dupc++;for(auto&[k,n]:bc)if(n!=1)dupb++;
    int missing=0,extra=0;for(auto&[k,n]:cc)if(!bc.count(k))missing++;for(auto&[k,n]:bc)if(!cc.count(k))extra++;
    cout<<"PARTITION cases="<<cs.size()<<" unique="<<cc.size()<<" dup="<<dupc
        <<" cert="<<bs.size()<<" cert_unique="<<bc.size()<<" cert_dup="<<dupb
        <<" missing="<<missing<<" extra="<<extra<<"\n";
    if(dupc||dupb||missing||extra)return 3;
    map<Key,Budget> BM;for(auto&b:bs)BM[key(b)]=b;
    int lo=0,hi=cs.size();bool wit=false;
    vector<string> rest;for(int i=3;i<argc;i++)rest.push_back(argv[i]);
    if(rest.size()>=2 && rest[0]!="--witness"){lo=stoi(rest[0]);hi=stoi(rest[1]);}
    for(auto&s:rest)if(s=="--witness")wit=true;lo=max(lo,0);hi=min(hi,(int)cs.size());
    map<pair<int,int>,int> cnt,mx;long long states=0,attempts=0,closures=0;
    for(int z=lo;z<hi;z++){auto&C=cs[z];auto R=verify(C,BM[key(C)],wit);if(!R.ok){cerr<<"FAIL idx="<<z<<" "<<C.B<<","<<C.E<<" "<<C.core<<" sp="<<C.sp<<" "<<R.why<<"\n";cerr<<"CAP";for(int x:R.cap)cerr<<" "<<x;cerr<<"\n";return 10;}auto s=make_pair(C.B,C.E);cnt[s]++;mx[s]=max(mx[s],BM[key(C)].sum);states+=R.states;attempts+=R.attempts;closures+=R.closures;
        if(wit)cout<<"CASE "<<z<<" "<<C.B<<","<<C.E<<" "<<C.core<<" sp="<<C.sp<<" budget="<<BM[key(C)].sum<<" y="<<(C.E?17:16)<<" longest="<<R.longest<<" pair="<<R.longpair.first<<","<<R.longpair.second<<" word="<<([&](){string s;for(int x:R.longword)s+=to_string(x)+" ";return s;})()<<"\n";
    }
    cout<<"PASS RANGE "<<lo<<" "<<hi<<" states="<<states<<" attempts="<<attempts<<" closures="<<closures<<"\n";
    for(auto&[s,n]:cnt)cout<<"SECTOR "<<s.first<<","<<s.second<<" cases="<<n<<" max_budget="<<mx[s]<<" y="<<(s.second?17:16)<<"\n";
}
