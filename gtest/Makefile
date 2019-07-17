all:
	g++ -o redis-tools main.cpp CL_Config.cpp TestRedis.cpp -I/workspace/boost_1_53_0 -I/workspace/hiredis-vip -I/workspace/googletest/googletest/include \
	/workspace/googletest/mybuild/libgtest.a \
	/workspace/hiredis-vip/libhiredis_vip.a -lpthread \
	/usr/local/lib/libboost_system.a  /usr/local/lib/libboost_thread.a /usr/local/lib/libboost_filesystem.a
clean:
	rm -rf redis-tools test  test_sync test_pipe test_async
