#!/bin/bash

javac NSServer.java
javac NSServerThread.java
javac ShutdownIntercept.java
javac NSClient.java
java NSServer > log 2> /dev/null &
echo $! > server.PID
java NSClient
kill `cat server.PID`
rm server.PID