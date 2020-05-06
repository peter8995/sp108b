# PowerJitCall

## 執行方式
```
PS C:\Users\ldhsi\Desktop\系統程式\sp108b\homework\power> gcc .\powerjit.c -o power
PS C:\Users\ldhsi\Desktop\系統程式\sp108b\homework\power> ./power
power(3, 3)=-1199308912
```

## 原理

```
PS C:\Users\ldhsi\Desktop\系統程式\sp108b\homework\power> gcc -c power.c
PS C:\Users\ldhsi\Desktop\系統程式\sp108b\homework\power> objdump -d power.o

power.o:     file format pe-x86-64

Disassembly of section .text:

0000000000000000 <power>:
   0:   55                      push   %rbp
   1:   48 89 e5                mov    %rsp,%rbp
   4:   48 83 ec 10             sub    $0x10,%rsp
   8:   89 4d 10                mov    %ecx,0x10(%rbp)
   b:   89 55 18                mov    %edx,0x18(%rbp)
   e:   c7 45 fc 01 00 00 00    movl   $0x1,-0x4(%rbp)
  15:   c7 45 f8 01 00 00 00    movl   $0x1,-0x8(%rbp)
  21:   0f af 45 10             imul   0x10(%rbp),%eax
  25:   89 45 fc                mov    %eax,-0x4(%rbp)
  28:   83 45 f8 01             addl   $0x1,-0x8(%rbp)
  2c:   8b 45 f8                mov    -0x8(%rbp),%eax
  2f:   3b 45 18                cmp    0x18(%rbp),%eax
  32:   7c ea                   jl     1e <power+0x1e>
  34:   8b 45 fc                mov    -0x4(%rbp),%eax
  37:   48 83 c4 10             add    $0x10,%rsp
  3e:   90                      nop
  3f:   90                      nop