#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "connect to server and see new prompt" {
    ./dsh -s &

    run ./dsh -c <<EOF                
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "run ls and read a pre-made directory" {
    ./dsh -s &

    rm -rf /tmp/random-data
    mkdir -p /tmp/random-data

    touch /tmp/random-data/file1
    mkdir -p /tmp/random-data/dir1

    run ./dsh -c <<EOF                
ls /tmp/random-data
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>dir1file1rdsh-exec:rc=0dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "run a command that will fail with stderr logging" {
    ./dsh -s &

    run ./dsh -c <<EOF                
ls /non-existent-directory-that-hopefully-really-doesnt-exist
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>ls:cannotaccess'/non-existent-directory-that-hopefully-really-doesnt-exist':Nosuchfileordirectoryrdsh-exec:rc=2dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "run error when running two servers" {
    ./dsh -s &
    pid=$!

    run ./dsh -s
    kill -SIGTERM $pid

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="error:couldn'tbindsocket:Addressalreadyinusesocketservermode:addr:0.0.0.0:1234->Single-ThreadedModecmdloopreturned-50"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "run client fails to connect when specifying weird port" {
    ./dsh -s &

    run ./dsh -c -p 99999 <<EOF
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="error:can'tconnecttoserverstartclient:Connectionrefusedsocketclientmode:addr:127.0.0.1:99999cmdloopreturned-52"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "connect to server: simple piping command" {
    ./dsh -s &

    run ./dsh -c <<EOF                
echo "this is a pipe" | cat
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>thisisapiperdsh-exec:rc=0dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "connect to server: max commands 8 works" {
    ./dsh -s &

    run ./dsh -c <<EOF                
echo "wow" | echo "this" | echo "this" | echo "is" | echo "so" | echo "cool" | cat | grep "cool"
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>coolrdsh-exec:rc=0dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "connect to server: more than 8 commands fail" {
    ./dsh -s &

    run ./dsh -c <<EOF                
echo "wow" | echo "this" | echo "this" | echo "is" | echo "so" | echo "cool" | echo "!" | cat | grep "cool"
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="serverappearedtoterminate-exitingsocketclientmode:addr:127.0.0.1:1234dsh4>cmdloopreturned-50"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "can't connect because did not start server" {
    run ./dsh -c <<EOF                
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="error:can'tconnecttoserverstartclient:Connectionrefusedsocketclientmode:addr:127.0.0.1:1234cmdloopreturned-52"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "can't connect becatse did not start server" {
    ./dsh -s -p 10001 &
    
    run ./dsh -c -p 10001 <<EOF
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:10001dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}
