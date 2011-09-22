#!/bin/bash

javac dns/*.java
java dns/NSServer > log 2> /dev/null &
echo $! > server.PID
java dns/NSClient
kill `cat server.PID`
rm server.PID
