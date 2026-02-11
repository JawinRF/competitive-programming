#include <bits/stdc++.h>  
using namespace std ; 
#define int long long
signed main(){
	int k ;  
	cin >> k  ; 
	// find the number of digits in the number the kth element belongs to 
	int p = 0 , curr = 1 ; 
	while( k - 9*(p+1)*curr > 0 ) {  
		k = k - 9*(p+1)*curr ;  
		p++ ;   
		curr = curr*10 ;  
	}
 
	int digits = p+1 ;
	int number = curr + (k-1)/digits ; 
	string s = to_string(number);
    	cout << s[(k - 1) % digits] << endl;
	return 0;   
}


