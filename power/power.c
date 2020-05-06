int power(int a,int b){
    int ans = 1;
    int i;
    for(i=1;i<b;i++){
        ans *= a;
    }
    return ans;
}