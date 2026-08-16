#include <bits/stdc++.h>
using namespace std;
using U = uint64_t;

struct Option {
    int i,j;
    vector<int> seq;
    bool operator<(Option const& o) const {
        if(i!=o.i) return i<o.i;
        if(j!=o.j) return j<o.j;
        return seq<o.seq;
    }
    bool operator==(Option const& o) const { return i==o.i && j==o.j && seq==o.seq; }
};
struct CaseRec {
    int id=-1,qid=-1,sp=-1,B=0,EP=0,E=0;
    vector<int> cap;
    vector<pair<int,int>> edges;
    vector<Option> oldopts;
};

static vector<string> splitws(string const& s){
    istringstream in(s); vector<string> z; string x; while(in>>x) z.push_back(x); return z;
}
static pair<int,int> parse_edge(string s){ auto p=s.find(','); return {stoi(s.substr(0,p)),stoi(s.substr(p+1))}; }

// Independent exact simple-path oracle: iterative explicit stack, no recursive DFS.
static bool exact_simple_path(const vector<U>& adj,int s,int t,int L){
    struct S{int v,d;U used;};
    vector<S> st; st.reserve(2048); st.push_back({s,0,1ULL<<s});
    while(!st.empty()){
        S q=st.back(); st.pop_back();
        if(q.d==L){ if(q.v==t) return true; continue; }
        U c=adj[q.v]&~q.used;
        if(q.d<L-1) c &= ~(1ULL<<t);
        while(c){ U b=c&-c;c-=b;int w=__builtin_ctzll(b); st.push_back({w,q.d+1,q.used|b}); }
    }
    return false;
}

static vector<U> base_graph(int B, vector<pair<int,int>> const& ed){
    int EP=ed.size(), n=B+EP; vector<U> a(n,0);
    auto add=[&](int u,int v){a[u]|=1ULL<<v;a[v]|=1ULL<<u;};
    for(int i=0;i<EP;i++){add(B+i,ed[i].first);add(B+i,ed[i].second);} return a;
}

// Verify base graph itself has no C4/C8/C16, using all edges and the iterative oracle.
static bool base_forbidden_free(vector<U> const& a){
    int n=a.size();
    for(int u=0;u<n;u++){
        U c=a[u] & ~((1ULL<<(u+1))-1);
        while(c){U b=c&-c;c-=b;int v=__builtin_ctzll(b);
            if(exact_simple_path(a,u,v,3)||exact_simple_path(a,u,v,7)||exact_simple_path(a,u,v,15)) return false;
        }
    }
    return true;
}

// Build the COMPLETE local graph for one path component and test every F-edge in that graph.
// This is intentionally different from the original incremental safe_add generator.
static bool full_local_safe(int B, vector<pair<int,int>> const& ed, int i,int j, vector<int> const& seq){
    int EP=ed.size(), k=seq.size();
    vector<U> a=base_graph(B,ed); a.resize(B+EP+k,0);
    auto add=[&](int u,int v){a[u]|=1ULL<<v;a[v]|=1ULL<<u;};
    vector<int> pv; pv.push_back(B+i);
    for(int r=0;r<k;r++){int x=B+EP+r; add(x,seq[r]); pv.push_back(x);} pv.push_back(B+j);
    vector<pair<int,int>> fedges;
    for(int r=0;r+1<(int)pv.size();r++){add(pv[r],pv[r+1]);fedges.push_back({pv[r],pv[r+1]});}
    for(auto [u,v]:fedges){
        if(exact_simple_path(a,u,v,3)||exact_simple_path(a,u,v,7)||exact_simple_path(a,u,v,15)) return false;
    }
    return true;
}

static bool qpath(vector<U> const& qadj,int s,int t,int L){ return exact_simple_path(qadj,s,t,L); }

// Enumerate ALL label words allowed by exact capacities plus the independently derived
// distance-2 necessary condition. No generator cutoff K is read from the certificate.
static vector<vector<int>> all_words(int B, vector<pair<int,int>> const& ed, vector<int> const& cap){
    vector<U> qa(B,0); for(auto [u,v]:ed){qa[u]|=1ULL<<v;qa[v]|=1ULL<<u;}
    bool D[16][16]{};
    vector<int> Uv; for(int v=0;v<B;v++)if(cap[v]>0)Uv.push_back(v);
    for(int x=0;x<(int)Uv.size();x++)for(int y=x+1;y<(int)Uv.size();y++){
        int p=Uv[x],q=Uv[y];
        if(!qpath(qa,p,q,2) && !qpath(qa,p,q,6)) D[p][q]=D[q][p]=true;
    }
    struct W { vector<int> s,rem; };
    vector<vector<int>> out; vector<W> frontier;
    frontier.push_back({{},cap}); out.push_back({});
    while(!frontier.empty()){
        vector<W> next;
        for(auto const& st:frontier){
            for(int v:Uv){
                if(st.rem[v]==0) continue;
                if(st.s.size()>=2){ int p=st.s[st.s.size()-2]; if(p==v || !D[p][v]) continue; }
                W w=st; w.s.push_back(v); w.rem[v]--; out.push_back(w.s); next.push_back(move(w));
            }
        }
        frontier.swap(next);
    }
    return out;
}

static bool trigger(int B, vector<pair<int,int>> const& ed,int i,int j,vector<int> const& seq,int sp){
    int c[16]{}; c[ed[i].first]++;c[ed[i].second]++;c[ed[j].first]++;c[ed[j].second]++;
    for(int x:seq)c[x]++;
    for(int v=0;v<B;v++){int need=(v==sp?3:2); if(c[v]>=need)return true;} return false;
}

static vector<Option> regenerate(CaseRec const& C, long long &tested){
    auto words=all_words(C.B,C.edges,C.cap);
    vector<Option> out; tested=0;
    if(!base_forbidden_free(base_graph(C.B,C.edges))) throw runtime_error("base graph has forbidden cycle");
    for(int i=0;i<C.EP;i++)for(int j=i+1;j<C.EP;j++){
        for(auto const& s:words){
            if(!trigger(C.B,C.edges,i,j,s,C.sp)) continue;
            tested++;
            if(full_local_safe(C.B,C.edges,i,j,s)) out.push_back({i,j,s});
        }
    }
    sort(out.begin(),out.end()); return out;
}

struct StateKey{
    U mask; vector<unsigned char> rem;
    bool operator==(StateKey const&o)const{return mask==o.mask&&rem==o.rem;}
};
struct KH{ size_t operator()(StateKey const& s) const noexcept{ size_t h=s.mask*11400714819323198485ull; for(auto x:s.rem)h=(h^x)*1099511628211ull; return h; }};

// Independent exact-cover DP: choose the SMALLEST uncovered endpoint, no certificate DAG/pivot/pruning.
static bool independent_sat(CaseRec const& C, vector<Option> const& opts, long long &states){
    vector<vector<int>> by(C.EP); for(int z=0;z<(int)opts.size();z++){by[opts[z].i].push_back(z);by[opts[z].j].push_back(z);}    
    unordered_set<StateKey,KH> dead; states=0;
    function<bool(U,vector<unsigned char> const&)> rec=[&](U mask,vector<unsigned char> const& rem)->bool{
        states++;
        U full=(1ULL<<C.EP)-1;
        if(mask==full){for(auto x:rem)if(x)return false;return true;}
        StateKey key{mask,rem}; if(dead.count(key))return false;
        int v=0;while(mask>>v&1ULL)v++;
        for(int oi:by[v]){
            auto const&o=opts[oi];int w=(o.i==v?o.j:o.i); if(mask>>w&1ULL)continue;
            auto nr=rem; bool ok=true; for(int x:o.seq){if(nr[x]==0){ok=false;break;}nr[x]--;}
            if(!ok)continue;
            if(rec(mask|(1ULL<<v)|(1ULL<<w),nr))return true;
        }
        dead.insert(move(key));return false;
    };
    vector<unsigned char> r(C.B);for(int v=0;v<C.B;v++)r[v]=C.cap[v];
    return rec(0,r);
}

static vector<CaseRec> parse_cert(string path){
    ifstream in(path); if(!in) throw runtime_error("cannot open "+path);
    string line; getline(in,line); auto H=splitws(line); if(H.size()<3||H[0]!="CERT_V1")throw runtime_error("bad header");
    string kind=H[1]; int B,EP,E; bool varopt=false; int expected=-1;
    if(kind=="B15E0"){B=15;EP=22;E=0;expected=stoi(H[2]);}
    else if(kind=="B15E1"){B=15;EP=22;E=1;expected=stoi(H[2]);}
    else if(kind=="B14E0"){B=14;EP=20;E=0;expected=stoi(H[2]);varopt=true;}
    else if(kind=="B14E1_EMPTY_CHUNK"){B=14;EP=20;E=1;expected=stoi(H[3])-stoi(H[2]);}
    else if(kind=="B14E1_BAD_CHUNK"){B=14;EP=20;E=1;expected=stoi(H[3])-stoi(H[2]);varopt=true;}
    else throw runtime_error("unsupported kind "+kind);
    vector<CaseRec> out;
    while(getline(in,line)){
        if(line=="ENDCERT")break; if(line.empty())continue;
        auto P=splitws(line); if(P[0]!="CASE")throw runtime_error("expected CASE");
        CaseRec C; C.B=B;C.EP=EP;C.E=E; C.id=stoi(P[1]); int nopt=0,nstate=0,root=0,capstart=0;
        if(kind=="B15E0"||kind=="B15E1"){
            C.sp=stoi(P[2]);nopt=stoi(P[3]);nstate=stoi(P[4]);root=stoi(P[5]);capstart=6;
        }else if(kind=="B14E0"){
            C.qid=stoi(P[2]);C.sp=stoi(P[3]); /* P[4] is old K, deliberately ignored */ nopt=stoi(P[5]);nstate=stoi(P[6]);root=stoi(P[7]);capstart=8;
        }else{
            C.qid=stoi(P[2]);C.sp=stoi(P[3]);nopt=stoi(P[4]);nstate=stoi(P[5]);root=stoi(P[6]);capstart=7;
        }
        C.cap.clear();for(int z=capstart;z<(int)P.size();z++)C.cap.push_back(stoi(P[z]));
        if((int)C.cap.size()!=B)throw runtime_error("cap size");
        getline(in,line);auto Eline=splitws(line);if(Eline[0]!="EDGES")throw runtime_error("edges");
        for(int z=1;z<(int)Eline.size();z++)C.edges.push_back(parse_edge(Eline[z]));
        if((int)C.edges.size()!=EP)throw runtime_error("edge count");
        for(int oi=0;oi<nopt;oi++){
            getline(in,line);auto O=splitws(line);if(O[0]!="OPT")throw runtime_error("OPT");
            Option o;o.i=stoi(O[2]);o.j=stoi(O[3]);
            if(varopt){int k=stoi(O[4]);for(int z=0;z<k;z++)o.seq.push_back(stoi(O[5+z]));}
            else{int k=stoi(O[4]);if(k>=1)o.seq.push_back(stoi(O[5]));if(k>=2)o.seq.push_back(stoi(O[6]));}
            C.oldopts.push_back(move(o));
        }
        sort(C.oldopts.begin(),C.oldopts.end());
        // Skip STATE/BR records without interpreting their transitions.
        while(getline(in,line)){if(line=="ENDCASE")break;}
        out.push_back(move(C));
    }
    if(expected>=0 && (int)out.size()!=expected)throw runtime_error("case count mismatch");
    return out;
}

int main(int argc,char**argv){
    if(argc<2){cerr<<"usage: second_local_option_verifier cert...\n";return 2;}
    long long allopts=0,allcand=0,alldp=0;int allcases=0;
    for(int ai=1;ai<argc;ai++){
        string path=argv[ai]; auto cases=parse_cert(path); long long fopts=0,fcand=0,fdp=0;
        for(auto const& C:cases){
            // Recompute capacities solely from Q and sector data.
            vector<int> deg(C.B,0);for(auto[u,v]:C.edges){if(u==v||u<0||v<0||u>=C.B||v>=C.B){cerr<<"bad Q edge\n";return 3;}deg[u]++;deg[v]++;}
            vector<int> cap(C.B);for(int v=0;v<C.B;v++){cap[v]=4+(C.E==1&&v==C.sp)-deg[v];if(cap[v]<0){cerr<<"negative cap\n";return 4;}}
            if(cap!=C.cap){cerr<<"CAP DISAGREEMENT file="<<path<<" case="<<C.id<<"\n";return 5;}
            long long tested=0; auto neo=regenerate(C,tested);
            if(neo!=C.oldopts){
                cerr<<"OPTION DISAGREEMENT file="<<path<<" case="<<C.id<<" qid="<<C.qid<<" sp="<<C.sp<<" old="<<C.oldopts.size()<<" new="<<neo.size()<<"\n";
                set<Option> A(C.oldopts.begin(),C.oldopts.end()),Bset(neo.begin(),neo.end());
                int shown=0;for(auto const&o:A)if(!Bset.count(o)&&shown++<5){cerr<<" old-only "<<o.i<<" "<<o.j<<" [";for(int x:o.seq)cerr<<x<<",";cerr<<"]\n";}
                shown=0;for(auto const&o:Bset)if(!A.count(o)&&shown++<5){cerr<<" new-only "<<o.i<<" "<<o.j<<" [";for(int x:o.seq)cerr<<x<<",";cerr<<"]\n";}
                cerr<<" QEDGES";for(auto[u,v]:C.edges)cerr<<" "<<u<<","<<v;cerr<<"\n";return 10;
            }
            long long ds=0; bool sat=independent_sat(C,neo,ds);
            if(sat){cerr<<"SAT DISAGREEMENT file="<<path<<" case="<<C.id<<"\n";return 11;}
            fopts+=neo.size();fcand+=tested;fdp+=ds;
        }
        cout<<"PASS2 "<<filesystem::path(path).filename().string()<<" CASES "<<cases.size()<<" OPTIONS "<<fopts<<" TRIGGERED_CANDIDATES "<<fcand<<" DP_STATES "<<fdp<<"\n";
        allcases+=cases.size();allopts+=fopts;allcand+=fcand;alldp+=fdp;
    }
    cout<<"TOTAL CASES "<<allcases<<" OPTIONS "<<allopts<<" TRIGGERED_CANDIDATES "<<allcand<<" DP_STATES "<<alldp<<"\n";
    return 0;
}
