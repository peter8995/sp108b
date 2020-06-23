# 參考資料
[理解http server](https://www.bilibili.com/video/BV1S7411R7kF?p=7)      
[分析http requset](https://www.jianshu.com/p/f5a5db039737)  
[研讀之http sever程式碼](https://gist.github.com/laobubu/d6d0e9beb934b60b2e552c2d03e1409e)

[主要參考之書籍](http://beej-zhtw.netdpi.net/)
[strtok](https://blog.wu-boy.com/2010/04/cc-%E5%88%87%E5%89%B2%E5%AD%97%E4%B8%B2%E5%87%BD%E6%95%B8%EF%BC%9Astrtok-network-mac-address-%E5%88%86%E5%89%B2/)  
[malloc](https://openhome.cc/Gossip/CGossip/MallocFree.html)    
[socket 相關函式](https://www.itread01.com/content/1549576655.html)    
[memset](http://tw.gitbook.net/c_standard_library/c_function_memset.html)

[SIGCHLD](https://www.itread01.com/content/1546180142.html)     
[getaddrinfo](http://beej-zhtw.netdpi.net/05-system-call-or-bust/5-1-getaddrinfo-start)     
[dub2](https://www.geeksforgeeks.org/dup-dup2-linux-system-call/)

# 使用msys2實作
```
User@LAPTOP-D648SS44 MINGW64 /d/codes/sp108b/final
$ make
gcc -c -o main.o main.c
gcc -c -o httpd.o httpd.c
gcc -o server main.o httpd.o

User@LAPTOP-D648SS44 MINGW64 /d/codes/sp108b/final
$ ./sever
bash: ./sever: No such file or directory

User@LAPTOP-D648SS44 MINGW64 /d/codes/sp108b/final
$ ./server
Server started http://127.0.0.1:12913
 + [GET] /
[H] Host: 127.0.0.1:12913
[H] Connection: keep-alive
[H] Upgrade-Insecure-Requests: 1
[H] User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36
[H] Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
[H] Sec-Fetch-Site: none
[H] Sec-Fetch-Mode: navigate
[H] Sec-Fetch-User: ?1
[H] Sec-Fetch-Dest: document
[H] Accept-Encoding: gzip, deflate, br
[H] Accept-Language: zh-TW,zh;q=0.9,en-US;q=0.8,en;q=0.7
 + [GET] /favicon.ico
[H] Host: 127.0.0.1:12913
[H] Connection: keep-alive
[H] User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36
[H] Accept: image/webp,image/apng,image/*,*/*;q=0.8
[H] Sec-Fetch-Site: same-origin
[H] Sec-Fetch-Mode: no-cors
[H] Sec-Fetch-Dest: empty
[H] Referer: http://127.0.0.1:12913/
[H] Accept-Encoding: gzip, deflate, br
[H] Accept-Language: zh-TW,zh;q=0.9,en-US;q=0.8,en;q=0.7

```