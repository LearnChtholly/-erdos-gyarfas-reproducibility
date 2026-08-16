#include <bits/stdc++.h>
using namespace std; using U=uint64_t;
const int B=15,EP=22,MAXN=45;

struct G{
 U a[MAXN]{}; int n=0;
 void add(int u,int v){a[u]|=1ULL<<v;a[v]|=1ULL<<u;}
 void del(int u,int v){a[u]&=~(1ULL<<v);a[v]&=~(1ULL<<u);}
 bool exact_path(int s,int t,int L){
   function<bool(int,int,U)> dfs=[&](int v,int d,U used)->bool{
     if(d==L)return v==t;
     U c=a[v]&~used; if(d<L-1)c&=~(1ULL<<t);
     while(c){U b=c&-c;c-=b;int w=__builtin_ctzll(b);if(dfs(w,d+1,used|b))return true;}
     return false;
   };
   return dfs(s,0,1ULL<<s);
 }
 bool safe_add(int u,int v){
   return !((a[u]>>v)&1ULL) && !exact_path(u,v,3) && !exact_path(u,v,7) && !exact_path(u,v,15);
 }
};
struct Opt{int i,j,k,p,q; bool operator==(Opt const&o)const{return tie(i,j,k,p,q)==tie(o.i,o.j,o.k,o.p,o.q);} };
struct Branch{int oi,child;};
struct State{U mask;int pivot;vector<int> rem;vector<Branch> br;};

bool trigger(pair<int,int> ei,pair<int,int> ej,int k,int p,int q,int sp){
 int c[15]{};c[ei.first]++;c[ei.second]++;c[ej.first]++;c[ej.second]++;
 if(k>=1)c[p]++;if(k>=2)c[q]++;
 for(int v=0;v<15;v++){int need=(v==sp?3:2);if(c[v]>=need)return true;}
 return false;
}
bool local_safe(G &g,int i,int j,int k,int p,int q){
 int old=g.n;vector<pair<int,int>> added;vector<int> vv{B+i};
 if(k>=1){int x=g.n++;g.add(x,p);added.push_back({x,p});vv.push_back(x);}
 if(k>=2){int x=g.n++;g.add(x,q);added.push_back({x,q});vv.push_back(x);}
 vv.push_back(B+j);
 for(int r=0;r+1<(int)vv.size();r++){
   int u=vv[r],v=vv[r+1];
   if(!g.safe_add(u,v)){for(auto it=added.rbegin();it!=added.rend();++it)g.del(it->first,it->second);g.n=old;return false;}
   g.add(u,v);added.push_back({u,v});
 }
 for(auto it=added.rbegin();it!=added.rend();++it)g.del(it->first,it->second);g.n=old;return true;
}

int main(int argc,char**argv){
 if(argc!=2){cerr<<"usage: checker cert\n";return 2;}
 ifstream in(argv[1]); if(!in){cerr<<"cannot open cert\n";return 2;}
 string tok,kind;int nc;
 in>>tok>>kind>>nc; if(tok!="CERT_V1"||kind!="B15E0"||nc!=14){cerr<<"bad header\n";return 3;}
 long long totalStates=0,totalOpts=0,totalBranches=0;
 for(int ci=0;ci<nc;ci++){
   int caseid,sp,nopt,nstate,root;vector<int> cap(15);
   in>>tok>>caseid>>sp>>nopt>>nstate>>root; if(tok!="CASE"||caseid!=ci){cerr<<"bad case\n";return 4;}
   for(int&x:cap)in>>x;
   in>>tok;if(tok!="EDGES"){cerr<<"missing EDGES\n";return 4;}
   vector<pair<int,int>> ed;
   for(int i=0;i<EP;i++){string s;in>>s;auto p=s.find(',');int u=stoi(s.substr(0,p)),v=stoi(s.substr(p+1));ed.push_back({u,v});}
   vector<Opt> certOpts(nopt);
   for(int oi=0;oi<nopt;oi++){
     int id;in>>tok>>id>>certOpts[oi].i>>certOpts[oi].j>>certOpts[oi].k>>certOpts[oi].p>>certOpts[oi].q;
     if(tok!="OPT"||id!=oi){cerr<<"bad OPT\n";return 5;}
   }
   vector<State> st(nstate);
   for(int si=0;si<nstate;si++){
     int id,nb;in>>tok>>id>>st[si].mask>>st[si].pivot>>nb;if(tok!="STATE"||id!=si){cerr<<"bad STATE\n";return 6;}
     st[si].rem.resize(15);for(int&x:st[si].rem)in>>x;
     st[si].br.resize(nb);for(int z=0;z<nb;z++){in>>tok>>st[si].br[z].oi>>st[si].br[z].child;if(tok!="BR"){cerr<<"bad BR\n";return 6;}}
   }
   in>>tok;if(tok!="ENDCASE"){cerr<<"missing ENDCASE\n";return 7;}

   // Reconstruct all legal local options independently.
   int deg[15]{};G g;g.n=B+EP;
   for(int i=0;i<EP;i++){auto [u,v]=ed[i];if(u==v||u<0||v<0||u>=15||v>=15){cerr<<"bad edge\n";return 8;}deg[u]++;deg[v]++;int a=B+i;g.add(a,u);g.add(a,v);}
   vector<int> expectedCap(15),labs;
   for(int v=0;v<15;v++){expectedCap[v]=4-deg[v];if(expectedCap[v]<0){cerr<<"negative cap\n";return 8;}if(expectedCap[v])labs.push_back(v);}
   if(expectedCap!=cap){cerr<<"cap mismatch case "<<ci<<"\n";return 9;}
   vector<Opt> opts;
   for(int i=0;i<EP;i++)for(int j=i+1;j<EP;j++){
     if(trigger(ed[i],ed[j],0,-1,-1,-1)&&local_safe(g,i,j,0,-1,-1))opts.push_back({i,j,0,-1,-1});
     for(int p:labs)if(cap[p]>=1)if(trigger(ed[i],ed[j],1,p,-1,-1)&&local_safe(g,i,j,1,p,-1))opts.push_back({i,j,1,p,-1});
     for(int p:labs)for(int q:labs){if(p==q&&cap[p]<2)continue;if(trigger(ed[i],ed[j],2,p,q,-1)&&local_safe(g,i,j,2,p,q))opts.push_back({i,j,2,p,q});}
   }
   if(opts!=certOpts){cerr<<"option completeness mismatch case "<<ci<<" expected "<<opts.size()<<" cert "<<certOpts.size()<<"\n";return 10;}

   vector<vector<int>> by(EP);for(int oi=0;oi<nopt;oi++){by[opts[oi].i].push_back(oi);by[opts[oi].j].push_back(oi);}
   // State records are postorder: any non-bound child must be an earlier record.
   for(int si=0;si<nstate;si++){
     auto &S=st[si];
     if(S.mask>>EP){cerr<<"mask overflow\n";return 11;}
     if(S.mask==(1ULL<<EP)-1){
       bool zero=true;for(int x:S.rem)if(x)zero=false;
       if(zero){cerr<<"SAT terminal in certificate\n";return 12;}
       if(S.pivot!=-1||!S.br.empty()){cerr<<"bad terminal record\n";return 12;}
       continue;
     }
     vector<int> bestcand;int best=-1;
     for(int v=0;v<EP;v++)if(!((S.mask>>v)&1ULL)){
       vector<int> cand;
       for(int oi:by[v]){
         auto&o=opts[oi];int w=(o.i==v?o.j:o.i);if((S.mask>>w)&1ULL)continue;
         bool ok=true;if(o.k>=1&&S.rem[o.p]<1)ok=false;
         if(o.k>=2){if(o.p==o.q){if(S.rem[o.p]<2)ok=false;}else if(S.rem[o.q]<1)ok=false;}
         if(ok)cand.push_back(oi);
       }
       if(cand.empty()){best=v;bestcand.clear();break;}
       if(best<0||cand.size()<bestcand.size()){best=v;bestcand=cand;}
     }
     if(S.pivot!=best){cerr<<"pivot mismatch case "<<ci<<" state "<<si<<"\n";return 13;}
     if(S.br.size()!=bestcand.size()){cerr<<"branch count mismatch case "<<ci<<" state "<<si<<"\n";return 14;}
     for(size_t z=0;z<bestcand.size();z++){
       if(S.br[z].oi!=bestcand[z]){cerr<<"branch option mismatch\n";return 15;}
       auto&o=opts[bestcand[z]];auto nr=S.rem;if(o.k>=1)nr[o.p]--;if(o.k>=2)nr[o.q]--;
       U nm=S.mask|(1ULL<<o.i)|(1ULL<<o.j);
       int left=(EP-__builtin_popcountll(nm))/2,total=0;for(int x:nr)total+=x;
       int ch=S.br[z].child;
       if(total>2*left){if(ch!=-1){cerr<<"bound branch should be -1\n";return 16;}}
       else{
         if(ch<0||ch>=si){cerr<<"child not earlier state\n";return 17;}
         if(st[ch].mask!=nm||st[ch].rem!=nr){cerr<<"child transition mismatch\n";return 18;}
       }
     }
   }
   if(root<0||root>=nstate||st[root].mask!=0||st[root].rem!=cap){cerr<<"bad root\n";return 19;}
   totalStates+=nstate;totalOpts+=nopt;for(auto&S:st)totalBranches+=S.br.size();
 }
 in>>tok;if(tok!="ENDCERT"){cerr<<"missing ENDCERT\n";return 20;}
 cout<<"PASS\n";
 cout<<"cases=14 options="<<totalOpts<<" states="<<totalStates<<" branches="<<totalBranches<<"\n";
 cout<<"Every local option set was independently regenerated from edge lists.\n";
 cout<<"Every dead-state branch is either quota-bound or points to an earlier verified dead state.\n";
}
