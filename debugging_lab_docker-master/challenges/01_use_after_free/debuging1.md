주로 봐야 하는것 123,105를 참고하여 169부분 위에서부터 작성 하여 169부분 활성화

static void widget_destroy(Widget *w) {
    free(w);          
}  //105부분.

static void screen_render(Screen *s) {
    for (int i = 0; i < s->count; i++) {
        Widget *w = s->items[i];
        w->vtbl->render(w);      
    }
}  //123 부분

(gdb) run
Starting program: /work/challenges/01_use_after_free/debug 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/aarch64-linux-gnu/libthread_db.so.1".
frame 1:
  Label #10: Welcome
  [Button #11] "OK"
  <<Dialog #12>> Are you sure?
  [Button #13] "Cancel"
STATUS: dialog closed
frame 2:
  Label #10: Welcome
  [Button #11] "OK"

Program received signal SIGSEGV, Segmentation fault.
screen_render (s=0xfffffffff100) at bug.c:123
123             w->vtbl->render(w);      

(gdb) bt
#0  screen_render (s=0xfffffffff100) at bug.c:123
#1  0x0000aaaaaaaa0f54 in main () at bug.c:169   (작성해야하는 부분으로 완성.)

(gdb) pritn w
Undefined command: "pritn".  Try "help".

(gdb) print w
$1 = (Widget *) 0xaaaaaaac1300

(gdb) print w->vtbl
$2 = (const VTable *) 0x203a535554415453

(gdb) print s->items[2]
$3 = (Widget *) 0xaaaaaaac1300

(gdb) break widget_destroy
Breakpoint 1 at 0xaaaaaaaa0c6c: file bug.c, line 105.

//원인은 **Use-After-Free(해제 후 사용)**입니다.  ####처음이여서 갈피를 잡으려고 어떤 원인인지에 대한 조사.

1. 증상: w->vtbl이 쓰레기 값
print w->vtbl이 0x203a535554415453인데, 이걸 리틀엔디안 ASCII로 읽으면 "STATUS: "입니다. 정상적인 vtable 포인터가 아니라 문자열 데이터가 들어 있다는 뜻이고, w가 가리키는 메모리가 이미 다른 용도로 재사용됐다는 증거입니다.

2. 원인: dangling 포인터
w와 s->items[2]가 같은 주소(0xaaaaaaac1300)입니다. 즉 screen_render가 items[2]를 그대로 꺼내 쓰고 있는데, 이 항목이 가리키는 객체가 이미 해제된 상태입니다.

3. 어떤 객체인가
frame 1에는 Dialog #12가 있었고, frame 2에서는 사라졌습니다. 그 사이에 STATUS: dialog closed가 출력됐습니다. 따라서 items[2]는 Dialog #12이고, 닫힐 때 widget_destroy로 해제됐습니다. 그런데 screen의 items 배열에서는 제거되지 않았습니다.

4. 메모리 재사용 경로
해제된 Dialog 청크가 "STATUS: dialog closed" 문자열을 만들 때 재할당됐고, 그 문자열의 앞 8바이트("STATUS: ")가 원래 vtbl 자리에 덮어써졌습니다.

5. 크래시 지점
frame 2 렌더링 중 bug.c:123의 w->vtbl->render(w)에서 이 쓰레기 vtbl을 따라가다 SIGSEGV가 났습니다.

////////처음에 확인하는 과정에서 문제가 있다고 한부분 관련을 보고 원인을 찾은 후의 우선순위./////////////////

정리하면, 위젯을 파괴할 때 screen의 items에서 제거(또는 NULL 처리)하지 않고, 렌더 루프도 해제 여부를 검사하지 않는 것이 근본 원인입니다.

로그만으로는 알 수 없는 것은 다음과 같습니다.

widget_destroy가 정확히 어떤 코드로 해제하는지 (bug.c:105 부근)
다이얼로그를 닫는 함수가 items 배열을 왜 정리하지 않는지
이 문자열이 malloc으로 할당됐는지, 어떤 함수에서 만들어졌는지

이걸 확인하려면 break widget_destroy 후 실행해서 bt와 print w로 해제 시점의 주소를 보고, 그 주소가 0xaaaaaaac1300과 같은지 대조하면 됩니다. 이후 watch나 x/s 0xaaaaaaac1300으로 재사용되는 순간을 잡으면 원인이 확정됩니다.
//

123과 105는 핵심이고, 168은 우선순위가 낮습니다. 대신 하나를 더 보셔야 합니다.

123번 줄 (w->vtbl->render(w)): 크래시 지점
여기서는 원인이 아니라 결과를 보게 됩니다. 확인할 것은 screen_render의 루프가 s->items[i]를 꺼낼 때 NULL이나 해제 여부를 검사하는지입니다.

105번 줄 (widget_destroy): 해제 지점
브레이크포인트를 건 줄이라 함수 시작 부분일 가능성이 높습니다. 실제 free()가 몇 번째 줄인지 확인하고, 해제 전에 vtbl이나 포인터를 정리하는지 보세요. 여기서 s->items를 건드리지 않는다면 그게 바로 버그입니다.

168번 줄 (main): 호출 지점
screen_render를 호출하는 곳이라 대개 버그와 직접 관련이 없습니다. 다만 다이얼로그를 닫는 코드(dialog closed 출력 부근)가 main에 있다면 그 부분은 봐야 합니다.

추가로 볼 곳

다이얼로그를 닫는 함수: STATUS: dialog closed를 출력하고 widget_destroy를 호출하는 코드입니다. items 배열에서 제거하는 처리가 빠져 있을 가능성이 가장 높습니다.
STATUS: 문자열을 만드는 코드: 해제된 청크를 재사용하는 쪽입니다. 원인은 아니지만 왜 같은 청크가 재사용됐는지 이해하는 데 도움이 됩니다.

보는 순서 추천

break widget_destroy 후 실행해서 bt로 누가 호출했는지 확인 (닫는 함수 찾기)
그 함수에서 items 정리 여부 확인
123번 줄의 루프 조건 확인

bt 결과 (제일 중요)