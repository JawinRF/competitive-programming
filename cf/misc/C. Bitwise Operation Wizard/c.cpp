void solve(){
    int n  ; cin >> n ;  
    // finding n-1
    int max_idx = 0 ;   
    for(int i = 1 ; i<n ; ++i){
        cout<<"? "<<max_idx<<" "<<max_idx<<" "<<i<<" "<<i<<endl; 
        char res ; cin >> res ; 
        if(res=='<'){
            max_idx = i ; 
        }
    }
    
    // find candidates j that OR with n-1 and give mask in k+1 bits where k = msb(n-1)
    vector<int> id = { max_idx } ;
    for(int i = 0 ; i<n ; ++i){
        cout<<"? "<<id[0]<<" "<<max_idx<<" "<<i<<" "<<max_idx<<endl; 
        char res ; cin >> res ; 
        if(res=='<'){
            id.clear() ;  
            id.pb(i) ; 
        }
        else if(res=='='){
            id.pb(i) ;  
        }
    }
    
    // step 3 find smallest such j to 
    int j = id[0] ;  
    for(int i = 1 ; i<id.size() ; ++i){
        cout<<"? "<<j<<" "<<j<<" "<<id[i]<<" "<<id[i]<<endl ;  
        char res ; cin >> res ; 
        if(res=='>'){
            j = id[i] ; 
        }
    }
    cout<<"! "<<max_idx<<" "<<j<<endl ;
}
