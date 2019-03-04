#include<stdio.h>

int N;
int ans[30];

int fibo(){
	int i;
	ans[0] = ans[1] = 1;
	for(i = 2; i < N; ++i) {
		ans[i] = ans[i-1]+ans[i-2];
	}
	for(i = 0; i < N; ++i) {
		printf("%d ", ans[i]);
	}
}

int main(){
    scanf("%d", &N);
	fibo();

    return 0;
}
