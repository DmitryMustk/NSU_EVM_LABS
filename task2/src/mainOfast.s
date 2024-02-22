fill_arr:
        movq    %rdi, %rcx //адрес массива помещаем в rcx
        testl   %esi, %esi //проверяем равен ли размер массива нулю
        jle     .L1 //если меньше или равно нулю прыгаем в l1
        leal    -1(%rsi), %eax //в eax загружаем размер массива минус один
        cmpl    $2, %eax //сравниваем размер с 2
        jbe     .L6 //если меньше или равно двум прыгаем в Л6
        movl    %esi, %edx //засовываем размер массива в edx
        movd    %esi, %xmm5 //загружаем размер массива в xmm5
        movq    %rdi, %rax //адрес массива загружаем в rax
        movdqa  .LC0(%rip), %xmm0 //загружаем константу из памяти в 
        shrl    $2, %edx //сдвигаем edx на 2 бита вправо (деление на 4)
        movdqa  .LC1(%rip), %xmm3 //загружаем константу из памяти в xmm3
        pshufd  $0, %xmm5, %xmm4 //выполняем операцию перестановки байт в xmm5 и помещаем в xmm4
        salq    $4, %rdx //сдвиг влево на 4 бита в rdx (умножение на 16)
        addq    %rdi, %rdx //прибавляем к адресу массива значение в rdx
.L4:
        movdqa  %xmm0, %xmm2 //копируем xmm0 в xmm2
        movdqa  %xmm4, %xmm1 //копируем xmm4 в xmm1
        paddd   %xmm3, %xmm0 //складываем xmm0 и xmm3 и сохраняем в xmm0
        addq    $16, %rax //увеличиваем адрес массива на 16 байт
        psubd   %xmm2, %xmm1 //вычитание
        movups  %xmm1, -16(%rax) //загружаем 16 байт из xmm1 в память начиная с rax - 16
        cmpq    %rdx, %rax //сравниваем адрес массива с конечным адресом
        jne     .L4 //если не равен конечному адресу возвращаемся в начало метки
        movl    %esi, %eax //перемещаем размер массива в eax
        andl    $-4, %eax //побитовое и между eax и -4
        testb   $3, %sil //проверяем установлен ли третий бит в sil
        je      .L9 //если не установлен переходим к метке L9
.L3:
        movl    %esi, %r8d //перемещаем размер массива в r8d
        movslq  %eax, %rdx //расширяем 32 битного значения в eax до 64 бит и сохраняем в rdx
        subl    %eax, %r8d //вычитаем значение в eax из размера массива и сохраняем в r8d
        leaq    0(,%rdx,4), %rdi //вычисляем адресс массива плюс индекс умноженый на 4 и сохраняем в rdi
        movl    %r8d, (%rcx,%rdx,4) // сохраняем r8d в элемент массива по адресу rcx + rdx * 4
        leal    1(%rax), %edx //rax+1 и сохраняем в edx
        cmpl    %edx, %esi //сравниваем edx с размером массива
        jle     .L1 //edx <= esi прыгаем в L1
        movl    %esi, %r8d //перемещаем размер массива в r8d
        addl    $2, %eax //eax+=2
        subl    %edx, %r8d//вычитаем edx из размера массива и сохраняем в r8d
        movl    %r8d, 4(%rcx,%rdi) //сохраняем r8d в элемент массива по адресу rcx + rdi + 4
        cmpl    %eax, %esi //сравниваем eax с размером массива
        jle     .L1 //если меньше или равно то прыгаем в l1
        subl    %eax, %esi //вычитаем eax из размера массива и сохраняем в esi
        movl    %esi, 8(%rcx,%rdi) //сохраняем esi в элемент массива rcx+rdi+8
.L1:
        ret //возвращаемся из fill_arr
.L9:
        ret //возвращаемся из fill_arr
.L6:
        xorl    %eax, %eax //устанавливаем eax в ноль
        jmp     .L3 //прыгаем в L3
swap:
        movl    (%rdi), %eax
        movl    (%rsi), %edx
        movl    %edx, (%rdi)
        movl    %eax, (%rsi)
        ret
bubble_sort:
        cmpl    $1, %esi
        jle     .L11
        leaq    4(%rdi), %r9
.L13:
        cmpl    $1, %esi
        jle     .L26
        leal    -2(%rsi), %edx
        movq    %rdi, %rax
        leaq    (%r9,%rdx,4), %r8
.L15:
        movq    (%rax), %xmm0
        pshufd  $0xe5, %xmm0, %xmm1
        movd    %xmm0, %edx
        movd    %xmm1, %ecx
        cmpl    %edx, %ecx
        jge     .L14
        pshufd  $225, %xmm0, %xmm0
        movq    %xmm0, (%rax)
.L14:
        addq    $4, %rax
        cmpq    %r8, %rax
        jne     .L15
        subl    $1, %esi
        cmpl    $1, %esi
        jne     .L13
.L11:
        ret
.L26:
        subl    $1, %esi
        jmp     .L13
main:
        pushq   %rbp        //сохраняем rbp на вершине стека
        movq    8(%rsi), %rdi //перемещаем в первый аргумент значение на которое указывает второй аргуемент (argv[1])
        movl    $10, %edx     //помещаем 10 в edx
        xorl    %esi, %esi    //обнуляем esi
        movq    %rsp, %rbp   //rbp = rsp (создаем стековый фрейм для main)
        call    strtol       //вызываем strtol для преобразования строки в число
        movq    %rax, %rdx  //перемещаем результат вызова функции в rdx (адрес конца строки)
        movl    %eax, %esi //перемещаем результат вызова функции в esi (число элементов массива)
        cltq              //знаково расширяем eax до rax
        leaq    15(,%rax,4), %rax //вычисляем количество байт которое нужно выделить для массива
        andq    $-16, %rax //выравниваем количество байт по границе 16 байт
        subq    %rax, %rsp //выделяем память на стеке для массива
        movq    %rsp, %rdi //передаем адрес массива первым аргументом в fill_arr
        testl   %edx, %edx //проверяем равен ли edx нулю
        jle     .L28      //если <= 0 прыгаем в L28
        leal    -1(%rdx), %eax //загружаем результат вызова функции минус один в eax
        cmpl    $2, %eax  //сравниваем результат вызова функции с двойкой
        jbe     .L32  	//<= 2 прыгаем в l32
        movl    %edx, %ecx //размер массива перемещаем в ecx
        movd    %edx, %xmm5 //результат вызова функции в xmm5
        movq    %rdi, %rax //адресс массива в rax
        movdqa  .LC0(%rip), %xmm0 //константа из памяти в xmm0
        shrl    $2, %ecx //делим размер массива на 4
        movdqa  .LC1(%rip), %xmm3 //константа из мамяти в xmm3
        pshufd  $0, %xmm5, %xmm4 //выполняем перестановку байт в xmm5 результат в xmm4
        salq    $4, %rcx //умножаем размер массива на 16
        addq    %rdi, %rcx //добавляем размер массива к адресу массива
.L30:
        movdqa  %xmm0, %xmm2 //копируем xmm0 в xmm2
        movdqa  %xmm4, %xmm1 //xmm4 в xmm1
        paddd   %xmm3, %xmm0 //прибавляем к xmm0 xmm3
        addq    $16, %rax //увеличиваем адрес массива на 16
        psubd   %xmm2, %xmm1 //вычитаем из xmm1 xmm2
        movups  %xmm1, -16(%rax) //перемещаем xmm1 в память по адресу rax - 16
        cmpq    %rcx, %rax //сравниваем адрес массива с конечным адресом
        jne     .L30 //если не равно конечному адресу переходим к l30
        testb   $3, %dl //установлен ли 3 бит в dl
        je      .L28 //если не установлен прыгаем в L28
        movl    %edx, %eax //перемещаем edx в eax
        andl    $-4, %eax //
.L29:
        movl    %edx, %r9d //перемещаем результат вызова в r9d
        movslq  %eax, %rcx //расширяем знак в eax до 64 бит и сохраняем в rcx
        subl    %eax, %r9d //вычитаем результат вызова функции из eax и сохраняем в r9d
        leaq    0(,%rcx,4), %r8 //вычисляем адрес массива + rcx * 4 и сохраняем в r8
        movl    %r9d, (%rdi,%rcx,4) //помещаем значение r9d в массив по адресу rdi + rcx * 4
        leal    1(%rax), %ecx //увеличиваем rax на 1 и сохраняем в ecx
        cmpl    %ecx, %edx //сравниваем ecx с результатом вызова функции
        jle     .L28
        movl    %edx, %r9d
        addl    $2, %eax
        subl    %ecx, %r9d
        movl    %r9d, 4(%rdi,%r8)
        cmpl    %eax, %edx
        jle     .L28
        subl    %eax, %edx
        movl    %edx, 8(%rdi,%r8)
.L28:
        call    bubble_sort //вызываем bubble_sort
        xorl    %eax, %eax  //устанавливаем eax в ноль
        leave   //восстанавливаем указатель стека
        ret    //возвращаемся из main
.L32:
        xorl    %eax, %eax
        jmp     .L29
.LC0:
        .long   0
        .long   1
        .long   2
        .long   3
.LC1:
        .long   4
        .long   4
        .long   4
        .long   4
