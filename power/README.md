# PowerJitCall

## 執行方式
```
PS D:\codes\sp108b\power> gcc .\powerjit.c -o power
PS D:\codes\sp108b\power> ./power
power(3, 3)=-1199308912
```

## 原理

```
PS D:\codes\sp108b\power> gcc -c power.c
PS D:\codes\sp108b\power> objdump -d power.o

power.o:     file format pe-i386 


Disassembly of section .text:

00000000 <power>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   83 ec 10                sub    $0x10,%esp
   6:   c7 45 fc 01 00 00 00    movl   $0x1,-0x4(%ebp)
   d:   c7 45 f8 01 00 00 00    movl   $0x1,-0x8(%ebp)
  14:   eb 0e                   jmp    24 <_power+0x24>
  16:   8b 45 fc                mov    -0x4(%ebp),%eax
  19:   0f af 45 08             imul   0x8(%ebp),%eax
  1d:   89 45 fc                mov    %eax,-0x4(%ebp)
  20:   83 45 f8 01             addl   $0x1,-0x8(%ebp)
  24:   8b 45 f8                mov    -0x8(%ebp),%eax
  27:   3b 45 0c                cmp    0xc(%ebp),%eax
  2a:   7c ea                   jl     16 <_power+0x16>
  2c:   8b 45 fc                mov    -0x4(%ebp),%eax
  2f:   c9                      leave
  30:   c3                      ret
  31:   90                      nop
  32:   90                      nop
  33:   90                      nop