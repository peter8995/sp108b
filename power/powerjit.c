#include <stdio.h>

unsigned char powercode[] = {
    0x55,                                   //push   %rbp
    0x48,0x89,0xe5,                        //mov    %rsp,%rbp
    0x48,0x83,0xec,0x10,                    //sub    $0x10,%rsp
    0x89,0x4d,0x10,                         //mov    %ecx,0x10(%rbp)
    0x89,0x55,0x18,                         //mov    %edx,0x18(%rbp)
    0xc7,0x45,0xfc,0x01,0x00,0x00,0x00,     //movl   $0x1,-0x4(%rbp)
    0xc7,0x45,0xf8,0x01,0x00,0x00,0x00,     //movl   $0x1,-0x8(%rbp)
    0xeb,0x0e,                              //jmp    2c <power+0x2c>
    0x8b,0x45,0xfc,                         //mov    -0x4(%rbp),%eax
    0x0f,0xaf,0x45,0x10,                    //imul   0x10(%rbp),%eax
    0x89,0x45,0xfc,                         //mov    %eax,-0x4(%rbp)
    0x83,0x45,0xf8,0x01,                    //addl   $0x1,-0x8(%rbp)
    0x8b,0x45,0xf8,                         //mov    -0x8(%rbp),%eax
    0x3b,0x45,0x18,                         //cmp    0x18(%rbp),%eax
    0x7c,0xea,                              //jl     1e <power+0x1e>
    0x8b,0x45,0xfc,                         //mov    -0x4(%rbp),%eax
    0x48,0x83,0xc4,0x10,                    //add    $0x10,%rsp
    0x5d,                                   //pop    %rbp
    0xc3,                                   //retq
    0x90,                                   //nop
    0x90,                                   //nop
    0x90,                                   //nop      
};

int (*power)(int a ,int b);

int main(){
    power = (int (*)(int, int)) powercode;
    printf("power(3, 3)=%d\n", power(3, 3));
};