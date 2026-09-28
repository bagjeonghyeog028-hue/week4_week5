//TODO: realloc 은 반드시 "새 용량(newcap)" 으로 호출하고, l->cap 갱신과 순서를 맞춰야 한다.
//       (성장 로직은 '용량 필드'와 '실제 확보량'이 항상 같도록 유지해야 한다)




0x503000000090 is located 0 bytes after 32-byte region [0x503000000070,0x503000000090)
allocated by thread T0 here:
    #0 0xf438a34b646c in realloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:85
    #1 0xba028aef1018 in list_ensure /work/challenges/03_heap_buffer_overflow/bug.c:64
    #2 0xba028aef11cc in list_push /work/challenges/03_heap_buffer_overflow/bug.c:78
    #3 0xba028aef1568 in main /work/challenges/03_heap_buffer_overflow/bug.c:100


    #4 0xf438a32384c0  (/lib/aarch64-linux-gnu/libc.so.6+0x284c0) (BuildId: 27027b96e5b8c475fc327aa445bea1c71d37b4e2)
    #5 0xf438a3238594 in __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x28594) (BuildId: 27027b96e5b8c475fc327aa445bea1c71d37b4e2)
    #6 0xba028aef0dac in _start (/work/challenges/03_heap_buffer_overflow/debug+0xdac) (BuildId: 55bcf415524a23541672768ff8778da06f8ac208)

SUMMARY: AddressSanitizer: heap-buffer-overflow /work/challenges/03_heap_buffer_overflow/bug.c:79 in list_push
    

#0  0x0000fffff7e774d8 in ?? () from /lib/aarch64-linux-gnu/libc.so.6
#1  0x0000fffff7e2cb3c in raise () from /lib/aarch64-linux-gnu/libc.so.6
#2  0x0000fffff7e17e00 in abort () from /lib/aarch64-linux-gnu/libc.so.6
#3  0x0000fffff7e6aac4 in ?? () from /lib/aarch64-linux-gnu/libc.so.6


#4  0x0000fffff7e81fdc in ?? () from /lib/aarch64-linux-gnu/libc.so.6
#5  0x0000fffff7e85ec8 in ?? () from /lib/aarch64-linux-gnu/libc.so.6


#6  0x0000fffff7e8728c in realloc () from /lib/aarch64-linux-gnu/libc.so.6
#7  0x0000aaaaaaaa0a48 in list_ensure (l=0xfffffffff080, need=17) at bug.c:64
#8  0x0000aaaaaaaa0b28 in list_push (l=0xfffffffff080, x=16) at bug.c:78
#9  0x0000aaaaaaaa0c80 in main () at bug.c:100

 

//建材
Shadow bytes around the buggy address:
  0x502ffffffe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x502ffffffe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x502fffffff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x502fffffff80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x503000000000: fa fa 00 00 00 fa fa fa fd fd fd fd fa fa 00 00
=>0x503000000080: 00 00[fa]fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x503000000100: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x503000000180: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x503000000200: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x503000000280: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x503000000300: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==38537==ABORTING


make run  NAME=03_heap_buffer_overflow

cc -std=c11 -Wall -Wextra -Wno-unused-parameter -g -O0 -fno-omit-frame-pointer challenges/03_heap_buffer_overflow/bug.c -o build/03_heap_buffer_overflow
==> ./build/03_heap_buffer_overflow 실행 (버그 코드는 크래시하는 것이 정상)
realloc(): invalid next size
Aborted (core dumped)
   (종료 코드 134 — 크래시 발생. gdb 로 원인 추적하세요)