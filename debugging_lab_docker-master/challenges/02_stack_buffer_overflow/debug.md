94-112 line
int main(void) {
    int tri[SIZE];

    build_pascal(tri, ROWS);          

    for (int i = 0; i < ROWS; i++) print_row(tri, i);

    printf("SIZE = %d\n", SIZE);

    /* [Thinking Point]
     * 이 프로그램은 위 printf 까지 정상 출력을 마치고도, 왜 하필 이 return 0; 에서
     * 크래시(SIGABRT, "stack smashing detected")가 날까?
     *   tip 2. 컴파일러는 배열(tri[]) 같은 지역 변수 뒤에 '스택 카나리(canary)'라는
     *          감시 값을 심어두고, 함수가 return 하기 직전에 그 값이 그대로인지 검사한다.
     *   생각해보기: build_pascal 이 tri[] 경계를 넘어 쓰면 카나리가 훼손된다. 그렇다면
     *               크래시가 "배열을 넘어 쓰는 순간"이 아니라 "return 시점"에 나는 이유는?
     *               (힌트: 오버플로 자체는 조용히 일어나고, 검사는 return 직전에 이뤄진다) */
    return 0;     *   tip 1. return 은 단순히 "0을 돌려준다"가 아니라, main 의 스택 프레임을 정리하고
     *          호출처로 '되돌아가는' 동작이다. 이때 스택에 저장된 복귀 정보가 사용된다.
}



tri, i, j 부분 완성 (참고)  65-80

^^^^^^^^^^^^^^^^^^^^
/* 파스칼의 삼각형을 tri[] 에 채운다. */
static void build_pascal(int *tri, int rows) {
    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= i; j++) {
            int idx = tri_index(i, j);
            if (j == 0 || j == i) {
                tri[idx] = 1;                         /* 양 끝은 1 */
            } else {
                int up_left  = tri_index(i - 1, j - 1);
                int up_right = tri_index(i - 1, j);
                tri[idx] = tri[up_left] + tri[up_right];
            }
        }
    }
}