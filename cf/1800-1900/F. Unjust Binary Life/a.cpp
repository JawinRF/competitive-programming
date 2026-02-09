f(x,y)  = min(cnt0(x,y) , cnt1(x,y))   

max + min = a+b 
max - min = |a-b| 

2*min = a+b - |a-b|  

=> min =  ( a + b - |a-b| )/2  
 
a + b = cnt0(x,y)+cnt1(x,y) = x + y   

| cnt0 - cnt1 |  

sum over x = 1 to n , y = 1 to n  
sum f(x,y) = 1/2  * ( sum(x+y) - sum(|cnt0 - cnt1|))  

	= 1/2 * ( sum( nx + n*(n+1)/2) - sum(|cnt0 - cnt1|)) 
	
	=1/2 *( (n^2 * (n+1)/2 +n^2*(n+1)/2) - sum(|cnt0 - cnt1|)) 
	 
	= n^2 * (n+1)/2  - sum(|cnt0-cnt1|)  

consider over x = 1 to n , y = 1 to n 
sum(|cnt0 - cnt1|)

= keep x fixed |( cnt0(x',y) - cnt1(x',y))| over all y
= keep x fixed |( cnt0(x',y) - cnt1(x',y))| over all y


|cnt0(y) - cnt1(y) + cnt0(x') - cnt1(x') |

cnt0(x') - cnt1(x') = C   
if x'+1 is 1 then C-- else C++  

cnt0(y) - cnt1(y)  = pref(y)

then pref(y) + C >=0 then ntg 
if pref(y) + C <0 then abs will make it pref(y)-C
			

preb(y) + C >=0   
preb(y) >= -C  	

 
void solve(){
    int n ; cin >> n  ;
    string x,y  ; cin >> x >> y  ;
    int ans = n*n*(n+1)/2 ;
    vector<int> prefy(n) , prefx(n) ;
    int curry = 0 , C = 0 ;
    
    for(int i = 0 ; i<n ; ++i){
    	if(y[i]=='0')curry++ ;
    	else curry-- ;   
    	prefy[i] = curry ;
    }
	sort(all(prefy)) ;  
	
	for(int i = 0 ; i<n ; ++i){
		if(x[i]=='0')C++ ;  
		else C-- ; 
		prefx[i] = C ;  
	}
	sort(all(prefx)) ;
	
	int l_sum = getsum(prefy) ; 
	
	int r_sum = 0 ; 
	int r = n-1  ; 
	
	int ans2 = 0 ; 
	for(int i = 0 ; i<n ; ++i){
		while(r>=0 && prefx[i]+prefy[r]>=0){
			l_sum -= prefy[r] ; 
			r_sum += prefy[r] ; 
			r-- ; 
		}
		ans2 += -l_sum - (r+1)*prefx[i] + (r_sum) + (n-r-1)*prefx[i] ; 
	}
	
	put(ans-(ans2/2)) ;  
}

