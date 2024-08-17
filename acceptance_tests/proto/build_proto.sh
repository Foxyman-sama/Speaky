#!/bin/bash

path="/home/froxy/Documents/Projects/C++/Speaky/proto"

protoc --proto_path=$path -I=. --python_out=. --pyi_out=. "$path/chat.proto"
protoc --proto_path=$path -I=. --python_out=. --pyi_out=. "$path/user_connection.proto"