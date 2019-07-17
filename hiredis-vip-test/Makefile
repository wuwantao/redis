all:
	gcc -g -o test test.c -I/workspace/hiredis-vip   /workspace/hiredis-vip/libhiredis_vip.a -lpthread
	gcc -g -o test_pipe test_pipe.c -I/workspace/hiredis-vip   /workspace/hiredis-vip/libhiredis_vip.a -lpthread
	gcc -g -o test_sync test_sync.c -I/workspace/hiredis-vip   /workspace/hiredis-vip/libhiredis_vip.a -lpthread
	gcc -g -o test_async test_async.c -I/workspace/  /workspace/hiredis-vip/libhiredis_vip.a -lpthread -levent 
clean:
	rm -rf redis-tools test  test_sync test_pipe test_async
