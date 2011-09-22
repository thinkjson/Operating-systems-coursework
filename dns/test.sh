#!/bin/bash

javac dns/NSServer.java
javac dns/NSServerThread.java
javac dns/ShutdownIntercept.java
javac dns/NSClient.java
java dns/NSServer > log 2> /dev/null &
echo $! > server.PID
java dns/NSClient
kill `cat server.PID`
rm server.PID
